
// LCD display
void UpdateEntireLcdDisplay(void){
    // erace entire area
    lcd.clear();    
    // Mode 0: play
    if(appVars.mode==MODE_PLAY){
        if(appVars.selected_settings>=NUM_OF_KNOB) appVars.selected_settings=NUM_OF_KNOB-1;
        // name
        lcd.noBlink();
        lcd.setCursor(0, 0);
#ifdef PT2399
        lcd.print("EFC FBK TIM msec");
#else
        lcd.print("EFC FBK TIME(ms)");
#endif
        // params
        PrintInt2Digit(appParam.effect, 1,1);
        PrintInt2Digit(appParam.feedback, 5,1);
#ifdef PT2399
        PrintInt2Digit(appParam.time, 9,1);
        PrintInt3Digit(appVars.tempo_target_msec, 13,1);
#else
        PrintInt3Digit(appParam.time, 9,1);
        // PrintInt3Digit(GetBbdDelayActualStep(appParam.time), 13, 1); // show AD8304 step data
#endif

        // '>' cursor 
        switch(appVars.selected_knob){
            case KNOB_EFFECT:
                lcd.setCursor(0,1);
                break;
            case KNOB_FEEDBACK:
                lcd.setCursor(4,1);
                break;        
            case KNOB_TIME:
                lcd.setCursor(8,1);
                break;
            default:
                break;        
        }
        lcd.print(">");
    }
    // Mode 1: settings
    if(appVars.mode==MODE_SETTINGS){
        if(appVars.selected_settings>=NUM_OF_SETTINGS) appVars.selected_settings=NUM_OF_SETTINGS-1;
        // name
        lcd.noBlink();
        lcd.setCursor(0, 0);
        lcd.print("SET");

        // params
        lcd.setCursor(5,0);
        lcd.print("MD:");
        lcd.setCursor(12,0);
        lcd.print("PM:");
        lcd.setCursor(1,1);
        lcd.print("WF:");
        lcd.setCursor(9,1);
        lcd.print("SW:");
        
        // midi ch
        PrintInt2Digit(appParam.midi_ch, 8,0);

        // preset max
        lcd.setCursor(15,0);
        lcd.print(appParam.preset_max);

        // WiFi
        lcd.setCursor(4,1);
        switch(appParam.wifi_enable){
            case true:
                lcd.print("On ");
                break;
            case false:
                lcd.print("Off");
                break;                
            default:
                break;        
        }


        // footsw mode
        lcd.setCursor(12,1);
        switch(appParam.footsw_mode){
            case FOOTSW_PRESET:
                lcd.print("Pst");
                break;
            case FOOTSW_ALTERNATE:
                lcd.print("Alt");
                break;                
            default:
                break;        
        }

        // '>' cursor 
        switch(appVars.selected_settings){
            case SETTING_MIDI_CH:
                lcd.setCursor(4,0);
                break;
            case SETTING_PRESET_MAX:
                lcd.setCursor(11,0);
                break;        
            case SETTING_WIFI_ONOFF:
                lcd.setCursor(0,1);
                break;        
            case SETTING_FOOTSW:
                lcd.setCursor(8,1);
                break;        
             default:
                break;        
        }
        lcd.print(">");
    }
    // Mode 2: change mode
    if(appVars.mode==MODE_CHANGEMODE){
        if(appVars.selected_mode>=NUM_OF_CHANGEMODE) appVars.selected_mode=NUM_OF_CHANGEMODE-1;
        // name
        lcd.noBlink();
        lcd.setCursor(0,0);
        lcd.print("Mode:  PLAY  SET");
        lcd.setCursor(0,1);
        // params
        lcd.print(" URI  SSID  PASS");


        // '>' cursor 
        switch(appVars.selected_mode){
            case MODECHANGE_PLAY:
                lcd.setCursor(6,0);
                break;
            case MODECHANGE_SETTING:
                lcd.setCursor(12,0);
                break;        
            case MODECHANGE_WIFI:
                lcd.setCursor(0,1);
                break; 
            case MODECHANGE_SSID:
                lcd.setCursor(5,1);
                break; 
            case MODECHANGE_PASS:
                lcd.setCursor(11,1);
                break;
            default:
                break;        
        }
        lcd.print(">");
    }
    // Mode 3: wifi info
    if(appVars.mode==MODE_SHOW_URI){
        lcd.noBlink();
        // name
        if(appVars.available_wifi==true){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("http://");
            lcd.print(MDNS_STR);
            lcd.print(".local");
            // params
            lcd.setCursor(0,1);
            lcd.print(WiFi.localIP());
        }
        else{
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Wifi disabled.");
            // params
            lcd.setCursor(0,1);
            lcd.print("(No connection)");
        }
    }
    // mode 4: preset wrirte confirmation
    if(appVars.mode==MODE_CONFIRM_WRITE_PRESET
    || appVars.mode==MODE_CONFIRM_SSID
    || appVars.mode==MODE_CONFIRM_PASS
    || appVars.mode==MODE_CONFIRM_RESET ){
        if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
        // name
        lcd.noBlink();
        lcd.setCursor(0, 0);
        if(appVars.mode==MODE_CONFIRM_WRITE_PRESET){
            lcd.print("Write preset ");
            lcd.print(appParam.presetNo);
            lcd.print(" ?");
        }
        else if(appVars.mode==MODE_CONFIRM_SSID){
            lcd.print("Change SSID?");
        }
        else if(appVars.mode==MODE_CONFIRM_PASS)
        {
            lcd.print("Change PASS?");
        }
        else if(appVars.mode==MODE_CONFIRM_RESET)
        {
            lcd.print("Reboot pedal?");
        }        
     
        // params
        lcd.setCursor(3,1);
        lcd.print("No");
        lcd.setCursor(11,1);
        lcd.print("Yes");

        // '>' cursor 
        switch(appVars.selected_yesno){
            case CONFIRM_NO:
                lcd.setCursor(2,1);
                break;
            case CONFIRM_YES:
                lcd.setCursor(10,1);
                break;        
            default:
                break;        
        }
        lcd.print(">");
    }

    // Wifi connection mode
    if(appVars.mode==MODE_WIFI_CONNECTING){
        if(appVars.selected_yesno>=NUM_OF_CONFIRM) appVars.selected_yesno=NUM_OF_CONFIRM-1;
        // name
        lcd.noBlink();
        lcd.setCursor(0, 0);
        lcd.print("WiFi operating..");

        // params
        lcd.setCursor(2,1);
        lcd.print("Stay");
        lcd.setCursor(9,1);
        lcd.print("Reboot");

        // '>' cursor 
        switch(appVars.selected_yesno){
            case CONFIRM_NO:
                lcd.setCursor(1,1);
                break;
            case CONFIRM_YES:
                lcd.setCursor(8,1);
                break;        
            default:
                break;        
        }
        lcd.print(">");
    }

    // edit SSID
    if(appVars.mode==MODE_EDIT_SSID){
        char tmpstr[17];
        // name
        lcd.blink();
        lcd.setCursor(0, 0);
        lcd.print("Edit SSID ");
        sprintf( tmpstr, "%2d/%2d", appVars.ssid_currectPos+1, WIFI_SSID_MAX_LEN);
        lcd.setCursor(11, 0);
        lcd.print(tmpstr);
        // edit
        int cursor_disp_pos, disp_start_pos;
        if(appVars.ssid_currectPos<16){
            cursor_disp_pos = appVars.ssid_currectPos;
            disp_start_pos = 0;
        }
        else{
            cursor_disp_pos = 15;
            disp_start_pos = appVars.ssid_currectPos-15;
        }
        // display scope
        char tmpDispSsid[33];
        strcpy(tmpDispSsid, &(appVars.edited_wifi_ssid[disp_start_pos]));
        if(strlen(tmpDispSsid)>16) tmpDispSsid[16]='\0';
        lcd.setCursor(0,1);
        lcd.print(tmpDispSsid);
        lcd.setCursor(cursor_disp_pos, 1);
    }
    // edit PASSPHRASE
    if(appVars.mode==MODE_EDIT_PASS){
        char tmpstr[17];
        // name
        lcd.blink();
        lcd.setCursor(0, 0);
        lcd.print("Edit PASS ");
        sprintf( tmpstr, "%2d/%2d", appVars.pass_currectPos+1, WIFI_PASS_MAX_LEN);
        lcd.setCursor(11, 0);
        lcd.print(tmpstr);
        // edit
        int cursor_disp_pos, disp_start_pos;
        if(appVars.pass_currectPos<16){
            cursor_disp_pos = appVars.pass_currectPos;
            disp_start_pos = 0;
        }
        else{
            cursor_disp_pos = 15;
            disp_start_pos = appVars.pass_currectPos-15;
        }
        // display scope
        char tmpDispPass[64];
        strcpy(tmpDispPass, &(appVars.edited_wifi_pass[disp_start_pos]));
        if(strlen(tmpDispPass)>16) tmpDispPass[16]='\0';
        lcd.setCursor(0,1);
        lcd.print(tmpDispPass);
        lcd.setCursor(cursor_disp_pos, 1);
    }
}
