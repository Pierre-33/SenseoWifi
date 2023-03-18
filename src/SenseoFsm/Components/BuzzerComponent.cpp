#include "BuzzerComponent.h"
#include <ezBuzzer.h>
#include "MusicLibrary.h"
#include <Homie.h>

#define REST 0

String BuzzerComponent::getValidTunes() const
{
    String validTunes;
    for (const auto & item : musicLibrary)
    {
        if (!validTunes.isEmpty()) validTunes += ", ";
        validTunes += item.first;
    }

    return validTunes;
}

bool BuzzerComponent::playMelody(const String & tune) 
{
    Homie.getLogger() << "Playing " << tune << endl;
    auto iter = musicLibrary.find(tune);
    if (iter != musicLibrary.end())
    {
        assert(iter->second.notes.size() == iter->second.durations.size());
        // the buffer should be const in the library
        myBuzzer.playMelody(const_cast<int*>(iter->second.notes.data()), const_cast<int*>(iter->second.durations.data()), iter->second.durations.size());
        return true;
    }
    else
    {
        Homie.getLogger() << "Tune not found. Valid tunes are: " <<  getValidTunes() << endl;
        return false;
    }
}

void BuzzerComponent::update() {
    myBuzzer.loop();
}