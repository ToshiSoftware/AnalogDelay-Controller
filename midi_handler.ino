// MIDI handler

#include "ESP32C3_PedalCtrl.h"
#include "myUtil.h"
#include "mySPI.h"
#include "web_server.h"
#include "ui_handler.h"


int HandleMidi(void){
    int isChanged=false;
    //MIDI
    if (MIDI.read())                // Is there a MIDI message incoming ?
    {
        switch(MIDI.getType())      // Get the type of the message we caught
        {
            case midi::ProgramChange:       // If it is a Program Change (0xCn),
                // ch
                if(MIDI.getChannel()==appParam.midi_ch)
                {
                    int receivedPresetno = MIDI.getData1();
                    if(receivedPresetno>=0 && receivedPresetno<10){
                        appParam.presetNo = receivedPresetno;
                        // 
                        LoadPresetToCurrentParam(appParam.presetNo);
                        //
                        isChanged = true;
                    }
                }

                break;
            // See the online reference for other message types
            default:
                break;
        }
    }
    return isChanged;
}
