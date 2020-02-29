#include "stdafx.h"
#include "MediaPlayer_handler.h"


MediaPlayer_handler::MediaPlayer_handler(void)
{
}


MediaPlayer_handler::~MediaPlayer_handler(void)
{
}


//���ϴ��̾�α��� �ڵ��� ã������ �Լ� 
//��ü �������� ���μ����� ĸ���̸��� Ŭ�����̸��� ������ �ش��������ڵ��� �˾Ƴ���.

HWND Meidapl_Child_hwnd; 
BOOL CALLBACK EnumWindowsProc_Mediaplayer(HWND hwnd,LPARAM lParam)  
{
	char Caption[255];
	CString CaptionName;

	GetWindowText(hwnd,Caption,255);
	CaptionName = (LPSTR)(LPCSTR)Caption;
	TRACE("EnumWindowsProc_media player CaptionName : %s \n",CaptionName);
	if(CaptionName.Find("����") != -1 || CaptionName.Find("������Ʈ") != -1)  //���Ͽ��� �����츦 ã��
	{
		//AfxMessageBox(CaptionName);
		Meidapl_Child_hwnd = hwnd;
	}
	return TRUE;
}

void MediaPlayer_handler::MediaplayerCMD_Parsing(RXCMD RX_Data) 
{
	Mediaplayer_RX = RX_Data;
	TRACE("MediaplayerCMD_Parsing Mediaplayer_RX.subCMD : %d \n",Mediaplayer_RX.subCMD);
	TRACE("MediaplayerCMD_Parsing Mediaplayer_RX.action : %d \n",Mediaplayer_RX.action);

	if(Mediaplayer_RX.action == POWER_ACTION)
	{
		POWERCMD_handler();
	}
	else if(Mediaplayer_RX.action == PLAY_ACTION)
	{
		PLAYCMD_handler();
	}
	else if(Mediaplayer_RX.action == OK_ACTION)
	{
		OKCMD_handler();
	}
	else if(Mediaplayer_RX.action == UP_ACTION)
	{
		UPCMD_handler();
	}
	else if(Mediaplayer_RX.action == DOWN_ACTION)
	{
		DOWNCMD_handler();
	}
	else if(Mediaplayer_RX.action == RIGHT_ACTION)
	{
		RIGHTCMD_handler();
	}
	else if(Mediaplayer_RX.action == LEFT_ACTION)
	{
		LEFTCMD_handler();
	}
	else if(Mediaplayer_RX.action == STOP_ACTION)
	{
		STOPCMD_handler();
	}
	else if(Mediaplayer_RX.action == FF_ACTION)
	{
		FFCMD_handler();
	}
	else if(Mediaplayer_RX.action == RW_ACTION)
	{
		RWCMD_handler();
	}
	else if(Mediaplayer_RX.action == VOLUMEUP_ACTION)
	{
		VOLUMEUP_CMD_handler();
	}
	else if(Mediaplayer_RX.action == VOLUMEDOWN_ACTION)
	{
		VOLUMEDOWN_CMD_handler();
	}
	else if(Mediaplayer_RX.action == OPEN_ACTION)
	{
		OPENCMD_handler();
	}
	else if(Mediaplayer_RX.action == MUTE_ACTION)
	{
		MUTECMD_handler();
	}
	else if(Mediaplayer_RX.action == CANCEL_ACTION)
	{
		CANCELCMD_handler();
	}
	else if(Mediaplayer_RX.action == TAB_ACTION)
	{
		TABCMD_handler();
	}
	else
	{
		TRACE("remote �ش� subĿ�ǵ� ���� \n");
		return;
	}
}

CString MediaPlayer_handler::RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName)  //HKEY_CURRENT_USER ,���� ���, �о�� key��
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


void MediaPlayer_handler::POWERCMD_handler() 
{

	HWND hwnd = FindWindow(NULL, "Windows Media Player");   //���� ����Ǿ��ִ� app�߿� CabinetWClass�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); //���� ����Ǿ��ִ� app�߿� Windows Media Player�� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)  //Windows Media Player�� ������� ���� ���� 
	{
		TRACE("Media Player Start");
		char buff[MAX_PATH + 1];
		SHGetSpecialFolderPath(NULL,buff,CSIDL_PROGRAM_FILES, 0);  //program files�� ��θ� ���� 

		CString CMD_str = buff;
		CMD_str = CMD_str + "\\Windows Media Player\\wmplayer.exe";

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
			AfxMessageBox("�̵���÷��̾� ���� ����");
			return;
		}
	}
	else  //���� ����Ǿ� ������ ����
	{
		TRACE("Media Player end");
		SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

void MediaPlayer_handler::PLAYCMD_handler() 
{
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   //���� ����Ǿ��ִ� app�߿� CabinetWClass�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); //���� ����Ǿ��ִ� app�߿� Windows Media Player�� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Meidapl_Child_hwnd != NULL)  //���� ���Ⱑ �������� �� play��ư ó��(OK��ư�� �����ϰ� ó��) 
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

		Meidapl_Child_hwnd = NULL;
		EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
		if(Meidapl_Child_hwnd == NULL)
		{
			//play�� ���� �� ������ �����Ǿ� �־ �ٷ� �������� Play�Ǵ� ���
			SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
			SetFocus(hwnd);
			ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		}
		else
		{
			//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
			//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
			//Up/Down/left/right Ű�� ���ؼ� Setting
			SetForegroundWindow(Meidapl_Child_hwnd);
			SetFocus(Meidapl_Child_hwnd);
			ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 

			keybd_event(VK_SHIFT, 0, 0, 0);
			keybd_event(VK_TAB, 0, 0, 0);  
			keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
			Sleep(200);
		}
	}

}

void MediaPlayer_handler::OKCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
	}

	//EnterŰ�̺�Ʈ ����
	keybd_event( VK_RETURN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void MediaPlayer_handler::UPCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
	}

	//UpŰ�̺�Ʈ ����
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void MediaPlayer_handler::DOWNCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
	}

	//DownŰ�̺�Ʈ ����
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void MediaPlayer_handler::RIGHTCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	//Sleep(100);

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
	}

	//Right Ű�̺�Ʈ ����
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void MediaPlayer_handler::LEFTCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
	}

	//Left Ű�̺�Ʈ ����
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void MediaPlayer_handler::STOPCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
		return;
	}

	//Stop ����Ű Ctrl+S
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(0x53, 0, 0, 0);  
	keybd_event(0x53, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void MediaPlayer_handler::FFCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
		return;
	}

	//FF ����Ű Ctrl+F  //10�� ������
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(0x46, 0, 0, 0);  //Ű���� F => 0x46 
	keybd_event(0x46, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void MediaPlayer_handler::RWCMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
		return;
	}

	//RW ����Ű Ctrl+SHIFT+B  //10�� �ڷ�
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(0x42, 0, 0, 0);   //Ű���� B => 0x42
	keybd_event(0x42, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void MediaPlayer_handler::VOLUMEUP_CMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//UpŰ�̺�Ʈ ����  //F9
	keybd_event( VK_F9,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_F9, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void MediaPlayer_handler::VOLUMEDOWN_CMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//DownŰ�̺�Ʈ ����  //F8
	keybd_event( VK_F8,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_F8, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void MediaPlayer_handler::OPENCMD_handler(){ 

	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//File open ����Ű ctrl + o
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(0x4f, 0, 0, 0);    //O => 0x4f
	keybd_event(0x4f, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);

	Sleep(500);

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Meidapl_Child_hwnd == NULL)
	{
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else
	{
		//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
		//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
		//Up/Down/left/right Ű�� ���ؼ� Setting
		SetForegroundWindow(Meidapl_Child_hwnd);
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 

		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_TAB, 0, 0, 0);  
		keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
		Sleep(200);
	}
}

void MediaPlayer_handler::MUTECMD_handler(){
	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 
		//return;
	}

	//Mute ����Ű  F7
	keybd_event(VK_F7, 0, 0, 0);   //Ű���� M => 0x4D
	keybd_event(VK_F7, 0,  KEYEVENTF_KEYUP, 0);
}

void MediaPlayer_handler::CANCELCMD_handler(){

	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("Windows Media Player�� ã�� �� �����ϴ�.\n Windows Media Player �����ϼ���.");
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //Windows Media Player ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 

		//CANCEL ����Ű Ctrl+B  //10�� �ڷ�
		keybd_event(VK_ESCAPE, 0, 0, 0);   //ESCŰ
		keybd_event(VK_ESCAPE, 0,KEYEVENTF_KEYUP, 0);
	}
}


void MediaPlayer_handler::TABCMD_handler()
{

	HWND hwnd = FindWindow(NULL, "Windows Media Player");   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "Windows Media Player"); 

	if(hwnd == NULL)
	{
		return;
	}

	Meidapl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Mediaplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Meidapl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Meidapl_Child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Meidapl_Child_hwnd);
		ShowWindow(Meidapl_Child_hwnd,SW_SHOW); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
}