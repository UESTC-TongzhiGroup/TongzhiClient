// UITreeCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Frame.h"
#include "UITreeCtrl.h"
#include "resource.h"
// CUITreeCtrl

IMPLEMENT_DYNAMIC(CUITreeCtrl, CTreeCtrl)

CUITreeCtrl::CUITreeCtrl()
{
	m_imgCollapse.LoadFromResource(AfxGetInstanceHandle(),IDB_COLLAPSE);
	m_imgExpand.LoadFromResource(AfxGetInstanceHandle(),IDB_EXPAND);
}

CUITreeCtrl::~CUITreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CUITreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, CUITreeCtrl::OnTreeSelChange)
	ON_NOTIFY_REFLECT(NM_DBLCLK, CUITreeCtrl::OnSelDblClk)
END_MESSAGE_MAP()

void CUITreeCtrl::OnTreeSelChange(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView->itemNew.hItem)
	{
		BOOL bHasChildren = ItemHasChildren(pNMTreeView->itemNew.hItem);
		if (!bHasChildren)//ѡ��Ҷ�ӽڵ����Ϣ��Ӧ
		{
			SetItemHeightEx(pNMTreeView->itemNew.hItem, 40);
		}
	}
	if (pNMTreeView->itemOld.hItem)
	{
		BOOL bHasChildren = ItemHasChildren(pNMTreeView->itemOld.hItem);
		if (!bHasChildren)
		{
			SetItemHeightEx(pNMTreeView->itemOld.hItem, 20);
		}
	}
}

void CUITreeCtrl::OnSelDblClk(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	::SendMessage(eventHandler,
		TREE_UI_EVENT(GetDlgCtrlID()),
		pNMTreeView->hdr.idFrom,
		LPARAM(pNMHDR));
}


// CUITreeCtrl ��Ϣ�������




void CUITreeCtrl::OnPaint()
{
	CPaintDC dc(this); 
	DrawBackground(&dc);

	HTREEITEM hTreeItem = GetFirstVisibleItem();
	while(hTreeItem)
	{
		DrawItem(&dc,hTreeItem);
		hTreeItem = GetNextVisibleItem(hTreeItem);
	}
}

void CUITreeCtrl::DrawBackground(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient,RGB(240,248,251));
}

void CUITreeCtrl::DrawItem(CDC* pDC, HTREEITEM hTreeItem)
{
	CRect rcClient;
	GetClientRect(rcClient);
	BOOL bHasChildren = ItemHasChildren(hTreeItem);
	if(bHasChildren)
	{
		//���ڵ�
		CRect rcItem;
		CRect rcIcon;
		CRect rcLable;
		GetItemRect(hTreeItem,rcItem,TRUE);
		rcItem.right = rcClient.right ;

		rcIcon = rcItem;
		rcIcon.right = rcIcon.left + rcIcon.Height (); 
		rcIcon.DeflateRect (2,2,2,2);

		rcLable = rcItem;
		rcLable.left = rcIcon.Height ();

		CString strText = GetItemText(hTreeItem);

		UINT uStateExpand = GetItemState(hTreeItem,TVIS_EXPANDED);
		if(uStateExpand &TVIS_EXPANDED)
		{
			//����չ��״̬
			m_imgExpand.Draw (pDC->m_hDC ,rcIcon);
		}
		else
		{
			//���ڹر�״̬
			m_imgCollapse.Draw (pDC->m_hDC ,rcIcon);
		}

		pDC->DrawText (strText,rcLable,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_WORD_ELLIPSIS);
	}
	else
	{
		//Ҷ�ڵ�
		CRect rcItem;
		CRect rcIcon;
		CRect rcLable;
		CRect rcSelectedItem;

		GetItemRect(hTreeItem,rcItem,TRUE);
		rcItem.right = rcClient.right ;
		rcSelectedItem = rcItem;
		rcSelectedItem.left = 0;

		rcIcon = rcItem;
		rcIcon.right = rcIcon.left + rcIcon.Height (); 
		rcIcon.DeflateRect (2,2,2,2);

		rcLable = rcItem;
		rcLable.left = rcIcon.left +rcIcon.Height ();

		CString strText = GetItemText(hTreeItem);

		UINT uStateExpand = GetItemState(hTreeItem,TVIS_EXPANDED);
		UINT uStateSelected = GetItemState(hTreeItem,TVIS_SELECTED);

		if(uStateSelected&TVIS_SELECTED)
		{
			//����ѡ��״̬
			pDC->FillSolidRect( rcSelectedItem,RGB(252,240,192));
		}
		else
		{
			//���ڷ�ѡ��״̬
			pDC->FillSolidRect( rcSelectedItem,RGB(240,248,251));
		}

		HICON hIcon = NULL;
		hIcon = LoadIcon(NULL, IDI_APPLICATION);

		if(hIcon)
		{
			::DrawIconEx (pDC->m_hDC,rcIcon.left ,rcIcon.top ,hIcon,rcIcon.Width (),rcIcon.Height (),0,0,DI_NORMAL);
		}
		pDC->DrawText(strText, rcLable, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);// 
	}
}

BOOL CUITreeCtrl::SetItemHeightEx(HTREEITEM hTreeItem,UINT cyItemHeight)
{
	int nOldItemHeight = GetItemHeight();
	TVITEMEX itemEx;
	itemEx.mask = TVIF_INTEGRAL;
	itemEx.hItem = hTreeItem;
	itemEx.iIntegral = cyItemHeight/nOldItemHeight;
	BOOL bRet = (BOOL)SendMessage(TVM_SETITEM,0,LPARAM(&itemEx));
	return bRet;
}

