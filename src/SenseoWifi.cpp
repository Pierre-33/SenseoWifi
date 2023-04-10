/*
SenseoWifi.cpp - base file for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include <Homie.h>
#include <memory>

#include "pins.h"
#include "constants.h"
#include "testIO.cpp"
#include "HomeAssistant.h"
#include "SenseoFsm/SenseoFsm.h"
#include "SenseoFsm/Components/BuzzerComponent.h"
#include "SenseoFsm/Components/CupComponent.h"
#include "SenseoFsm/Components/programComponent.h"
#include "ModularFsm/FsmState.h"
#include "SenseoInputButtons.h"
#include "SenseoLed/SenseoLedTimerBased.h"
#include "SenseoLed/SenseoLed.h"

#ifdef EXECUTE_IF_COMPONENT_EXIST 
#undef EXECUTE_IF_COMPONENT_EXIST
#endif
#define EXECUTE_IF_COMPONENT_EXIST(fsm,component,...) if (fsm.getComponent<component>() != nullptr) { fsm.getComponent<component>()->__VA_ARGS__; }

HomieNode senseoNode("machine", "senseo-wifi", "senseo-wifi");
HomieSetting<bool> CupDetectorAvailableSetting("cupdetector", "Enable cup detection (TCRT5000)");
HomieSetting<bool> BuzzerSetting("buzzer", "Enable buzzer sounds (no water, cup finished, ...)");
HomieSetting<bool> PublishHomeAssistantDiscoveryConfig("homeassistantautodiscovery", "Publish HomeAssistant discovery config, ...)");
HomieSetting<bool> UseCustomizableButtonsAddon("usecustomizablebuttonsaddon", "Use the additional pcb to customize button behavior, ...)");

SenseoFsm mySenseo(senseoNode);
//SenseoLedTimerBased mySenseoLed(senseoNode,ocSenseLedPin); 
SenseoLed mySenseoLed(senseoNode,ocSenseLedPin); 
std::unique_ptr<SenseoInputButtons> myInputbuttons;

/**
* Called by Homie upon an MQTT message to '.../power'
* MQTT response is sent from this routine, as pessimistic feedback from state machine is too slow and triggers a timeout in e.g. Home Assistant.
*/
bool powerHandler(const HomieRange& range, const String& value) 
{
  if (value != "true" && value !="false" && value != "reset") 
  {
    senseoNode.setProperty("debug").send("power: malformed message content. Allowed: [true,false,reset].");
    return false;
  }

  if (value == "true" && mySenseo.isOff()) 
  {
    mySenseo.sendCommands(CommandComponent::TurnOn);
    senseoNode.setProperty("power").send("true");
  }
  else if (value == "false" && !mySenseo.isOff()) 
  {
    mySenseo.sendCommands(CommandComponent::TurnOff);
    senseoNode.setProperty("power").send("false");
  }
  else if (value == "reset") 
  {
    senseoNode.setProperty("power").send("false");
    EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("reset"));
    Homie.reset();
  }
  return true;
}

bool program1CupHandler(const HomieRange& range, const String& value) 
{
  if (value == "true") 
  {
    EXECUTE_IF_COMPONENT_EXIST(mySenseo,ProgramComponent,requestProgram(ProgramComponent::oneCup));
  }
  else 
  {
    EXECUTE_IF_COMPONENT_EXIST(mySenseo,ProgramComponent,clearProgram(ProgramComponent::oneCup));
  }
  return true;
}

bool program2CupHandler(const HomieRange& range, const String& value) 
{
  if (value == "true") 
  {
    EXECUTE_IF_COMPONENT_EXIST(mySenseo,ProgramComponent,requestProgram(ProgramComponent::twoCup));
  }
  else 
  {
    EXECUTE_IF_COMPONENT_EXIST(mySenseo,ProgramComponent,clearProgram(ProgramComponent::twoCup));
  }
  return true;
}

/**
* Called by Homie upon an MQTT message to '.../brew'.
* No MQTT response is sent from this routine, as pessimistic feedback will be handled in the state machine.
*/
bool brewHandler(const HomieRange& range, const String& value) 
{
  if (value != "1cup" && value !="2cup" && value !="descale") 
  {
    senseoNode.setProperty("debug").send("brew: malformed message content. Allowed: [1cup,2cup,descale].");
    return false;
  }

  //TODO: move that in the FSM once I implemented a command patern
  CupComponent * cupComponent = mySenseo.getComponent<CupComponent>();
  if (cupComponent != nullptr) 
  {
    if (cupComponent->isNotAvailable() || cupComponent->isFull()) 
    {
      senseoNode.setProperty("debug").send("brew: no or full cup present. Not executing.");
      return false;
    }
  }

  if (value == "1cup" || value == "2cup")
  {
    CommandComponent::CommandBitFields commands = value == "1cup" ? CommandComponent::Brew1Cup : CommandComponent::Brew2Cup;
    if (mySenseo.isOff()) commands |= CommandComponent::TurnOn | CommandComponent::TurnOffAfterBrewing;
    mySenseo.sendCommands(commands);
  }
  else if (value == "descale") 
  {
    assert(!"not implemented");
    //myControl.pressLeftRightButton();
  }
  return true;
}

/**
* Called by Homie upon an MQTT message to '.../buzzer'.
*/
bool buzzerHandler(const HomieRange& range, const String& value) 
{
  BuzzerComponent * buzzerComponent = mySenseo.getComponent<BuzzerComponent>();
  if (buzzerComponent != nullptr) 
  {
    senseoNode.setProperty("buzzer").send(value);
    bool success = buzzerComponent->playMelody(value);
    if (!success) 
    {
      String errorMsg = String("buzzer: malformed message content. Allowed: [") + buzzerComponent->getValidTunes() + "]";
      senseoNode.setProperty("debug").send(errorMsg);
    }
    senseoNode.setProperty("buzzer").send("");
    return success;
  }
  else 
  {
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

    if (CupDetectorAvailableSetting.get()) 
    {
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

    success = ha.publishSensorConfig("Led State","ledState",{{"icon", "mdi:state-machine"}});    
    Homie.getLogger() << "ledState: " << (success ? "success" : "failed") << endl;

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
    if (UseCustomizableButtonsAddon.get())
    {
      success = ha.publishSwitchConfig("Program 1 Cup","program1Cup",{{"icon","mdi:coffee"}});    
      Homie.getLogger() << "program1Cup: " << (success ? "success" : "failed") << endl;
      success = ha.publishSwitchConfig("Program 2 Cups","program2Cup",{{"icon","mdi:coffee"}});    
      Homie.getLogger() << "program2Cup: " << (success ? "success" : "failed") << endl;
    }

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
void onHomieEvent(const HomieEvent &event) 
{
  switch (event.type) 
  {
  case HomieEventType::WIFI_CONNECTED:
    mySenseoLed.attachInterrupt();
    break;
  case HomieEventType::WIFI_DISCONNECTED:
    mySenseoLed.detachInterrupt();
    break;
  default:
    break;
  case HomieEventType::MQTT_READY:
    if (PublishHomeAssistantDiscoveryConfig.get()) 
    {
      publishHomeAssistandDiscoveryConfig();
    }
    break;

  }
}

void brewCup(CommandComponent::Command command)
{
  CommandComponent::CommandBitFields commands = command;
  if (mySenseo.isOff()) commands |= CommandComponent::TurnOn | CommandComponent::TurnOffAfterBrewing;
  mySenseo.sendCommands(commands);
}

void togglePower()
{
  if (mySenseo.isOff()) mySenseo.sendCommands(CommandComponent::TurnOn);
  else mySenseo.sendCommands(CommandComponent::TurnOff);
}

void holdCupButtonHandler(ProgramComponent::Program program)
{
  ProgramComponent * programComponent = mySenseo.getComponent<ProgramComponent>();
  if (programComponent)
  {
    if (programComponent->hasAnyProgram())
    {
      programComponent->clearProgram(ProgramComponent::all);
      EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("beep"));
    }
    else
    {
      programComponent->requestProgram(program);
      EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("programcup"));
    }
  }
}

/**
*
*/
void setupHandler() 
{
   /**
  * Homie: Advertise custom SenseoWifi MQTT topics
  */
  // configuring the state machine
  mySenseo.setup(mySenseoLed,CupDetectorAvailableSetting.get(),BuzzerSetting.get(),UseCustomizableButtonsAddon.get());

  /*if (mySenseo.getComponent<BuzzerComponent>() != nullptr) 
  {
    //mySenseo.getComponent<BuzzerComponent>()->getValidTunes()
    //senseoNode.advertise("buzzer").setName("Buzzer").settable(buzzerHandler).setDatatype("enum").setFormat("BlaBlaBla");
    //senseoNode.getProperty("buzzer")->setFormat("blobloblo");
    Homie.getLogger() << "Advertising Buzzer" << endl;
  }*/
    

  //configuring the button handler
  if (UseCustomizableButtonsAddon.get())
  {    
    senseoNode.setProperty("program1Cup").send("false");
    senseoNode.setProperty("program2Cup").send("false");
  

    myInputbuttons = std::make_unique<SenseoInputButtons>(senseoButtonsInputPin);
    myInputbuttons->addButtonReleaseHandler(A0buttonPwr,50,togglePower);
    myInputbuttons->addButtonReleaseHandler(A0buttonPwr,9000,[]() { Homie.getLogger() << "Reset Senseo" << endl;/* Homie.reset();*/ });
    myInputbuttons->addButtonReleaseHandler(A0buttonPwr,2000,[]() { Homie.getLogger() << "Reset Canceled" << endl; });
    myInputbuttons->addButtonHoldHandler(A0buttonPwr,3000,[]() { EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("beep")); });
    myInputbuttons->addButtonHoldHandler(A0buttonPwr,5000,[]() { EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("beep")); });
    myInputbuttons->addButtonHoldHandler(A0buttonPwr,7000,[]() { EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("mario")); });

    myInputbuttons->addButtonReleaseHandler(A0button1Cup,50,[]() { brewCup(CommandComponent::Brew1Cup); });
    myInputbuttons->addButtonHoldHandler(A0button1Cup,1000,[]() { holdCupButtonHandler(ProgramComponent::oneCup); });
    myInputbuttons->addButtonHoldHandler(A0button1Cup,3000,[]() { holdCupButtonHandler(ProgramComponent::oneCup); });
    myInputbuttons->addButtonReleaseHandler(A0button1Cup,1000,[]() { }); //this one is to prevent the BrewCup release to trigger

    myInputbuttons->addButtonReleaseHandler(A0button2Cup,50,[]() { brewCup(CommandComponent::Brew2Cup); });
    myInputbuttons->addButtonHoldHandler(A0button2Cup,1000,[]() { holdCupButtonHandler(ProgramComponent::twoCup); });
    myInputbuttons->addButtonHoldHandler(A0button2Cup,3000,[]() { holdCupButtonHandler(ProgramComponent::twoCup); });
    myInputbuttons->addButtonReleaseHandler(A0button2Cup,1000,[]() { }); //this one is to prevent the BrewCup release to trigger
  }

  //EXECUTE_IF_COMPONENT_EXIST(mySenseo,BuzzerComponent,playMelody("beep"));

  Homie.getLogger() << endl << "☕☕☕☕ Enjoy your SenseoWifi ☕☕☕☕" << endl << endl;

  //Do we really need that? The Fsm should properly send the information on it's first update
  //senseoNode.setProperty("opState").send(UnknownState::s_StateName);
  CupComponent * cupComponent = mySenseo.getComponent<CupComponent>();
  if (cupComponent != nullptr) 
  {
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
void loopHandler() 
{
  /**
  * Update the low level LED state machine based on the measured LED timings.
  * (off, slow blinking, fast blinking, on)
  */
  mySenseoLed.updateState();

  if (myInputbuttons) myInputbuttons->update();

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
  if (cupComponent != nullptr) 
  {
    //myCup.updateState();
    if (cupComponent->isAvailableChanged()) 
    {
      senseoNode.setProperty("cupAvailable").send(cupComponent->isAvailable() ? "true" : "false");
    }
    if (cupComponent->isFullChanged()) 
    {
      senseoNode.setProperty("cupFull").send(cupComponent->isFull() ? "true" : "false");
    }
  }  
}

void setup() 
{
  Serial.begin(115200);

  /**
  * Wemos D1 mini pin initializations
  */

  //TODO: move those initialization in their respective component
  //pinMode(ocSenseLedPin, INPUT_PULLUP);

  // it seems at this point Homie configuration variable are not set
  /*if (CupDetectorAvailableSetting.get()) 
  {
    pinMode(cupDetectorPin, INPUT_PULLUP); 
  }*/

  // pinMode(senseoButtonsInputPin, INPUT);  
  // while (true) 
  // {
  //   Serial.print("A0 = ");
  //   Serial.println(analogRead(senseoButtonsInputPin));
  //   delay(50);
  // }



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
  Homie_setFirmware("senseo-wifi", "2.0");
  Homie_setBrand("SenseoWifi");
  //Homie.disableResetTrigger();

  //setResetTrigger need to be call before the setup and therefor can't be based on the configuration variable
  //TODO find a way to auto detect that without the use of configuration variable
  if (false)
  {
    pinMode(resetButtonPin, INPUT_PULLUP);
    Homie.setResetTrigger(resetButtonPin, LOW, 5000);
  }

  Homie.disableLedFeedback();
  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  /**
  * Homie: Options, see at the top of this file.
  */
  CupDetectorAvailableSetting.setDefaultValue(true);
  BuzzerSetting.setDefaultValue(false);
  PublishHomeAssistantDiscoveryConfig.setDefaultValue(false);
  UseCustomizableButtonsAddon.setDefaultValue(false);

  //Nodes apparently need to be configured before setup
  //Unfortunately the config is only available after the setup so we'll end up configuring useless properties
  senseoNode.advertise("buzzer").setName("Buzzer").settable(buzzerHandler).setDatatype("enum").setFormat(BuzzerComponent::getValidTunes());
  senseoNode.advertise("debug").setName("Debugging Information").setDatatype("string").setRetained(false);
  senseoNode.advertise("pendingCommands").setName("Current Commands (debug)").setDatatype("string").setRetained(false);
  senseoNode.advertise("processedCommands").setName("Current Commands (debug)").setDatatype("string").setRetained(false);
  senseoNode.advertise("opState").setName("Operational State").setDatatype("enum").setFormat("SENSEO_unknown,SENSEO_OFF,SENSEO_HEATING,SENSEO_READY,SENSEO_BREWING,SENSEO_NOWATER");
  senseoNode.advertise("ledState").setName("Led State").setDatatype("enum").setFormat("LED_unknown,LED_OFF,LED_SLOW,LED_FAST,LED_ON");
  senseoNode.advertise("power").setName("Power").setDatatype("boolean").settable(powerHandler);
  senseoNode.advertise("brew").setName("Brew").settable(brewHandler).setDatatype("enum").setFormat("1cup,2cup");
  senseoNode.advertise("brewedSize").setName("Brew Size").setDatatype("string").setRetained(false);
  senseoNode.advertise("outOfWater").setName("Out of Water").setDatatype("boolean");
  senseoNode.advertise("cupAvailable").setName("Cup Available");
  senseoNode.advertise("cupFull").setName("Cup Full");
  senseoNode.advertise("program1Cup").setName("Program1Cup").setDatatype("boolean").settable(program1CupHandler);
  senseoNode.advertise("program2Cup").setName("Program2Cup").setDatatype("boolean").settable(program2CupHandler);

  Homie.onEvent(onHomieEvent);
  Homie.setup();
}

void loop() 
{
    Homie.loop();
}
