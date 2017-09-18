#include "StdAfx.h"
#include "DeskContant.h"
#include "resource.h"

CDeskContant::CDeskContant(void)
{
}


CDeskContant::~CDeskContant(void)
{
}

BEGIN_MESSAGE_MAP(CDeskContant, CPanel)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CDeskContant::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.left = rcClient.left + 20;
	CWnd* pWnd = GetWindow(GW_CHILD);
	if(pWnd && pWnd->m_hWnd )
	{
		pWnd->MoveWindow (rcClient);
	}
}
void CDeskContant::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);

	dc.FillSolidRect(rcClient, RGB(240, 248, 251));
}

