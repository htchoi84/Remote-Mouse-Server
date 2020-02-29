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
	ON_MESSAGE(WM_TRAYICON_MSG, TrayIconMsg)  //Ʈ���̾����� Ŭ���� ó���Ǵ� �ڵ鷯
	ON_COMMAND(ID_TRAY_MENUITEM_SHOWDLG, OnTrayMenuitemShowdlg)  //Ʈ���̾����� �˾����� ���� �׸� ����
	ON_COMMAND(ID_TRAY_MENUITEM_ABOUT, OnTrayMenuitemAbout)  
	ON_COMMAND(ID_TRAY_MENUITEM_EXIT, OnTrayMenuitemexit)   // Ʈ���� ������ ���� �׸� ����
	ON_COMMAND(ID_TRAY_MENUITEM_AUTORUN, OnTrayMenuitemAutorun)  //Ʈ���� ������ ������ ���۽� ���� �׸� ���ý�
	ON_REGISTERED_MESSAGE(g_uRestartTb, OnRestartTraybar)   //explorer���μ����� ���۵ɶ� ȣ��LRESULT CTrayTest01Dlg::OnRestartTraybar(WPARAM wParam, LPARAM lParam) 
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

	// explorer ���μ����� ������Ҷ� Ʈ���� �������� �ٽ� �׸��� 
	// �� �������� �޽��� ���
	g_uRestartTb = RegisterWindowMessage("TaskbarCreated");
	
	if(__argc > 1 && __argv[1])
	{
		CString str;
		str=__argv[1];
		// ù��°�� �Էµ� ���ڿ��� /mouse �̶��...
		// CompareNoCase�Լ��� ������ �񱳽� ��ҹ��ڿ� ������� ���� (�� a�� A�� ���� ����)
		if (str.CompareNoCase(AUTORUN_ADDARGU)==0)
		{
			ShowWindowEx(SW_HIDE);   //oninit���� ShowWindow�� �ȸԾ ShowWindowEX���� ����
		}
	}
	else
	{
		ShowWindowEx(SW_SHOW);
	}

	m_bIsTrayIcon = FALSE;
	RegistTrayIcon();   //Ʈ���� ������ ���

	if (m_Animation.Load(MAKEINTRESOURCE(IDR_ANIMATION),_T("GIF")))
		m_Animation.Draw();

	IP_Addr_Str = get_IP_Adress();
	Create_StatusBar();  //StatusBar ������ IP ��� 

	UpdateData(FALSE);
	Socket_Connect();  //���� ���Խú��� �ܺο��� TCP ���� �����ϵ��� server Connect

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
	  //��ܿ� x ��ư�� ��������
	 else if(nID == SC_CLOSE)
	 {
		ShowWindowEx(SW_HIDE);
	 }
	 // �ּ�ȭ ��ư�� ��������
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

//Byte �迭�� short������ ��ȯ
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
		AfxMessageBox("���� �������Դϴ�.");
	}
}

void CMouseServerDlg::Socket_Close() 
{
	DWORD dwExitCode;     //���� ���� thread ���� Ȯ�� �ڵ�...
	if (pRecvThread != NULL) 
	{
		GetExitCodeThread(pRecvThread->m_hThread , &dwExitCode);
		if(dwExitCode == STILL_ACTIVE)  //������ Ȱ��ȭ���� üũ 
		{
			TerminateThread(pRecvThread->m_hThread, 0);  //������ ���� ����
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
	pRecvThread = AfxBeginThread(RecvThread, this); //RX data recieve ������ ����
	return true;

}

UINT CMouseServerDlg::RecvThread(LPVOID aParam)
{
	CMouseServerDlg *mouseDlg = (CMouseServerDlg*)aParam;
	int clntAddrSize;

	//������ ���� �ʱ�ȭ //ù��° ���� App���� ����ϱ� ���ϴ� ���� ����// �ι�° �ý��ۿ��� �ʱ�ȭ�� �������� ��ȯ 
	if(WSAStartup(MAKEWORD(2,2),&mouseDlg->wsaData)!=0)   
	{
		mouseDlg->ErrorHandling("WSAStartup error");
	}

	mouseDlg->hServSock=socket(PF_INET, SOCK_DGRAM, 0);  //���� ���� // PF_INET => IPv4 // SOCK_DGRAM => ���� Ÿ��(UDP ����)
	if(mouseDlg->hServSock == INVALID_SOCKET)
	{
		mouseDlg->ErrorHandling("UDP ���� ���� ����");
	}

	//IP/Port setting
	memset(&mouseDlg->servAddr, 0, sizeof(mouseDlg->servAddr));  
	mouseDlg->servAddr.sin_family = AF_INET;
	mouseDlg->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	mouseDlg->servAddr.sin_port = htons(7777);

	if(bind(mouseDlg->hServSock, (SOCKADDR*)&mouseDlg->servAddr, sizeof(mouseDlg->servAddr))==SOCKET_ERROR) //IP Adress�� port ����
	{
		mouseDlg->ErrorHandling("bind() error");
	}

	Sleep(5);

	while(1)   //Ŭ���̾�Ʈ���� ���۵Ǵ� data ��ٸ�
	{
		clntAddrSize = sizeof(mouseDlg->clntAddr);

		ZeroMemory(mouseDlg->RX_Buf,9);
		mouseDlg->RXLen = recvfrom(mouseDlg->hServSock,(CHAR*)mouseDlg->RX_Buf, BUFSIZE, 0,
			(SOCKADDR*)&mouseDlg->clntAddr, &clntAddrSize);

		mouseDlg->RX_CMD.cmdLength = mouseDlg->RXLen;
		mouseDlg->RX_CMD.mainCMD = mouseDlg->RX_Buf[0];
		mouseDlg->RX_CMD.subCMD = mouseDlg->RX_Buf[1];
		mouseDlg->RX_CMD.action = mouseDlg->RX_Buf[2];
		mouseDlg->RX_CMD.data1 = mouseDlg->shortAt(mouseDlg->RX_Buf,3);  //��ȯ�� �����ε��� �־��ָ� �ȴ�. //index=3,4
		mouseDlg->RX_CMD.data2 = mouseDlg->shortAt(mouseDlg->RX_Buf,5);  //��ȯ index=5,6
		mouseDlg->RX_CMD.data3 = mouseDlg->shortAt(mouseDlg->RX_Buf,7);  //��ȯ index=7,8

		if(mouseDlg->RXLen==SOCKET_ERROR)
		{
			AfxMessageBox("Could not Recieve");
		}
		else
		{
			if(mouseDlg->RX_CMD.mainCMD == CONNECT_CMD)  //Connect Ŀ�ǵ� �϶��� ���������� Connect�� �Ǿ��ٴ� Ŀ�ǵ带 Client�� �ٽ�����
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
		TRACE("������Ŀ�ǵ�\n");
		mRemote_handler.RemoteCMD_Parsing(RX_CMD);
	}
	else if(RX_CMD.mainCMD == MOUSE_CMD)
	{
		TRACE("���콺Ŀ�ǵ�\n");
		mMouse_handler.MouseCMD_Parsing(RX_CMD);
	}
	else if(RX_CMD.mainCMD == PPTMODE_CMD)
	{
		TRACE("PPTĿ�ǵ�\n");
		mPPTmodeCMD_handler.PPTmodeCMD_Parsing(RX_CMD);
	}
	else if(RX_CMD.mainCMD == KEYBOARD_CMD)
	{
		TRACE("Ű����Ŀ�ǵ�\n");
		mKeyboardCMD_handler.KeyboardCMD_Parsing(RX_CMD);
	}
	/*else if(RX_CMD.mainCMD == SENSORMOUSE_CMD)
	{
		TRACE("�������콺Ŀ�ǵ�\n");
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Socket_Close(); 

	if(m_bIsTrayIcon) // ���� Ʈ���� ���������� �����Ǿ����� Ȯ�� 
	{
		NOTIFYICONDATA  nid;
		nid.cbSize = sizeof(nid);
		nid.hWnd = m_hWnd; // ���� ������ �ڵ�
		nid.uID = IDR_MAINFRAME;

		// �۾� ǥ����(TaskBar)�� ���� ������ �������� �����Ѵ�.
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

	hostname_str = (LPCTSTR)(LPSTR)LocalName;  //ȣ��Ʈ���� ����- statusbar�� ������ؼ�

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
	CFont* font = new CFont(); //CFont object Ȯ��

	font->CreateFont( // object�� ����
		12, // ���� ��
		12, // ���� ���� 
		0, // ���� ����
		0, // ���� ����
		FW_HEAVY, // ���� ����
		FALSE, // ���� ��︲ ���
		FALSE, // �� ��
		FALSE, // ��Ҽ�
		DEFAULT_CHARSET, // ���� ��
		OUT_DEFAULT_PRECIS, // ��� ��Ȯ��
		CLIP_DEFAULT_PRECIS, // ų���� ��Ȯ��
		DEFAULT_QUALITY, // ����� ��
		DEFAULT_PITCH, // �ڰ� �Ÿ�
		"�ü�" // ���� ���
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

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch(nCtlColor)
	{
		case CTLCOLOR_DLG:
			//return (HBRUSH)GetStockObject(BLACK_BRUSH);   //���̾�α� ���� ����
			break;
	}    
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


void CMouseServerDlg::RegistTrayIcon()
{
	NOTIFYICONDATA  nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // ���� ������ �ڵ�
	nid.uID = IDR_MAINFRAME;  // ������ ���ҽ� ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // �÷��� ����
	nid.uCallbackMessage = WM_TRAYICON_MSG; // �ݹ�޽��� ����
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // ������ �ε�
	char strTitle[256];
	GetWindowText(strTitle, sizeof(strTitle)); // ĸ�ǹٿ� ��µ� ���ڿ� ����
	lstrcpy(nid.szTip, strTitle); 
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bIsTrayIcon = TRUE;
}

// WM_TRAYICON_MSG �� �޼����� ������ ����..
LRESULT CMouseServerDlg::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{

	switch(lParam)
	{
	case WM_LBUTTONDBLCLK:   // Ʈ���̾����� ���ʹ�ư ����Ŭ���� â�� �ٽ� ������.
		{ 
			ShowWindowEx(SW_SHOWNORMAL);
			//ShowWindowEx(SW_SHOW);
		}
		break;
	case WM_RBUTTONDOWN:     // Ʈ���̾����� �����ʹ�ư Ŭ���� �˾��޴�â ����.
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
	RegistTrayIcon();   //explorer ���μ��� ����۽� �ٽ� Ʈ���� ������ ���
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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