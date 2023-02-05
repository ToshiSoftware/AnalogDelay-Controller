#include "mySPI.h"


void SetupReadHC165(void) {
  pinMode(SRD_SDI, INPUT);
  pinMode(SRD_CS, OUTPUT);
  pinMode(SRD_CLK, OUTPUT);
  hc165.data = 0;
  hc165.flag_read = 0;
  digitalWrite(SRD_CLK, LOW);
  digitalWrite(SRD_CS, LOW);
}

// launch as an interruppted function
void ReadHC165(void) {
  int i;
  int readbit;
  // set read flag
  hc165.flag_read = 1;
  // init
  hc165.data = 0;
  // serial read
  digitalWrite(SRD_CS, HIGH);  // rise CS,  HC165 latches the data
                               // and output 1st data 'H' on SRD_SDI
  // read 'H'
  hc165.data = hc165.data & 0xfe;  // mask LSB with 0
  if (digitalRead(SRD_SDI) == HIGH) { // read data
    hc165.data = hc165.data | 0x01;
  }
  // disable CLK
  digitalWrite(SRD_CLK, LOW); 

  // read form G to A
  for (i = 0; i < 7; i++) {
    // bit shift
    hc165.data = hc165.data << 1;
    hc165.data = hc165.data & 0xfe;  // mask LSB with 0
    // enable CLK
    digitalWrite(SRD_CLK, HIGH); // HC165 shift and output data
    digitalWrite(SRD_CLK, HIGH); // delay
    digitalWrite(SRD_CLK, HIGH); // delay
    // read sdi
    if (digitalRead(SRD_SDI) == HIGH) { // read data
      hc165.data = hc165.data | 0x01;
    }
    // disable CLK
    digitalWrite(SRD_CLK, LOW); 
  }
  digitalWrite(SRD_CS, LOW);  // lower CS
  // Decode
  HandleEncoder();
}


void SetupParseSdiData(void) {
  int i;
  // sw
  for (i = 0; i < NUM_SW; i++) {
    sdi.sw[i].enable_long_press = 0;
    sdi.sw[i].state = 0;
    sdi.sw[i].counter = 0;
    sdi.sw[i].button_in_use = 0;
    sdi.sw[i].trigger_shortpress = 0;
    sdi.sw[i].trigger_longpress=0;
  }
  // enable long press
  sdi.sw[BUTTON_ID_ENTER].enable_long_press=true;
  // encoder
  for (i = 0; i < NUM_ENCODER; i++) {
    sdi.encoder[i].A = 0;
    sdi.encoder[i].B = 0;
    sdi.encoder[i].prev_pinValue = 0;
    sdi.encoder[i].value = 500;
  }
}

void ParseSdiData(void) {

  // set sw data
  //SetupParseSdiData();
  sdi.sw[0].state = 0;
  sdi.sw[1].state = 0;
  sdi.sw[2].state = 0;
  sdi.sw[3].state = 0;
  if (hc165.data & 0x01) sdi.sw[0].state = 1;
  if (hc165.data & 0x02) sdi.sw[1].state = 1;
  if (hc165.data & 0x04) sdi.sw[2].state = 1;
  if (hc165.data & 0x08) sdi.sw[3].state = 1;
  // set encoder bit
  sdi.encoder[0].B = 0;
  sdi.encoder[0].A = 0;
  sdi.encoder[1].B = 0;
  sdi.encoder[1].A = 0;
  if (hc165.data & 0x10) sdi.encoder[0].B = 1;
  if (hc165.data & 0x20) sdi.encoder[0].A = 1;
  if (hc165.data & 0x40) sdi.encoder[1].B = 1;
  if (hc165.data & 0x80) sdi.encoder[1].A = 1;
}

/* -------------------------------------------------------------------
                         a      b    c  d    e              f
                    -------------------->2sec----------------------
Long Press          ~~~~~|__________________________________|~~~~~~
                         <----in use -->
trigger_longpress   ____________________|~CB~|______________________

Short Press         ~~~~~|______|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                         <-use-->
trigger_shortpress  ____________|~CB~|______________________________


a)button down
b)if button up before long press time(2sec), enable 'trigger_shortpress'
c)callback func disables 'trigger_shortpress'
d)if button up after long press time(2sec), enable 'trigger_longpress'
e)callback func disables 'trigger_shortpress'
f)button up
------------------------------------------------------------------- */

void HandleSw(void) {
  int i;
  for (i = 0; i < NUM_SW; i++) {
      // if pressed, count up
      if(sdi.sw[i].state==BT_PRESSED){
        sdi.sw[i].counter++;
      }
      // reset count
      if(sdi.sw[i].state==BT_RELEASED){
        sdi.sw[i].counter=0;
      }

      // [ONLY short press] Button: '<', >', 'FOOT'
      if(sdi.sw[i].enable_long_press==false){

        if (sdi.sw[i].button_in_use==false && sdi.sw[i].state==BT_PRESSED && sdi.sw[i].trigger_shortpress==false) {
          // set flag in using
          sdi.sw[i].button_in_use = true;
          // call callback function
          sdi.sw[i].trigger_shortpress = true;
          //Debug_Pulse_D3();
        }
        // wait until sw state changes to LOW
        else if (sdi.sw[i].button_in_use==true && sdi.sw[i].state==BT_RELEASED) {
          // reset flag in using
          sdi.sw[i].button_in_use = false;
        }

      }
      // [LONG PRESS enabled] Button: 'enter'
      else{

        if (sdi.sw[i].button_in_use==0 && sdi.sw[i].state==BT_PRESSED
            && sdi.sw[i].trigger_shortpress==false && sdi.sw[i].trigger_longpress==false) {
           // set flag in using
          sdi.sw[i].button_in_use = 1;
        }
        else if(sdi.sw[i].button_in_use==1 && sdi.sw[i].state==BT_RELEASED ) {
          // reset flag in using
          if(sdi.sw[i].counter<LONG_PRESS_COUNT){
            sdi.sw[i].button_in_use = 0;
            sdi.sw[i].trigger_shortpress=true;
          }
        }
        else if(sdi.sw[i].button_in_use==1 && sdi.sw[i].counter==LONG_PRESS_COUNT)
        {
          sdi.sw[i].button_in_use = 2;
          sdi.sw[i].trigger_longpress=true;
        }
        else if(sdi.sw[i].button_in_use==2 && sdi.sw[i].state==BT_RELEASED)
        {
          sdi.sw[i].button_in_use = 0;
        }
      }
    }
}

// Rotary encoder
// https://sheep-me.me/2019/12/05/geidai_21/
int cw[] = {1, 3, 0, 2};
int ccw[] = {2, 0, 3, 1};

void HandleEncoder(void) {
  int i;
  int pinValue;

  for (i = 0; i < NUM_ENCODER; i++) {
    // if encoder A/B is changed..
    pinValue = sdi.encoder[i].A + sdi.encoder[i].B * 2;
    pinValue %= 4;
    if (sdi.encoder[i].prev_pinValue != pinValue) {
      if (pinValue == cw[sdi.encoder[i].prev_pinValue]) sdi.encoder[i].value++;
      if (pinValue == ccw[sdi.encoder[i].prev_pinValue]) sdi.encoder[i].value--;
      sdi.encoder[i].prev_pinValue = pinValue;
      //Debug_Pulse_D3();
    }
 
    // value for app
    int dif;
    dif = sdi.encoder[i].value - sdi.encoder[i].prev_appValue;
    if(dif>ENCODER_TOLERANCE){
      sdi.encoder[i].rotation_dir = ROTATION_PLUS;
      sdi.encoder[i].prev_appValue = sdi.encoder[i].value;
    }
    else if(dif<-ENCODER_TOLERANCE){
      sdi.encoder[i].rotation_dir = ROTATION_MINUS;    
      sdi.encoder[i].prev_appValue = sdi.encoder[i].value;
    }
    else{
      sdi.encoder[i].rotation_dir = ROTATION_STOP;    
    }
 
  }

}


unsigned char seg7Value[] = {
  // GFEDCBA
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F,
    0x77,
    0x7C,
    0x39,
    0x5E,
    0x79,
    0x71
};

unsigned char Get7SegSerialData(int digit, int dot) {
  unsigned char sdValue = 0;

  if (digit < 0) digit = 0;
  else if (digit > 0x0f) digit = 0x0f;

  // anode comon: '~' is required
  if (dot) {
    sdValue = ~seg7Value[digit] & 0x7F;
  } else {
    sdValue = ~seg7Value[digit];
  }

  return sdValue;
}

// if mute is true, effect and feedback is set to 0
void SendMyHC595Serial(int mute)
{
  unsigned char seg7=0, hc595=0;
  int step_effect, step_feedback, step_time;

  // 7seg
  seg7 = Get7SegSerialData(appParam.presetNo, appVars.preset_changed);
  // hc595
  if(appParam.onoff == true){ 
    hc595 = (hc595 | 0x01); // relay on
    hc595 = (hc595 | 0x02); // led1 on
  }
  // tempo
  if(appVars.tempo_led_status==true){
    hc595 = (hc595 | 0x04); // led2 on
  }

  // AD8405 step
  step_effect   = 255*appParam.effect/99;
  step_feedback = 255*appParam.feedback/99;
  step_time =     127*appParam.time/99;

  if(mute==true){
    step_effect   = 0;
    step_feedback = 0;
  }

  // effect
  MySpiTransfer(seg7, hc595, 0, step_effect);
  MySpiTransfer(seg7, hc595, 1, step_feedback);
  MySpiTransfer(seg7, hc595, 2, step_time);
}

// blink TEMPO LED
int led_interval_counter=0;
int wifi_7seg_counter=0;
// called every 1msec
void TempoLedHandler(void)
{
  led_interval_counter++;
  wifi_7seg_counter++;
  if(led_interval_counter > appVars.tempo_target_msec/2)
  {
    led_interval_counter=0;
    appVars.changed_tempo_led = true;
  }
  if(wifi_7seg_counter > 100) // 100ms
  {
    wifi_7seg_counter=0;
    appVars.changed_wifi_7seg = true;
  }
  // mute counter
  appVars.mute_counter--;
  if(appVars.mute_counter<0) appVars.mute_counter=0;
}