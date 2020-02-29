// MouseServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MouseServer.h"
#include "MouseServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMouseServerApp

BEGIN_MESSAGE_MAP(CMouseServerApp, CWinApp)
	//{{AFX_MSG_MAP(CMouseServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMouseServerApp construction

CMouseServerApp::CMouseServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMouseServerApp object

CMouseServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMouseServerApp initialization

BOOL CMouseServerApp::InitInstance()
{
	CWnd* pWnd = NULL;
	pWnd = CWnd::FindWindow(NULL, _T("PC control Server (for Android)"));
	if(pWnd)       // 중복 실행이 되었을 경우에는 재실행하지않고 리턴한다.
	{
		// 중복 실행 중인 프로그램 화면에 표시하는 코드
		pWnd->ShowWindow(SW_SHOW);
		pWnd->SetForegroundWindow();
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
     
    if(AfxSocketInit()==FALSE)
	{
		AfxMessageBox("Sockets Could Not Be Initialized");
		return FALSE;
	}
	CMouseServerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
