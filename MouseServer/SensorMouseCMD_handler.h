#pragma once

#include "RXCMD.h"

struct SensorMousePosition{
	short X_pos;
	short Y_pos;
};

class SensorMouseCMD_handler
{
private:
	RXCMD SensormouseRX;  //RXdata ������ ����ü����
	SensorMousePosition Current_Mouse; //���콺 �̵��Ÿ� ����ü����
	ScreenSize screenSize;
	short before_mousewheel_Y;  //���콺 �� ���� y��ǥ ����
	short mousewheel_Y_range;  //���콺�� y��ǥ�� �̵��Ÿ�(+�� ������ ��ũ�� -�� ���� ��ũ��)
public:
	SensorMouseCMD_handler(void);
	~SensorMouseCMD_handler(void);
	void SensorMouseCMD_Parsing(RXCMD RX_Data);
	void SensorMouse_Move_handler();  //�������콺 ��ǥ���� Ŀ�ǵ� �ڵ鷯
	void SensorMouse_Wheel_handler();
	void SensorMouse_RightButton_handler();
	void SensorMouse_LeftButton_handler();
};

