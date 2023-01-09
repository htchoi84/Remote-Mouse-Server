#include "stdafx.h"
#include "DaumPotPlayer_handler.h"


DaumPotPlayer_handler::DaumPotPlayer_handler(void)
{
}


DaumPotPlayer_handler::~DaumPotPlayer_handler(void)
{
}


//파일다이얼로그의 핸들을 찾기위한 함수 
//전체 실행중인 프로세스의 캡션이름과 클래스이름을 가져와 해당윈도우핸들을 알아낸다.

HWND Daumplayer_child_hwnd;  //다음 팟플레이어의 fileopen윈도우 핸들 저장할 변수
BOOL CALLBACK EnumWindowsProc_Daumplayer(HWND hwnd,LPARAM lParam)   
{
	char Caption_buff[255];
	char Class_buff[255];
	CString CaptionName;
	CString ClassName;

	GetClassName( hwnd, Class_buff, 256); // 클래스이름
	GetWindowText(hwnd,Caption_buff,256);  //캡션 이름 
	CaptionName = (LPSTR)(LPCSTR)Caption_buff;
	ClassName = (LPSTR)(LPCSTR)Class_buff;
	TRACE("EnumWindowsProc_Daumplayer CaptionName : %s \n",CaptionName);
	TRACE("EnumWindowsProc_Daumplayer ClassName : %s \n",ClassName);
	if(CaptionName.Find("열기") != -1 
		|| (CaptionName.Find("[") != -1 && CaptionName.Find("]") != -1 && CaptionName.Find("/") != -1))  //ex) [1/2]는 동영상 재생시에 파일열기 창의 캡션 찾음
	{
		//AfxMessageBox(CaptionName);
		Daumplayer_child_hwnd = hwnd;
	}
	return TRUE;
}

void DaumPotPlayer_handler::DaumPotPlayerCMD_Parsing(RXCMD RX_Data) 
{
	DaumPotPlayer_RX = RX_Data;
	TRACE("팟플레이어CMD_Parsing DaumPotPlayer_RX.subCMD : %d \n",DaumPotPlayer_RX.subCMD);
	TRACE("팟플레이어CMD_Parsing DaumPotPlayer_RX.action : %d \n",DaumPotPlayer_RX.action);

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
		TRACE("remote 해당 sub커맨드 없음 \n");
		return;
	}
}

CString DaumPotPlayer_handler::RegReadString(HKEY hKey,LPCTSTR lpsubKey,LPCTSTR keyName)  //HKEY_CURRENT_USER ,서브 경로, 읽어올 key값
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
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    

	HWND hwnd = FindWindow("PotPlayer", NULL);   //현재 실행되어있는 app중에 PotPlayer의 classname을 찾아서 윈도우 핸들 반환
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); //현재 실행되어있는 app중에 팟플레이어의 caption을 찾아서 윈도우 핸들 반환

	if(hwnd == NULL)  //팟플레이어가 실행되지 않은 상태 
	{
		TRACE("DaumPotPlayer Start");
		char buff[MAX_PATH + 1];
		SHGetSpecialFolderPath(NULL,buff,CSIDL_PROGRAM_FILES, 0);  //program files의 경로를 얻음 
		CString CMD_str = buff;
		CMD_str = CMD_str + "\\Daum\\PotPlayer\\PotPlayer.exe";

		TRACE("DaumPotPlayer 경로 : %s \n",CMD_str);

		int result = WinExec(CMD_str,SW_SHOWMAXIMIZED);
		if(result > 31)
		{
			TRACE("열기 성공 result : %d \n",result);
			return;
		}
		else
		{
			TRACE("열기 실패 result : %d \n",result);
			AfxMessageBox("알쇼 열기 실패");
			return;
		}
	}
	else  //곰플 실행되어 있으면 종료
	{
		TRACE("DaumPotPlayer end");
		SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

void DaumPotPlayer_handler::PLAYCMD_handler() 
{
	HWND hwnd = FindWindow("PotPlayer", NULL);   //현재 실행되어있는 app중에 PotPlayer의 classname을 찾아서 윈도우 핸들 반환
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); //현재 실행되어있는 app중에 팟플레이어의 caption을 찾아서 윈도우 핸들 반환

	if(hwnd == NULL)
	{
		//AfxMessageBox("팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수   
	if(Daumplayer_child_hwnd != NULL)  //파일 열기가 열려있을 때 play버튼 처리(OK버튼과 동일하게 처리) 
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

		//재생 단축키 space
		keybd_event(VK_SPACE, 0, 0, 0);
		keybd_event(VK_SPACE, 0,KEYEVENTF_KEYUP, 0);

		Sleep(1000);

		Daumplayer_child_hwnd = NULL;
		EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
		if(Daumplayer_child_hwnd == NULL)
		{
			//play를 누른 후 파일이 설정되어 있어서 바로 동영상이 Play되는 경우
			SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
			SetFocus(hwnd);
			ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
		}
		else
		{
			//play를 누른 후 파일이 설정되어 있지 않아서 파일 열기 다이얼로그가 뜬 경우
			//파일 선택할 수 있게 상위 창으로 이동 단축키 shift+tab
			//Up/Down/left/right 키를 위해서 Setting
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
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Enter키이벤트 전달
	keybd_event( VK_RETURN,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_RETURN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void DaumPotPlayer_handler::UPCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수   
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Up키이벤트 전달
	keybd_event( VK_UP,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_UP, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::DOWNCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Down키이벤트 전달
	keybd_event( VK_DOWN,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_DOWN, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}


void DaumPotPlayer_handler::RIGHTCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	//Sleep(100);

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Right 키이벤트 전달
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::LEFTCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
	}

	//Left 키이벤트 전달
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::STOPCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		return;
	}

	//Stop 단축키 F4
	keybd_event(VK_F4, 0, 0, 0);    
	keybd_event(VK_F4, 0,  KEYEVENTF_KEYUP, 0);
}

void DaumPotPlayer_handler::FFCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수    
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		return;
	}

	//FF 단축키 Right  //10초 앞으로
	keybd_event( VK_RIGHT,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_RIGHT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::RWCMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		return;
	}

	//RW 단축키 left  //10초 뒤로
	keybd_event( VK_LEFT,0,0,0); 
	Sleep(1);   //시간 1ms 지연
	keybd_event( VK_LEFT, 0, KEYEVENTF_KEYUP , 0 );
	Sleep(1);
}

void DaumPotPlayer_handler::VOLUMEUP_CMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
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

void DaumPotPlayer_handler::VOLUMEDOWN_CMD_handler(){
	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
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

void DaumPotPlayer_handler::OPENCMD_handler(){ 

	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
	SetFocus(hwnd);
	ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

	//File open 단축키 F2  
	keybd_event(VK_F3, 0, 0, 0);  
	keybd_event(VK_F3, 0,  KEYEVENTF_KEYUP, 0);

	Sleep(500);

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Daumplayer_child_hwnd == NULL)
	{
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else
	{
		//play를 누른 후 파일이 설정되어 있지 않아서 파일 열기 다이얼로그가 뜬 경우
		//파일 선택할 수 있게 상위 창으로 이동 단축키 shift+tab
		//Up/Down/left/right 키를 위해서 Setting
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
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 
		//return;
	}

	//Mute 단축키 
	keybd_event(0x4D, 0, 0, 0);   //키보드 M => 0x4D
	keybd_event(0x4D, 0,  KEYEVENTF_KEYUP, 0);
}

void DaumPotPlayer_handler::CANCELCMD_handler(){

	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		//AfxMessageBox("다음 팟플레이어를 찾을 수 없습니다.\n 다음 팟플레이어 실행하세요.");
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  //다음 팟플레이어 윈도우 최상위
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 

		//CANCEL 단축키 Ctrl+B  //10초 뒤로
		keybd_event(VK_ESCAPE, 0, 0, 0);   //ESC키
		keybd_event(VK_ESCAPE, 0,KEYEVENTF_KEYUP, 0);
	}
}

void DaumPotPlayer_handler::TABCMD_handler()
{

	HWND hwnd = FindWindow("PotPlayer", NULL);   
	if(hwnd == NULL)
		hwnd = FindWindow(NULL, "팟플레이어"); 

	if(hwnd == NULL)
	{
		return;
	}

	Daumplayer_child_hwnd = NULL;
	EnumWindows(EnumWindowsProc_Daumplayer, NULL);  //파일 열기 창의 윈도우 핸들 찾고 저장하는 콜백함수 
	if(Daumplayer_child_hwnd == NULL)
	{		
		SetForegroundWindow(hwnd);  
		SetFocus(hwnd);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED); 

		keybd_event(VK_TAB, 0, 0, 0);   //TAB키
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
	else  //파일 다이얼로그가 열려있는 경우
	{
		SetForegroundWindow(Daumplayer_child_hwnd);  //파일다이얼로그 윈도우 최상위
		SetFocus(Daumplayer_child_hwnd);
		ShowWindow(Daumplayer_child_hwnd,SW_SHOW); 

		keybd_event(VK_TAB, 0, 0, 0);   //TAB키
		keybd_event(VK_TAB, 0,KEYEVENTF_KEYUP, 0);
	}
}