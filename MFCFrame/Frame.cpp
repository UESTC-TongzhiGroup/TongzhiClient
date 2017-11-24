
// Frame.cpp : ����Ӧ�ó��������Ϊ��
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


// CFrameApp ����

CFrameApp::CFrameApp()
{
}


// Ψһ��һ�� CFrameApp ����

CFrameApp theApp;


// CFrameApp ��ʼ��

BOOL CFrameApp::InitInstance()
{
	//_CrtSetBreakAlloc(1169);
	CWinApp::InitInstance();

	CUserLogin ldlg;
	if (ldlg.DoModal() == IDOK)
	{
		/*******��������*********/
		Config::loadServerCfg();
		CMainFrame* pMainFrame = new CMainFrame();
		pMainFrame->CreateEx(0, _T("MAINFRAME"),
			_T("BT    �ͻ���"),
			WS_POPUPWINDOW | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL);
		//__PreCreateWindow ע�ᴰ����
		m_pMainWnd = pMainFrame;
		pMainFrame->CenterWindow();
		pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
		pMainFrame->UpdateWindow();
	}
	return TRUE;
}

