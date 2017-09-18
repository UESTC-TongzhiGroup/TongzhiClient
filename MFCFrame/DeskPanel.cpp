#include "StdAfx.h"
#include "DeskPanel.h"
#include "resource.h"

#define DESK_HEIGHT 25
#define IDC_DESKBUTTON 10001
#define IDC_DESKPANEL 10002
CDeskPanel::CDeskPanel(void)
{
	m_hWnd = NULL;
}


CDeskPanel::~CDeskPanel(void)
{
	int nDeskCount = m_vecDesk.size ();
	for(int i=0;i<nDeskCount;i++)
	{
		DESK* pDeskTemp = m_vecDesk.at(i);
		delete pDeskTemp;
		pDeskTemp = NULL;
	}
	m_vecDesk.clear();
}
BEGIN_MESSAGE_MAP(CDeskPanel, CPanel)
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CDeskPanel::AddDesk(CString strTitle)
{
	if(NULL == m_hWnd)return 0;
	CRect rcNewDesk;
	int nDeskCount = m_vecDesk.size ();
	if (m_vecDesk.size() != 0)
		m_vecDesk.back()->destButton.SetSelected(FALSE);
	DESK* pDest = new DESK;
	pDest->destButton.Create(strTitle,WS_VISIBLE | WS_CHILD,rcNewDesk,this,IDC_DESKBUTTON);
	pDest->destButton.SetTabForButton (IDB_DESKBUTTON);
	pDest->destButton.SetSelected (TRUE);
	pDest->wndPanel.Create(strTitle,WS_VISIBLE | WS_CHILD,rcNewDesk,this,IDC_DESKPANEL);
	m_vecDesk.push_back(pDest);

	ArrangeDesk();

	return nDeskCount;
}

void CDeskPanel::ArrangeDesk()
{
	if(NULL == m_hWnd)
		return;
	CRect rcClient;
	GetClientRect(rcClient);

	int nDeskCount = m_vecDesk.size();
	if(nDeskCount==0)
		return;

	int nPos = rcClient.top;
	CRect rcDesk;
	rcDesk.left = rcClient.left;
	rcDesk.right = rcClient.right;
	for(int i=0;i<nDeskCount;i++)
	{
		DESK* pDeskTemp = m_vecDesk[i];
		rcDesk.top = nPos;
		rcDesk.bottom = rcDesk.top + DESK_HEIGHT;
		pDeskTemp->destButton.MoveWindow (rcDesk);
		nPos += DESK_HEIGHT;

		BOOL bIsSelected = pDeskTemp->destButton.GetSelected();
		if(bIsSelected)
		{
			int height = rcClient.Height() - nDeskCount*DESK_HEIGHT;
			rcDesk.top = rcDesk.bottom ;
			rcDesk.bottom = rcDesk.top + height;
			pDeskTemp->wndPanel.MoveWindow(rcDesk);
			pDeskTemp->wndPanel.ShowWindow(SW_SHOW);
			nPos += height;
		}
		else
		{
			pDeskTemp->wndPanel.ShowWindow (SW_HIDE);
		}
	}
}

void CDeskPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);
	ArrangeDesk();
}

BOOL CDeskPanel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	if(wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT (hWndButton);

		int nCount = m_vecDesk.size();
		for(int i=0;i<nCount; i++)
		{
			DESK*  pDesk = m_vecDesk.at(i);
			ASSERT (pDesk);

			if(pDesk->destButton.m_hWnd == hWndButton)
			{
				pDesk->destButton.SetSelected(TRUE);
			}
			else
			{
				pDesk->destButton.SetSelected(FALSE);
			}
			
		}
		ArrangeDesk();
	}

	return CPanel::OnCommand(wParam, lParam);
}

DESK* CDeskPanel::GetDesk(int nIndex)
{
	ASSERT(nIndex>=0 && nIndex<=m_vecDesk.size());
	return m_vecDesk.at(nIndex);
}