// MouseServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MouseServer.h"
#include "MouseServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUFSIZE 9
#define RUNKEY             "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define AUTORUN_VALUENAME  "MouseServer"
#define AUTORUN_ADDARGU    "/mouse"  
UINT g_uRestartTb;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMouseServerDlg dialog

CMouseServerDlg::CMouseServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMouseServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMouseServerDlg)
	m_port = 7777;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMouseServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOADING_ANIMATION, m_Animation);
	//{{AFX_DATA_MAP(CMouseServerDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMouseServerDlg, CDialog)
	//{{AFX_MSG_MAP(CMouseServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_TRAYICON_MSG, TrayIconMsg)  //트레이아이콘 클릭시 처리되는 핸들러
	ON_COMMAND(ID_TRAY_MENUITEM_SHOWDLG, OnTrayMenuitemShowdlg)  //트레이아이콘 팝업에서 열기 항목 선택
	ON_COMMAND(ID_TRAY_MENUITEM_ABOUT, OnTrayMenuitemAbout)  
	ON_COMMAND(ID_TRAY_MENUITEM_EXIT, OnTrayMenuitemexit)   // 트레이 아이콘 종료 항목 선택
	ON_COMMAND(ID_TRAY_MENUITEM_AUTORUN, OnTrayMenuitemAutorun)  //트레이 아이콘 윈도우 시작시 실행 항목 선택시
	ON_REGISTERED_MESSAGE(g_uRestartTb, OnRestartTraybar)   //explorer프로세스가 시작될때 호출LRESULT CTrayTest01Dlg::OnRestartTraybar(WPARAM wParam, LPARAM lParam) 
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMouseServerDlg message handlers

BOOL CMouseServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_port=7777;

	hServSock = NULL;
	pRecvThread = NULL;

	// explorer 프로세스가 재시작할때 트레이 아이콘을 다시 그리게 
	// 할 목적으로 메시지 등록
	g_uRestartTb = RegisterWindowMessage("TaskbarCreated");
	
	if(__argc > 1 && __argv[1])
	{
		CString str;
		str=__argv[1];
		// 첫번째로 입력된 문자열이 /mouse 이라면...
		// CompareNoCase함수는 영문자 비교시 대소문자에 상관없이 비교함 (즉 a와 A는 같은 문자)
		if (str.CompareNoCase(AUTORUN_ADDARGU)==0)
		{
			ShowWindowEx(SW_HIDE);   //oninit에서 ShowWindow가 안먹어서 ShowWindowEX새로 정의
		}
	}
	else
	{
		ShowWindowEx(SW_SHOW);
	}

	m_bIsTrayIcon = FALSE;
	RegistTrayIcon();   //트레이 아이콘 등록

	if (m_Animation.Load(MAKEINTRESOURCE(IDR_ANIMATION),_T("GIF")))
		m_Animation.Draw();

	IP_Addr_Str = get_IP_Adress();
	Create_StatusBar();  //StatusBar 생성후 IP 출력 

	UpdateData(FALSE);
	Socket_Connect();  //최초 진입시부터 외부에서 TCP 접속 가능하도록 server Connect

	//ServerStart();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMouseServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	  //상단에 x 버튼을 눌렀을때
	 else if(nID == SC_CLOSE)
	 {
		ShowWindowEx(SW_HIDE);
	 }
	 // 최소화 버튼을 눌렀을때
	 else if(nID == SC_MINIMIZE)
	 { 
		 ShowWindowEx(SW_MINIMIZE);
	 }
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMouseServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMouseServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//Byte 배열을 short형으로 변환
short CMouseServerDlg::shortAt(byte buff[], int index ) {
	return (short) (((buff[index] << 8)) | ((buff[index + 1] & 0xff)));
};

void CMouseServerDlg::Socket_Connect() 
{
	if(hServSock == NULL && pRecvThread == NULL)
	{
		ServerStart();
	}
	else
	{
		AfxMessageBox("아직 접속중입니다.");
	}
}

void CMouseServerDlg::Socket_Close() 
{
	DWORD dwExitCode;     //저장 관련 thread 종료 확인 코드...
	if (pRecvThread != NULL) 
	{
		GetExitCodeThread(pRecvThread->m_hThread , &dwExitCode);
		if(dwExitCode == STILL_ACTIVE)  //쓰레드 활성화인지 체크 
		{
			TerminateThread(pRecvThread->m_hThread, 0);  //쓰레드 강제 종료
			CloseHandle(pRecvThread->m_hThread); 
			pRecvThread = NULL;
		}
	}

	closesocket(hServSock);
	hServSock = NULL;
	WSACleanup();
}

void CMouseServerDlg::ErrorHandling(char* message)
{
	TRACE("================== ERROR = %s",message);
	exit(1);
}

bool CMouseServerDlg::ServerStart() 
{
	pRecvThread = AfxBeginThread(RecvThread, this); //RX data recieve 쓰레드 생성
	return true;

}

UINT CMouseServerDlg::RecvThread(LPVOID aParam)
{
	CMouseServerDlg *mouseDlg = (CMouseServerDlg*)aParam;
	int clntAddrSize;

	//윈도우 소켓 초기화 //첫번째 인자 App에서 사용하기 원하는 소켓 버전// 두번째 시스템에서 초기화된 소켓정보 반환 
	if(WSAStartup(MAKEWORD(2,2),&mouseDlg->wsaData)!=0)   
	{
		mouseDlg->ErrorHandling("WSAStartup error");
	}

	mouseDlg->hServSock=socket(PF_INET, SOCK_DGRAM, 0);  //소켓 생성 // PF_INET => IPv4 // SOCK_DGRAM => 소켓 타입(UDP 사용시)
	if(mouseDlg->hServSock == INVALID_SOCKET)
	{
		mouseDlg->ErrorHandling("UDP 소켓 생성 오류");
	}

	//IP/Port setting
	memset(&mouseDlg->servAddr, 0, sizeof(mouseDlg->servAddr));  
	mouseDlg->servAddr.sin_family = AF_INET;
	mouseDlg->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	mouseDlg->servAddr.sin_port = htons(7777);

	if(bind(mouseDlg->hServSock, (SOCKADDR*)&mouseDlg->servAddr, sizeof(mouseDlg->servAddr))==SOCKET_ERROR) //IP Adress와 port 연결
	{
		mouseDlg->ErrorHandling("bind() error");
	}

	Sleep(5);

	while(1)   //클라이언트에서 전송되는 data 기다림
	{
		clntAddrSize = sizeof(mouseDlg->clntAddr);

		ZeroMemory(mouseDlg->RX_Buf,9);
		mouseDlg->RXLen = recvfrom(mouseDlg->hServSock,(CHAR*)mouseDlg->RX_Buf, BUFSIZE, 0,
			(SOCKADDR*)&mouseDlg->clntAddr, &clntAddrSize);

		mouseDlg->RX_CMD.cmdLength = mouseDlg->RXLen;
		mouseDlg->RX_CMD.mainCMD = mouseDlg->RX_Buf[0];
		mouseDlg->RX_CMD.subCMD = mouseDlg->RX_Buf[1];
		mouseDlg->RX_CMD.action = mouseDlg->RX_Buf[2];
		mouseDlg->RX_CMD.data1 = mouseDlg->shortAt(mouseDlg->RX_Buf,3);  //변환할 시작인덱스 넣어주면 된다. //index=3,4
		mouseDlg->RX_CMD.data2 = mouseDlg->shortAt(mouseDlg->RX_Buf,5);  //변환 index=5,6
		mouseDlg->RX_CMD.data3 = mouseDlg->shortAt(mouseDlg->RX_Buf,7);  //변환 index=7,8

		if(mouseDlg->RXLen==SOCKET_ERROR)
		{
			AfxMessageBox("Could not Recieve");
		}
		else
		{
			if(mouseDlg->RX_CMD.mainCMD == CONNECT_CMD)  //Connect 커맨드 일때는 정상적으로 Connect가 되었다는 커맨드를 Client로 다시전송
			{
				mouseDlg->TX_CMD[0] = CONNECT_CMD;
				sendto(mouseDlg->hServSock, (CHAR*)mouseDlg->TX_CMD, BUFSIZE, 0, (SOCKADDR*)&mouseDlg->clntAddr, sizeof(mouseDlg->clntAddr));
			}

			mouseDlg->RxDataParsing();
		}
	}
}


bool CMouseServerDlg::RxDataParsing() 
{
	if(RX_CMD.mainCMD == REMOTE_CMD)
	{
		TRACE("리모컨커맨드\n");
		mRemote_handler.RemoteCMD_Parsing(RX_CMD);
	}
	else if(RX_CMD.mainCMD == MOUSE_CMD)
	{
		TRACE("마우스커맨드\n");
		mMouse_handler.MouseCMD_Parsing(RX_CMD);
	}
	else if(RX_CMD.mainCMD == PPTMODE_CMD)
	{
		TRACE("PPT커맨드\n");
		mPPTmodeCMD_handler.PPTmodeCMD_Parsing(RX_CMD);
	}
	else if(RX_CMD.mainCMD == KEYBOARD_CMD)
	{
		TRACE("키보드커맨드\n");
		mKeyboardCMD_handler.KeyboardCMD_Parsing(RX_CMD);
	}
	/*else if(RX_CMD.mainCMD == SENSORMOUSE_CMD)
	{
		TRACE("센서마우스커맨드\n");
		SensorMouseCMD_handler.SensorMouseCMD_Parsing(RX_CMD);
	}*/
	else
	{
		return true;
	}
	return true;
}

void CMouseServerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Socket_Close(); 

	if(m_bIsTrayIcon) // 현재 트레이 아이콘으로 설정되었는지 확인 
	{
		NOTIFYICONDATA  nid;
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hWnd; // 메인 윈도우 핸들
		nid.uID = IDR_MAINFRAME;

		// 작업 표시줄(TaskBar)의 상태 영역에 아이콘을 삭제한다.
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}

}

CString CMouseServerDlg::get_IP_Adress() 
{
	WSADATA wsa;
	IN_ADDR addr;

	char LocalName[256] = {0.};
    char IPAddr[16] = {0,}; 
	char i = 0;

	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) 
		return "";

	if(gethostname(LocalName, 256) == SOCKET_ERROR) 
		return "";

	hostname_str = (LPCTSTR)(LPSTR)LocalName;  //호스트네임 저장- statusbar에 출력위해서

	HOSTENT *ptr = gethostbyname(LocalName);
	if(ptr == NULL) 
		return "";

	while(ptr->h_addr_list[i] != NULL)
	{
		memcpy(&addr, ptr->h_addr_list[i], ptr->h_length);
		wsprintf(IPAddr, "%s", inet_ntoa(addr));
		i++;
	}

	WSACleanup();
	Sleep(100);

	return (LPCTSTR)(LPSTR)IPAddr;
}

void CMouseServerDlg::Create_StatusBar() 
{
	CFont* font = new CFont(); //CFont object 확보

	font->CreateFont( // object의 생성
		12, // 문자 폭
		12, // 문자 높이 
		0, // 기울기 각도
		0, // 문자 방향
		FW_HEAVY, // 문자 굵기
		FALSE, // 문자 기울림 모양
		FALSE, // 밑 줄
		FALSE, // 취소선
		DEFAULT_CHARSET, // 문자 셋
		OUT_DEFAULT_PRECIS, // 출력 정확도
		CLIP_DEFAULT_PRECIS, // 킬립핑 정확도
		DEFAULT_QUALITY, // 출력의 질
		DEFAULT_PITCH, // 자간 거리
		"궁서" // 문자 모양
		);

	CRect rect;
	this->GetClientRect(&rect);	//get client rect...

	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);

	int strPartDim[4]= {rect.Width()/2, rect.Width()};
	m_StatusBar.SetParts(2, strPartDim);

	m_StatusBar.SetText(hostname_str, 0, 0);
	m_StatusBar.SetIcon(0, AfxGetApp()->LoadIcon(IDI_HOST));
	m_StatusBar.SetText(IP_Addr_Str, 1, 0);
	m_StatusBar.SetIcon(1, AfxGetApp()->LoadIcon(IDI_IP));
	m_StatusBar.SetFont(font);
	m_StatusBar.SetBkColor(RGB(255, 255, 255));

	delete font;
}


HBRUSH CMouseServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch(nCtlColor)
	{
		case CTLCOLOR_DLG:
			//return (HBRUSH)GetStockObject(BLACK_BRUSH);   //다이얼로그 배경색 변경
			break;
	}    
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CMouseServerDlg::RegistTrayIcon()
{
	NOTIFYICONDATA  nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // 메인 윈도우 핸들
	nid.uID = IDR_MAINFRAME;  // 아이콘 리소스 ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
	nid.uCallbackMessage = WM_TRAYICON_MSG; // 콜백메시지 설정
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 아이콘 로드
	char strTitle[256];
	GetWindowText(strTitle, sizeof(strTitle)); // 캡션바에 출력된 문자열 얻음
	lstrcpy(nid.szTip, strTitle); 
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bIsTrayIcon = TRUE;
}

// WM_TRAYICON_MSG 이 메세지를 받으면 구동..
LRESULT CMouseServerDlg::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{

	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:   // 트레이아이콘 왼쪽버튼 더블클릭시 창이 다시 열린다.
		{ 
			ShowWindowEx(SW_SHOWNORMAL);
			//ShowWindowEx(SW_SHOW);
		}
		break;
	case WM_RBUTTONDOWN:     // 트레이아이콘 오른쪽버튼 클릭시 팝업메뉴창 띄운다.
		{
			CPoint ptMouse;
			::GetCursorPos(&ptMouse);

			char   szStr[MAX_PATH*3];
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TRAYPOPUP);	
			CMenu *pMenu = menu.GetSubMenu(0);
			pMenu->CheckMenuItem(ID_TRAY_MENUITEM_AUTORUN, MF_UNCHECKED);
			if (SHRegReadString(HKEY_LOCAL_MACHINE, RUNKEY, AUTORUN_VALUENAME, "", szStr, MAX_PATH*2))
			{
				CString str;
				str=szStr;
				str.TrimLeft();
				str.TrimRight();

				if (str.GetLength() > 0)
				{
					if (str.GetAt(0)=='\"') str.Delete(0);

					int index1=str.Find(" /");
					str.Delete(index1-1, str.GetLength()+1-index1);
				}

				if (str.CompareNoCase(__argv[0])==0)
				{
					pMenu->CheckMenuItem(ID_TRAY_MENUITEM_AUTORUN, MF_CHECKED);
				}
			}

			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				ptMouse.x, ptMouse.y, AfxGetMainWnd());
		}
		break;
	}
	return 0;
}

void CMouseServerDlg::OnTrayMenuitemShowdlg() 
{
	// TODO: Add your command handler code here
	//SetRestoreFromTray();
	//ShowWindowEx(SW_NORMAL);
	ShowWindowEx(SW_SHOWNORMAL);
	//ShowWindowEx(SW_SHOW);
}

void CMouseServerDlg::OnTrayMenuitemexit() 
{
	// TODO: Add your command handler code here
	DestroyWindow();
}

void CMouseServerDlg::OnTrayMenuitemAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();	
}

void CMouseServerDlg::OnTrayMenuitemAutorun() 
{
	CString strWrite;
	char    szStr[MAX_PATH*3]={0};

	if (SHRegReadString(HKEY_LOCAL_MACHINE, RUNKEY, AUTORUN_VALUENAME, " ", szStr, MAX_PATH*2))
	{
		CString str;
		str=szStr;
		str.TrimLeft();
		str.TrimRight();
		if (str.GetLength() > 0)
		{
			if (str.GetAt(0)=='\"') str.Delete(0);

			int index1=str.Find(" /");
			str.Delete(index1-1, str.GetLength()+1-index1);			
		}

		if (str.CompareNoCase(__argv[0])!=0)
		{
			strWrite="\""; 
			strWrite+= __argv[0];
			strWrite+= "\" ";
			strWrite+= AUTORUN_ADDARGU;
			SHRegWriteString(HKEY_LOCAL_MACHINE, RUNKEY, AUTORUN_VALUENAME, strWrite);
			//AfxMessageBox("SHRegWriteString");
		}else{
			SHRegDeleteValue(HKEY_LOCAL_MACHINE, RUNKEY, AUTORUN_VALUENAME);
			//AfxMessageBox("DeleteReg");
		}
	}else{
		strWrite="\""; 
		strWrite+= __argv[0];
		strWrite+= "\" ";
		strWrite+= AUTORUN_ADDARGU;
		SHRegWriteString(HKEY_LOCAL_MACHINE, RUNKEY, AUTORUN_VALUENAME, strWrite);
	}
}


BOOL CMouseServerDlg::SHRegReadString(HKEY hKey, LPCTSTR lpKey, 
	LPCTSTR lpValue, LPCTSTR lpDefault, 
	LPTSTR lpRet, DWORD nSize) 
{
	HKEY  key;
	DWORD dwDisp;
	DWORD Size;

	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return FALSE;

	Size=nSize;

	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)lpRet, &Size)
		!=ERROR_SUCCESS) {
			strcpy(lpRet, lpDefault);
			return FALSE;
	}

	RegCloseKey(key);

	return TRUE;
}


BOOL CMouseServerDlg::SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY  key;
	DWORD dwDisp;

	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return FALSE;

	if (RegSetValueEx(key, lpValue,0,REG_SZ,(LPBYTE)lpData,strlen(lpData)+1)
		!=ERROR_SUCCESS) 
		return FALSE;

	RegCloseKey(key);

	return TRUE;
}


BOOL CMouseServerDlg::SHRegDeleteValue(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue)
{
	HKEY  key;

	if (RegOpenKeyEx(hKey, lpKey, 0, KEY_ALL_ACCESS, &key)!=ERROR_SUCCESS) 
		return FALSE;

	if (RegDeleteValue(key, lpValue)!=ERROR_SUCCESS) 
		return FALSE;

	RegCloseKey(key);

	return TRUE;
}

LRESULT CMouseServerDlg::OnRestartTraybar(WPARAM wParam, LPARAM lParam) 
{ 
	m_bIsTrayIcon = FALSE;
	RegistTrayIcon();   //explorer 프로세스 재시작시 다시 트레이 아이콘 등록
	//ShowWindow(SW_MINIMIZE);
	return 0;
} 

void CMouseServerDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here
	if(m_bShowFlag)
		lpwndpos->flags |= SWP_SHOWWINDOW;
	else
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
}


BOOL CMouseServerDlg::ShowWindowEx(int nCmdShow)
{
	m_bShowFlag = (nCmdShow == SW_SHOW) || (nCmdShow == SW_SHOWNORMAL);
	return (GetSafeHwnd()) ? ShowWindow(nCmdShow) : TRUE;
}

BOOL CMouseServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( pMsg -> message == WM_KEYDOWN)
	{
		if( pMsg -> wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if(pMsg -> wParam == VK_ESCAPE)
		{
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}