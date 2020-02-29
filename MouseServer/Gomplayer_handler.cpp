#include "stdafx.h"
#include "Gomplayer_handler.h"


Gomplayer_handler::Gomplayer_handler(void)
{
}


Gomplayer_handler::~Gomplayer_handler(void)
{
}


//���ϴ��̾�α��� �ڵ��� ã������ �Լ� 
//��ü �������� ���μ����� ĸ���̸��� Ŭ�����̸��� ������ �ش��������ڵ��� �˾Ƴ���.

HWND Gompl_Child_hwnd;  //���÷��̾��� fileopen������ �ڵ� ������ ����
BOOL CALLBACK EnumWindowsProc_Gomplayer(HWND hwnd,LPARAM lParam)  
{
	char Caption[255];
	CString CaptionName;

	GetWindowText(hwnd,Caption,255);
	CaptionName = (LPSTR)(LPCSTR)Caption;
	if(CaptionName.Find("���� ����") != -1)  //���Ͽ��� �����츦 ã��
	{
		//AfxMessageBox(CaptionName);
		Gompl_Child_hwnd = hwnd;
	}
	return TRUE;
}

void Gomplayer_handler::GomplayerCMD_Parsing(RXCMD RX_Data) 
{
	Gomplayer_RX = RX_Data;
	TRACE("GomplayerCMD_Parsing Gomplayer_RX.subCMD : %d \n",Gomplayer_RX.subCMD);
	TRACE("GomplayerCMD_Parsing Gomplayer_RX.action : %d \n",Gomplayer_RX.action);

	if(Gomplayer_RX.action == POWER_ACTION)
	{
		POWERCMD_handler();
	}
	else if(Gomplayer_RX.action == PLAY_ACTION)
	{
		PLAYCMD_handler();
	}
	else if(Gomplayer_RX.action == OK_ACTION)
	{
		OKCMD_handler();
	}
	else if(Gomplayer_RX.action == UP_ACTION)
	{
		UPCMD_handler();
	}
	else if(Gomplayer_RX.action == DOWN_ACTION)
	{
		DOWNCMD_handler();
	}
	else if(Gomplayer_RX.action == RIGHT_ACTION)
	{
		RIGHTCMD_handler();
	}
	else if(Gomplayer_RX.action == LEFT_ACTION)
	{
		LEFTCMD_handler();
	}
	else if(Gomplayer_RX.action == STOP_ACTION)
	{
		STOPCMD_handler();
	}
	else if(Gomplayer_RX.action == FF_ACTION)
	{
		FFCMD_handler();
	}
	else if(Gomplayer_RX.action == RW_ACTION)
	{
		RWCMD_handler();
	}
	else if(Gomplayer_RX.action == VOLUMEUP_ACTION)
	{
		VOLUMEUP_CMD_handler();
	}
	else if(Gomplayer_RX.action == VOLUMEDOWN_ACTION)
	{
		VOLUMEDOWN_CMD_handler();
	}
	else if(Gomplayer_RX.action == OPEN_ACTION)
	{
		OPENCMD_handler();
	}
	else if(Gomplayer_RX.action == MUTE_ACTION)
	{
		MUTECMD_handler();
	}
	else if(Gomplayer_RX.action == CANCEL_ACTION)
	{
		CANCELCMD_handler();
	}
	else if(Gomplayer_RX.action == TAB_ACTION)
	{
		TABCMD_handler();
	}
	else
	{
		TRACE("remote �ش� subĿ�ǵ� ���� \n");
		return;
	}
}

CString Gomplayer_handler::RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName)  //HKEY_CURRENT_USER ,���� ���, �о�� key��
{
	DWORD dwType = REG_SZ;
	DWORD dwSize = 128;
	HKEY hresultKey;
	TCHAR szBuffer[128] = {'\0' ,};
	//CString reg_str;
	LONG lResult = RegOpenKeyEx(hKey,lpsubKey,0,KEY_READ,&hresultKey);
	if(ERROR_SUCCESS == lResult)
	{
		RegQueryValueEx(hresultKey , keyName , NULL , &dwType , (LPBYTE)szBuffer , &dwSize);
	}
	RegCloseKey(hresultKey);
	//reg_str=szBuffer;

	return szBuffer;
}


void Gomplayer_handler::POWERCMD_handler() 
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   //���� ����Ǿ��ִ� app�߿� GomPlayer1.x�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); //���� ����Ǿ��ִ� app�߿� ���÷��̾��� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)  //���÷��̾ ������� ���� ���� 
	{
		TRACE("Gom Player Start");
		char buff[MAX_PATH + 1];
		SHGetSpecialFolderPath(NULL,buff,CSIDL_PROGRAM_FILES, 0);  //program files�� ��θ� ���� 

		CString CMD_str = buff;
		CMD_str = CMD_str + "\\GRETECH\\GomPlayer\\GOM.EXE";

		TRACE("���� ��� : %s \n",CMD_str);

		int result = WinExec(CMD_str,SW_SHOWMAXIMIZED);
		if(result > 31)
		{
			TRACE("���� ���� result : %d \n",result);
			return;
		}
		else
		{
			TRACE("���� ���� result : %d \n",result);
			AfxMessageBox("���÷��̾� ���� ����");
			return;
		}
	}
	else  //���� ����Ǿ� ������ ����
	{
		TRACE("Gom Player end");
		SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

void Gomplayer_handler::PLAYCMD_handler() 
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   //���� ����Ǿ��ִ� app�߿� GomPlayer1.x�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); //���� ����Ǿ��ִ� app�߿� ���÷��̾��� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Gompl_Child_hwnd != NULL)  //���� ���Ⱑ �������� �� play��ư ó��(OK��ư�� �����ϰ� ó��) 
	{
		//EnterŰ�̺�Ʈ ����
		keybd_event( VK_RETURN,0,0,0); 
		Sleep(1);   //�ð� 1ms ����
		keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
		Sleep(1);
	}
	else   //���� ���Ⱑ �������� ���� �� play��ư ó�� 
	{
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		Sleep(500);

		//��� ����Ű Ctrl+p
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event(0x50, 0, 0, 0);  //0x50 => Ű���� P
		keybd_event(0x50, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);

		Sleep(1000);

		Gompl_Child_hwnd = NULL;
		EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
		if(Gompl_Child_hwnd == NULL)
		{
			//play�� ���� �� ������ �����Ǿ� �־ �ٷ� �������� Play�Ǵ� ���
			SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
			SetFocus(hwnd);
			ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		}
		else
		{
			//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
			//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
			//Up/Down/left/right Ű�� ���ؼ� Setting
			SetForegroundWindow(Gompl_Child_hwnd);
			SetFocus(Gompl_Child_hwnd);
			ShowWindow(Gompl_Child_hwnd,SW_SHOW); 

			keybd_event(VK_SHIFT, 0, 0, 0);
			keybd_event(VK_TAB, 0, 0, 0);  
			keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
			Sleep(200);
		}
	}

}

void Gomplayer_handler::OKCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//EnterŰ�̺�Ʈ ����
	keybd_event( VK_RETURN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void Gomplayer_handler::UPCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//UpŰ�̺�Ʈ ����
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::DOWNCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//DownŰ�̺�Ʈ ����
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void Gomplayer_handler::RIGHTCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	//Sleep(100);

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Right Ű�̺�Ʈ ����
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::LEFTCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Left Ű�̺�Ʈ ����
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::STOPCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		return;
	}

	//Stop ����Ű Ctrl+Space
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(VK_SPACE, 0, 0, 0);  
	keybd_event(VK_SPACE, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::FFCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		return;
	}

	//FF ����Ű Ctrl+F  //10�� ������
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(0x46, 0, 0, 0);  //Ű���� F => 0x46 
	keybd_event(0x46, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::RWCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		return;
	}

	//RW ����Ű Ctrl+B  //10�� �ڷ�
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(0x42, 0, 0, 0);   //Ű���� B => 0x42
	keybd_event(0x42, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::VOLUMEUP_CMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//UpŰ�̺�Ʈ ����  //���� ��
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::VOLUMEDOWN_CMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//DownŰ�̺�Ʈ ����  //���� �ٿ�
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::OPENCMD_handler()
{ 

	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//File open ����Ű F2  
	keybd_event(VK_F2, 0, 0, 0);  
	keybd_event(VK_F2, 0,  KEYEVENTF_KEYUP, 0);

	Sleep(500);

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Gompl_Child_hwnd == NULL)
	{
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else
	{
		//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
		//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
		//Up/Down/left/right Ű�� ���ؼ� Setting
		SetForegroundWindow(Gompl_Child_hwnd);
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 

		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_TAB, 0, 0, 0);  
		keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
		Sleep(200);
	}
}

void Gomplayer_handler::MUTECMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		//return;
	}

	//Mute ����Ű   //10�� �ڷ�
	keybd_event(0x4D, 0, 0, 0);   //Ű���� M => 0x4D
	keybd_event(0x4D, 0,  KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::CANCELCMD_handler()
{

	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 

		//CANCEL ����Ű Ctrl+B  //10�� �ڷ�
		keybd_event(VK_ESCAPE, 0, 0, 0);   //ESCŰ
		keybd_event(VK_ESCAPE, 0,KEYEVENTF_KEYUP, 0);
	}
}


void Gomplayer_handler::TABCMD_handler()
{

	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���÷��̾� �����ϼ���.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
}