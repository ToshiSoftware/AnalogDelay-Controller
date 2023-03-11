/*
  Arduino Noiseless Analog delay pedal controller using ESP32C3 (Seeed XIAO)
  Web server TEST URI:
    open the following URIs:
      http://nld.local/
      http://nld.local/preset?e0=41&f0=50&t0=60&e1=40&f1=50&t1=60&e2=40&f2=50&t2=60&e3=40&f3=50&t3=60&e4=40&f4=50&t4=60&e5=40&f5=50&t5=60&e6=40&f6=50&t6=60&e7=40&f7=50&t7=60&e8=40&f8=50&t8=60&e9=40&f9=50&t9=60&sn=Submit+preset+data

      TEST
      http://nld.local/preset?e0=99&f0=0&t0=20&e1=99&f1=0&t1=50&e2=99&f2=0&t2=99&e3=40&f3=50&t3=60&e4=40&f4=50&t4=60&e5=40&f5=50&t5=60&e6=40&f6=50&t6=60&e7=40&f7=50&t7=60&e8=40&f8=50&t8=60&e9=40&f9=50&t9=60&sn=Submit+preset+data
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>
#include <time.h>
//#include <Encoder.h>
//#include <SPI.h>
#include <MIDI.h>
#include <Wire.h>
#include <ST7032.h>
#include <EEPROM.h>

#include "ESP32C3_PedalCtrl.h"
#include "myUtil.h"
#include "mySPI.h"
#include "web_server.h"
#include "ui_handler.h"

// debug
int debug_output_to_html=false;
char debug_message[1024]="Debug message..<br />\n";

// Def: LCD
ST7032 lcd;

// Def: Wifi+WebServer
const char* host = "esp32-webupdate";
extern char index_html[];
WebServer httpServer(80);
HTTPUpdateServer httpUpdater;

// Def: eeprom data
EEPROM_DATA eeprom;

// Def: MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

// Def: HwTimer
hw_timer_t *timer0 = NULL;
hw_timer_t *timer1 = NULL;

// Def: mySPI
Type_HC165Manage hc165;
Type_InputManage sdi;

// Def: app palams
Type_AppParams appParam;
Type_PresetParams presetParams[NUM_PRESET];
Type_AppInternalVars appVars;

// SSID PASSPHRASE available charactors
char valid_ssid_char[]=VALID_SSID;
char valid_pass_char[]=VALID_PASS;


void SetupAppParams(void)
{
  // parameters to save to eeprom 
  memset(&appParam, 0, sizeof(Type_AppParams));
  appParam.effect = 0;
  appParam.feedback = 0;
  appParam.time = 0;
  appParam.presetNo = 0;
  appParam.midi_ch = 1; // 1-16
  appParam.footsw_mode = 0;
  memset(appParam.wifi_ssid, 0, WIFI_SSID_MAX_LEN+1);
  memset(appParam.wifi_pass, 0, WIFI_PASS_MAX_LEN+1);
  appParam.wifi_enable = false;
  appParam.preset_max = 9;

  // variables internal use
  memset(&appVars, 0, sizeof(Type_AppInternalVars));
  appVars.selected_knob=0;
  appVars.selected_mode=0;
  appVars.selected_settings=0;
  appVars.selected_yesno=0;
  appVars.preset_changed=false;
  appVars.available_wifi=false;
  appVars.mode = 0;
  appVars.changed_via_wifi = false;
  appVars.tempo_target_msec = DELAY_TIME_MAX;
  appVars.changed_tempo_led = false;
  appVars.changed_wifi_7seg = false;
  appVars.tempo_led_status = false;

  memset(appVars.edited_wifi_ssid, 0, WIFI_SSID_MAX_LEN+1);
  memset(appVars.edited_wifi_pass, 0, WIFI_PASS_MAX_LEN+1);

  appVars.edit_cursor_char_id = 0;
  appVars.wifi_onoff_changed = false;
  appVars.mute_counter = 0;
  appVars.mute_flag = false;

  // preset param
  int i;
  for(i=0;i<NUM_PRESET;i++){
    presetParams[i].effect=50;
    presetParams[i].feedback=50;
    presetParams[i].time=50;
  }
}


// System setup
void setup(void) {
  // SYSTEM HW -------------------------------------------
  // init app params
  SetupAppParams();

  // TEST pin
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);

  // SPI for digital pot
  SetupAD8304();
  MySpiTransfer(0,0,0,0);

  // LCD Display -------------------------------------------
  // LCD: setup
  char STARTING_STR[]="Starting up...";
  
  lcd.begin(16, 2);
  lcd.setContrast(45);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Analog Delay 2.1");

  lcd.setCursor(0, 1);
  for(int i=0; i<strlen(STARTING_STR); i++){
    char text[2];
    text[0]=STARTING_STR[i];
    text[1]='\0';
    lcd.print(text);
    delay(200);
  }
  delay(2000);

  // EEPROM -------------------------------------------
  // EEPROM: start
  memset(&eeprom, 0, sizeof(EEPROM_DATA));
  EEPROM.begin(sizeof(EEPROM_DATA)+0x100); 
  load_eeprom();

  // $$$$ for test
  //strcpy(appParam.wifi_ssid, STASSID);
  //strcpy(appParam.wifi_pass, STAPSK);

  // WiFi(web server) -------------------------------------------
  // WiFi: start
  if(appParam.wifi_enable == true){
    WiFi.mode(WIFI_AP_STA);
    int tmpct=0;
    while(1)
    {
      WiFi.begin(appParam.wifi_ssid, appParam.wifi_pass);
      if(WiFi.waitForConnectResult() == WL_CONNECTED) 
      {
        appVars.available_wifi=true;
        break;
      }
      delay(500);
      tmpct++;
      if(tmpct>WIFI_CONNECTION_RETRY) break;
      // 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Connecting Wifi.");
      lcd.setCursor(0, 1);
      lcd.print("Retry:");
      lcd.print(tmpct);
      lcd.print("/");
      lcd.print(WIFI_CONNECTION_RETRY);
    }
    // 
    if(appVars.available_wifi==true)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("http://");
      lcd.print(MDNS_STR);
      lcd.print(".local");
      // params
      lcd.setCursor(0,1);
      lcd.print(WiFi.localIP());
      delay(2000);
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wifi unavailable");
      lcd.setCursor(0, 1);
      lcd.print("Wrong SSID/PASS");
      delay(2000);
    }
  }
  else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wifi disabled");
      lcd.setCursor(0, 1);
      lcd.print("Go to SET menu");
      delay(2000);
  }

  if(appVars.available_wifi==true)
  {  
    // WiFi: DNS start  , register "esp32.local" to Multicast Domain Name System(mDNS)
    MDNS.begin(host);
    if (MDNS.begin(MDNS_STR)) {
      //Serial.println("mDNS responder started");
    }
    lcd.setCursor(0, 0);

    // Time: get JST time
    configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");

    // Web Server: setup
    httpUpdater.setup(&httpServer);

    // Web Server: handler when root accessed
    httpServer.on("/", handleRoot);
    httpServer.on("/preset", handleSetVol);
    httpServer.onNotFound(handleRoot);

    // Web Server: launch web server
    httpServer.begin();
    MDNS.addService("http", "tcp", 80);
  }

  // MIDI -------------------------------------------
  #ifndef USE_SERIAL
    MIDI.begin(MIDI_CHANNEL_OMNI);
  #endif
 
  // INTERRUPT for INPUT HW(Button, Encoder) -------------------------------------------
  // interrupt for mySPI
  SetupReadHC165();
  SetupParseSdiData();
  timer0 = timerBegin(0, PRESCALER_DIVIDE_RATE, true);
  timerAttachInterrupt(timer0, &ReadHC165, true); // calllback
  timerAlarmWrite(timer0, TIMER_UI_INTERVAL, true); // 1000 microsec
  timerAlarmEnable(timer0); // start timer

  // INTERRUPT for TEMPO LED-------------------------------------------
  timer1 = timerBegin(1, PRESCALER_DIVIDE_RATE, true);
  timerAttachInterrupt(timer1, &TempoLedHandler, true); // calllback
  timerAlarmWrite(timer1, TIMER_TEMPO_INTERVAL, true); // 1000 microsec
  timerAlarmEnable(timer1); // start timer

  
  // INIT APP STATE -------------------------------------------
  // load preset data to the current params
  LoadPresetToCurrentParam(appParam.presetNo);

  // Wifi connection
  if(appVars.available_wifi == true)
  {
    appVars.mode=MODE_WIFI_CONNECTING;
    appParam.onoff = false;
    appParam.presetNo = 0;
    appParam.effect = 0;
    appParam.feedback = 0;
  #ifdef PT2399
    appParam.time = 50;
  #else
    appParam.time = 300;
  #endif
  }
  else{
    if(appParam.footsw_mode==FOOTSW_PRESET){
      appParam.onoff = true;
    }
  } 

  // initial state
  lcd.clear();
  UpdateEntireLcdDisplay();
  SendMyHC595Serial(false);
}

// Event loop
int updated_button=false;
int updated_encoder=false;
int updated_midi=false;

void loop(void) {
  static int counter=0;
  int i;

  // Sdi handler
  if(hc165.flag_read) { // set by ReadHC165()
    hc165.flag_read = false; // wait next interrupt
    // handle serial input data
    ParseSdiData();
    HandleSw();
    HandleEncoder();

    // UI handler
    updated_button = UiButtonHandler();
    updated_encoder = UiEncoderHandler();
    updated_midi = HandleMidi(); 

    // LCD for ui handler
    if(updated_button || updated_encoder || updated_midi){
      // update display
      UpdateEntireLcdDisplay();
      SendMyHC595Serial(appVars.mute_flag);
    }

  }

  // LCD for preset table change via wifi
  if(appVars.changed_via_wifi == true)
  {
    appVars.changed_via_wifi = false;
    // load preset data to the current params
    LoadPresetToCurrentParam(appParam.presetNo);
    // update display
    UpdateEntireLcdDisplay();
    SendMyHC595Serial(appVars.mute_flag);
  }

  // HTTPD handler
  if(appVars.available_wifi==true){
    httpServer.handleClient();
  }

  // tempo
  if(appVars.available_wifi == false && appVars.changed_tempo_led == true )
  {
    appVars.changed_tempo_led = false;
    appVars.tempo_led_status = 1 - appVars.tempo_led_status;
    //EraceLCD(0,0,8);
    //lcd.print(appVars.tempo_target_msec);
    SendMyHC595Serial(appVars.mute_flag);
  }

  // disable mute
  if(appVars.mute_flag == true && appVars.mute_counter == 0){
    appVars.mute_flag = false;
    SendMyHC595Serial(appVars.mute_flag);
  }

  // blink when wifi
  if(appVars.available_wifi == true && appVars.changed_wifi_7seg == true )
  {
    static int counter=0;
    appVars.changed_wifi_7seg = false;
    counter++;
    counter%=6;
    // set serial
    unsigned char seg7=0, hc595=0;
    int step_effect, step_feedback, step_time;

    // 7seg
    seg7 = ~(0x01<<counter);

    // AD8405 step
    step_effect   = 0;
    step_feedback = 0;
    step_time =     64;
    // effect
    MySpiTransfer(seg7, hc595, 0, step_effect);
    MySpiTransfer(seg7, hc595, 1, step_feedback);
    MySpiTransfer(seg7, hc595, 2, step_time);

  }

}

