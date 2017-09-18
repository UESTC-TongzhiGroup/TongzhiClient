#include "StdAfx.h"
#include "CptionBar.h"
#include "resource.h"
#include "Aboutdlg.h"

CCptionBar::CCptionBar(void)
{

}

CCptionBar::~CCptionBar(void)
{
}
BEGIN_MESSAGE_MAP(CCptionBar, CPanel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CCptionBar::OnPaint()
{
	CPaintDC dc(this);
	CRect rt;
	GetClientRect(rt);
	if (!m_imgCapLeft.IsNull())
	{
		m_imgCapLeft.Draw(dc.GetSafeHdc(), rt);
	}
}

int CCptionBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle(0, WS_CLIPCHILDREN);
	if (m_imgCapLeft.IsNull())
	{
		m_imgCapLeft.LoadFromResource(AfxGetInstanceHandle(), IDB_CAPTION);
	}

	return 0;
}

void CCptionBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPanel::OnLButtonDown(nFlags, point);
	CAboutdlg* pMainFrame = (CAboutdlg*)GetParent();

	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 1);
	}

}


BOOL CCptionBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
