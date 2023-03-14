#pragma once
#include "ModularFsm/ModularFsm.h"

class SenseoLed;
class HomieNode;

class SenseoFsm : public ModularFsm
{
    public:
        void setup(const SenseoLed & led, HomieNode & node,bool useCupDetector, bool useBuzzer, bool useCustomizableButton);
        bool isOff();
};