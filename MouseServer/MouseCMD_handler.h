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
	RXCMD mouseRX;  //RXdata ������ ����ü����
	MousePosition Change_Mouse; //���콺 �̵��Ÿ� ����ü����
	short before_mousewheel_Y;  //���콺 �� ���� y��ǥ ����
	short mousewheel_Y_range;  //���콺�� y��ǥ�� �̵��Ÿ�(+�� ������ ��ũ�� -�� ���� ��ũ��)
public:
	MouseCMD_handler(void);
	~MouseCMD_handler(void);
	void MouseCMD_Parsing(RXCMD RX_Data); //Ŀ�ǵ�� parsing�ϴ� �Լ� 
	void KeybordCMD_handler();  //Ű���� Ŀ�ǵ� �ڵ鷯
	void Mouse_Move_handler(); //���콺 ��ǥ���� Ŀ�ǵ� �ڵ鷯 
	void Mouse_RightButton_handler();  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
	void Mouse_LeftButton_handler();  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
	void Mouse_Wheel_handler();  //���콺 �� Ŀ�ǵ� �ڵ鷯

};

#endif