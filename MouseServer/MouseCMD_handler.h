#if !defined(AFX_MouseCMD_handler_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_)
#define AFX_MouseCMD_handler_H__166D4120_2F94_4231_AE60_7C719E3EC05C__INCLUDED_

#pragma once

#include "RXCMD.h"

struct MousePosition{
	byte action;
	short X_pos;
	short Y_pos;
};

class MouseCMD_handler
{
private:
	RXCMD mouseRX;  //RXdata 저장할 구조체변수
	MousePosition Change_Mouse; //마우스 이동거리 구조체변수
	short before_mousewheel_Y;  //마우스 휠 이전 y좌표 저장
	short mousewheel_Y_range;  //마우스휠 y좌표의 이동거리(+면 밑으로 스크롤 -면 위로 스크롤)
public:
	MouseCMD_handler(void);
	~MouseCMD_handler(void);
	void MouseCMD_Parsing(RXCMD RX_Data); //커맨드들 parsing하는 함수 
	void KeybordCMD_handler();  //키보드 커맨드 핸들러
	void Mouse_Move_handler(); //마우스 좌표조절 커맨드 핸들러 
	void Mouse_RightButton_handler();  //마우스 왼족버튼 커맨드 핸들러
	void Mouse_LeftButton_handler();  //마우스 왼족버튼 커맨드 핸들러
	void Mouse_Wheel_handler();  //마우스 휠 커맨드 핸들러

};

#endif