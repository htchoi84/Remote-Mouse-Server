#pragma once

#include "RXCMD.h"

struct SensorMousePosition{
	short X_pos;
	short Y_pos;
};

class SensorMouseCMD_handler
{
private:
	RXCMD SensormouseRX;  //RXdata 저장할 구조체변수
	SensorMousePosition Current_Mouse; //마우스 이동거리 구조체변수
	ScreenSize screenSize;
	short before_mousewheel_Y;  //마우스 휠 이전 y좌표 저장
	short mousewheel_Y_range;  //마우스휠 y좌표의 이동거리(+면 밑으로 스크롤 -면 위로 스크롤)
public:
	SensorMouseCMD_handler(void);
	~SensorMouseCMD_handler(void);
	void SensorMouseCMD_Parsing(RXCMD RX_Data);
	void SensorMouse_Move_handler();  //매직마우스 좌표조절 커맨드 핸들러
	void SensorMouse_Wheel_handler();
	void SensorMouse_RightButton_handler();
	void SensorMouse_LeftButton_handler();
};

