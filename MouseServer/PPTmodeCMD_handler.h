#pragma once

#include "RXCMD.h"

struct Position{
	short X_pos;
	short Y_pos;
};

class PPTmodeCMD_handler
{
private:
	RXCMD PPTmodeRX;  //RXdata ������ ����ü����
	Position Current_Mouse; //���콺 �̵��Ÿ� ����ü����
	//ScreenSize screenSize;
	//short before_mousewheel_Y;  //���콺 �� ���� y��ǥ ����
	//short mousewheel_Y_range;  //���콺�� y��ǥ�� �̵��Ÿ�(+�� ������ ��ũ�� -�� ���� ��ũ��)
public:
	PPTmodeCMD_handler(void);
	~PPTmodeCMD_handler(void);
	void PPTmodeCMD_Parsing(RXCMD RX_Data);
	void SensorMouse_handler();  //�������콺 ��ǥ���� Ŀ�ǵ� �ڵ鷯
	void BeginSlide_handler();
	void CurrentSlide_handler();  
	void EndSlide_handler();
	void BeforePage_handler();
	void NextPage_handler();
	void LeftButton_handler();
	void RightButton_handler();
};

