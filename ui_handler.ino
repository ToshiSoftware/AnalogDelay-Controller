// My UI handler

#include "ESP32C3_PedalCtrl.h"
#include "myUtil.h"
#include "mySPI.h"
#include "web_server.h"
#include "ui_handler.h"

int GetSsidCurrentCharId()
{
  int i, len;
  char cursorChar;

  len = strlen(appVars.edited_wifi_ssid);
  cursorChar = appVars.edited_wifi_ssid[appVars.ssid_currectPos];

  for(i=0; i<len; i++){
    if(valid_ssid_char[i] == cursorChar){
      return i;
    }
  }
  return 0;
}

int GetPassCurrentCharId()
{
  int i, len;
  char cursorChar;

  len = strlen(appVars.edited_wifi_pass);
  cursorChar = appVars.edited_wifi_pass[appVars.pass_currectPos];

  for(i=0; i<len; i++){
    if(valid_pass_char[i] == cursorChar){
      return i;
    }
  }
  return 0;
}


int UiButtonHandler(void){
  int ret=false;
    if(appVars.mode == MODE_PLAY){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_knob--;
            if(appVars.selected_knob<0) appVars.selected_knob=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_knob++;
            if(appVars.selected_knob>=NUM_OF_KNOB) appVars.selected_knob=NUM_OF_KNOB-1;
            ret=true;
        }
        // if button 'enter' short-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            if(appVars.preset_changed==true){
              appVars.mode = MODE_CONFIRM_WRITE_PRESET;
              appVars.selected_yesno=0;
            }
            ret=true;
       }
        // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // goto mode change
            appVars.mode = MODE_CHANGEMODE;
            ret=true;
        }
        // if button 'foot' pressed
        if(sdi.sw[UI_BUTTON_FOOT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_FOOT].trigger_shortpress=false;
            if(appParam.footsw_mode==FOOTSW_PRESET) {
                appParam.presetNo++;
                if(appParam.presetNo>appParam.preset_max) appParam.presetNo=0;
                //
                LoadPresetToCurrentParam(appParam.presetNo);
            }            
            if(appParam.footsw_mode==FOOTSW_ALTERNATE) {
                appParam.onoff = 1-appParam.onoff;
            }
            ret=true;
        }
    }
    else if(appVars.mode == MODE_SETTINGS){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_settings--;
            if(appVars.selected_settings<0) appVars.selected_settings=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_settings++;
            if(appVars.selected_settings>=NUM_OF_SETTINGS) appVars.selected_settings=NUM_OF_SETTINGS-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appParam.footsw_mode==FOOTSW_PRESET) appParam.onoff = true;
            save_eeprom(); // write to eeprom
            if(appVars.wifi_onoff_changed == true){
              appVars.mode = MODE_CONFIRM_RESET;
              appVars.wifi_onoff_changed = false;
            }
            else{
              appVars.mode = MODE_PLAY;
            }
            ret=true; 
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }
    else if(appVars.mode == MODE_CHANGEMODE){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_mode--;
            if(appVars.selected_mode<0) appVars.selected_mode=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_mode++;
            if(appVars.selected_mode>=NUM_OF_CHANGEMODE) appVars.selected_mode=NUM_OF_CHANGEMODE-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appVars.selected_mode==MODECHANGE_PLAY){
                appVars.mode = MODE_PLAY;
            }
            else if(appVars.selected_mode==MODECHANGE_SETTING)
            {
                appVars.mode = MODE_SETTINGS;
            }
            else if(appVars.selected_mode==MODECHANGE_WIFI)
            {
                appVars.mode = MODE_SHOW_URI;
            }
            else if(appVars.selected_mode==MODECHANGE_SSID)
            {
                appVars.mode = MODE_EDIT_SSID;
                appVars.ssid_currectPos=0;
                // init edit budffer
                memset(appVars.edited_wifi_ssid, ' ', WIFI_SSID_MAX_LEN);
                appVars.edited_wifi_ssid[WIFI_SSID_MAX_LEN]='\0';
                memcpy(appVars.edited_wifi_ssid, appParam.wifi_ssid, strlen(appParam.wifi_ssid));
                appVars.edit_cursor_char_id = GetSsidCurrentCharId();
            }
            else //if(appVars.selected_mode==MODE_EDIT_PASS)
            {
                appVars.mode = MODE_EDIT_PASS;
                appVars.pass_currectPos=0;
                //
                // init edit budffer
                memset(appVars.edited_wifi_pass, ' ', WIFI_PASS_MAX_LEN);
                appVars.edited_wifi_pass[WIFI_PASS_MAX_LEN]='\0';
                memcpy(appVars.edited_wifi_pass, appParam.wifi_pass, strlen(appParam.wifi_pass));
                appVars.edit_cursor_char_id = GetPassCurrentCharId();
             } 
            ret=true;
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }
    else if(appVars.mode == MODE_SHOW_URI){
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            appVars.mode = MODE_PLAY;
            if(appParam.footsw_mode==FOOTSW_PRESET) appParam.onoff = true;
            ret=true;
        }      
    }
    // preset write confirmation
    else if(appVars.mode == MODE_CONFIRM_WRITE_PRESET){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_yesno--;
            if(appVars.selected_yesno<0) appVars.selected_yesno=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_yesno++;
            if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appVars.selected_yesno==CONFIRM_NO){
                appVars.mode = MODE_PLAY;
            }
            else //if(appVars.selected_yesno==CONFIRM_YES)
            {
                // return to play
                appVars.mode = MODE_PLAY; // 
                // write params to preset memory
                presetParams[appParam.presetNo].effect =appParam.effect;
                presetParams[appParam.presetNo].feedback = appParam.feedback;
                presetParams[appParam.presetNo].time = appParam.time;
                appVars.preset_changed=false;

                // write to EEPROM
                save_eeprom();
                // message
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Preset ");
                lcd.print(appParam.presetNo);
                lcd.setCursor(0,1);
                lcd.print("Write completed.");
                delay(2000);
            }
            ret=true;
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }

    // reboot  confirmation
    else if(appVars.mode == MODE_CONFIRM_RESET){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_yesno--;
            if(appVars.selected_yesno<0) appVars.selected_yesno=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_yesno++;
            if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appVars.selected_yesno==CONFIRM_NO){
                appVars.mode = MODE_PLAY;
            }
            else //if(appVars.selected_yesno==CONFIRM_YES)
            {
                // reboot
                ESP.restart();
            }
            ret=true;
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }

    // reboot  confirmation
    else if(appVars.mode == MODE_WIFI_CONNECTING){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_yesno--;
            if(appVars.selected_yesno<0) appVars.selected_yesno=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_yesno++;
            if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appVars.selected_yesno==CONFIRM_NO){
                appVars.mode = MODE_WIFI_CONNECTING;
            }
            else //if(appVars.selected_yesno==CONFIRM_YES)
            {
                // reboot
                appParam.wifi_enable = false;
                save_eeprom();
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("WiFi disabled.");
                lcd.setCursor(0,1);
                lcd.print("Rebooting..");
                delay(2000);
                ESP.restart();
            }
            ret=true;
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }

    // SSID write confirmation
    else if(appVars.mode == MODE_CONFIRM_SSID){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_yesno--;
            if(appVars.selected_yesno<0) appVars.selected_yesno=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_yesno++;
            if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appVars.selected_yesno==CONFIRM_NO){
                appVars.mode = MODE_PLAY;
            }
            else //if(appVars.selected_yesno==CONFIRM_YES)
            {
                // return to play
                appVars.mode = MODE_PLAY; // 
                // write SSID edit buffer to appParam
                // top space replace with termination('\0')
                for(int i=0; i<WIFI_SSID_MAX_LEN; i++){
                  if(appVars.edited_wifi_ssid[i]==' '){
                    appVars.edited_wifi_ssid[i]='\0';
                  }
                  strcpy(appParam.wifi_ssid, appVars.edited_wifi_ssid);
                }

                // write to EEPROM
                if(debug_output_to_html && strlen(debug_message)<LEN_DEBUG_MESSAGE-100){
                  char text[100];
                  sprintf(text, "SSID: [%s]<br />", appParam.wifi_ssid);
                  strcat(debug_message, text);
                }
                save_eeprom();
                // message
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("SSID changed. ");
                lcd.setCursor(0,1);
                lcd.print(appParam.wifi_ssid);
                delay(2000);
            }
            ret=true;
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }

    // PASS write confirmation
    else if(appVars.mode == MODE_CONFIRM_PASS){
        // if button 0 pressed
        if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
            appVars.selected_yesno--;
            if(appVars.selected_yesno<0) appVars.selected_yesno=0;
            ret=true;
        }
        // if button 1 pressed
        if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
            appVars.selected_yesno++;
            if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
            ret=true;
        }
        // if button 'enter' short- pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
            // goto mode0
            if(appVars.selected_yesno==CONFIRM_NO){
                appVars.mode = MODE_PLAY;
            }
            else //if(appVars.selected_yesno==CONFIRM_YES)
            {
                // return to play
                appVars.mode = MODE_PLAY; // 
                // write SSID edit buffer to appParam
                // top space replace with termination('\0')
                for(int i=0; i<WIFI_PASS_MAX_LEN; i++){
                  if(appVars.edited_wifi_pass[i]==' '){
                    appVars.edited_wifi_pass[i]='\0';
                  }
                  strcpy(appParam.wifi_pass, appVars.edited_wifi_pass);
                }

                // write to EEPROM
                if(debug_output_to_html && strlen(debug_message)<LEN_DEBUG_MESSAGE-100){
                  char text[100];
                  sprintf(text, "PASS: [%s]<br />", appParam.wifi_pass);
                  strcat(debug_message, text);
                }
                save_eeprom();
                // message
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("PASS changed. ");
                lcd.setCursor(0,1);
                lcd.print(appParam.wifi_pass);
                delay(2000);
            }
            ret=true;
        }
         // if button 'enter' long-pressed
        if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
            sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
            // do nothing
            ret=true;
        }
    }

    // edit SSID
    else if(appVars.mode == MODE_EDIT_SSID){
      // if button 0 pressed
      if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
          sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
          appVars.ssid_currectPos--;
          if(appVars.ssid_currectPos<0) appVars.ssid_currectPos=0;
          appVars.edit_cursor_char_id = GetSsidCurrentCharId();
          ret=true;
      }
      // if button 1 pressed
      if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
          sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
          appVars.ssid_currectPos++;
          //int ssidlen = strlen(appParam.wifi_ssid);
          if(appVars.ssid_currectPos>=WIFI_SSID_MAX_LEN) appVars.ssid_currectPos=WIFI_SSID_MAX_LEN-1;
          appVars.edit_cursor_char_id = GetSsidCurrentCharId();
          ret=true;
      }
      // if button 'enter' short- pressed
      if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
          sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
          // goto mode0
          appVars.mode = MODE_CONFIRM_SSID;
          appVars.selected_yesno = 0;
          ret=true;
      }
        // if button 'enter' long-pressed
      if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
          sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
          // do nothing
          ret=true;
      }
     }

    // edit PASS
    else if(appVars.mode == MODE_EDIT_PASS){
      // if button 0 pressed
      if(sdi.sw[UI_BUTTON_LEFT].trigger_shortpress==true){
          sdi.sw[UI_BUTTON_LEFT].trigger_shortpress=false;
          appVars.pass_currectPos--;
          if(appVars.pass_currectPos<0) appVars.pass_currectPos=0;
          appVars.edit_cursor_char_id = GetPassCurrentCharId();
          ret=true;
      }
      // if button 1 pressed
      if(sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress==true){
          sdi.sw[UI_BUTTON_RIGHT].trigger_shortpress=false;
          appVars.pass_currectPos++;
          //int passlen = strlen(appParam.wifi_pass);
          if(appVars.pass_currectPos>=WIFI_PASS_MAX_LEN) appVars.pass_currectPos=WIFI_PASS_MAX_LEN-1;
          appVars.edit_cursor_char_id = GetPassCurrentCharId();
          ret=true;
      }
      // if button 'enter' short- pressed
      if(sdi.sw[UI_BUTTON_ENTER].trigger_shortpress==true){
          sdi.sw[UI_BUTTON_ENTER].trigger_shortpress=false;
          // goto mode0
          appVars.mode = MODE_CONFIRM_PASS;
          appVars.selected_yesno = 0;
          ret=true;
      }
        // if button 'enter' long-pressed
      if(sdi.sw[UI_BUTTON_ENTER].trigger_longpress==true){
          sdi.sw[UI_BUTTON_ENTER].trigger_longpress=false;
          // do nothing
          ret=true;
      }
     }

    //
    return ret;
}


int UiEncoderHandler(void){
  int ret=false;
  // mode0: play
  if(appVars.mode == MODE_PLAY){
    // knob param
    switch(appVars.selected_knob){
      case KNOB_EFFECT:
        if(sdi.encoder[UI_ENCODER_PARAM].rotation_dir == ROTATION_PLUS){
          sdi.encoder[UI_ENCODER_PARAM].rotation_dir = ROTATION_STOP; // reset rotation_dir
          appParam.effect++;
          if(appParam.effect>99) appParam.effect=99;
          appVars.preset_changed=true;
          ret=true;
        }
        else if(sdi.encoder[UI_ENCODER_PARAM].rotation_dir == ROTATION_MINUS){
          sdi.encoder[UI_ENCODER_PARAM].rotation_dir = ROTATION_STOP; // reset rotation_dir
          appParam.effect--;
          if(appParam.effect<0) appParam.effect=0;
          appVars.preset_changed=true;
          ret=true;
        }
        break;
      case KNOB_FEEDBACK:
        if(sdi.encoder[UI_ENCODER_PARAM].rotation_dir == ROTATION_PLUS){
          sdi.encoder[UI_ENCODER_PARAM].rotation_dir = ROTATION_STOP; // reset rotation_dir
          appParam.feedback++;
          if(appParam.feedback>99) appParam.feedback=99;
          appVars.preset_changed=true;
          ret=true;
        }
        else if(sdi.encoder[UI_ENCODER_PARAM].rotation_dir == ROTATION_MINUS){
          sdi.encoder[UI_ENCODER_PARAM].rotation_dir = ROTATION_STOP; // reset rotation_dir
          appParam.feedback--;
          if(appParam.feedback<0) appParam.feedback=0;
           appVars.preset_changed=true;
          ret=true;
       }
        break;
      case KNOB_TIME:
        if(sdi.encoder[UI_ENCODER_PARAM].rotation_dir == ROTATION_PLUS){
          sdi.encoder[UI_ENCODER_PARAM].rotation_dir = ROTATION_STOP; // reset rotation_dir
          appParam.time++;
          if(appParam.time>99) appParam.time=99;
          appVars.preset_changed=true;
          appVars.tempo_target_msec = GetTempoTargetDelayTime(appParam.time);
          ret=true;
        }
        else if(sdi.encoder[UI_ENCODER_PARAM].rotation_dir == ROTATION_MINUS){
          sdi.encoder[UI_ENCODER_PARAM].rotation_dir = ROTATION_STOP; // reset rotation_dir
          appParam.time--;
          if(appParam.time<0) appParam.time=0;
          appVars.preset_changed=true;
          appVars.tempo_target_msec = GetTempoTargetDelayTime(appParam.time);
          ret=true;
        }
        break;
      default:
        break;
    }
    // change preset
    if(sdi.encoder[UI_ENCODER_PRESET].rotation_dir == ROTATION_PLUS){
        sdi.encoder[UI_ENCODER_PRESET].rotation_dir = ROTATION_STOP; // reset rotation_dir
        appParam.presetNo++;
        if(appParam.presetNo>=NUM_PRESET) appParam.presetNo=NUM_PRESET-1; 
        appVars.preset_changed=false;
        ret=UI_CHANGE_PRESET;
    }
    else if(sdi.encoder[UI_ENCODER_PRESET].rotation_dir == ROTATION_MINUS){
        sdi.encoder[UI_ENCODER_PRESET].rotation_dir = ROTATION_STOP; // reset rotation_dir
        appParam.presetNo--;
        if(appParam.presetNo<0) appParam.presetNo=0;
        appVars.preset_changed=false;
        ret=UI_CHANGE_PRESET;
    }
    // write selected preset memory to current param
    if(ret == UI_CHANGE_PRESET)
    {
      LoadPresetToCurrentParam(appParam.presetNo);
    }
  }

  // mode1: settings
  else if(appVars.mode == MODE_SETTINGS){
    switch(appVars.selected_settings){
      case SETTING_MIDI_CH:
         if(sdi.encoder[0].rotation_dir == ROTATION_PLUS){
          appParam.midi_ch++;
          if(appParam.midi_ch>16) appParam.midi_ch=16;
          ret=true;
        }
        else if(sdi.encoder[0].rotation_dir == ROTATION_MINUS){
          appParam.midi_ch--;
          if(appParam.midi_ch<1) appParam.midi_ch=1;
          ret=true;
        }       
        break;
      case SETTING_FOOTSW:
         if(sdi.encoder[0].rotation_dir == ROTATION_PLUS){
          appParam.footsw_mode++;
          if(appParam.footsw_mode>1) appParam.footsw_mode=1;
          ret=true;
        }
        else if(sdi.encoder[0].rotation_dir == ROTATION_MINUS){
          appParam.footsw_mode--;
          if(appParam.footsw_mode<0) appParam.footsw_mode=0;
          ret=true;
        }  
        break;     
      case SETTING_WIFI_ONOFF:
         if(sdi.encoder[0].rotation_dir == ROTATION_PLUS){
          appParam.wifi_enable++;
          if(appParam.wifi_enable>1) appParam.wifi_enable=1;
          appVars.wifi_onoff_changed = true;
          ret=true;
        }
        else if(sdi.encoder[0].rotation_dir == ROTATION_MINUS){
          appParam.wifi_enable--;
          if(appParam.wifi_enable<0) appParam.wifi_enable=0;
          appVars.wifi_onoff_changed = true;
          ret=true;
        }       
        break;
      case SETTING_PRESET_MAX:
         if(sdi.encoder[0].rotation_dir == ROTATION_PLUS){
          appParam.preset_max++;
          if(appParam.preset_max>9) appParam.preset_max=9;
          ret=true;
        }
        else if(sdi.encoder[0].rotation_dir == ROTATION_MINUS){
          appParam.preset_max--;
          if(appParam.preset_max<2) appParam.preset_max=2;
          ret=true;
        }       
        break;      
      default:
        break;
    }
  }

  // edit SSID
  else if(appVars.mode == MODE_EDIT_SSID){
    if(sdi.encoder[0].rotation_dir == ROTATION_PLUS){
      appVars.edit_cursor_char_id++;
      int validlen = strlen(VALID_SSID);
      if(appVars.edit_cursor_char_id>=validlen) appVars.edit_cursor_char_id=0;
      appVars.edited_wifi_ssid[appVars.ssid_currectPos] = valid_ssid_char[appVars.edit_cursor_char_id];
      ret=true;
    }
    else if(sdi.encoder[0].rotation_dir == ROTATION_MINUS){
      appVars.edit_cursor_char_id--;
      int validlen = strlen(VALID_SSID);
      if(appVars.edit_cursor_char_id<0) appVars.edit_cursor_char_id=validlen-1;
      appVars.edited_wifi_ssid[appVars.ssid_currectPos] = valid_ssid_char[appVars.edit_cursor_char_id];
      ret=true;
    }       
  }

  // edit PASS
  else if(appVars.mode == MODE_EDIT_PASS){
    if(sdi.encoder[0].rotation_dir == ROTATION_PLUS){
      appVars.edit_cursor_char_id++;
      int validlen = strlen(VALID_PASS);
      if(appVars.edit_cursor_char_id>=validlen) appVars.edit_cursor_char_id=0;
      appVars.edited_wifi_pass[appVars.pass_currectPos] = valid_pass_char[appVars.edit_cursor_char_id];
      ret=true;
    }
    else if(sdi.encoder[0].rotation_dir == ROTATION_MINUS){
      appVars.edit_cursor_char_id--;
      int validlen = strlen(VALID_PASS);
      if(appVars.edit_cursor_char_id<0) appVars.edit_cursor_char_id=validlen-1;
      appVars.edited_wifi_pass[appVars.pass_currectPos] = valid_pass_char[appVars.edit_cursor_char_id];
      ret=true;
    }       
  }

  //
  return ret;
}

