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
	mouseRX = RX_Data;  //RXĿ�ǵ� ���� 

	if(mouseRX.subCMD == MOUSE_KEYBOARD)   //Ű���� Ŀ�ǵ� //Ŀ�ǵ� ���� mouse-cmd+KEYBOARD=���ڿ�
	{
		KeybordCMD_handler();
	}
	else if(mouseRX.subCMD == MOUSE_LBUTTON)  //��ġ�е� ���ʹ�ư Ŀ�ǵ�
	{
		Mouse_LeftButton_handler();
	}
	else if(mouseRX.subCMD == MOUSE_RBUTTON)  //��ġ�е� ����mouseRX.subCMD == MOUSE_RBUTTON�ʹ�ư Ŀ�ǵ�
	{
		Mouse_RightButton_handler();
	}
	else if(mouseRX.subCMD == MOUSE_TOUCH)  //���콺 ��ǥ ���� Ŀ�ǵ� //Ŀ�ǵ� ���� mouse-cmd+TOUCH=DOWN,100,100
	{
		//���� ���� ��ǥ ���� ����
		Change_Mouse.action = RX_Data.action; 
		Change_Mouse.X_pos = RX_Data.data1;
		Change_Mouse.Y_pos = RX_Data.data2;

		if(mouseRX.action == DOWN)
		{
			TRACE("MOUSE DOWN \n");
			//Before_Mouse = Current_Mouse;  //DOWN �̺�Ʈ�� ������ ���� ��ǥ�� ����
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

void MouseCMD_handler::KeybordCMD_handler()  //Ű���� Ŀ�ǵ� �ڵ鷯
{
	CString strbuff;
	strbuff.Format("%c", mouseRX.data1);
	TRACE("keyborad String : %s \n",strbuff); 

	POINT pt;
	GetCursorPos(&pt);  //���� ���콺 ��ġ ����
	HWND hwnd = WindowFromPoint(pt);  //���콺 ��ǥ�� ������ �ڵ��� ��������

	if(hwnd == NULL )
	{
		TRACE("�ڵ��� ���ü� ����"); 
		return;
	}

	int cmd_length = strbuff.GetLength();
	char* mtext = (char*)((LPCSTR)strbuff); 

	for(int i = 0; i < cmd_length; i++)
		SendMessage(hwnd,WM_CHAR,(WPARAM) mtext[i],0);  //���콺 ��ǥ�� �����쿡 ���޵Ǵ� text �ѷ���  
}

void MouseCMD_handler::Mouse_Move_handler()  //���콺 ��ǥ���� Ŀ�ǵ� �ڵ鷯
{
	mouse_event(MOUSEEVENTF_MOVE,Change_Mouse.X_pos,Change_Mouse.Y_pos,0,0);  //���콺 ������ �̵��Ÿ���ŭ �̵�
}

void MouseCMD_handler::Mouse_LeftButton_handler()  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
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

void MouseCMD_handler::Mouse_RightButton_handler()  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
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

void MouseCMD_handler::Mouse_Wheel_handler()  //���콺 ������ư Ŀ�ǵ� �ڵ鷯
{
	if(mouseRX.action == DOWN)
	{
		before_mousewheel_Y = mouseRX.data2;  //down�� ��ǥ�� before�� ���� 
	}
	else if(mouseRX.action == MOVE)
	{
		TRACE("Mouse_Wheel MOVE");
		mousewheel_Y_range = mouseRX.data2 - before_mousewheel_Y;
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

		before_mousewheel_Y = mouseRX.data2;  //���� y��ǥ before�� ����(���� ��ǥ�� ���� �� �ֵ���)
	}
	else if(mouseRX.action == UP)
	{
	}
	else{}
}


