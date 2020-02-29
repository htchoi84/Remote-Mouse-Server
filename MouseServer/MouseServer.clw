; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMouseServerDlg
LastTemplate=CAsyncSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MouseServer.h"

ClassCount=4
Class1=CMouseServerApp
Class2=CMouseServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=MyMouseSocket
Resource3=IDD_MouseSERVER_DIALOG

[CLS:CMouseServerApp]
Type=0
HeaderFile=MouseServer.h
ImplementationFile=MouseServer.cpp
Filter=N

[CLS:CMouseServerDlg]
Type=0
HeaderFile=MouseServerDlg.h
ImplementationFile=MouseServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMouseServerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MouseServerDlg.h
ImplementationFile=MouseServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MouseSERVER_DIALOG]
Type=1
Class=CMouseServerDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_EDIT2,edit,1352728708
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATUS,static,1342308352
Control8=IDC_STATIC,button,1342177287

[CLS:MyMouseSocket]
Type=0
HeaderFile=MyMouseSocket.h
ImplementationFile=MyMouseSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q
LastObject=MyMouseSocket

