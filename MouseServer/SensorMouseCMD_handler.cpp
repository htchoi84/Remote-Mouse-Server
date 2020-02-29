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
	SensormouseRX = RX_Data;  //RX커맨드 저장 

	if(SensormouseRX.subCMD == SENSORMOUSE_ON) 
	{
		//is_first = true;

		//screenSize.width = GetSystemMetrics(SM_CXVIRTUALSCREEN);  //메인+서브모니터 X size
		//screenSize.height = GetSystemMetrics(SM_CYVIRTUALSCREEN);  //메인+서브모니터 Y size

		//int x_pos = screenSize.width/2;   //폰에서 좌표값 얻어올 수 있는 반경 180도 
		//int y_pos = screenSize.height/2;

		//SetCursorPos(x_pos, y_pos);   //모니터 중간에 마우스 이동

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
	else if(SensormouseRX.subCMD == SENSORMOUSE_LBUTTON)  //왼쪽버튼 커맨드
	{
		SensorMouse_LeftButton_handler();
	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_RBUTTON)  //오른쪽버튼 커맨드
	{
		SensorMouse_RightButton_handler();
	}
	else if(SensormouseRX.subCMD == SENSORMOUSE_WHEEL)  //오른쪽버튼 커맨드
	{
		SensorMouse_Wheel_handler();
	}
	else{}
}

void SensorMouseCMD_handler::SensorMouse_Move_handler()  //매직마우스 좌표조절 커맨드 핸들러
{


	static short X_average_buf[5] = {0,};
	static short Y_average_buf[5] = {0,};
	static short X_average = 0;
	static short Y_average = 0;


	TRACE("======SensorMouse_Move_handler Current_Mouse.X_pos: %d \n",Current_Mouse.X_pos);   //X좌표
	TRACE("======SensorMouse_Move_handler Current_Mouse.Y_pos: %d \n",Current_Mouse.Y_pos);  //Y좌표

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

	TRACE("======SensorMouse_Move_handler X_average: %d \n",X_average);  //X평균 이동
	TRACE("======SensorMouse_Move_handler Y_average: %d \n",Y_average);  //Y평균 이동

	mouse_event(MOUSEEVENTF_MOVE,X_average,Y_average,0,0);

}

void SensorMouseCMD_handler::SensorMouse_LeftButton_handler()  //마우스 왼족버튼 커맨드 핸들러
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

void SensorMouseCMD_handler::SensorMouse_RightButton_handler()  //마우스 왼족버튼 커맨드 핸들러
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

void SensorMouseCMD_handler::SensorMouse_Wheel_handler()  //마우스 왼족버튼 커맨드 핸들러
{
	if(SensormouseRX.action == DOWN)
	{
		before_mousewheel_Y = SensormouseRX.data2;  //down시 좌표를 before에 저장 
	}
	else if(SensormouseRX.action == MOVE)
	{
		TRACE("Mouse_Wheel MOVE");
		mousewheel_Y_range = SensormouseRX.data2 - before_mousewheel_Y;
		TRACE("Y 좌표 이동거리 : %d \n",mousewheel_Y_range);

		if(mousewheel_Y_range > 0)  //0보다 크면 밑으로 이동(아래로 스크롤)
		{
			mouse_event(MOUSEEVENTF_WHEEL,0,0,-80,0);
			TRACE("MouseWheel down");
		}
		else if(mousewheel_Y_range < 0) //0보다 작으면 위로 이동(위로 스크롤)
		{
			mouse_event(MOUSEEVENTF_WHEEL,0,0,80,0);
			TRACE("MouseWheel up");
		}
		else{}

		before_mousewheel_Y = SensormouseRX.data2;  //현재 y좌표 before에 저장(현재 좌표와 비교할 수 있도록)
	}
	else if(SensormouseRX.action == UP)
	{
	}
	else{}
}
