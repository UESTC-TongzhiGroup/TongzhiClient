// MainFrame.cpp : 实现文件
//
#include "stdafx.h"
#include "GlobalSetting.h"
#include "AlarmMonitor.h"
#include "AlarmLog.h"
#include "Frame.h"
#include "MainFrame.h"
#include "resource.h"


#define  NC_BORDER        3
#define IDC_CAPTIONPANEL 10001
//#define IDC_TABPANEL  10002
#define  ID_CLOSEBTN   10004
#define  ID_MAXBTN 10005
#define  ID_MINBTN  10006

#define  IDS_CLOSE_BROWSE   10007
#define  IDS_MAXMIZE_BROWSE 10008
#define  IDS_MINIMIZE_BROWSE  10009
#define  IDS_RESTORE_BROWSE	  10010
#define IDI_BROWSER  10011
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CWnd)

CMainFrame::CMainFrame()
{

	m_AlarmMonitor.m_hWnd = NULL;
	m_AlarmLog.m_hWnd = NULL;
	m_AlarmSet.m_hWnd = NULL;
	m_UserManagement.m_hWnd = NULL;
    m_PeopleManagement.m_hWnd = NULL;
	m_MenuBarPanel.m_hWnd = NULL;
	m_btnClose.m_hWnd = NULL;
	m_btnMax.m_hWnd = NULL;
	m_btnMin.m_hWnd = NULL;
	m_pThis = this;
}

CMainFrame::~CMainFrame()
{
	int nCount = m_vecTabButton.size();
	if(nCount == 0)return;
	for(int i=0;i<nCount; i++)
	{
		CTabButton* pTabButton = m_vecTabButton.at(i);
		delete pTabButton;
		pTabButton = NULL;
	}
	m_vecTabButton.clear ();
}


BEGIN_MESSAGE_MAP(CMainFrame, CWnd)
	ON_WM_GETMINMAXINFO()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_NCACTIVATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CMainFrame 消息处理程序




BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	HINSTANCE hInstance = (HINSTANCE)AfxGetInstanceHandle();
	ASSERT(hInstance);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	BOOL bRet = GetClassInfoEx(hInstance,cs.lpszClass,&wcex);
	if(bRet)
	{
		return TRUE;
	}

	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//窗口的风格
	wcex.lpfnWndProc = AfxWndProc;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;
	wcex.hInstance=hInstance;
	wcex.hIcon=wcex.hIconSm=(HICON)::LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));//图标
	wcex.hCursor=::LoadCursor(NULL,IDC_ARROW);//光标
	wcex.hbrBackground=(HBRUSH)::GetStockObject(WHITE_BRUSH);//背景
	wcex.lpszMenuName=NULL;
	wcex.lpszClassName=cs.lpszClass;//类名
	bRet=RegisterClassEx(&wcex);
	return bRet;
	
}


void CMainFrame::PostNcDestroy()
{
	delete this;

	CWnd::PostNcDestroy();
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	/*lpMMI->ptMinTrackSize.x=550;
	lpMMI->ptMinTrackSize.y=320;*/
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 550;
	lpMMI->ptMaxPosition.x=0;
	lpMMI->ptMaxPosition.y=0;
	//得到工作区的大小
	CRect rcWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	lpMMI->ptMaxSize.x=lpMMI->ptMaxTrackSize.x=rcWorkArea.Width();
	lpMMI->ptMaxSize.y=lpMMI->ptMaxTrackSize.y=rcWorkArea.Height();

	CWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CWnd::OnNcCalcSize(bCalcValidRects,lpncsp);
	
	if(bCalcValidRects)
	{
		int nCxBorder = ::GetSystemMetrics(SM_CXFRAME);//获取边框的大小
		int nCyBorder = ::GetSystemMetrics(SM_CYFRAME);
		
		lpncsp->rgrc[0].left -= nCxBorder + NC_BORDER;
		lpncsp->rgrc[0].right += nCxBorder + NC_BORDER;
		lpncsp->rgrc[0].top -= nCyBorder + NC_BORDER;
		lpncsp->rgrc[0].bottom += nCyBorder + NC_BORDER;
		
		/*lpncsp->rgrc[0].left += NC_BORDER;
		lpncsp->rgrc[0].right -= NC_BORDER;
		lpncsp->rgrc[0].top += NC_BORDER;
		lpncsp->rgrc[0].bottom -= NC_BORDER;*/
	}
	
}


void CMainFrame::OnNcPaint()
{
	if(m_imgBorder.IsNull())return ;
	
	CWindowDC dc(this);
	CRect rcMainFrame;
	GetWindowRect(rcMainFrame);
	
	m_imgBorder.Draw(dc.m_hDC, 0, 0, rcMainFrame.Width(), 4);
	m_imgBorder.Draw(dc.m_hDC, 0, 0, 4, rcMainFrame.Height());
	m_imgBorder.Draw(dc.m_hDC, rcMainFrame.Width() - 4, 4, 4, rcMainFrame.Height() - 4);
	m_imgBorder.Draw(dc.m_hDC, 4, rcMainFrame.Height() - 4, rcMainFrame.Width() - 4, 4);
}

BOOL CMainFrame::OnNcActivate(BOOL bActive)
{
	BOOL bRet =CWnd::OnNcActivate(bActive);
	SendMessage(WM_NCPAINT,0,0);
	Invalidate();
	return bRet;
}

CMainFrame * CMainFrame::GetInstance()
{
	return m_pThis;
}

CMainFrame * CMainFrame::m_pThis = NULL;

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//去掉vista特效
	HINSTANCE hInst = LoadLibrary(_T("dwmapi.dll"));
	typedef HRESULT (WINAPI * TmpFun)(HWND,DWORD,LPCVOID,DWORD);
	if (hInst)
	{
		TmpFun DwmSetWindowAttributeEX = (TmpFun)::GetProcAddress(hInst,"DwmSetWindowAttribute");
		 if (DwmSetWindowAttributeEX)
		{
			DWORD dwAttr = 1;
			DwmSetWindowAttributeEX(GetSafeHwnd(),2,&dwAttr,4);
		}
		FreeLibrary(hInst);
	}
	 hInst = LoadLibrary(_T("UxTheme.dll"));
	 typedef HRESULT (WINAPI *PFUN_SetWindowTheme)(HWND,LPCTSTR,LPCTSTR);
	if (hInst)
	{
		PFUN_SetWindowTheme pFun = (PFUN_SetWindowTheme)GetProcAddress(hInst,"SetWindowTheme");
		if(pFun)
		{
			pFun(m_hWnd,_T(" "),_T(" "));
		}
		FreeLibrary(hInst);
	}
	//去掉xp主体

	if(m_imgBorder.IsNull())
	{
		m_imgBorder.LoadFromResource(AfxGetInstanceHandle(),IDB_BORDER);
	}
	if (m_imgCaption.IsNull())
	{
		m_imgCaption.LoadFromResource(AfxGetInstanceHandle(), IDB_CAPTION);
	}
	CRect rt;
	GetClientRect(rt);
	if (m_btnClose.m_hWnd == NULL)
	{
		m_btnClose.CreateEx(_T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hWnd, (HMENU)ID_CLOSEBTN);
		m_btnClose.SetImageForButton(IDB_CLOSEBUTTON);
		CGlobalSetting::CreateMyToolTip(m_btnClose.GetSafeHwnd(), IDS_CLOSE_BROWSE);
	}
	if (m_btnMax.m_hWnd == NULL)
	{
		m_btnMax.CreateEx(_T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hWnd, (HMENU)ID_MAXBTN);
		m_btnMax.SetImageForButton(IDB_MAXBUTTON);
		CGlobalSetting::CreateMyToolTip(m_btnMax.GetSafeHwnd(), IDS_MAXMIZE_BROWSE);
	}
	if (m_btnMin.m_hWnd == NULL)
	{
		m_btnMin.CreateEx(_T(""), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_hWnd, (HMENU)ID_MINBTN);
		m_btnMin.SetImageForButton(IDB_MINBUTTON);
		CGlobalSetting::CreateMyToolTip(m_btnMin.GetSafeHwnd(), IDS_MINIMIZE_BROWSE);
	}
	if(m_imgTabPanel.IsNull ())
	{
		m_imgTabPanel.LoadFromResource(AfxGetInstanceHandle(),IDB_TABBAR);
	}

	AddItem (_T("报警监控"));
	AddItem (_T("报警日志"));
	AddItem (_T("用户管理"));
	AddItem (_T("报警设置"));
	AddItem (_T("人员管理"));
	if (m_MenuBarPanel.GetSafeHwnd() == NULL)
	{
		m_MenuBarPanel.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, rt.Width(), 25), this, 10001);
	}
	if(m_AlarmMonitor.m_hWnd == NULL)
	{
		m_AlarmMonitor.Create(IDD_DIALOG_ALARMMONITOR,this);
	}
	if(m_AlarmLog.m_hWnd == NULL)
	{
		m_AlarmLog.Create(IDD_ALARMLOG,this);
	}
	if(m_AlarmSet.m_hWnd == NULL)
	{
		m_AlarmSet.Create(IDD_ALARM_SET,this);
	}
	if(m_UserManagement.m_hWnd == NULL)
	{
		m_UserManagement.Create(IDD_USER_MANGEMENT,this);
	}
    if(m_PeopleManagement.m_hWnd == NULL)
	{
		m_PeopleManagement.Create(IDD_PEOPLE_MANAGEMENT,this);
	}
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(rcClient);

	if (m_btnClose.GetSafeHwnd() == NULL)
		return;
	if (m_btnMax.GetSafeHwnd() == NULL)
		return;
	if (m_btnMin.GetSafeHwnd() == NULL)
		return;
	CRect rt;
	GetClientRect(rt);
	//关闭按钮
	CRect rcClose = rt;
	rcClose.left = rt.right - 40;
	rcClose.bottom = 21;
	//最大化按钮
	CRect rcMax = rt;
	rcMax.right = rcClose.left;
	rcMax.left = rcMax.right - 32;
	rcMax.bottom = 21;
	//最小化按钮
	CRect rcMin = rt;
	rcMin.right = rcMax.left;
	rcMin.left = rcMin.right - 31;
	rcMin.bottom = 21;

	m_btnClose.MoveWindow(rcClose);
	m_btnMax.MoveWindow(rcMax);
	m_btnMin.MoveWindow(rcMin);
	if (m_MenuBarPanel.GetSafeHwnd())
	{
		m_MenuBarPanel.MoveWindow(CRect(0, 0, 250, 25));
	}
	OnTabPanel(M_TabLabels);
}


void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, RGB(240, 248, 251));
	if (m_imgTabPanel.IsNull()) return;
	CRect rcTabPanel = rcClient;
	rcTabPanel.top =25;
	rcTabPanel.bottom =55;
	rcTabPanel.right = rcClient.Width() - 5;
	m_imgTabPanel.Draw (dc.m_hDC ,rcTabPanel);

	CRect rcBox = rcClient;
	rcBox.bottom = 25;
	if (m_imgCaption.IsNull())return;
	m_imgCaption.Draw(dc.m_hDC, rcBox);

	//绘制图标
	CRect rcBrowser = rcClient;
	int rcWidth = rcClient.Width() / 2;
	rcBrowser.left = rcWidth+10;
	rcBrowser.right = rcBrowser.left + 10;
	HICON hBrowser = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_BROWSER));
	DrawIconEx(dc.GetSafeHdc(), 2, 0, hBrowser, 29, 27, NULL, NULL, DI_NORMAL);
	CString csMainFrame;
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain)
	{
		pMain->GetWindowText(csMainFrame);
	}
	//绘制浏览器文本
	CRect rcText = rcClient;
	rcText.left = rcWidth +10;
	rcText.right = rcClient.right -35;
	rcText.bottom = 21;
	int nOldMode = dc.SetBkMode(TRANSPARENT);
	if (!csMainFrame.IsEmpty())
	{
		DrawText(dc.GetSafeHdc(), csMainFrame, csMainFrame.GetLength(), rcText, DT_SINGLELINE | DT_WORD_ELLIPSIS | DT_VCENTER | DT_LEFT);
	}
	dc.SetBkMode(nOldMode);


}

int CMainFrame::AddItem(CString strText)
{
	CRect rcTabButton;
	int nIndex = m_vecTabButton.size ();
	ASSERT(nIndex>=0);
	if(nIndex == 0)
	{
		
		rcTabButton.left = 0;
		rcTabButton.right = 194.4;
		rcTabButton.top = 25;
		rcTabButton.bottom =55;
	}
	else
	{
		CTabButton* pLastTabButton = m_vecTabButton.at(nIndex-1);
		ASSERT(pLastTabButton);
		ASSERT(pLastTabButton->m_hWnd);

		CRect rcLastTabButton;
		pLastTabButton->GetWindowRect(rcLastTabButton);
		ScreenToClient(rcLastTabButton);

		rcTabButton.left = rcLastTabButton.right;
		rcTabButton.right = rcLastTabButton.left + 388.8;
		rcTabButton.top = 25;
		rcTabButton.bottom =55;
	}

	CTabButton* pTabButton = new CTabButton();
	ASSERT(pTabButton);
	pTabButton->Create(strText,WS_VISIBLE|WS_CHILD,rcTabButton,this,10002);
	ASSERT(pTabButton->m_hWnd);
	pTabButton->SetTabForButton(IDB_TABBUTTON);

	m_vecTabButton.push_back (pTabButton);
	return nIndex;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	DWORD dwID = LOWORD(wParam);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);

		int nCount = m_vecTabButton.size();
		for (int i = 0; i < nCount; i++)
		{
			CTabButton* pTabButton = m_vecTabButton.at(i);
			ASSERT(pTabButton);
			ASSERT(pTabButton->m_hWnd);

			if (pTabButton->m_hWnd == hWndButton)
			{
				pTabButton->SetSelected(TRUE);
				M_TabLabels = i;
			}
			else
			{
				pTabButton->SetSelected(FALSE);
			}

		}
	}
	if (pMainFrame)
	{
			switch (dwID)
			{
			case ID_CLOSEBTN:
			{
				pMainFrame->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
				return TRUE;
				break;
			}
				break;
			case ID_MAXBTN:
			{
				if (pMainFrame->IsZoomed())
				{
					CGlobalSetting::CreateMyToolTip(m_btnMax.GetSafeHwnd(), IDS_MAXMIZE_BROWSE);
					pMainFrame->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

				}
				else
				{
					CGlobalSetting::CreateMyToolTip(m_btnMax.GetSafeHwnd(), IDS_RESTORE_BROWSE);
					pMainFrame->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				}
			}
				break;
			case ID_MINBTN:
			{
				 pMainFrame->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
				break;
			default:
			{
				//ASSERT(FALSE);
			}
			}
	}
	OnTabPanel(M_TabLabels);
	return CWnd::OnCommand(wParam, lParam);
}
void CMainFrame::OnTabPanel(int M_TabLabels)
{

	CRect rcArea;
	GetClientRect(rcArea);
	rcArea.top = 55;
	m_AlarmSet.ShowWindow(SW_HIDE);
	m_AlarmLog.ShowWindow(SW_HIDE);
	m_AlarmMonitor.ShowWindow(SW_HIDE);
	m_UserManagement.ShowWindow(SW_HIDE);
	m_PeopleManagement.ShowWindow(SW_HIDE);
	switch (M_TabLabels) {
	case 0:
	{
		m_AlarmMonitor.MoveWindow(rcArea);
		m_AlarmMonitor.ShowWindow(SW_SHOW);
		break;
	}
	case 1:
	{
		m_AlarmLog.MoveWindow(rcArea);
		m_AlarmLog.ShowWindow(SW_SHOW);
		break;
	}
	case 2:
	{
		m_UserManagement.MoveWindow(rcArea);
		m_UserManagement.ShowWindow(SW_SHOW);
		break;
	}
	case 3:
	{
		m_AlarmSet.MoveWindow(rcArea);
		m_AlarmSet.ShowWindow(SW_SHOW);
		break;
	}
	case 4:
	{
		m_PeopleManagement.MoveWindow(rcArea);
		m_PeopleManagement.ShowWindow(SW_SHOW);
		break;
	}
	}

}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CMainFrame::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		if (pMainFrame->IsZoomed())
		{
			pMainFrame->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else
		{
			pMainFrame->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}

	}
	CWnd::OnLButtonDblClk(nFlags, point);
}
