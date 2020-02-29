// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BA0ED324_F513_410F_A554_9CD9FABD7A80__INCLUDED_)
#define AFX_STDAFX_H__BA0ED324_F513_410F_A554_9CD9FABD7A80__INCLUDED_

#define _AFXDLL   //정적라이브러리로 배포하기 위해서 추가(htchoi)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "afxsock.h"
#include <windows.h>  //키이벤트때문에 추가 
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BA0ED324_F513_410F_A554_9CD9FABD7A80__INCLUDED_)
