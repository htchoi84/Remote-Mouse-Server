#include "stdafx.h"
#include "MouseCMD_handler.h"

MouseCMD_handler::MouseCMD_handler(void)
{
	//Sensitivity_value = 1;
}


MouseCMD_handler::~MouseCMD_handler(void)
{
}

void MouseCMD_handler::MouseCMD_Parsing(RXCMD RX_Data) 
{
	mouseRX = RX_Data;  //RX커맨드 저장 

	if(mouseRX.subCMD == MOUSE_KEYBOARD)   //키보드 커맨드 //커맨드 형식 mouse-cmd+KEYBOARD=문자열
	{
		KeybordCMD_handler();
	}
	else if(mouseRX.subCMD == MOUSE_LBUTTON)  //터치패드 왼쪽버튼 커맨드
	{
		Mouse_LeftButton_handler();
	}
	else if(mouseRX.subCMD == MOUSE_RBUTTON)  //터치패드 오른mouseRX.subCMD == MOUSE_RBUTTON쪽버튼 커맨드
	{
		Mouse_RightButton_handler();
	}
	else if(mouseRX.subCMD == MOUSE_TOUCH)  //마우스 좌표 조절 커맨드 //커맨드 형식 mouse-cmd+TOUCH=DOWN,100,100
	{
		//현재 들어온 좌표 정보 저장
		Change_Mouse.action = RX_Data.action; 
		Change_Mouse.X_pos = RX_Data.data1;
		Change_Mouse.Y_pos = RX_Data.data2;

		if(mouseRX.action == DOWN)
		{
			TRACE("MOUSE DOWN \n");
			//Before_Mouse = Current_Mouse;  //DOWN 이벤트가 들어오면 최초 좌표값 설정
		}
		else if(mouseRX.action == MOVE)
		{
			TRACE("MOUSE MOVE \n");
			TRACE("MouseCMD_Parsing Change_Mouse.action: %d \n",Change_Mouse.action);
			TRACE("MouseCMD_Parsing Change_Mouse.X_pos: %d \n",Change_Mouse.X_pos);
			TRACE("MouseCMD_Parsing Change_Mouse.Y_pos: %d \n",Change_Mouse.Y_pos);
			Mouse_Move_handler();
		}
		else if(mouseRX.action == UP)
		{
			TRACE("MOUSE UP \n");
		}
		else{}
	}
	else if(mouseRX.subCMD == MOUSE_WHEEL)
	{
		Mouse_Wheel_handler();
	}
	else{}
}

void MouseCMD_handler::KeybordCMD_handler()  //키보드 커맨드 핸들러
{
	CString strbuff;
	strbuff.Format("%c", mouseRX.data1);
	TRACE("keyborad String : %s \n",strbuff); 

	POINT pt;
	GetCursorPos(&pt);  //현재 마우스 위치 얻어옴
	HWND hwnd = WindowFromPoint(pt);  //마우스 좌표의 윈도우 핸들을 리턴해줌

	if(hwnd == NULL )
	{
		TRACE("핸들을 얻어올수 없음"); 
		return;
	}

	int cmd_length = strbuff.GetLength();
	char* mtext = (char*)((LPCSTR)strbuff); 

	for(int i = 0; i < cmd_length; i++)
		SendMessage(hwnd,WM_CHAR,(WPARAM) mtext[i],0);  //마우스 좌표의 윈도우에 전달되는 text 뿌려줌  
}

void MouseCMD_handler::Mouse_Move_handler()  //마우스 좌표조절 커맨드 핸들러
{
	mouse_event(MOUSEEVENTF_MOVE,Change_Mouse.X_pos,Change_Mouse.Y_pos,0,0);  //마우스 포지션 이동거리만큼 이동
}

void MouseCMD_handler::Mouse_LeftButton_handler()  //마우스 왼족버튼 커맨드 핸들러
{
	if(mouseRX.action == DOWN)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	}
	else if(mouseRX.action == UP)
	{
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
}

void MouseCMD_handler::Mouse_RightButton_handler()  //마우스 왼족버튼 커맨드 핸들러
{
	if(mouseRX.action == DOWN)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	}
	else if(mouseRX.action == UP)
	{
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
}

void MouseCMD_handler::Mouse_Wheel_handler()  //마우스 왼족버튼 커맨드 핸들러
{
	if(mouseRX.action == DOWN)
	{
		before_mousewheel_Y = mouseRX.data2;  //down시 좌표를 before에 저장 
	}
	else if(mouseRX.action == MOVE)
	{
		TRACE("Mouse_Wheel MOVE");
		mousewheel_Y_range = mouseRX.data2 - before_mousewheel_Y;
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

		before_mousewheel_Y = mouseRX.data2;  //현재 y좌표 before에 저장(현재 좌표와 비교할 수 있도록)
	}
	else if(mouseRX.action == UP)
	{
	}
	else{}
}


