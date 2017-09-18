// MonitorPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "MonitorPanel.h"
#include "afxdialogex.h"

#define IDC_SIDEBAR   10001
#define IDC_EXPAND    10002
// CMonitorPanel 对话框

IMPLEMENT_DYNAMIC(CMonitorPanel, CDialogEx)

CMonitorPanel::CMonitorPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorPanel::IDD, pParent)
{
	m_sidebar.m_hWnd = NULL ;
	m_videoPanel.m_hWnd = NULL;
	m_expand.m_hWnd = NULL ;
	m_expandSidebar =TRUE;
}

CMonitorPanel::~CMonitorPanel() {}

void CMonitorPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorPanel, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMonitorPanel 消息处理程序


int CMonitorPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);
	if(m_sidebar.m_hWnd == NULL )
	{
		m_sidebar.Create(_T(""),WS_VISIBLE|WS_CHILD,CRect(0,0,300,lpCreateStruct->cy),this,IDC_SIDEBAR);
	}
	if(m_expand.m_hWnd == NULL)
	{
		m_expand.CreateEx(_T(""),WS_VISIBLE|WS_CHILD,300,rcClient.Height()/2-2,5,10,m_hWnd,(HMENU)IDC_EXPAND);
		m_expand.SetImageForButton(IDB_COLLAPSEBTN);
	}
	if(m_imgRight.IsNull() )
	{
		m_imgRight.LoadFromResource(AfxGetInstanceHandle(),IDB_SIDEBAR_RIGHT);
	}
	if(m_videoPanel.m_hWnd == NULL )
	{
		m_videoPanel.Create(IDD_VIDEOPANEL,this);
	}
	return 0;
}


void CMonitorPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(&rcClient);

	if(m_expandSidebar)
	{
		if(m_expand.m_hWnd)
		{
			m_expand.MoveWindow(300,rcClient.Height()/2-5,5,10);
		}

		if(NULL == m_sidebar.m_hWnd) return;
		CRect rcSideBar;
		m_sidebar.GetWindowRect(rcSideBar);
		m_sidebar.MoveWindow(0,0,rcSideBar.Width(),rcClient.Height ());
		m_sidebar.ShowWindow( SW_SHOW );
		if(NULL == m_videoPanel.m_hWnd) return ;
		CRect rcVideoPanel = rcClient ;
		rcVideoPanel.left = rcClient.left+305;
		m_videoPanel.MoveWindow(rcVideoPanel);
		m_videoPanel.ShowWindow( SW_SHOW );
	}
	else
	{
		if(m_expand.m_hWnd)
		{
			m_expand.MoveWindow(0,rcClient.Height()/2-5,10,10);
		}

		//if(NULL == m_sidebar.m_hWnd) return;
		//CRect rcSideBar;
		//m_sidebar.GetWindowRect(rcSideBar);
		//m_sidebar.MoveWindow(0,0,rcSideBar.Width(),rcClient.Height ());
		m_sidebar.ShowWindow( SW_HIDE );
		if(NULL == m_videoPanel.m_hWnd) return ;
		CRect rcVideoPanel = rcClient ;
		rcVideoPanel.left = rcClient.left+ 10;
		m_videoPanel.MoveWindow(rcVideoPanel);
		m_videoPanel.ShowWindow( SW_SHOW );
	}
}

void CMonitorPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect (rcClient,RGB(240,248,251));
	if(!m_imgRight.IsNull())
	{
		CRect rcSplit = rcClient;
		rcSplit.left = rcClient.left +300;
		rcSplit.right = rcClient.left +305;
		m_imgRight.Draw (dc.m_hDC,rcSplit);
	}
}

BOOL CMonitorPanel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	if(wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT (hWndButton);
		if(m_expand.m_hWnd == hWndButton)
		{
			CRect rcClient;
			GetClientRect(&rcClient);
			if(m_expandSidebar)
			{
				m_expandSidebar = FALSE;
				m_sidebar.ShowWindow( SW_HIDE );
				if(m_expand.m_hWnd)
				{
				m_expand.MoveWindow(0,rcClient.Height()/2-5,5,10);
				}
				if(m_videoPanel.m_hWnd)
				{
					CRect rcVideoPanel = rcClient ;
					rcVideoPanel.left = rcClient.left+ 5;
					m_videoPanel.MoveWindow(rcVideoPanel);
					m_videoPanel.ShowWindow( SW_SHOW );
				}
				m_expand.SetImageForButton(IDB_EXPANDBTN);
			}
			else
			{
				m_expandSidebar = TRUE;
				m_sidebar.ShowWindow( SW_SHOW );
				if(m_expand.m_hWnd)
				{
				m_expand.MoveWindow(300,rcClient.Height()/2-5,5,10);
				}
				if(m_videoPanel.m_hWnd)
				{
					CRect rcVideoPanel = rcClient ;
					rcVideoPanel.left = rcClient.left+ 305;
					m_videoPanel.MoveWindow(rcVideoPanel);
					m_videoPanel.ShowWindow( SW_SHOW );
				}
				m_expand.SetImageForButton(IDB_COLLAPSEBTN);
			}
		}
	}
	Invalidate();
	return CDialogEx::OnCommand(wParam, lParam);
}