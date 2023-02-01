#pragma once

// World clock
#define JST     3600*9

// WiFi SSID&PW
#ifndef STASSID
#define STASSID "ssid"
#define STAPSK  "passphrase"
#endif
#define WIFI_CONNECTION_RETRY 10

// DNSm name
#define MDNS_STR "nld"

// EEPROM 
#define SIGNATURE "SGNT102"

// digital pins
#define LED1_PIN D3
#define SW1 D2
#define SW2 D3

// Def: HwTimer
#define PRESCALER_DIVIDE_RATE 80 // devide to 1 usec cycle
#define TIMER_UI_INTERVAL 1000 // callback every specified usec 
#define TIMER_TEMPO_INTERVAL 1000 // callback every specified usec 

// Def: UI-HW
#define UI_BUTTON_LEFT  0
#define UI_BUTTON_RIGHT 1
#define UI_BUTTON_ENTER 2
#define UI_BUTTON_FOOT  3

#define UI_ENCODER_PARAM 0
#define UI_ENCODER_PRESET 1

// app mode
#define NUM_OF_MODE 11
#define MODE_PLAY 0
#define MODE_SETTINGS 1
#define MODE_CHANGEMODE 2
#define MODE_SHOW_URI 3
#define MODE_CONFIRM_WRITE_PRESET 4
#define MODE_EDIT_SSID 5
#define MODE_EDIT_PASS 6
#define MODE_CONFIRM_SSID 7
#define MODE_CONFIRM_PASS 8
#define MODE_CONFIRM_RESET 9
#define MODE_WIFI_CONNECTING 10


// knob
#define NUM_OF_KNOB 3
#define KNOB_EFFECT 0
#define KNOB_FEEDBACK 1
#define KNOB_TIME 2

// settings
#define NUM_OF_SETTINGS 4
#define SETTING_MIDI_CH 0
#define SETTING_PRESET_MAX 1
#define SETTING_WIFI_ONOFF 2
#define SETTING_FOOTSW 3

// mode change
#define NUM_OF_CHANGEMODE 5
#define MODECHANGE_PLAY 0
#define MODECHANGE_SETTING 1
#define MODECHANGE_WIFI 2
#define MODECHANGE_SSID 3
#define MODECHANGE_PASS 4

// preset write confirmation
#define NUM_OF_CONFIRM 2
#define CONFIRM_NO 0
#define CONFIRM_YES 1

// foot sw mode
#define FOOTSW_ALTERNATE 0
#define FOOTSW_PRESET 1

// ui change status
#define UI_CHANGE_PRESET 11
#define UI_CHANGE_PLAY_SELECTION 12
#define UI_CHANGE_PLAY_EFFECT 13
#define UI_CHANGE_PLAY_FEEDBACK 14
#define UI_CHANGE_PLAY_TIME 15
#define UI_CHANGE_SET_SELECTION 16
#define UI_CHANGE_SET_MIDI 17
#define UI_CHANGE_SET_FOOTSW 18
#define UI_CHANGE_MODE_SELECTION 19

// delay time range
#define DELAY_TIME_MAX 580
#define DELAY_TIME_MIN 50

// WiFi: SSID & PASS
#define WIFI_SSID_MAX_LEN 32
#define WIFI_PASS_MAX_LEN 63

// char type
#define VALID_SSID " 0123456789abcdefghijklnmopqrstuvwxyzABCDEFGHIJKLNMOPQRSTUVWZYZ_-"
#define VALID_PASS " 0123456789abcdefghijklnmopqrstuvwxyz"

#define LEN_DEBUG_MESSAGE 1024

typedef struct {
  int onoff;
  int effect;
  int feedback;
  int time;
  int presetNo; // 0-9
  int midi_ch; // 0-15 (1ch-16ch)
  int footsw_mode; // 0:preset, 1:alternate on/off
  char wifi_ssid[WIFI_SSID_MAX_LEN+1]; // SSID(32 chars)
  char wifi_pass[WIFI_PASS_MAX_LEN+1]; // passphrase(63 chars)
  int wifi_enable; // 0:disable
  int preset_max;
}Type_AppParams;

// app params (do not save to eeprom)
typedef struct{
  int selected_knob;
  int selected_settings;
  int selected_mode;
  int selected_yesno;
  int preset_changed;
  int available_wifi;  // true: if wifi can be used
  int mode; // 0:play 1:settings(midi-ch etc)
  int changed_via_wifi; // true: when receiving preset data via wifi
  int tempo_target_msec;
  int changed_tempo_led; // true: when tempo counter time comes
  int changed_wifi_7seg; // true: when tempo counter time comes
  int tempo_led_status; // 0:turn off 1:turn on
  int ssid_currectPos;
  int pass_currectPos;
  char edited_wifi_ssid[WIFI_SSID_MAX_LEN+1]; // SSID(32 chars)
  char edited_wifi_pass[WIFI_PASS_MAX_LEN+1]; // passphrase(63 chars)
  int edit_cursor_char_id;
  int wifi_onoff_changed;
  int mute_counter; // mute time in msec
  int mute_flag; // mute trigger
}Type_AppInternalVars;

// preset memorized
#define NUM_PRESET 10
typedef struct{
  int effect;
  int feedback;
  int time;
}Type_PresetParams;

// EEPROM
typedef struct {
    char signature[10];
    Type_AppParams params;
    Type_PresetParams presets[10];
}EEPROM_DATA;