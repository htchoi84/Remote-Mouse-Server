#if !defined(AFX_RXCMD_H__91CEEDDC_5EB4_4AE7_8175_D8A85EA9CA07__INCLUDED_)
#define AFX_RXCMD_H__91CEEDDC_5EB4_4AE7_8175_D8A85EA9CA07__INCLUDED_

#include "stdafx.h"

struct RXCMD{
	int cmdLength;
	byte mainCMD;  //리모컨인지 마우스인지 매직마우스인지
	byte subCMD;   //Mouse인지 keyboard인지 등
	byte action;   //down/move/up등9
	short data1;  //data1에는 X이거나 아님 keyboard 이벤트
	short data2;  //data2에는 Y
	short data3;  //data3에는 z
};

struct ScreenSize{
	short width;
	short height;
};

//MainCMD
#define REMOTE_CMD 0x01
#define MOUSE_CMD 0x02
#define PPTMODE_CMD 0x03
#define KEYBOARD_CMD 0x04
#define SENSORMOUSE_CMD 0x05
#define CONNECT_CMD 0x06

//마우스 /키보드 / PPT모드 action  
#define DOWN 0x01
#define MOVE 0x02
#define UP 0x03

///////리모컨 관련 정의///////////////
//리모컨 subCMD
#define GOMPLAYER 0x01
#define ALSHOW 0x02
#define WINMEDIAPLAYER 0x03
#define DAUMPOTPLAYER 0x04
//#define DAUMPOTP 0x02
//#define WINMEDIAP 0x03
//#define KMPP 0x04
//#define ALSHOW 0x05
//#define WINDOW 0x06

//리모컨 action
#define POWER_ACTION 0x00
#define PLAY_ACTION 0x01
#define OK_ACTION 0x02
#define UP_ACTION 0x03
#define DOWN_ACTION 0x04
#define RIGHT_ACTION 0x05
#define LEFT_ACTION 0x06
#define STOP_ACTION 0x07
#define FF_ACTION 0x08
#define RW_ACTION 0x09
#define VOLUMEUP_ACTION 0x0a
#define VOLUMEDOWN_ACTION 0x0b
#define OPEN_ACTION 0x0c
#define MUTE_ACTION 0x0d
#define CANCEL_ACTION 0x0e
#define TAB_ACTION 0x0f


///////마우스 관련 정의///////////////
//마우스 subCMD  
#define MOUSE_TOUCH 0x01
#define MOUSE_LBUTTON 0x02
#define MOUSE_RBUTTON 0x03
#define MOUSE_KEYBOARD 0x04
#define SENSITIVITY 0x05
#define MOUSE_WHEEL 0x06


/////////////PPT 모드 관련 정의 ////////////  
//PPT subCMD
#define PPTMODE_BEGIN_SLIDE 0x01
#define PPTMODE_CURRENT_SLIDE 0x02
#define PPTMODE_END_SLIDE 0x03
#define PPTMODE_BEFORE_PAGE 0x04
#define PPTMODE_NEXT_PAGE 0x05
#define PPTMODE_SENSORMOUSE 0x06
#define PPTMODE_LBUTTON 0x07
#define PPTMODE_RBUTTON 0x08


///////센서마우스 관련 정의///////////////
//센서마우스 subCMD  
#define SENSORMOUSE_ON 0x01
#define SENSORMOUSE_OFF 0x02
#define SENSORMOUSE_MOVE 0x03
#define SENSORMOUSE_LBUTTON 0x04
#define SENSORMOUSE_RBUTTON 0x05
#define SENSORMOUSE_WHEEL 0x06


///////키보드 관련 정의///////////////
#define ENGLISH 0x01
#define HANGUL 0x02
#define CAPSLOCK 0x03

#endif