#include "stdafx.h"
#include "PPTmodeCMD_handler.h"


PPTmodeCMD_handler::PPTmodeCMD_handler(void)
{
}


PPTmodeCMD_handler::~PPTmodeCMD_handler(void)
{
}


HWND hwnd_PPT;  //�Ŀ�����Ʈ ������ �ڵ� ������ ����
HWND hwnd_PPT_slideshow;  //�Ŀ�����Ʈ ������ �ڵ� ������ ����
BOOL CALLBACK EnumWindowsProc_PPT(HWND hwnd,LPARAM lParam)  
{
	char Caption[255];
	CString CaptionName;

	GetWindowText(hwnd,Caption,255);
	CaptionName = (LPSTR)(LPCSTR)Caption;
	TRACE("======CaptionName: %s \n",CaptionName); 
	if(CaptionName.Find("Microsoft PowerPoint") != -1)
	{
		hwnd_PPT = hwnd;
	}
	if(CaptionName.Find("PowerPoint �����̵� ��") != -1)
	{
		hwnd_PPT_slideshow = hwnd;
	}
	return TRUE;
}

void PPTmodeCMD_handler::PPTmodeCMD_Parsing(RXCMD RX_Data) 
{
	PPTmodeRX = RX_Data;  //RXĿ�ǵ� ���� 

	if(PPTmodeRX.subCMD == PPTMODE_BEGIN_SLIDE) 
	{
		BeginSlide_handler();
	}
	else if(PPTmodeRX.subCMD == PPTMODE_CURRENT_SLIDE) 
	{
		CurrentSlide_handler();
	}

	else if(PPTmodeRX.subCMD == PPTMODE_END_SLIDE)  
	{
		EndSlide_handler();
	}
	else if(PPTmodeRX.subCMD == PPTMODE_BEFORE_PAGE)  
	{
		BeforePage_handler();
	}
	else if(PPTmodeRX.subCMD == PPTMODE_NEXT_PAGE)  
	{
		NextPage_handler();
	}
	else if(PPTmodeRX.subCMD == PPTMODE_SENSORMOUSE)   
	{
		Current_Mouse.X_pos = PPTmodeRX.data1;
		Current_Mouse.Y_pos = PPTmodeRX.data2;
		SensorMouse_handler();
	}
	else if(PPTmodeRX.subCMD == PPTMODE_LBUTTON)   
	{
		LeftButton_handler();
	}
	else if(PPTmodeRX.subCMD == PPTMODE_RBUTTON)   
	{
		RightButton_handler();
	}
	else{}
}


void PPTmodeCMD_handler::BeginSlide_handler()  
{
	hwnd_PPT = NULL;
	EnumWindows(EnumWindowsProc_PPT, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(hwnd_PPT != NULL)
	{
		SetForegroundWindow(hwnd_PPT);  //�Ŀ�����Ʈ �ֻ��� 
		SetFocus(hwnd_PPT);

		//DownŰ�̺�Ʈ ����
		keybd_event( VK_F5,0,0,0); 
		keybd_event( VK_F5, 0, KEYEVENTF_KEYUP , 0 );
	}
}

void PPTmodeCMD_handler::CurrentSlide_handler()  
{
	hwnd_PPT = NULL;
	EnumWindows(EnumWindowsProc_PPT, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(hwnd_PPT != NULL)
	{
		SetForegroundWindow(hwnd_PPT);  //�Ŀ�����Ʈ �ֻ���
		SetFocus(hwnd_PPT);

		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_F5, 0, 0, 0);  
		keybd_event(VK_F5, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
	}
}

void PPTmodeCMD_handler::EndSlide_handler()  
{
	hwnd_PPT_slideshow = NULL;
	EnumWindows(EnumWindowsProc_PPT, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�  

	if(hwnd_PPT_slideshow != NULL)
	{
		SetForegroundWindow(hwnd_PPT_slideshow);  //�Ŀ�����Ʈ �ֻ���
		SetFocus(hwnd_PPT_slideshow);

		//Esc Ű�̺�Ʈ ����
		keybd_event( VK_ESCAPE,0,0,0); 
		keybd_event( VK_ESCAPE, 0, KEYEVENTF_KEYUP , 0 );
	}
}

void PPTmodeCMD_handler::BeforePage_handler()  
{
	hwnd_PPT = NULL;
	hwnd_PPT_slideshow = NULL;
	EnumWindows(EnumWindowsProc_PPT, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�  

	if(hwnd_PPT_slideshow != NULL)
	{
		SetForegroundWindow(hwnd_PPT_slideshow);  //�Ŀ�����Ʈ �ֻ���
		SetFocus(hwnd_PPT_slideshow);
		//Up Ű�̺�Ʈ ����
		keybd_event( VK_UP,0,0,0); 
		keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	}
	else if(hwnd_PPT != NULL)
	{
		SetForegroundWindow(hwnd_PPT);  //�Ŀ�����Ʈ �ֻ���
		SetFocus(hwnd_PPT);
		//Up Ű�̺�Ʈ ����
		keybd_event( VK_UP,0,0,0); 
		keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	}
}

void PPTmodeCMD_handler::NextPage_handler() 
{
	hwnd_PPT = NULL;
	hwnd_PPT_slideshow = NULL;
	EnumWindows(EnumWindowsProc_PPT, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�  

	if(hwnd_PPT_slideshow != NULL)
	{
		SetForegroundWindow(hwnd_PPT_slideshow);  //�Ŀ�����Ʈ �ֻ���
		SetFocus(hwnd_PPT_slideshow);

		//Down Ű�̺�Ʈ ����
		keybd_event( VK_DOWN,0,0,0); 
		keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	}
	else if(hwnd_PPT != NULL)
	{
		SetForegroundWindow(hwnd_PPT);  //�Ŀ�����Ʈ �ֻ���
		SetFocus(hwnd_PPT);
		//Down Ű�̺�Ʈ ����
		keybd_event( VK_DOWN,0,0,0); 
		keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	}
}

void PPTmodeCMD_handler::LeftButton_handler()  //�������콺 ������ư Ŀ�ǵ� �ڵ鷯
{
	if(PPTmodeRX.action == DOWN)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	}
	else if(PPTmodeRX.action == UP)
	{
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
}

void PPTmodeCMD_handler::RightButton_handler()  //�������콺 ������ư Ŀ�ǵ� �ڵ鷯
{
	if(PPTmodeRX.action == DOWN)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	}
	else if(PPTmodeRX.action == UP)
	{
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
}

void PPTmodeCMD_handler::SensorMouse_handler()  //�������콺 ��ǥ���� Ŀ�ǵ� �ڵ鷯
{
	static short X_average = 0;
	static short Y_average = 0;


	TRACE("======SensorMouse_Move_handler Current_Mouse.X_pos: %d \n",Current_Mouse.X_pos);   //X��ǥ
	TRACE("======SensorMouse_Move_handler Current_Mouse.Y_pos: %d \n",Current_Mouse.Y_pos);  //Y��ǥ

	//////////////////////���޵Ǵ� ���ӵ� 3�� ���///////////////////////
	static short X_average_buf[3] = {0,};
	static short Y_average_buf[3] = {0,};

	X_average_buf[0] = X_average_buf[1];
	X_average_buf[1] = X_average_buf[2];
	X_average_buf[2] = Current_Mouse.X_pos;

	Y_average_buf[0] = Y_average_buf[1];
	Y_average_buf[1] = Y_average_buf[2];
	Y_average_buf[2] = Current_Mouse.Y_pos;

	X_average = (X_average_buf[0] + X_average_buf[1] + X_average_buf[2])/3;
	Y_average = (Y_average_buf[0] + Y_average_buf[1] + Y_average_buf[2])/3;


	//////////////////////���޵Ǵ� ���ӵ� 5�� ���///////////////////////
	//static short X_average_buf[5] = {0,}; 
	//static short Y_average_buf[5] = {0,};

	/*X_average_buf[0] = X_average_buf[1];
	X_average_buf[1] = X_average_buf[2];
	X_average_buf[2] = X_average_buf[3];
	X_average_buf[3] = X_average_buf[4];
	X_average_buf[4] = Current_Mouse.X_pos;

	Y_average_buf[0] = Y_average_buf[1];
	Y_average_buf[1] = Y_average_buf[2];
	Y_average_buf[2] = Y_average_buf[3];
	Y_average_buf[3] = Y_average_buf[4];
	Y_average_buf[4] = Current_Mouse.Y_pos;

	X_average = (X_average_buf[0] + X_average_buf[1] + X_average_buf[2] + X_average_buf[3] + X_average_buf[4])/5;
	Y_average = (Y_average_buf[0] + Y_average_buf[1] + Y_average_buf[2] + Y_average_buf[3] + Y_average_buf[4])/5;*/

	TRACE("======SensorMouse_Move_handler X_average: %d \n",X_average);  //X��� �̵�
	TRACE("======SensorMouse_Move_handler Y_average: %d \n",Y_average);  //Y��� �̵�

	mouse_event(MOUSEEVENTF_MOVE,X_average,Y_average,0,0);
}