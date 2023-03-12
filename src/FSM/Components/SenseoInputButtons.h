/*
  SenseoInputButtons.h - Library for the SenseoWifi project.
  Released under some license.
*/
#ifndef SenseoInputButtons_h
#define SenseoInputButtons_h

#include "Homie.h"
#include "enums.h"
#include "constants.h"
#include <set>
#include <vector>
#include "FSM/FsmComponent.h"

/**
 * SenseoInputButtons is use to associate callback with Senseo Buttons
 */
class SenseoInputButtons : public  FsmComponent<SenseoInputButtons>
{
  public:
    typedef std::function<void ()> ButtonHandler;

    SenseoInputButtons(int analogPin);

    void addButtonPushHandler(int buttonValue,const ButtonHandler &handler);
    void addButtonHoldHandler(int buttonValue,unsigned long time,const ButtonHandler &handler);
    void addButtonReleaseHandler(int buttonValue,unsigned long time,const ButtonHandler &handler);

    void update();

    void test() { Homie.getLogger() << "SenseoInputButtons ClassId = " << getClassId() << endl; }

  private:
    enum HandlerType {
      ON_PUSH,
      ON_HOLD,
      ON_RELEASE,
    };

    struct HandlerData
    {
      HandlerType type;
      int buttonValue;
      ButtonHandler handler;
      unsigned long time = 0; //use for Hold and Release Handler
    };
    bool hasHandler(int reading);
    unsigned long lastReadingTime = 0;
    unsigned long lastReadingChangeTime = 0;
    int analogPin;
    int previousReading = 0;

    std::set<int> handlersByValue;
    std::vector<HandlerData> handlers;
};

#endif
