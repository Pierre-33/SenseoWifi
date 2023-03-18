#pragma once
#include <map>
#include <ezBuzzer.h>
#include <vector>
#define REST 0

//most of the melody came from : https://github.com/hibit-dev/buzzer

struct BuzzerMelody
{
    std::vector<int> notes;
    std::vector<int> durations;
};

static inline const std::map<String, const BuzzerMelody> musicLibrary = {
    { "beep", {
        { NOTE_C7 },
        { 2 }
    }},
    { "melody2", {
        { NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5 } ,
        { 4, 8, 8, 2 } 
    }},
    { "melody3", {
        { NOTE_C4, NOTE_C5 } ,
        { 4, 8 } 
    }},
    { "cupready", { 
        { NOTE_A4, NOTE_AS4, NOTE_B4, NOTE_C5 }, 
        { 8, 8, 8,2 }
    }},
    
    /*{ "potc", { 
        { 
            NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
            NOTE_A4, NOTE_G4, NOTE_A4, REST,
            
            NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
            NOTE_A4, NOTE_G4, NOTE_A4, REST,
            
            NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
            NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
            NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
            NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,
            
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_D5, NOTE_E5, NOTE_A4, REST,
            NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
            NOTE_C5, NOTE_A4, NOTE_B4, REST,
            
            NOTE_A4, NOTE_A4,
            //Repeat of first part
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
            NOTE_A4, NOTE_G4, NOTE_A4, REST,
            
            NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
            NOTE_A4, NOTE_G4, NOTE_A4, REST,
            
            NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
            NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
            NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
            NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,
            
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_D5, NOTE_E5, NOTE_A4, REST,
            NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
            NOTE_C5, NOTE_A4, NOTE_B4, REST,
            //End of Repeat
            
            NOTE_E5, REST, REST, NOTE_F5, REST, REST,
            NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
            NOTE_D5, REST, REST, NOTE_C5, REST, REST,
            NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4,
            
            NOTE_E5, REST, REST, NOTE_F5, REST, REST,
            NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
            NOTE_D5, REST, REST, NOTE_C5, REST, REST,
            NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4 
        }, 
        { 
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8,
            
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8,
            
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 8, 4, 8,
            
            8, 8, 4, 8, 8,
            4, 8, 4, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 4,
            
            4, 8,
            //Repeat of First Part
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8,
            
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8,
            
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 8, 4, 8,
            
            8, 8, 4, 8, 8,
            4, 8, 4, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 4,
            //End of Repeat
            
            4, 8, 4, 4, 8, 4,
            8, 8, 8, 8, 8, 8, 8, 8, 4,
            4, 8, 4, 4, 8, 4,
            8, 8, 8, 8, 8, 2,
            
            4, 8, 4, 4, 8, 4,
            8, 8, 8, 8, 8, 8, 8, 8, 4,
            4, 8, 4, 4, 8, 4,
            8, 8, 8, 8, 8, 2
        }
    }},
    { "harry", {
        {
            REST, NOTE_D4,
            NOTE_G4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_D5,
            NOTE_C5, 
            NOTE_A4,
            NOTE_G4, NOTE_AS4, NOTE_A4,
            NOTE_F4, NOTE_GS4,
            NOTE_D4, 
            NOTE_D4,
            
            NOTE_G4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_D5,
            NOTE_F5, NOTE_E5,
            NOTE_DS5, NOTE_B4,
            NOTE_DS5, NOTE_D5, NOTE_CS5,
            NOTE_CS4, NOTE_B4,
            NOTE_G4,
            NOTE_AS4,
            
            NOTE_D5, NOTE_AS4,
            NOTE_D5, NOTE_AS4,
            NOTE_DS5, NOTE_D5,
            NOTE_CS5, NOTE_A4,
            NOTE_AS4, NOTE_D5, NOTE_CS5,
            NOTE_CS4, NOTE_D4,
            NOTE_D5, 
            REST, NOTE_AS4,  
            
            NOTE_D5, NOTE_AS4,
            NOTE_D5, NOTE_AS4,
            NOTE_F5, NOTE_E5,
            NOTE_DS5, NOTE_B4,
            NOTE_DS5, NOTE_D5, NOTE_CS5,
            NOTE_CS4, NOTE_AS4,
            NOTE_G4
        },
        {
            2, 4,
            4, 8, 4,
            2, 4,
            2, 
            2,
            4, 8, 4,
            2, 4,
            1, 
            4,
            
            4, 8, 4,
            2, 4,
            2, 4,
            2, 4,
            4, 8, 4,
            2, 4,
            1,
            4,
            
            2, 4,
            2, 4,
            2, 4,
            2, 4,
            4, 8, 4,
            2, 4,
            1, 
            4, 4,  
            
            2, 4,
            2, 4,
            2, 4,
            2, 4,
            4, 8, 4,
            2, 4,
            1
        }
    }},
    { "mario", {
        {
            NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
            NOTE_G5, REST, NOTE_G4, REST, 
            NOTE_C5, NOTE_G4, REST, NOTE_E4,
            NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
            REST, NOTE_E5,NOTE_C5, NOTE_D5, NOTE_B4,
            NOTE_C5, NOTE_G4, REST, NOTE_E4,
            NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
            REST, NOTE_E5,NOTE_C5, NOTE_D5, NOTE_B4,
            
            REST, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5,
            REST, NOTE_GS4, NOTE_A4, NOTE_C4, REST, NOTE_A4, NOTE_C5, NOTE_D5,
            REST, NOTE_DS5, REST, NOTE_D5,
            NOTE_C5, REST,
            
            REST, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5,
            REST, NOTE_GS4, NOTE_A4, NOTE_C4, REST, NOTE_A4, NOTE_C5, NOTE_D5,
            REST, NOTE_DS5, REST, NOTE_D5,
            NOTE_C5, REST,
            
            NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5,
            NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
            
            NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5, NOTE_E5,
            REST, 
            NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5,
            NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
            NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
            NOTE_G5, REST, NOTE_G4, REST, 
            NOTE_C5, NOTE_G4, REST, NOTE_E4,
            
            NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
            REST, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,
            
            NOTE_C5, NOTE_G4, REST, NOTE_E4,
            NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
            REST, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,
            
            NOTE_E5, NOTE_C5, NOTE_G4, REST, NOTE_GS4,
            NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4,
            NOTE_D5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5,
            
            NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
            NOTE_E5, NOTE_C5, NOTE_G4, REST, NOTE_GS4,
            NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4,
            NOTE_B4, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5,
            NOTE_C5, NOTE_E4, NOTE_E4, NOTE_C4,
            
            NOTE_E5, NOTE_C5, NOTE_G4, REST, NOTE_GS4,
            NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4,
            NOTE_D5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5,
            
            NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
            NOTE_E5, NOTE_C5, NOTE_G4, REST, NOTE_GS4,
            NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4,
            NOTE_B4, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5,
            NOTE_C5, NOTE_E4, NOTE_E4, NOTE_C4,
            NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5, NOTE_E5,
            REST,
            
            NOTE_C5, NOTE_C5, NOTE_C5, REST, NOTE_C5, NOTE_D5,
            NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
            NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
            NOTE_G5, REST, NOTE_G4, REST, 
            NOTE_E5, NOTE_C5, NOTE_G4, REST, NOTE_GS4,
            NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4,
            NOTE_D5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5,
            
            NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4,
            NOTE_E5, NOTE_C5, NOTE_G4, REST, NOTE_GS4,
            NOTE_A4, NOTE_F5, NOTE_F5, NOTE_A4,
            NOTE_B4, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5,
            NOTE_C5, NOTE_E4, NOTE_E4, NOTE_C4,
            
            // Game over sound
            NOTE_C5, NOTE_G4, NOTE_E4,
            NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_AS4, NOTE_GS4,
            NOTE_G4, NOTE_D4, NOTE_E4
        },
        {
            8, 8, 8, 8, 8, 8, 8,
            4, 4, 8, 4, 
            4, 8, 4, 4,
            4, 4, 8, 4,
            8, 8, 8, 4, 8, 8,
            8, 4,8, 8, 4,
            4, 8, 4, 4,
            4, 4, 8, 4,
            8, 8, 8, 4, 8, 8,
            8, 4,8, 8, 4,
            
            
            4, 8, 8, 8, 4, 8,
            8, 8, 8, 8, 8, 8, 8, 8,
            4, 4, 8, 4,
            2, 2,
            
            4, 8, 8, 8, 4, 8,
            8, 8, 8, 8, 8, 8, 8, 8,
            4, 4, 8, 4,
            2, 2,
            
            8, 4, 8, 8, 8, 4,
            8, 4, 8, 2,
            
            8, 4, 8, 8, 8, 8, 8,
            1, 
            8, 4, 8, 8, 8, 4,
            8, 4, 8, 2,
            8, 8, 8, 8, 8, 8, 4,
            4, 4, 4, 4, 
            4, 8, 4, 4,
            
            4, 4, 8, 4,
            8, 8, 8, 4, 8, 8,
            8, 4, 8, 8, 4,
            
            4, 8, 4, 4,
            4, 4, 8, 4,
            8, 8, 8, 4, 8, 8,
            8, 4, 8, 8, 4,
            
            8, 4, 8, 4, 4,
            8, 4, 8, 2,
            8, 8, 8, 8, 8, 8,
            
            8, 4, 8, 2,
            8, 4, 8, 4, 4,
            8, 4, 8, 2,
            8, 4, 8, 8, 8, 8,
            8, 4, 8, 2,
            
            8, 4, 8, 4, 4,
            8, 4, 8, 2,
            8, 8, 8, 8, 8, 8,
            
            8, 4, 8, 2,
            8, 4, 8, 4, 4,
            8, 4, 8, 2,
            8, 4, 8, 8, 8, 8,
            8, 4, 8, 2,
            8, 4, 8, 8, 8, 8, 8,
            1,
            
            8, 4, 8, 8, 8, 4,
            8, 4, 8, 2,
            8, 8, 8, 8, 8, 8, 4,
            4, 4, 4, 4, 
            8, 4, 8, 4, 4,
            8, 4, 8, 2,
            8, 8, 8, 8, 8, 8,
            
            8, 4, 8, 2,
            8, 4, 8, 4, 4,
            8, 4, 8, 2,
            8, 4, 8, 8, 8, 8,
            8, 4, 8, 2,
            
            //game over sound
            4, 4, 4,
            8, 8, 8, 8, 8, 8,
            8, 8, 2
        }
    }},
    { "tetris", {
        {
            NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
            NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
            NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
            NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,
            
            NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
            NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
            NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
            NOTE_C5, NOTE_A4, NOTE_A4, REST, 
            
            NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
            NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
            NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
            NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,
            
            NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
            NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
            NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
            NOTE_C5, NOTE_A4, NOTE_A4, REST, 
            
            NOTE_E5, NOTE_C5,
            NOTE_D5, NOTE_B4,
            NOTE_C5, NOTE_A4,
            NOTE_GS4, NOTE_B4, REST, 
            NOTE_E5, NOTE_C5,
            NOTE_D5, NOTE_B4,
            NOTE_C5, NOTE_E5, NOTE_A5,
            NOTE_GS5
        },
        {
            4, 8, 8, 4, 8, 8,
            4, 8, 8, 4, 8, 8,
            4, 8, 4, 4,
            4, 4, 8, 4, 8, 8,
            
            4, 8, 4, 8, 8,
            4, 8, 4, 8, 8,
            4, 8, 8, 4, 4,
            4, 4, 4, 4,
            
            4, 8, 8, 4, 8, 8,
            4, 8, 8, 4, 8, 8,
            4, 8, 4, 4,
            4, 4, 8, 4, 8, 8,
            
            4, 8, 4, 8, 8,
            4, 8, 4, 8, 8,
            4, 8, 8, 4, 4,
            4, 4, 4, 4,
            
            2, 2,
            2, 2,
            2, 2,
            2, 4, 8, 
            2, 2,
            2, 2,
            4, 4, 2,
            2
        }
    }},
    { "got", {
        {
            NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
            NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
            NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
            NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4,
            NOTE_G4, NOTE_C4,
            
            NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
            NOTE_D4,
            NOTE_F4, NOTE_AS3,
            NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
            NOTE_DS4, NOTE_D4, NOTE_C4,
            
            NOTE_G4, NOTE_C4,
            
            NOTE_DS4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4,
            NOTE_D4,
            NOTE_F4, NOTE_AS3,
            NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3,
            NOTE_DS4, NOTE_D4, NOTE_C4,
            NOTE_G4, NOTE_C4,
            NOTE_DS4, NOTE_F4, NOTE_G4,  NOTE_C4, NOTE_DS4, NOTE_F4,
            
            NOTE_D4,
            NOTE_F4, NOTE_AS3,
            NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_AS3,
            NOTE_C4,
            NOTE_C5,
            NOTE_AS4,
            NOTE_C4,
            NOTE_G4,
            NOTE_DS4,
            NOTE_DS4, NOTE_F4,
            NOTE_G4,
            
            NOTE_C5,
            NOTE_AS4,
            NOTE_C4,
            NOTE_G4,
            NOTE_DS4,
            NOTE_DS4, NOTE_D4,
            NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
            NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_GS4, NOTE_AS4,
            
            REST, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5,
            NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_G5, NOTE_GS5, NOTE_AS5
        },
        {
            8, 8, 16, 16, 8, 8, 16, 16,
            8, 8, 16, 16, 8, 8, 16, 16,
            8, 8, 16, 16, 8, 8, 16, 16,
            8, 8, 16, 16, 8, 8, 16, 16,
            4, 4,
            
            16, 16, 4, 4, 16, 16,
            1,
            4, 4,
            16, 16, 4, 4,
            16, 16, 1,
            
            4, 4,
            
            16, 16, 4, 4, 16, 16,
            1,
            4, 4,
            16, 16, 4, 4,
            16, 16, 1,
            4, 4,
            16, 16, 4, 4, 16, 16,
            
            2,
            4, 4,
            8, 8, 8, 8,
            1,
            2,
            2,
            2,
            2,
            2,
            4, 4,
            1,
            
            2,
            2,
            2,
            2,
            2,
            4, 4,
            8, 8, 16, 16, 8, 8, 16, 16,
            8, 8, 16, 16, 8, 8, 16, 16,
            
            4, 16, 16, 8, 8, 16, 16,
            8, 16, 16, 16, 8, 8, 16, 16
        }
    }},
    { "sw", {
        {
            NOTE_AS4, NOTE_AS4, NOTE_AS4,
            NOTE_F5, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
            NOTE_F5, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

            NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
            NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
            NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
            NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

            NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
            NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
            NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
            NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
            NOTE_C6
        },
        {
            8, 8, 8,
            2, 2,
            8, 8, 8, 2, 4,
            8, 8, 8, 2, 4,
            8, 8, 8, 2, 8, 8, 8,
            2, 2,
            8, 8, 8, 2, 4,

            8, 8, 8, 2, 4,
            8, 8, 8, 2, 8, 16,
            4, 8, 8, 8, 8, 8,
            8, 8, 8, 4, 8, 4, 8, 16,
            4, 8, 8, 8, 8, 8,

            8, 16, 2, 8, 8,
            4, 8, 8, 8, 8, 8,
            8, 8, 8, 4, 8, 4, 8, 16,
            4, 8, 4, 8, 4, 8, 4, 8,
            1
        }
    }},
    { "pinkpanther", {
        {
            REST, REST, REST, NOTE_DS4, 
            NOTE_E4, REST, NOTE_FS4, NOTE_G4, REST, NOTE_DS4,
            NOTE_E4, NOTE_FS4,  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_B4,   
            NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, 
            NOTE_E4, REST, REST, NOTE_DS4,
            
            NOTE_E4, REST, NOTE_FS4, NOTE_G4, REST, NOTE_DS4,
            NOTE_E4, NOTE_FS4,  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_B4, NOTE_E5,
            NOTE_DS5,   
            NOTE_D5, REST, REST, NOTE_DS4, 
            NOTE_E4, REST, NOTE_FS4, NOTE_G4, REST, NOTE_DS4,
            NOTE_E4, NOTE_FS4,  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_B4,   
            
            NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, 
            NOTE_E4, REST,
            REST, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4,
            NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4,   
            NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4
        },
        {
            2, 4, 8, 8, 
            4, 8, 8, 4, 8, 8,
            8, 8,  8, 8, 8, 8, 8, 8,   
            2, 16, 16, 16, 16, 
            2, 4, 8, 4,
            
            4, 8, 8, 4, 8, 8,
            8, 8,  8, 8, 8, 8, 8, 8,
            1,   
            2, 4, 8, 8, 
            4, 8, 8, 4, 8, 8,
            8, 8,  8, 8, 8, 8, 8, 8,   
            
            2, 16, 16, 16, 16, 
            4, 4,
            4, 8, 8, 8, 8, 8, 8,
            16, 8, 16, 8, 16, 8, 16, 8,   
            16, 16, 16, 16, 16, 2 
        }
    }}
*/
};