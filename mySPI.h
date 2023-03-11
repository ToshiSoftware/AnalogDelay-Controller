#pragma once
// my serial/pararel interface
// using 74HC595, 74HC165

#define NUM_SW 4
#define NUM_ENCODER 2

typedef struct _Type_HC165Manage
{
	int flag_read; // set 1 when hc165 is read
	int data;      // 8-bit read data 
} Type_HC165Manage;

#define SRD_SDI D0
#define SRD_CS  D1
#define SRD_CLK D2

#define ENCODER_TOLERANCE 2
#define ROTATION_STOP 0
#define ROTATION_PLUS 1
#define ROTATION_MINUS -1

typedef struct _Type_EncoderBit
{
	// sdi
	int A;
	int B;
	int prev_pinValue;
	int value;
	// value for app
	int prev_appValue;
	int rotation_dir; // 0:stop -1:++ 1:--
} Type_EncoderBit;

#define LONG_PRESS_COUNT 800
#define BT_PRESSED 0
#define BT_RELEASED 1

#define BUTTON_ID_LEFT  0
#define BUTTON_ID_RIGHT 1
#define BUTTON_ID_ENTER 2
#define BUTTON_ID_FOOT  3

typedef struct _Type_SwBit
{
	int enable_long_press; // 0:disable, 1:enable
	int state; // set by ParseSdiData(), 1:up(H) 2:down(L)
	int counter; // coubter
	int button_in_use; // 0: free, 1:in use
	int trigger_shortpress; // callback trigger for short-press -> send to loop
	int trigger_longpress; // callback trigger for long-press -> send to loop
} Type_SwBit;


typedef struct _Type_InputManage
{
	Type_SwBit sw[NUM_SW];
	Type_EncoderBit encoder[NUM_ENCODER];

} Type_InputManage;



extern void SetupReadHC165(void);
extern void ReadHC165(void);
extern void SetupParseSdiData(void);
extern void ParseSdiData(void);
extern void HandleSw(void);
extern void HandleEncoder(void);
extern unsigned char Get7SegSerialData(int digit, int dot);
extern void SendMyHC595Serial(int mute);
extern int GetBbdDelayActualStep(int time);

extern Type_InputManage sdi;
extern Type_HC165Manage hc165;
