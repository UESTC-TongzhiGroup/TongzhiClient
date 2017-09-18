#include "StdAfx.h"
#include "WarningBtn.h"


#define IDC_WARNING 10001
#define IDC_TANHAO 10002

CWarningBtn::CWarningBtn(void)
{
	m_warning.m_hWnd = NULL ;
	m_tanhao.m_hWnd = NULL;
}


CWarningBtn::~CWarningBtn(void)
{
}
BEGIN_MESSAGE_MAP(CWarningBtn, CPanel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CWarningBtn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_imgWarningPanel.IsNull ()) return;
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgWarningPanel.Draw (dc.m_hDC ,rcClient);
}


int CWarningBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);

	if(m_imgWarningPanel.IsNull ())
	{
		m_imgWarningPanel.LoadFromResource(AfxGetInstanceHandle(),IDB_CAPTION);
	}

	if(m_warning.m_hWnd == NULL)
	{
		m_warning.CreateEx(_T("±¨¾¯"),WS_VISIBLE|WS_CHILD,25,0,50,rcClient.Height(),m_hWnd,(HMENU)IDC_WARNING);
		m_warning.SetImageForButton(IDB_CAPTION);
	}
	if(m_tanhao.m_hWnd == NULL)
	{
		m_tanhao.CreateEx(_T("   !"),WS_VISIBLE|WS_CHILD,0,0,25,rcClient.Height(),m_hWnd,(HMENU)IDC_TANHAO);
		m_tanhao.SetImageForButton(IDB_CAPTION);
	}

	return 0;
}


void CWarningBtn::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);
	if(m_warning.m_hWnd)
	{
		m_warning.MoveWindow(25,0,50,rcClient.Height());
	}
	if(m_tanhao.m_hWnd)
	{
		m_tanhao.MoveWindow(0,0,25,rcClient.Height());
	}
}
