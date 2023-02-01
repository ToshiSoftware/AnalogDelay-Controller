// Utility: string copy with length limit
void CopyCharByLength(char *org, char *dest, int max_length)
{
  int i=0;
  while(1)
  {
    if(org[i]=='\0') break;
    if(i>=max_length) break;
    dest[i] = org[i];
    i++;
  }
  dest[i] = '\0';
}

// Utility: get real time string HH:MM.SS
void getRTC( char* timestr )
{
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  sprintf(timestr, "%02d:%02d.%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);  
}

// Utility: print RTC on LCD
void LcdPrintRTC(void)
{
    // Real time
  char rtcstr[16];
  getRTC(rtcstr);
  lcd.setCursor(0, 1);
  lcd.print(rtcstr); 
}

// Utility: Erace LCD Chars (fill with space)
void EraceLCD(int posx, int posy, int numOfChar)
{
  int i;
  lcd.setCursor(posx, posy);
  for(i=0; i<numOfChar; i++)
  {
    lcd.print(" ");
  }
  lcd.setCursor(posx, posy);
}

// print 8bit binary to LCD
void PrintBinLCD(unsigned char bindata)
{
  int i;
  unsigned char tmp;
  tmp = bindata;
  for(i=0;i<8;i++){
    if(tmp&0x80) lcd.print("1");
    else lcd.print("0");
    tmp = tmp<<1;
  }
}

// print 2 digit integer with justifing to the right
void PrintInt2Digit(int value, int px, int py){
  int tmp;
  
  EraceLCD(px, py, 2);
  tmp=value;
  tmp%=100;
  if(tmp<10) lcd.print("0");
  lcd.print(tmp);
}

// print 3 digit integer with justifing to the right
void PrintInt3Digit(int value, int px, int py){
  int tmp;
  
  EraceLCD(px, py, 3);
  tmp=value;
  tmp%=1000;
  if(tmp<100) lcd.print("0");
  if(tmp<10) lcd.print("0");
  lcd.print(tmp);
}

// print 4 digit integer with justifing to the right
void PrintInt4Digit(int value, int px, int py){
  int tmp;
  
  EraceLCD(px, py, 4);
  tmp=value;
  tmp%=10000;
  if(tmp<1000) lcd.print("0");
  if(tmp<100) lcd.print("0");
  if(tmp<10) lcd.print("0");
  lcd.print(tmp);
}

//
void CheckAndCorrectPreset(void){
  int i;

  // check preset value
  for(i=0; i<NUM_PRESET; i++)
  {
    if(presetParams[i].effect<0) presetParams[i].effect=0;
    if(presetParams[i].feedback<0) presetParams[i].feedback=0;
    if(presetParams[i].time<0) presetParams[i].time=0;
    if(presetParams[i].effect>99) presetParams[i].effect=99;
    if(presetParams[i].feedback>99) presetParams[i].feedback=99;
    if(presetParams[i].time>99) presetParams[i].time=99;
  }  
}

// Read from EEPROM
int load_eeprom() {
    EEPROM.get<EEPROM_DATA>(0, eeprom);
    if(strcmp(eeprom.signature, SIGNATURE)==0)
    {
        memcpy( &appParam, &eeprom.params, sizeof(Type_AppParams) );
        memcpy( &presetParams[0], &eeprom.presets[0], sizeof(Type_PresetParams)*NUM_PRESET );    

        // verify preset value
        CheckAndCorrectPreset();
        return true;
    }
    else
    { 
      // Erase buffer memory if signature is wrong
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EEPROM Load ERR");
      lcd.setCursor(0,1);
      lcd.print("Wrong signature");
      //lcd.setCursor(0,1);
      //lcd.print(eeprom.signature);
      delay(3000);
      return false;
    }
}

// Save to EEPROM
void save_eeprom() {
    //EEPROMに設定を保存する。
    memset(&eeprom, 0, sizeof(EEPROM_DATA));
    strcpy(eeprom.signature, SIGNATURE);
    memcpy( &eeprom.params, &appParam, sizeof(Type_AppParams) );
    memcpy( &eeprom.presets[0], &presetParams[0], sizeof(Type_PresetParams)*NUM_PRESET );    
    EEPROM.put<EEPROM_DATA>(0, eeprom);
    EEPROM.commit(); //必須
}

// AD8304 control
#define AD8304_CLK D8
#define AD8304_SDI D10
#define AD8304_CS  D9

void SetupAD8304(void){
  // def
  pinMode(AD8304_CLK, OUTPUT);
  pinMode(AD8304_SDI, OUTPUT);
  pinMode(AD8304_CS, OUTPUT);
  // initial state
  digitalWrite(AD8304_CLK, HIGH);
  digitalWrite(AD8304_SDI, HIGH);
  digitalWrite(AD8304_CS, HIGH);
}

// Send serial data to HC595 and AD8304_ADDR
//   HC595 : external 8bit shift register (0-0x0f) connected to AD8304 SDO, 8bit
//   AD8304_ADDR : ch of AD8304 (0-3), 2bit
//   AD8304_DATA : step value of AD8304 (0-255), 8bit
void MySpiTransfer(int SEG7, int HC595, int AD8304_ADDR, int AD8304_DATA){
  int i;
  int shifted_data=0;
  // initial
  digitalWrite(AD8304_CLK, HIGH);
  digitalWrite(AD8304_SDI, HIGH);
  digitalWrite(AD8304_CS, HIGH); 

  // prepare to latch ( change CS to LOW )
  digitalWrite(AD8304_CS, LOW);

  // 7seg 
  shifted_data = HC595;
  for(i=0; i<8; i++)
  {
    // data set
    if(shifted_data & 0x80) digitalWrite(AD8304_SDI, HIGH);
    else                    digitalWrite(AD8304_SDI, LOW);
    shifted_data = (shifted_data << 1);
    // clk down -> up
    digitalWrite(AD8304_CLK, LOW);
    digitalWrite(AD8304_CLK, HIGH);
  }
  // pause for a while to identfy byte/2bit data
  for(i=0; i<8; i++) digitalWrite(AD8304_CLK, HIGH);

   // HC595
  shifted_data = SEG7;
  for(i=0; i<8; i++)
  {
    // data set
    if(shifted_data & 0x80) digitalWrite(AD8304_SDI, HIGH);
    else                    digitalWrite(AD8304_SDI, LOW);
    shifted_data = (shifted_data << 1);
    // clk down -> up
    digitalWrite(AD8304_CLK, LOW);
    digitalWrite(AD8304_CLK, HIGH);
  }
  // pause for a while to identfy byte/2bit data
  for(i=0; i<8; i++) digitalWrite(AD8304_CLK, HIGH);

  // AD8304_ADDR
  shifted_data = AD8304_ADDR;
  for(i=0; i<2; i++)
  {
    // data set
    if(shifted_data & 0x02) digitalWrite(AD8304_SDI, HIGH);
    else                    digitalWrite(AD8304_SDI, LOW);
    shifted_data = (shifted_data << 1);
    // clk down -> up
    digitalWrite(AD8304_CLK, LOW);
    digitalWrite(AD8304_CLK, HIGH);
  }
  // pause for a while to identfy byte/2bit data
  for(i=0; i<8; i++) digitalWrite(AD8304_CLK, HIGH);

  // AD8304_DATA
  shifted_data = AD8304_DATA;
  for(i=0; i<8; i++)
  {
    // data set
    if(shifted_data & 0x80) digitalWrite(AD8304_SDI, HIGH);
    else                    digitalWrite(AD8304_SDI, LOW);
    shifted_data = (shifted_data << 1);
    // clk down -> up
    digitalWrite(AD8304_CLK, LOW);
    digitalWrite(AD8304_CLK, HIGH);
  }
  // pause for a while to identfy byte/2bit data
  for(i=0; i<8; i++) digitalWrite(AD8304_CLK, HIGH);

  // latch ( change CS to HIGH )
  digitalWrite(AD8304_CS, HIGH);
 
  // termination
  digitalWrite(AD8304_SDI, HIGH);
}

void LoadPresetToCurrentParam(int prst_no)
{
  int prev_time = appParam.time;

  appParam.effect = presetParams[prst_no].effect;
  appParam.feedback = presetParams[prst_no].feedback;
  appParam.time = presetParams[prst_no].time;
  appVars.tempo_target_msec = GetTempoTargetDelayTime(appParam.time);

  // mute effect & feedback while 
  appVars.mute_counter = GetTempoTargetDelayTime(appParam.time) * 1.1;
  appVars.mute_flag = true;
}

int GetTempoTargetDelayTime(int appTime)
{
  return (DELAY_TIME_MAX-DELAY_TIME_MIN)*appTime/99+DELAY_TIME_MIN;
}