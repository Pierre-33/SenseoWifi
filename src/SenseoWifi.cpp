/*
SenseoWifi.cpp - base file for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include <Homie.h>

#include "SenseoLed.h"
#include "SenseoSM.h"
#include "SenseoControl.h"
#include "pins.h"
#include "constants.h"
#include "testIO.cpp"
#include "HomeAssistant.h"
//#include <ezBuzzer.h>
#include "FSM/FsmWithComponents.h"
#include "FSM/Components/SenseoLedComponent.h"
//#include "FSM/Components/SenseoInputButtons.h"
#include "FSM/Components/BuzzerComponent.h"
#include "FSM/Components/CupComponent.h"
#include "FSM/States/BrewingState.h"
#include "FSM/States/HeatingState.h"
#include "FSM/States/NoWaterState.h"
#include "FSM/States/OffState.h"
#include "FSM/States/ReadyState.h"
#include "FSM/States/UnknownState.h"

FsmWithComponents senseoFsm;
SenseoLed mySenseoLed(ocSenseLedPin);
//SenseoSM mySenseoSM;
SenseoControl myControl(ocPressPowerPin, ocPressLeftPin, ocPressRightPin);
//Cup myCup(cupDetectorPin);
//ezBuzzer myBuzzer(beeperPin);

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
  int secondInPrevState = (prevState->getTimeInState() + 500) / 1000;
  senseoNode.setProperty("debug").send(
    String("senseoState: Switching from ") + String(prevState->getStateName())
    + String(" to ") + String(nextState->getStateName())
    + String(" after ") + String(secondInPrevState) + String(" seconds")
  );
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

  if (value == "true" && senseoFsm.isInState<OffState>()) {
    myControl.pressPowerButton();
    senseoNode.setProperty("power").send("true");
  }
  else if (value == "false" && !senseoFsm.isInState<OffState>()) {
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
  CupComponent * cupComponent = senseoFsm.getComponent<CupComponent>();
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

/*void buzz(const String &value) {
  if (value == "tone1") tone(beeperPin, 4096, 300);
  if (value == "tone2") tone(beeperPin, 2048, 300);
  if (value == "tone3") tone(beeperPin, 1536, 300);
  if (value == "tone4") tone(beeperPin, 1024, 300);
  if (value == "melody1") {
    static int melody1[] = { NOTE_C7 };
    static int noteDurations1[] = { 2 };
    myBuzzer.playMelody(melody1, noteDurations1, sizeof(noteDurations1) / sizeof(int));
  }
  if (value == "melody2") {
    static int melody2[] = { NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5 };
    static int noteDurations2[] = { 4, 8, 8, 2 };
    myBuzzer.playMelody(melody2, noteDurations2, sizeof(noteDurations2) / sizeof(int));
  }
  if (value == "melody3") {
    static int melody3[] = { NOTE_C4, NOTE_C5 };
    static int noteDurations3[] = { 4, 8 };
    myBuzzer.playMelody(melody3, noteDurations3, sizeof(noteDurations3) / sizeof(int));
  }
}*/

/**
* Called by Homie upon an MQTT message to '.../buzzer'.
*/
bool buzzerHandler(const HomieRange& range, const String& value) {
  BuzzerComponent * buzzerComponent = senseoFsm.getComponent<BuzzerComponent>();
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

/**
* Senseo state machine, transition reaction: exit actions
*/
/*void senseoStateExitAction() {
  senseoNode.setProperty("debug").send(
    String("senseoState: Switching from ") + String(mySenseoSM.getStatePrevAsString())
    + String(" to ") + String(mySenseoSM.getStateAsString())
    + String(" after ") + String(mySenseoSM.getSecondsInLastState()) + String(" seconds")
  );
}*/

/**
* Senseo state machine, transition reaction: entry actions
*/
/*void senseoStateEntryAction() {
  switch (mySenseoSM.getState()) {
    case SENSEO_OFF: {
      senseoNode.setProperty("brewedSize").send("0");
      senseoNode.setProperty("power").send("false");
      break;
    }
    case SENSEO_HEATING: {
      break;
    }
    case SENSEO_READY: {
      if (BuzzerSetting.get()) buzz("melody1");
      break;
    }
    case SENSEO_BREWING: {
      if (CupDetectorAvailableSetting.get()) {
        if (myCup.isAvailable()) {
          myCup.setFilling();
        }
        else {
          senseoNode.setProperty("debug").send("cup: Brewing without detected cup, will not report the filling->full process.");
        }
      }
      senseoNode.setProperty("brew").send("true");
      senseoNode.setProperty("brewedSize").send("0");
      break;
    }
    case SENSEO_NOWATER: {
      if (BuzzerSetting.get()) buzz("melody2");
      senseoNode.setProperty("outOfWater").send("true");
      break;
    }
    case SENSEO_unknown: {
      break;
    }
  }
}*/

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
  senseoFsm.addComponent(std::make_unique<SenseoLedComponent>(senseoLedOutPin));
  if (BuzzerSetting.get()) senseoFsm.addComponent(std::make_unique<BuzzerComponent>(beeperPin));
  if (CupDetectorAvailableSetting.get()) senseoFsm.addComponent(std::make_unique<CupComponent>(cupDetectorPin));

  senseoFsm.addState(std::make_unique<BrewingState>(mySenseoLed,senseoNode));
  senseoFsm.addState(std::make_unique<HeatingState>(mySenseoLed));
  senseoFsm.addState(std::make_unique<NoWaterState>(mySenseoLed,senseoNode));
  senseoFsm.addState(std::make_unique<OffState>(mySenseoLed,senseoNode));
  senseoFsm.addState(std::make_unique<ReadyState>(mySenseoLed));
  senseoFsm.addState(std::make_unique<UnknownState>(mySenseoLed));
  senseoFsm.registerStateChangeHandler(onSenseoStateChange);

  senseoFsm.setInitialState<UnknownState>();

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

  senseoNode.setProperty("opState").send(UnknownState::s_StateName);
  CupComponent * cupComponent = senseoFsm.getComponent<CupComponent>();
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

  senseoFsm.update(millis());

  /**
  * Check and update the cup availability, based on the cup detector signal.
  * (no cup, cup available, cup full)
  */
 CupComponent * cupComponent = senseoFsm.getComponent<CupComponent>();
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
  * Update the higher level Senseo state machine based on the LED state.
  * (off, heating, ready, brewing, no water)
  */
  /*mySenseoSM.updateState(mySenseoLed.getState());
  if (mySenseoSM.stateHasChanged()) {
    if (true) Homie.getLogger() << "Senseo state machine, new Senseo state: " << mySenseoSM.getStateAsString() << endl;
    senseoNode.setProperty("opState").send(mySenseoSM.getStateAsString());

    senseoStateExitAction();
    senseoStateEntryAction();
  }*/

  /**
  * Non-blocking Low-High-Low transition.
  * Check for a simulated button press - release after > 100ms
  */
  myControl.releaseIfPressed();
  //myBuzzer.loop();
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
  pinMode(resetButtonPin, INPUT_PULLUP);

  if (CupDetectorAvailableSetting.get()) {
    pinMode(cupDetectorPin, INPUT_PULLUP); 
  }

  //pinMode(senseoButtonsInputPin, INPUT);  

  Homie.getLogger() << endl << "A0 read :" << analogRead(senseoButtonsInputPin) << endl;

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
  Homie.setResetTrigger(resetButtonPin, LOW, 5000);
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  /**
  * Homie: Options, see at the top of this file.
  */
  CupDetectorAvailableSetting.setDefaultValue(false);
  BuzzerSetting.setDefaultValue(false);
  PublishHomeAssistantDiscoveryConfig.setDefaultValue(false);
  UseCustomizableButtonsAddon.setDefaultValue(false);

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
}

void loop() {
  Homie.loop();
}
