#pragma once

extern void CopyCharByLength(char *org, char *dest, int max_length);
extern void getRTC( char* timestr );
extern void LcdPrintRTC(void);
extern void EraceLCD(int posx, int posy, int numOfChar);
extern int load_eeprom();
extern void save_eeprom();

extern void SetupAD8304(void);
extern void MySpiTransfer(int SEG7, int HC595, int AD8304_ADDR, int AD8304_DATA);
extern void LoadPresetToCurrentParam(int prst_no);
extern void TempoLedHandler(void);
extern int GetTempoTargetDelayTime(int appTime);
extern void CheckAndCorrectPreset(void);

extern void PrintInt2Digit(int value, int px, int py);
extern void PrintInt3Digit(int value, int px, int py);
extern void PrintInt4Digit(int value, int px, int py);
