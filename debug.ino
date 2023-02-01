#include "mySPI.h"
#include "myUtil.h"

int digit;

// pulse digital pin D3
void Debug_Pulse_D3(void)
{
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED1_PIN, LOW);
}

void Debug_EncoderValue(void)
{
  PrintInt4Digit(sdi.encoder[0].value, 0, 1);
  PrintInt4Digit(sdi.encoder[1].value, 4, 1);
}

//
void Debug_All_IO(void)
{
  int i;  
  // verify parsed data
  EraceLCD(8,0,8);
  lcd.setCursor(8, 0);
  for(i=1;i>=0;i--){
    if(sdi.encoder[i].A==1) lcd.print("1");
    else lcd.print("0");
    if(sdi.encoder[i].B==1) lcd.print("1");
    else lcd.print("0");
  }
  for(i=3;i>=0;i--){
    if(sdi.sw[i].state==1) lcd.print("1");
    else lcd.print("0");
  }

  //blinkflag = 1-blinkflag;
  //digitalWrite(LED1_PIN, blinkflag);

  digit++; digit%=0x0f;
  int tmp = (int)Get7SegSerialData(digit, digit%2);

  EraceLCD(0,0,8);
  lcd.setCursor(0, 0);
  lcd.print(tmp, HEX);
  lcd.setCursor(4, 0);
  lcd.print(digit); 
  // verify HC165 input
  lcd.setCursor(8, 1);
  PrintBinLCD(hc165.data);

  //
  MySpiTransfer((int)tmp, 0x04, 0, 0);
}