// AlarmMonitor.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "AlarmMonitor.h"



#define IDC_WARNINGBTN 10001
#define IDC_EXPANDSTATE 10002
// CAlarmMonitor 对话框

IMPLEMENT_DYNAMIC(CAlarmMonitor, CDialogEx)

CAlarmMonitor::CAlarmMonitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlarmMonitor::IDD, pParent)
{
	m_waringBtn.m_hWnd = NULL;
	m_warningPanel.m_hWnd = NULL;
	m_monitorPanel.m_hWnd =NULL;
	m_expandState.m_hWnd = NULL;
	m_expandSidebar =TRUE;
}

CAlarmMonitor::~CAlarmMonitor()
{
}

void CAlarmMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlarmMonitor, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAlarmMonitor 消息处理程序

int CAlarmMonitor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);
	if(m_warningPanel.m_hWnd == NULL)
	{
		m_warningPanel.Create(IDD_WARNINGPANEL,this);
	}
	if(m_waringBtn.m_hWnd == NULL)
	{
		m_waringBtn.Create(_T(""),WS_VISIBLE|WS_CHILD,CRect(0,lpCreateStruct->cy-100,lpCreateStruct->cx,lpCreateStruct->cy-70),this,IDC_WARNINGBTN);
	}
	if(m_monitorPanel.m_hWnd == NULL)
	{
		m_monitorPanel.Create(IDD_MONITORPANEL,this);
	}
	if(m_expandState.m_hWnd == NULL)
	{
		m_expandState.CreateEx(_T(""),WS_VISIBLE|WS_CHILD,rcClient.right -20,rcClient.bottom -100,50,20,m_hWnd,(HMENU)IDC_EXPANDSTATE);
		m_expandState.SetImageForButton(IDB_COLLAPSEBTNS);
	}

	return 0;
}


void CAlarmMonitor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(rcClient);
	if (m_expandSidebar)
	{
		if (m_waringBtn.m_hWnd)
		{
			CRect rcWaringBtn = rcClient;
			rcWaringBtn.top = rcClient.bottom - 150;
			rcWaringBtn.bottom = rcClient.bottom - 120;
			rcWaringBtn.right = rcClient.right - 20;
			m_waringBtn.MoveWindow(rcWaringBtn);
		}
		if (m_warningPanel.m_hWnd)
		{
			CRect rcWarningPanel = rcClient;
			rcWarningPanel.top = rcClient.bottom - 120;
			m_warningPanel.MoveWindow(rcWarningPanel);
			m_warningPanel.ShowWindow(SW_SHOW);
		}
		if (m_monitorPanel.m_hWnd)
		{
			CRect rcMonitorPanel = rcClient;
			rcMonitorPanel.bottom = rcClient.bottom - 150;
			m_monitorPanel.MoveWindow(rcMonitorPanel);
			m_monitorPanel.ShowWindow(SW_SHOW);
		}

		if(m_expandState.m_hWnd)
		{
			m_expandState.MoveWindow(rcClient.right -40,rcClient.bottom -150,40,30);
		}

	}
	else
	{
		if (m_waringBtn.m_hWnd)
		{
			CRect rcWaringBtn = rcClient;
			rcWaringBtn.top = rcClient.bottom - 30;
			rcWaringBtn.right = rcClient.right - 20;
			m_waringBtn.MoveWindow(rcWaringBtn);
		}
		m_warningPanel.ShowWindow(SW_HIDE);
		if (m_monitorPanel.m_hWnd)
		{
			CRect rcMonitorPanel = rcClient;
			rcMonitorPanel.bottom = rcClient.bottom - 30;
			m_monitorPanel.MoveWindow(rcMonitorPanel);
			m_monitorPanel.ShowWindow(SW_SHOW);
		}
		if(m_expandState.m_hWnd)
		{
			m_expandState.MoveWindow(rcClient.right -40,rcClient.bottom -30,40,30);
		}
	}
}


void CAlarmMonitor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect (rcClient,RGB(240,248,251));
}

BOOL CAlarmMonitor::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	
	if(wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT (hWndButton);
		if(m_expandState.m_hWnd == hWndButton)
		{
			CRect rcClient;
			GetClientRect(&rcClient);
			if (m_expandSidebar)
			{
				m_expandSidebar = FALSE;
				m_warningPanel.ShowWindow(SW_HIDE);
				if (m_monitorPanel.m_hWnd)
				{
					CRect rcMonitorPanel = rcClient;
					rcMonitorPanel.bottom = rcClient.bottom - 30;
					m_monitorPanel.MoveWindow(rcMonitorPanel);
					m_monitorPanel.ShowWindow(SW_SHOW);
				}
				if (m_waringBtn.m_hWnd)
				{
					CRect rcWaringBtn = rcClient;
					rcWaringBtn.top = rcClient.bottom - 30;
					rcWaringBtn.right = rcClient.right - 20;
					m_waringBtn.MoveWindow(rcWaringBtn);
				}
				m_expandState.SetImageForButton(IDB_EXPANDBTNS);
				if(m_expandState.m_hWnd)
				{
					m_expandState.MoveWindow(rcClient.right -40,rcClient.bottom -30,40,30);
				}
			}
			else
			{
				m_expandSidebar = TRUE;
				m_warningPanel.ShowWindow(SW_SHOW);
				if (m_monitorPanel.m_hWnd)
				{
					CRect rcMonitorPanel = rcClient;
					rcMonitorPanel.bottom = rcClient.bottom - 150;
					m_monitorPanel.MoveWindow(rcMonitorPanel);
					m_monitorPanel.ShowWindow(SW_SHOW);
				}
				if (m_waringBtn.m_hWnd)
				{
					CRect rcWaringBtn = rcClient;
					rcWaringBtn.top = rcClient.bottom - 150;
					rcWaringBtn.bottom = rcClient.bottom - 120;
					rcWaringBtn.right = rcClient.right - 20;
					m_waringBtn.MoveWindow(rcWaringBtn);
				}
				m_expandState.SetImageForButton(IDB_COLLAPSEBTNS);
				if(m_expandState.m_hWnd)
				{
					m_expandState.MoveWindow(rcClient.right -40,rcClient.bottom -150,40,30);
				}
			}
		}
	}
	Invalidate();
	return CDialogEx::OnCommand(wParam, lParam);
}