#include "stdafx.h"
#include "ALshow_handler.h"


ALshow_handler::ALshow_handler(void)
{
}


ALshow_handler::~ALshow_handler(void)
{
}


//���ϴ��̾�α��� �ڵ��� ã������ �Լ� 
//��ü �������� ���μ����� ĸ���̸��� Ŭ�����̸��� ������ �ش��������ڵ��� �˾Ƴ���.

HWND Alshow_child_hwnd;  //ALShow�� fileopen������ �ڵ� ������ ����
BOOL CALLBACK EnumWindowsProc_ALShow(HWND hwnd,LPARAM lParam)  
{
	char Caption[255];
	CString CaptionName;

	GetWindowText(hwnd,Caption,255);
	CaptionName = (LPSTR)(LPCSTR)Caption;
	if(CaptionName.Find("������ ����") != -1 
		|| CaptionName.Find("�⺻ ���α׷� ����") != -1)  //���Ͽ��� �����츦 ã��
	{
		//AfxMessageBox(CaptionName);
		Alshow_child_hwnd = hwnd;
	}
	return TRUE;
}

void ALshow_handler::ALShowCMD_Parsing(RXCMD RX_Data) 
{
	ALShow_RX = RX_Data;
	TRACE("ALShowCMD_Parsing ALShow_RX.subCMD : %d \n",ALShow_RX.subCMD);
	TRACE("ALShowCMD_Parsing ALShow_RX.action : %d \n",ALShow_RX.action);

	if(ALShow_RX.action == POWER_ACTION)
	{
		POWERCMD_handler();
	}
	else if(ALShow_RX.action == PLAY_ACTION)
	{
		PLAYCMD_handler();
	}
	else if(ALShow_RX.action == OK_ACTION)
	{
		OKCMD_handler();
	}
	else if(ALShow_RX.action == UP_ACTION)
	{
		UPCMD_handler();
	}
	else if(ALShow_RX.action == DOWN_ACTION)
	{
		DOWNCMD_handler();
	}
	else if(ALShow_RX.action == RIGHT_ACTION)
	{
		RIGHTCMD_handler();
	}
	else if(ALShow_RX.action == LEFT_ACTION)
	{
		LEFTCMD_handler();
	}
	else if(ALShow_RX.action == STOP_ACTION)
	{
		STOPCMD_handler();
	}
	else if(ALShow_RX.action == FF_ACTION)
	{
		FFCMD_handler();
	}
	else if(ALShow_RX.action == RW_ACTION)
	{
		RWCMD_handler();
	}
	else if(ALShow_RX.action == VOLUMEUP_ACTION)
	{
		VOLUMEUP_CMD_handler();
	}
	else if(ALShow_RX.action == VOLUMEDOWN_ACTION)
	{
		VOLUMEDOWN_CMD_handler();
	}
	else if(ALShow_RX.action == OPEN_ACTION)
	{
		OPENCMD_handler();
	}
	else if(ALShow_RX.action == MUTE_ACTION)
	{
		MUTECMD_handler();
	}
	else if(ALShow_RX.action == CANCEL_ACTION)
	{
		CANCELCMD_handler();
	}
	else if(ALShow_RX.action == TAB_ACTION)
	{
		TABCMD_handler();
	}
	else
	{
		TRACE("remote �ش� subĿ�ǵ� ���� \n");
		return;
	}
}

CString ALshow_handler::RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName)  //HKEY_CURRENT_USER ,���� ���, �о�� key��
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


void ALshow_handler::POWERCMD_handler() 
{
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   //���� ����Ǿ��ִ� app�߿� ALShowMainWindow�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); //���� ����Ǿ��ִ� app�߿� ALShow�� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)  //ALShow�� ������� ���� ���� 
	{
		TRACE("Alshow Start");
		char buff[MAX_PATH + 1];
		SHGetSpecialFolderPath(NULL,buff,CSIDL_PROGRAM_FILES, 0);  //program files�� ��θ� ���� 
		CString CMD_str = buff;
		CMD_str = CMD_str + "\\ESTsoft\\ALShow\\ALShow.exe";

		TRACE("�˼� ��� : %s \n",CMD_str);

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
		TRACE("Alshow end");
		SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

void ALshow_handler::PLAYCMD_handler() 
{
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   //���� ����Ǿ��ִ� app�߿� ALShowMainWindow�� classname�� ã�Ƽ� ������ �ڵ� ��ȯ
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); //���� ����Ǿ��ִ� app�߿� ALShow�� caption�� ã�Ƽ� ������ �ڵ� ��ȯ

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Alshow_child_hwnd != NULL)  //���� ���Ⱑ �������� �� play��ư ó��(OK��ư�� �����ϰ� ó��) 
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

		Alshow_child_hwnd = NULL;
		EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
		if(Alshow_child_hwnd == NULL)
		{
			//play�� ���� �� ������ �����Ǿ� �־ �ٷ� �������� Play�Ǵ� ���
			SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
			SetFocus(hwnd);
			ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		}
		else
		{
			//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
			//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
			//Up/Down/left/right Ű�� ���ؼ� Setting
			SetForegroundWindow(Alshow_child_hwnd);
			SetFocus(Alshow_child_hwnd);
			ShowWindow(Alshow_child_hwnd,SW_SHOW); 

			keybd_event(VK_SHIFT, 0, 0, 0);
			keybd_event(VK_TAB, 0, 0, 0);  
			keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
			Sleep(200);
		}
	}

}

void ALshow_handler::OKCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
	}

	//EnterŰ�̺�Ʈ ����
	keybd_event( VK_RETURN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void ALshow_handler::UPCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�   
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
	}

	//UpŰ�̺�Ʈ ����
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void ALshow_handler::DOWNCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
	}

	//DownŰ�̺�Ʈ ����
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void ALshow_handler::RIGHTCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	//Sleep(100);

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
	}

	//Right Ű�̺�Ʈ ����
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void ALshow_handler::LEFTCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
	}

	//Left Ű�̺�Ʈ ����
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void ALshow_handler::STOPCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
		return;
	}

	//Stop ����Ű S
	keybd_event(0x53, 0, 0, 0);    //S => 0x53
	keybd_event(0x53, 0,  KEYEVENTF_KEYUP, 0);
}

void ALshow_handler::FFCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ�    
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
		return;
	}

	//FF ����Ű Right  //10�� ������
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void ALshow_handler::RWCMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
		return;
	}

	//RW ����Ű left  //10�� �ڷ�
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //�ð� 1ms ����
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void ALshow_handler::VOLUMEUP_CMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
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

void ALshow_handler::VOLUMEDOWN_CMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
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

void ALshow_handler::OPENCMD_handler(){ 

	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//File open ����Ű F2  
	keybd_event(VK_F2, 0, 0, 0);  
	keybd_event(VK_F2, 0,  KEYEVENTF_KEYUP, 0);

	Sleep(500);

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Alshow_child_hwnd == NULL)
	{
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else
	{
		//play�� ���� �� ������ �����Ǿ� ���� �ʾƼ� ���� ���� ���̾�αװ� �� ���
		//���� ������ �� �ְ� ���� â���� �̵� ����Ű shift+tab
		//Up/Down/left/right Ű�� ���ؼ� Setting
		SetForegroundWindow(Alshow_child_hwnd);
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 

		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_TAB, 0, 0, 0);  
		keybd_event(VK_TAB, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SHIFT, 0,KEYEVENTF_KEYUP, 0);
		Sleep(200);
	}
}

void ALshow_handler::MUTECMD_handler(){
	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 
		//return;
	}

	//Mute ����Ű   //10�� �ڷ�
	keybd_event(0x4D, 0, 0, 0);   //Ű���� M => 0x4D
	keybd_event(0x4D, 0,  KEYEVENTF_KEYUP, 0);
}

void ALshow_handler::CANCELCMD_handler(){

	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("ALShow�� ã�� �� �����ϴ�.\n ALShow �����ϼ���.");
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //ALShow ������ �ֻ���
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 

		//CANCEL ����Ű Ctrl+B  //10�� �ڷ�
		keybd_event(VK_ESCAPE, 0, 0, 0);   //ESCŰ
		keybd_event(VK_ESCAPE, 0,KEYEVENTF_KEYUP, 0);
	}
}

void ALshow_handler::TABCMD_handler()
{

	HWND hwnd = FindWindow("ALShowMainWindow", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "ALShow"); 

	if(hwnd == NULL)
	{
		return;
	}

	Alshow_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_ALShow, NULL);  //���� ���� â�� ������ �ڵ� ã�� �����ϴ� �ݹ��Լ� 
	if(Alshow_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
	else  //���� ���̾�αװ� �����ִ� ���
	{
		SetForegroundWindow(Alshow_child_hwnd);  //���ϴ��̾�α� ������ �ֻ���
		SetFocus(Alshow_child_hwnd);
		ShowWindow(Alshow_child_hwnd,SW_SHOW); 

		keybd_event(VK_TAB, 0, 0, 0);   //TABŰ
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
}