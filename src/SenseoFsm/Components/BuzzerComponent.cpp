#include "BuzzerComponent.h"
#include <ezBuzzer.h>

bool BuzzerComponent::buzz(const String & tune) {
    if (tune == "melody1") {
        static int melody1[] = { NOTE_C7 };
        static int noteDurations1[] = { 2 };
        myBuzzer.playMelody(melody1, noteDurations1, sizeof(noteDurations1) / sizeof(int));
        return true;
    }
    if (tune == "melody2") {
        static int melody2[] = { NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5 };
        static int noteDurations2[] = { 4, 8, 8, 2 };
        myBuzzer.playMelody(melody2, noteDurations2, sizeof(noteDurations2) / sizeof(int));
        return true;
    }
    if (tune == "melody3") {
        static int melody3[] = { NOTE_C4, NOTE_C5 };
        static int noteDurations3[] = { 4, 8 };
        myBuzzer.playMelody(melody3, noteDurations3, sizeof(noteDurations3) / sizeof(int));
        return true;
    }
    return false;
}

void BuzzerComponent::update() {
    myBuzzer.loop();
}