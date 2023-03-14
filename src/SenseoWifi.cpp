/*
SenseoWifi.cpp - base file for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include <Homie.h>

#include "SenseoLed.h"
#include "SenseoControl.h"
#include "pins.h"
#include "constants.h"
#include "testIO.cpp"
#include "HomeAssistant.h"
#include "SenseoFsm/SenseoFsm.h"
#include "SenseoFsm/Components/BuzzerComponent.h"
#include "SenseoFsm/Components/CupComponent.h"
#include "ModularFsm/FsmState.h"


SenseoFsm mySenseo;
SenseoLed mySenseoLed(ocSenseLedPin);
SenseoControl myControl(ocPressPowerPin, ocPressLeftPin, ocPressRightPin);

HomieNode senseoNode("machine", "senseo-wifi", "senseo-wifi");
HomieSetting<bool> CupDetectorAvailableSetting("cupdetector", "Enable cup detection (TCRT5000)");
HomieSetting<bool> BuzzerSetting("buzzer", "Enable buzzer sounds (no water, cup finished, ...)");
HomieSetting<bool> PublishHomeAssistantDiscoveryConfig("homeassistantautodiscovery", "Publish HomeAssistant discovery config, ...)");
HomieSetting<bool> UseCustomizableButtonsAddon("usecustomizablebuttonsaddon", "Use the additional pcb to customize button behavior, ...)");

/**
* Called by the LED changed interrupt
*/
void IRAM_ATTR ledChangedHandler() {
  mySenseoLed.pinStateToggled();
}

void onSenseoStateChange(FsmState * prevState, FsmState * nextState) {
  if (true) Homie.getLogger() << "Senseo state machine, new Senseo state: " << nextState->getStateName() << endl;
  
  senseoNode.setProperty("opState").send(nextState->getStateName());
  if (prevState != nullptr) {
    int secondInPrevState = (prevState->getTimeInState() + 500) / 1000;
    senseoNode.setProperty("debug").send(
      String("senseoState: Switching from ") + String(prevState->getStateName())
      + String(" to ") + String(nextState->getStateName())
      + String(" after ") + String(secondInPrevState) + String(" seconds")
    );
  }
}

/**
* Called by Homie upon an MQTT message to '.../power'
* MQTT response is sent from this routine, as pessimistic feedback from state machine is too slow and triggers a timeout in e.g. Home Assistant.
*/
bool powerHandler(const HomieRange& range, const String& value) {
  if (value != "true" && value !="false" && value != "reset") {
    senseoNode.setProperty("debug").send("power: malformed message content. Allowed: [true,false,reset].");
    return false;
  }

  if (value == "true" && mySenseo.isOff()) {
    myControl.pressPowerButton();
    senseoNode.setProperty("power").send("true");
  }
  else if (value == "false" && !mySenseo.isOff()) {
    myControl.pressPowerButton();
    senseoNode.setProperty("power").send("false");
  }
  else if (value == "reset") {
    senseoNode.setProperty("power").send("false");
    tone(beeperPin, 4096, 8000);
    Homie.reset();
  }
  return true;
}

/**
* Called by Homie upon an MQTT message to '.../brew'.
* No MQTT response is sent from this routine, as pessimistic feedback will be handled in the state machine.
*/
bool brewHandler(const HomieRange& range, const String& value) {
  if (value != "1cup" && value !="2cup" && value !="descale") {
    senseoNode.setProperty("debug").send("brew: malformed message content. Allowed: [1cup,2cup,descale].");
    return false;
  }

  //TODO: move that in the FSM once I implemented a command patern
  CupComponent * cupComponent = mySenseo.getComponent<CupComponent>();
  if (cupComponent != nullptr) {
    if (cupComponent->isNotAvailable() || cupComponent->isFull()) {
      senseoNode.setProperty("debug").send("brew: no or full cup present. Not executing.");
      return false;
    }
  }

  if (value == "1cup") myControl.pressLeftButton();
  else if (value == "2cup") myControl.pressRightButton();
  else if (value == "descale") myControl.pressLeftRightButton();
  return true;
}

/**
* Called by Homie upon an MQTT message to '.../buzzer'.
*/
bool buzzerHandler(const HomieRange& range, const String& value) {
  BuzzerComponent * buzzerComponent = mySenseo.getComponent<BuzzerComponent>();
  if (buzzerComponent != nullptr) {
    senseoNode.setProperty("buzzer").send(value);
    bool success = buzzerComponent->buzz(value);
    if (!success) senseoNode.setProperty("debug").send("buzzer: malformed message content. Allowed: [melody1,melody2,melody3].");
    senseoNode.setProperty("buzzer").send("");
    return success;
  }
  else {
    senseoNode.setProperty("debug").send("buzzer: not configured.");
    return false;
  }
}

void publishHomeAssistandDiscoveryConfig()
{
    Homie.getLogger() << endl << "Creating HomeAssistant entities..." << endl;
    HomeAssistantDiscovery ha; 

    //binary_sensor
    bool success = ha.publishBinarySensorConfig("Out Of Water","outOfWater",{{"icon", "mdi:water-off-outline"},{"device_class","problem"}});    
    Homie.getLogger() << "OutOfWater: " << (success ? "success" : "failed") << endl;

    if (CupDetectorAvailableSetting.get()) {
        success = ha.publishBinarySensorConfig("Cup Available","cupAvailable",{{"icon", "mdi:coffee-outline"}});    
        Homie.getLogger() << "cupAvailable: " << (success ? "success" : "failed") << endl;

        success = ha.publishBinarySensorConfig("Cup Full","cupFull",{{"icon", "mdi:coffee"}});    
        Homie.getLogger() << "cupFull: " << (success ? "success" : "failed") << endl;
    }

    //sensor
    success = ha.publishSensorConfig("Brewed Size","brewedSize",{{"icon", "mdi:coffee-maker"}});    
    Homie.getLogger() << "brewedSize: " << (success ? "success" : "failed") << endl;

    success = ha.publishSensorConfig("Operating State","opState",{{"icon", "mdi:state-machine"}});    
    Homie.getLogger() << "opState: " << (success ? "success" : "failed") << endl;

    //debug sensor
    success = ha.publishSensorConfig("Debug","debug",{{"icon","mdi:comment-text-multiple-outline"},{"entity_category","diagnostic"}});    
    Homie.getLogger() << "debug: " << (success ? "success" : "failed") << endl;

    success = ha.publishStatConfig("Rssi","signal",{{"icon","mdi:signal-cellular-2"},{"unit_of_measurement","%"},{"device_class","signal_strength"},{"entity_category","diagnostic"}});    
    Homie.getLogger() << "rssi: " << (success ? "success" : "failed") << endl;

    success = ha.publishStatConfig("Uptime","uptime",{{"icon","mdi:av-timer"},{"unit_of_measurement","s"}});    
    Homie.getLogger() << "uptime: " << (success ? "success" : "failed") << endl;

    //switch
    success = ha.publishSwitchConfig("Power","power",{{"icon","mdi:power"}});    
    Homie.getLogger() << "power: " << (success ? "success" : "failed") << endl;

    //button
    success = ha.publishButtonConfig("Brew Coffee Normal","brew","1cup",{{"icon","mdi:coffee"}});    
    Homie.getLogger() << "brew 1cup: " << (success ? "success" : "failed") << endl;

    success = ha.publishButtonConfig("Brew Coffee Double","brew","2cup",{{"icon","mdi:coffee"}});    
    Homie.getLogger() << "brew 2cup: " << (success ? "success" : "failed") << endl;

    Homie.getLogger() << endl;
}

/**
* The device rebooted when attachInterrupt was called in setup()
* before Wifi was connected and interrupts were already coming in.
*/
void onHomieEvent(const HomieEvent &event) {
  switch (event.type) {
  case HomieEventType::WIFI_CONNECTED:
    attachInterrupt(digitalPinToInterrupt(ocSenseLedPin), ledChangedHandler, CHANGE);
    break;
  case HomieEventType::WIFI_DISCONNECTED:
    detachInterrupt(digitalPinToInterrupt(ocSenseLedPin));
    break;
  default:
    break;
  case HomieEventType::MQTT_READY:
    if (PublishHomeAssistantDiscoveryConfig.get()) {
      publishHomeAssistandDiscoveryConfig();
    }
    break;

  }
}

/**
*
*/
void setupHandler() {
  // configuring the state machine
  //senseoFsm.addComponent(std::make_unique<SenseoCommands>());
  //senseoFsm.addComponent(std::make_unique<SenseoControl>(ocPressPowerPin, ocPressLeftPin, ocPressRightPin));
  //senseoFsm.addComponent(std::make_unique<SenseoInputButtons>(senseoButtonsInputPin));
  //senseoFsm.addComponent(std::make_unique<SenseoLedComponent>(senseoLedOutPin));
  // if (BuzzerSetting.get()) senseoFsm.addComponent(std::make_unique<BuzzerComponent>(beeperPin));
  // if (CupDetectorAvailableSetting.get()) senseoFsm.addComponent(std::make_unique<CupComponent>(cupDetectorPin));
  // if (UseCustomizableButtonsAddon.get()) senseoFsm.addComponent(std::make_unique<SenseoLedComponent>(senseoLedOutPin));

  // senseoFsm.addState(std::make_unique<BrewingState>(mySenseoLed,senseoNode));
  // senseoFsm.addState(std::make_unique<HeatingState>(mySenseoLed));
  // senseoFsm.addState(std::make_unique<NoWaterState>(mySenseoLed,senseoNode));
  // senseoFsm.addState(std::make_unique<OffState>(mySenseoLed,senseoNode));
  // senseoFsm.addState(std::make_unique<ReadyState>(mySenseoLed));
  // senseoFsm.addState(std::make_unique<UnknownState>(mySenseoLed));
  mySenseo.setup(mySenseoLed,senseoNode,CupDetectorAvailableSetting.get(),BuzzerSetting.get(),UseCustomizableButtonsAddon.get());
  mySenseo.registerStateChangeHandler(onSenseoStateChange);

  // senseoFsm.setInitialState<UnknownState>();

  //configuring the button handler
  /*SenseoInputButtons * inputButtons = senseoFsm.getComponent<SenseoInputButtons>();
  if (inputButtons != nullptr) {
    inputButtons->addButtonReleaseHandler(A0buttonPwr,50,[]() { myControl.pressPowerButton(); });
    inputButtons->addButtonReleaseHandler(A0buttonPwr,4000,[]() { Homie.getLogger() << "Reset Senseo" << endl; });
    inputButtons->addButtonReleaseHandler(A0buttonPwr,2000,[]() { Homie.getLogger() << "Reset Canceled" << endl; });
    inputButtons->addButtonHoldHandler(A0buttonPwr,2000,[]() { buzz("tone2"); });
    inputButtons->addButtonHoldHandler(A0buttonPwr,3000,[]() { buzz("tone2"); });
    inputButtons->addButtonHoldHandler(A0buttonPwr,4000,[]() { buzz("reset"); });
    inputButtons->addButtonReleaseHandler(A0button1Cup,50,[]() { myControl.pressLeftButton(); });
    inputButtons->addButtonReleaseHandler(A0button2Cup,50,[]() { myControl.pressRightButton(); });
  }*/

  if (BuzzerSetting.get()) tone(beeperPin, 2048, 500);

  Homie.getLogger() << endl << "☕☕☕☕ Enjoy your SenseoWifi ☕☕☕☕" << endl << endl;

  //Do we really need that? The Fsm should properly send the information
  //senseoNode.setProperty("opState").send(UnknownState::s_StateName);
  CupComponent * cupComponent = mySenseo.getComponent<CupComponent>();
  if (cupComponent != nullptr) {
    senseoNode.setProperty("cupAvailable").send(cupComponent->isAvailable() ? "true" : "false");
    senseoNode.setProperty("cupFull").send(cupComponent->isFull() ? "true" : "false");
  }
  senseoNode.setProperty("outOfWater").send("false");
  senseoNode.setProperty("brew").send("false");
  senseoNode.setProperty("debug").send("Machine started");
}

/**
*
*/
void loopHandler() {
  /**
  * Update the low level LED state machine based on the measured LED timings.
  * (off, slow blinking, fast blinking, on)
  */
  mySenseoLed.updateState();
  if (mySenseoLed.hasChanged()) {
    if (true) Homie.getLogger() << "LED state machine, new LED state: " << mySenseoLed.getStateAsString() << endl;
  }

  /**
  * Update of the main state machine
  */
  mySenseo.update(millis());

  //TODO : move that somewhere else, most likely in the Component update
  /**
  * Check and update the cup availability, based on the cup detector signal.
  * (no cup, cup available, cup full)
  */
 CupComponent * cupComponent = mySenseo.getComponent<CupComponent>();
  if (cupComponent != nullptr) {
    //myCup.updateState();
    if (cupComponent->isAvailableChanged()) {
      senseoNode.setProperty("cupAvailable").send(cupComponent->isAvailable() ? "true" : "false");
    }
    if (cupComponent->isFullChanged()) {
      senseoNode.setProperty("cupFull").send(cupComponent->isFull() ? "true" : "false");
    }
  }  

  /**
  * Non-blocking Low-High-Low transition.
  * Check for a simulated button press - release after > 100ms
  */
  myControl.releaseIfPressed();
}

void setup() {
  Serial.begin(115200);

  /**
  * Wemos D1 mini pin initializations
  */
  pinMode(ocPressLeftPin, OUTPUT);
  pinMode(ocPressRightPin, OUTPUT);
  pinMode(ocPressPowerPin, OUTPUT);
  pinMode(ocSenseLedPin, INPUT_PULLUP);

  digitalWrite(ocPressPowerPin, LOW);
  digitalWrite(ocPressLeftPin, LOW);
  digitalWrite(ocPressRightPin, LOW);

  pinMode(beeperPin, OUTPUT);

  // it seems at this point Homie configuration variable are not set
  if (CupDetectorAvailableSetting.get()) {
    pinMode(cupDetectorPin, INPUT_PULLUP); 
  }

  //pinMode(senseoButtonsInputPin, INPUT);  

  /**
  * Testing routine. Activate only in development environemt.
  * Tests the circuit and Senseo connections, loops indefinitely.
  *
  * Wifi will NOT BE AVAILABLE, no OTA!
  */
  if (false) testIO();


  /**
  * Homie specific settings
  */
  Homie_setFirmware("senseo-wifi", "1.9");
  Homie_setBrand("SenseoWifi");
  //Homie.disableResetTrigger();
  Homie.disableLedFeedback();
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  /**
  * Homie: Options, see at the top of this file.
  */
  CupDetectorAvailableSetting.setDefaultValue(true);
  BuzzerSetting.setDefaultValue(false);
  PublishHomeAssistantDiscoveryConfig.setDefaultValue(false);
  UseCustomizableButtonsAddon.setDefaultValue(true);

  /**
  * Homie: Advertise custom SenseoWifi MQTT topics
  */
  senseoNode.advertise("debug").setName("Debugging Information").setDatatype("string").setRetained(false);
  senseoNode.advertise("opState").setName("Operational State").setDatatype("enum").setFormat("SENSEO_unknown,SENSEO_OFF,SENSEO_HEATING,SENSEO_READY,SENSEO_BREWING,SENSEO_NOWATER");
  senseoNode.advertise("power").setName("Power").setDatatype("boolean").settable(powerHandler);
  senseoNode.advertise("brew").setName("Brew").settable(brewHandler).setDatatype("enum").setFormat("1cup,2cup");
  senseoNode.advertise("brewedSize").setName("Brew Size").setDatatype("string").setRetained(false);
  senseoNode.advertise("outOfWater").setName("Out of Water").setDatatype("boolean");
  if (CupDetectorAvailableSetting.get()) senseoNode.advertise("cupAvailable").setName("Cup Available");
  if (CupDetectorAvailableSetting.get()) senseoNode.advertise("cupFull").setName("Cup Full");
  if (BuzzerSetting.get()) senseoNode.advertise("buzzer").setName("Buzzer").settable(buzzerHandler).setDatatype("enum").setFormat("melody1,melody2,melody3");

  if (BuzzerSetting.get()) tone(beeperPin, 1536, 2000);
  Homie.onEvent(onHomieEvent);
  Homie.setup();

  //TODO: test if I can move all setup things in the setup handler
  if (UseCustomizableButtonsAddon.get()) {
    Homie.getLogger() << "Shuting down the led" << endl;
    pinMode(senseoLedOutPin, OUTPUT);  
    digitalWrite(senseoLedOutPin, LOW);
  }
  else {
    pinMode(resetButtonPin, INPUT_PULLUP);
    Homie.setResetTrigger(resetButtonPin, LOW, 5000);
  }
}

void loop() {
  Homie.loop();
}
