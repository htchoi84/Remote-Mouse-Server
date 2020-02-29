#include "stdafx.h"
#include "SensorMouseCMD_handler.h"


SensorMouseCMD_handler::SensorMouseCMD_handler(void)
{
}


SensorMouseCMD_handler::~SensorMouseCMD_handler(void)
{
}

void SensorMouseCMD_handler::SensorMouseCMD_Parsing(RXCMD RX_Data) 
{
	SensormouseRX = RX_Data;  //RXĿ�ǵ� ���� 

	if(SensormouseRX.subCMD == SENSORMOUSE_ON) 
	{
		//is_first = true;

		//screenSize.width = GetSystemMetrics(SM_CXVIRTUALSCREEN);  //����+�������� X size
		//screenSize.height = GetSystemMetrics(SM_CYVIRTUALSCREEN);  //����+�������� Y size

		//int x_pos = screenSize.width/2;   //������ ��ǥ�� ���� �� �ִ� �ݰ� 180�� 
		//int y_pos = screenSize.height/2;

		//SetCursorPos(x_pos, y_pos);   //����� �߰��� ���콺 �̵�

	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_OFF) 
	{
		//is_first = false;
	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_MOVE)   
	{
		Current_Mouse.X_pos = SensormouseRX.data1;
		Current_Mouse.Y_pos = SensormouseRX.data2;

		SensorMouse_Move_handler();
	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_LBUTTON)  //���ʹ�ư Ŀ�ǵ�
	{
		SensorMouse_LeftButton_handler();
	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_RBUTTON)  //�����ʹ�ư Ŀ�ǵ�
	{
		SensorMouse_RightButton_handler();
	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_WHEEL)  //�����ʹ�ư Ŀ�ǵ�
	{
		SensorMouse_Wheel_handler();
	}
	else{}
}

void SensorMouseCMD_handler::SensorMouse_Move_handler()  //�������콺 ��ǥ���� Ŀ�ǵ� �ڵ鷯
{


	static short X_average_buf[5] = {0,};
	static short Y_average_buf[5] = {0,};
	static short X_average = 0;
	static short Y_average = 0;


	TRACE("======SensorMouse_Move_handler Current_Mouse.X_pos: %d \n",Current_Mouse.X_pos);   //X��ǥ
	TRACE("======SensorMouse_Move_handler Current_Mouse.Y_pos: %d \n",Current_Mouse.Y_pos);  //Y��ǥ

	X_average_buf[0] = X_average_buf[1];
	X_average_buf[1] = X_average_buf[2];
	X_average_buf[2] = X_average_buf[3];
	X_average_buf[3] = X_average_buf[4];
	X_average_buf[4] = Current_Mouse.X_pos;

	Y_average_buf[0] = Y_average_buf[1];
	Y_average_buf[1] = Y_average_buf[2];
	Y_average_buf[2] = Y_average_buf[3];
	Y_average_buf[3] = Y_average_buf[4];
	Y_average_buf[4] = Current_Mouse.Y_pos;

	if(X_average_buf[0] != 0 && X_average_buf[1] != 0 && X_average_buf[2] != 0 && X_average_buf[3] != 0 && X_average_buf[4] != 0)
	{
		X_average = (X_average_buf[0] + X_average_buf[1] + X_average_buf[2] + X_average_buf[3] + X_average_buf[4])/5;
	}
	else
	{
		X_average = 0;
	}

	if(Y_average_buf[0] != 0 && Y_average_buf[1] != 0 && Y_average_buf[2] != 0 && Y_average_buf[3] != 0 && Y_average_buf[4] != 0)
	{
		Y_average = (Y_average_buf[0] + Y_average_buf[1] + Y_average_buf[2] + Y_average_buf[3] + Y_average_buf[4])/5;
	}
	else
	{
		Y_average = 0;
	}

	TRACE("======SensorMouse_Move_handler X_average: %d \n",X_average);  //X��� �̵�
	TRACE("======SensorMouse_Move_handler Y_average: %d \n",Y_average);  //Y��� �̵�

	mouse_event(MOUSEEVENTF_MOVE,X_average,Y_average,0,0);

}

void SensorMouseCMD_handler::SensorMouse_LeftButton_handler()  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
{
	if(SensormouseRX.action == DOWN)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	}
	else if(SensormouseRX.action == UP)
	{
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
}

void SensorMouseCMD_handler::SensorMouse_RightButton_handler()  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
{
	if(SensormouseRX.action == DOWN)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	}
	else if(SensormouseRX.action == UP)
	{
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
}

void SensorMouseCMD_handler::SensorMouse_Wheel_handler()  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
{
	if(SensormouseRX.action == DOWN)
	{
		before_mousewheel_Y = SensormouseRX.data2;  //down�� ��ǥ�� before�� ���� 
	}
	else if(SensormouseRX.action == MOVE)
	{
		TRACE("Mouse_Wheel MOVE");
		mousewheel_Y_range = SensormouseRX.data2 - before_mousewheel_Y;
		TRACE("Y ��ǥ �̵��Ÿ� : %d \n",mousewheel_Y_range);

		if(mousewheel_Y_range > 0)  //0���� ũ�� ������ �̵�(�Ʒ��� ��ũ��)
		{
			mouse_event(MOUSEEVENTF_WHEEL,0,0,-80,0);
			TRACE("MouseWheel down");
		}
		else if(mousewheel_Y_range < 0) //0���� ������ ���� �̵�(���� ��ũ��)
		{
			mouse_event(MOUSEEVENTF_WHEEL,0,0,80,0);
			TRACE("MouseWheel up");
		}
		else{}

		before_mousewheel_Y = SensormouseRX.data2;  //���� y��ǥ before�� ����(���� ��ǥ�� ���� �� �ֵ���)
	}
	else if(SensormouseRX.action == UP)
	{
	}
	else{}
}
