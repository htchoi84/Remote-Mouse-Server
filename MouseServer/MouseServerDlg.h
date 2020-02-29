// MouseServerDlg.h : header file
//

#if !defined(AFX_MouseSERVERDLG_H__91CEEDDC_5EB4_4AE7_8175_D8A85EA9CA07__INCLUDED_)
#define AFX_MouseSERVERDLG_H__91CEEDDC_5EB4_4AE7_8175_D8A85EA9CA07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMouseServerDlg dialog
#include <winsock2.h>
#include "RemoteCMD_handler.h"
#include "MouseCMD_handler.h"
#include "SensorMouseCMD_handler.h"
#include "KeyboardCMD_handler.h"
#include "PPTmodeCMD_handler.h"
#include "RXCMD.h"
#include "PictureEx.h"	// Added by ClassView
#include "afxwin.h"

#define  WM_TRAYICON_MSG WM_USER + 1001

class CMouseServerDlg : public CDialog
{
// Construction
private:
	CStatusBar m_CStatusBar;
	CStatusBarCtrl m_StatusBar;
	CString	IP_Addr_Str;
	CString	hostname_str;
protected:
	bool m_bIsTrayIcon;
	bool m_bShowFlag;
public:
	CPictureEx m_Animation;
	CPictureEx Picture_Android;
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;

	int		m_port;
	CString	m_status;
	CString Rxstr_arr[2];   //Ŭ���̾�Ʈ���� ���޵Ǵ� data�� �߶� ���� �迭
	RemoteCMD_handler mRemote_handler;
	MouseCMD_handler mMouse_handler;
	SensorMouseCMD_handler mSensorMouseCMD_handler;
	KeyboardCMD_handler mKeyboardCMD_handler;
	PPTmodeCMD_handler mPPTmodeCMD_handler;

	byte RX_Buf[9];
	int RXLen;
	RXCMD RX_CMD;
	char TX_CMD[9];
	CWinThread* pRecvThread;
public:
	enum { IDD = IDD_MOUSE_SERVER_DIALOG };
	//void OnReceive();
	//void OnClose();
	//void OnAccept();
	bool RxDataParsing();  //Ŭ���̾�Ʈ���� ���޵Ǵ� Ŀ�ǵ� parsing�ؼ� ����� �´� �ڵ鷯�� �����ϴ� �Լ�
	short shortAt(byte buff[], int index ); //byte�迭�� short������ ��ȯ�ϴ� �Լ�
	CMouseServerDlg(CWnd* pParent = NULL);	// standard constructor
	void ErrorHandling(char* message);
	bool ServerStart();
	static UINT RecvThread(LPVOID aParam);
	CString get_IP_Adress();  //���� IP�ּ� ������ �Լ�
	void Create_StatusBar();  //StatusBar ������ IP ����Ѵ� �Լ� 
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMouseServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RegistTrayIcon();  //Ʈ���̾����� ���
	afx_msg LRESULT TrayIconMsg(WPARAM wParam, LPARAM lParam);  //WM_TRAYICON_MSG �ڵ鷯
	afx_msg void SetRestoreFromTray();
	afx_msg void OnTrayMenuitemShowdlg();
	afx_msg void OnTrayMenuitemexit();
	afx_msg void OnTrayMenuitemAbout();
	afx_msg void OnTrayMenuitemAutorun();
	BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, 
		LPCTSTR lpValue, LPCTSTR lpDefault, 
		LPTSTR lpRet, DWORD nSize);
	BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
	BOOL SHRegDeleteValue(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue);
	LRESULT OnRestartTraybar(WPARAM wParam, LPARAM lParam);
	BOOL ShowWindowEx(int nCmdShow);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMouseServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	void Socket_Connect();  //Connect ��ư click �ݹ��Լ�
	void Socket_Close();  //Close ��ư click �ݹ��Լ�
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic mPicture_Android;
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MouseSERVERDLG_H__91CEEDDC_5EB4_4AE7_8175_D8A85EA9CA07__INCLUDED_)
