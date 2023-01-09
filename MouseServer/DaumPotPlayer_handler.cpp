#include "stdafx.h"
#include "DaumPotPlayer_handler.h"


DaumPotPlayer_handler::DaumPotPlayer_handler(void)
{
}


DaumPotPlayer_handler::~DaumPotPlayer_handler(void)
{
}


//���ϴ��̾�α��� �ڵ��� ã������ �Լ� 
//��ü �������� ���μ����� ĸ���̸��� Ŭ�����̸��� ������ �ش��������ڵ��� �˾Ƴ���.

HWND Daumplayer_child_hwnd;  //���� ���÷��̾��� fileopen������ �ڵ� ������ ����
BOOL CALLBACK EnumWindowsProc_Daumplayer(HWND hwnd,LPARAM lParam)   
{
	char Caption_buff[255];
	char Class_buff[255];
	CString CaptionName;
	CString ClassName;

	GetClassName( hwnd, Class_buff, 256); // Ŭ�����̸�
	GetWindowText(hwnd,Caption_buff,256);  //ĸ�� �̸� 
	CaptionName = (LPSTR)(LPCSTR)Caption_buff;
	ClassName = (LPSTR)(LPCSTR)Class_buff;
	TRACE("EnumWindowsProc_Daumplayer CaptionName : %s \n",CaptionName);
	TRACE("EnumWindowsProc_Daumplayer ClassName : %s \n",ClassName);
	if(CaptionName.Find("����") != -1 
		|| (CaptionName.Find("[") != -1 && CaptionName.Find("]") != -1 && CaptionName.Find("/") != -1))  //ex) [1/2]�� ������ ����ÿ� ���Ͽ��� â�� ĸ�� ã��
	{
		//AfxMessageBox(CaptionName);
		Daumplayer_child_hwnd = hwnd;
	}
	return TRUE;
}

void DaumPotPlayer_handler::DaumPotPlayerCMD_Parsing(RXCMD RX_Data) 
{
	DaumPotPlayer_RX = RX_Data;
	TRACE("���÷��̾�CMD_Parsing DaumPotPlayer_RX.subCMD : %d \n",DaumPotPlayer_RX.subCMD);
	TRACE("���÷��̾�CMD_Parsing DaumPotPlayer_RX.action : %d \n",DaumPotPlayer_RX.action);

	if(DaumPotPlayer_RX.action == POWER_ACTION)
	{
		POWERCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == PLAY_ACTION)
	{
		PLAYCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == OK_ACTION)
	{
		OKCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == UP_ACTION)
	{
		UPCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == DOWN_ACTION)
	{
		DOWNCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == RIGHT_ACTION)
	{
		RIGHTCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == LEFT_ACTION)
	{
		LEFTCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == STOP_ACTION)
	{
		STOPCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == FF_ACTION)
	{
		FFCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == RW_ACTION)
	{
		RWCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == VOLUMEUP_ACTION)
	{
		VOLUMEUP_CMD_handler();
	}
	else if(DaumPotPlayer_RX.action == VOLUMEDOWN_ACTION)
	{
		VOLUMEDOWN_CMD_handler();
	}
	else if(DaumPotPlayer_RX.action == OPEN_ACTION)
	{
		OPENCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == MUTE_ACTION)
	{
		MUTECMD_handler();
	}
	else if(DaumPotPlayer_RX.action == CANCEL_ACTION)
	{
		CANCELCMD_handler();
	}
	else if(DaumPotPlayer_RX.action == TAB_ACTION)
	{
		TABCMD_handler();
	}
	else
	{
		TRACE("remote �ش� subĿ�ǵ� ���� \n");
		return;
	}
}

CString DaumPotPlayer_handler::RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName)  //HKEY_CURRENT_USER ,���� ���, �о�� key��
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


void DaumPotPlayer_handler::POWERCMD_handler() 
{
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    

	HWND hwnd = FindWindow("PotPlayer", NULL);   //���� ����Ǿ��ִ� app�߿� PotPlayer�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); //���� ����Ǿ��ִ� app�߿� ���÷��̾��� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)  //���÷��̾ ������� ���� ���� 
	{
		TRACE("DaumPotPlayer Start");
		char buff[MAX_PATH + 1];
		SHGetSpecialFolderPath(NULL,buff,CSIDL_PROGRAM_FILES, 0);  //program files�� ��θ� ���� 
		CString CMD_str = buff;
		CMD_str = CMD_str + "\\Daum\\PotPlayer\\PotPlayer.exe";

		TRACE("DaumPotPlayer ��� : %s \n",CMD_str);

		int result = WinExec(CMD_str,SW_SHOWMAXIMIZED);
		if(result > 31)
		{
			TRACE("���� ���� result : %d \n",result);
			return;
		}
		else
		{
			TRACE("���� ���� result : %d \n",result);
			AfxMessageBox("�˼� ���� ����");
			return;
		}
	}
	else  //���� ����Ǿ� ������ ����
	{
		TRACE("DaumPotPlayer end");
		SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

void DaumPotPlayer_handler::PLAYCMD_handler() 
{
	HWND hwnd = FindWindow("PotPlayer", NULL);   //���� ����Ǿ��ִ� app�߿� PotPlayer�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); //���� ����Ǿ��ִ� app�߿� ���÷��̾��� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)
	{
		//AfxMessageBox("���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Daumplayer_child_hwnd != NULL)  //���� ���Ⱑ �������� �� play��ư ó��(OK��ư�� �����ϰ� ó��) 
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

		//��� ����Ű space
		keybd_event(VK_SPACE, 0, 0, 0);
		keybd_event(VK_SPACE, 0,KEYEVENTF_KEYUP, 0);

		Sleep(1000);

		Daumplayer_child_hwnd = NULL;
		EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
		if(Daumplayer_child_hwnd == NULL)
		{
			//play�� ���� �� ������ �����Ǿ� �־ �ٷ� �������� Play�Ǵ� ���
			SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
			SetFocus(hwnd);
			ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		}
		else
		{
			//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
			//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
			//Up/Down/left/right Ű�� ���ؼ� Setting
			SetForegroundWindow(Daumplayer_child_hwnd);
			SetFocus(Daumplayer_child_hwnd);
			ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 

			keybd_event(VK_SHIFT, 0, 0, 0);
			keybd_event(VK_TAB, 0, 0, 0);  
			keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
			Sleep(200);
		}
	}

}

void DaumPotPlayer_handler::OKCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//EnterŰ�̺�Ʈ ����
	keybd_event( VK_RETURN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void DaumPotPlayer_handler::UPCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//UpŰ�̺�Ʈ ����
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::DOWNCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//DownŰ�̺�Ʈ ����
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void DaumPotPlayer_handler::RIGHTCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	//Sleep(100);

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Right Ű�̺�Ʈ ����
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::LEFTCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Left Ű�̺�Ʈ ����
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::STOPCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		return;
	}

	//Stop ����Ű F4
	keybd_event(VK_F4, 0, 0, 0);    
	keybd_event(VK_F4, 0,  KEYEVENTF_KEYUP, 0);
}

void DaumPotPlayer_handler::FFCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		return;
	}

	//FF ����Ű Right  //10�� ������
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::RWCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		return;
	}

	//RW ����Ű left  //10�� �ڷ�
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::VOLUMEUP_CMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
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

void DaumPotPlayer_handler::VOLUMEDOWN_CMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
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

void DaumPotPlayer_handler::OPENCMD_handler(){ 

	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//File open ����Ű F2  
	keybd_event(VK_F3, 0, 0, 0);  
	keybd_event(VK_F3, 0,  KEYEVENTF_KEYUP, 0);

	Sleep(500);

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Daumplayer_child_hwnd == NULL)
	{
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else
	{
		//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
		//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
		//Up/Down/left/right Ű�� ���ؼ� Setting
		SetForegroundWindow(Daumplayer_child_hwnd);
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 

		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_TAB, 0, 0, 0);  
		keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
		Sleep(200);
	}
}

void DaumPotPlayer_handler::MUTECMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		//return;
	}

	//Mute ����Ű 
	keybd_event(0x4D, 0, 0, 0);   //Ű���� M => 0x4D
	keybd_event(0x4D, 0,  KEYEVENTF_KEYUP, 0);
}

void DaumPotPlayer_handler::CANCELCMD_handler(){

	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("���� ���÷��̾ ã�� �� �����ϴ�.\n ���� ���÷��̾� �����ϼ���.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //���� ���÷��̾� ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 

		//CANCEL ����Ű Ctrl+B  //10�� �ڷ�
		keybd_event(VK_ESCAPE, 0, 0, 0);   //ESCŰ
		keybd_event(VK_ESCAPE, 0,KEYEVENTF_KEYUP, 0);
	}
}

void DaumPotPlayer_handler::TABCMD_handler()
{

	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "���÷��̾�"); 

	if(hwnd == NULL)
	{
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
}