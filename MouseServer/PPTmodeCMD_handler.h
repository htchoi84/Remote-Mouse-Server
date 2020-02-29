#pragma once

#include "RXCMD.h"

struct Position{
	short X_pos;
	short Y_pos;
};

class PPTmodeCMD_handler
{
private:
	RXCMD PPTmodeRX;  //RXdata 저장할 구조체변수
	Position Current_Mouse; //마우스 이동거리 구조체변수
	//ScreenSize screenSize;
	//short before_mousewheel_Y;  //마우스 휠 이전 y좌표 저장
	//short mousewheel_Y_range;  //마우스휠 y좌표의 이동거리(+면 밑으로 스크롤 -면 위로 스크롤)
public:
	PPTmodeCMD_handler(void);
	~PPTmodeCMD_handler(void);
	void PPTmodeCMD_Parsing(RXCMD RX_Data);
	void SensorMouse_handler();  //센서마우스 좌표조절 커맨드 핸들러
	void BeginSlide_handler();
	void CurrentSlide_handler();  
	void EndSlide_handler();
	void BeforePage_handler();
	void NextPage_handler();
	void LeftButton_handler();
	void RightButton_handler();
};

