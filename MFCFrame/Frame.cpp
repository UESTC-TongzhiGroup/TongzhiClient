
// Frame.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Frame.h"
#include "MainFrame.h"
#include "UserLogin.h"
#include "Config.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFrameApp

BEGIN_MESSAGE_MAP(CFrameApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFrameApp 构造

CFrameApp::CFrameApp()
{
}


// 唯一的一个 CFrameApp 对象

CFrameApp theApp;


// CFrameApp 初始化

BOOL CFrameApp::InitInstance()
{
	//_CrtSetBreakAlloc(1169);
	CWinApp::InitInstance();

	CUserLogin ldlg;
	if (ldlg.DoModal() == IDOK)
	{
		/*******创建窗口*********/
		Config::loadServerCfg();
		CMainFrame* pMainFrame = new CMainFrame();
		pMainFrame->CreateEx(0, _T("MAINFRAME"),
			_T("BT    客户端"),
			WS_POPUPWINDOW | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL);
		//__PreCreateWindow 注册窗口类
		m_pMainWnd = pMainFrame;
		pMainFrame->CenterWindow();
		pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
		pMainFrame->UpdateWindow();
	}
	return TRUE;
}

