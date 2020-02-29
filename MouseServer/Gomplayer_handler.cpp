#include "stdafx.h"
#include "Gomplayer_handler.h"


Gomplayer_handler::Gomplayer_handler(void)
{
}


Gomplayer_handler::~Gomplayer_handler(void)
{
}


//파일다이얼로그의 핸들을 찾기위한 함수 
//전체 실행중인 프로세스의 캡션이름과 클래스이름을 가져와 해당윈도우핸들을 알아낸다.

HWND Gompl_Child_hwnd;  //곰플레이어의 fileopen윈도우 핸들 저장할 변수
BOOL CALLBACK EnumWindowsProc_Gomplayer(HWND hwnd,LPARAM lParam)  
{
	char Caption[255];
	CString CaptionName;

	GetWindowText(hwnd,Caption,255);
	CaptionName = (LPSTR)(LPCSTR)Caption;
	if(CaptionName.Find("파일 열기") != -1)  //파일열기 윈도우를 찾음
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
		TRACE("remote 해당 sub커맨드 없음 \n");
		return;
	}
}

CString Gomplayer_handler::RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName)  //HKEY_CURRENT_USER ,서브 경로, 읽어올 key값
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
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   //현재 실행되어있는 app중에 GomPlayer1.x의 classname을 찾아서 윈도우 핸들 반환
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); //현재 실행되어있는 app중에 곰플레이어의 caption을 찾아서 윈도우 핸들 반환

	if(hwnd == NULL)  //곰플레이어가 실행되지 않은 상태 
	{
		TRACE("Gom Player Start");
		char buff[MAX_PATH + 1];
		SHGetSpecialFolderPath(NULL,buff,CSIDL_PROGRAM_FILES, 0);  //program files의 경로를 얻음 

		CString CMD_str = buff;
		CMD_str = CMD_str + "\\GRETECH\\GomPlayer\\GOM.EXE";

		TRACE("곰플 경로 : %s \n",CMD_str);

		int result = WinExec(CMD_str,SW_SHOWMAXIMIZED);
		if(result > 31)
		{
			TRACE("열기 성공 result : %d \n",result);
			return;
		}
		else
		{
			TRACE("열기 실패 result : %d \n",result);
			AfxMessageBox("곰플레이어 열기 실패");
			return;
		}
	}
	else  //곰플 실행되어 있으면 종료
	{
		TRACE("Gom Player end");
		SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

void Gomplayer_handler::PLAYCMD_handler() 
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   //현재 실행되어있는 app중에 GomPlayer1.x의 classname을 찾아서 윈도우 핸들 반환
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); //현재 실행되어있는 app중에 곰플레이어의 caption을 찾아서 윈도우 핸들 반환

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수   
	if(Gompl_Child_hwnd != NULL)  //파일 열기가 열려있을 때 play버튼 처리(OK버튼과 동일하게 처리) 
	{
		//Enter키이벤트 전달
		keybd_event( VK_RETURN,0,0,0); 
		Sleep(1);   //시간 1ms 지연
		keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
		Sleep(1);
	}
	else   //파일 열기가 열려있지 않을 때 play버튼 처리 
	{
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		Sleep(500);

		//재생 단축키 Ctrl+p
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event(0x50, 0, 0, 0);  //0x50 => 키보드 P
		keybd_event(0x50, 0,  KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);

		Sleep(1000);

		Gompl_Child_hwnd = NULL;
		EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
		if(Gompl_Child_hwnd == NULL)
		{
			//play를 누른 후 파일이 설정되어 있어서 바로 동영상이 Play되는 경우
			SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
			SetFocus(hwnd);
			ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		}
		else
		{
			//play를 누른 후 파일이 설정되어 있지 않아서 파일 열기 다이얼로그가 뜬 경우
			//파일 선택할 수 있게 상위 창으로 이동 단축키 shift+tab
			//Up/Down/left/right 키를 위해서 Setting
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
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Enter키이벤트 전달
	keybd_event( VK_RETURN,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void Gomplayer_handler::UPCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수   
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Up키이벤트 전달
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::DOWNCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Down키이벤트 전달
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void Gomplayer_handler::RIGHTCMD_handler(){
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	//Sleep(100);

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Right 키이벤트 전달
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::LEFTCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
	}

	//Left 키이벤트 전달
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::STOPCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		return;
	}

	//Stop 단축키 Ctrl+Space
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(VK_SPACE, 0, 0, 0);  
	keybd_event(VK_SPACE, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::FFCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		return;
	}

	//FF 단축키 Ctrl+F  //10초 앞으로
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(0x46, 0, 0, 0);  //키보드 F => 0x46 
	keybd_event(0x46, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::RWCMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		return;
	}

	//RW 단축키 Ctrl+B  //10초 뒤로
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(0x42, 0, 0, 0);   //키보드 B => 0x42
	keybd_event(0x42, 0,  KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::VOLUMEUP_CMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//Up키이벤트 전달  //볼륨 업
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::VOLUMEDOWN_CMD_handler()
{
	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//Down키이벤트 전달  //볼륨 다운
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void Gomplayer_handler::OPENCMD_handler()
{ 

	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//File open 단축키 F2  
	keybd_event(VK_F2, 0, 0, 0);  
	keybd_event(VK_F2, 0,  KEYEVENTF_KEYUP, 0);

	Sleep(500);

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Gompl_Child_hwnd == NULL)
	{
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else
	{
		//play를 누른 후 파일이 설정되어 있지 않아서 파일 열기 다이얼로그가 뜬 경우
		//파일 선택할 수 있게 상위 창으로 이동 단축키 shift+tab
		//Up/Down/left/right 키를 위해서 Setting
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
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 
		//return;
	}

	//Mute 단축키   //10초 뒤로
	keybd_event(0x4D, 0, 0, 0);   //키보드 M => 0x4D
	keybd_event(0x4D, 0,  KEYEVENTF_KEYUP, 0);
}

void Gomplayer_handler::CANCELCMD_handler()
{

	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 

		//CANCEL 단축키 Ctrl+B  //10초 뒤로
		keybd_event(VK_ESCAPE, 0, 0, 0);   //ESC키
		keybd_event(VK_ESCAPE, 0,KEYEVENTF_KEYUP, 0);
	}
}


void Gomplayer_handler::TABCMD_handler()
{

	HWND hwnd = FindWindow("GomPlayer1.x", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "곰플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("곰플레이어를 찾을 수 없습니다.\n 곰플레이어 실행하세요.");
		return;
	}

	Gompl_Child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Gomplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Gompl_Child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //곰플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

		keybd_event(VK_TAB, 0, 0, 0);   //TAB키
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Gompl_Child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Gompl_Child_hwnd);
		ShowWindow(Gompl_Child_hwnd,SW_SHOW); 

		keybd_event(VK_TAB, 0, 0, 0);   //TAB키
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
}