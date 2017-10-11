#include "StdAfx.h"
#include "SideBar.h"
#include "resource.h"
#include "MainFrame.h"

#include "Events.h"
#include <string.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace Events;

#define IDC_SIDEBAR_HEAD 10001
#define IDC_EDITSEARCH 10003
#define IDC_SEARCHBAR 10004
#define IDC_DESKPANEL  10005
#define SPLIT_UPDATE TREE_UI_EVENT(IDC_SPLIT_UITREE)
#define CAM_SEL_UPDATE TREE_UI_EVENT(IDC_CAM_UITREE)


CSideBar* CSideBar::instance = nullptr;

CSideBar::CSideBar(void)
{
	m_btnSideBarHead.m_hWnd = NULL;
	m_editSearch.m_hWnd = NULL;
	m_searchbar.m_hWnd = NULL;
	m_deskPanel.m_hWnd = NULL;
	m_CamTreeCtrl.m_hWnd = NULL;
	m_UISplitTreeCtrl.m_hWnd = NULL;
	m_AreaTreeCtrl.m_hWnd = NULL;

	CSideBar::instance = this;
}


CSideBar::~CSideBar(void)
{
}



BEGIN_MESSAGE_MAP(CSideBar, CPanel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_MESSAGE(SPLIT_UPDATE, CSideBar::OnSplitTreeDoubleClk)
	ON_MESSAGE(CAM_SEL_UPDATE,CSideBar::OnCamTreeDoubleClk)

	ON_MESSAGE(GLOBAL_EVENT, CSideBar::UpdateCamTree)
END_MESSAGE_MAP()


void CSideBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);

	dc.FillSolidRect (rcClient,RGB(240,248,251));
	
	if(!m_imgband.IsNull ())
	{
		CRect rcleft;
		rcleft.left = 10;
		rcleft.right =20;
		rcleft.top = 32;
		rcleft.bottom = 57;
		m_imgband.Draw (dc.m_hDC,rcleft,CRect(0,0,10,m_imgband.GetHeight ()));

		CRect rcRight = rcleft;
		rcRight.left =rcleft.right ;
		rcRight.right = rcleft.right +120;
		m_imgband.Draw (dc.m_hDC,rcRight,CRect(10,0,m_imgband.GetWidth (),m_imgband.GetHeight ()));
	}
}

int CSideBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	EventBus::regist(CamListUpdate::id(), GetSafeHwnd());
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);
	if (m_btnSideBarHead.m_hWnd == NULL)
	{
		m_btnSideBarHead.CreateEx(_T("报警监控点"), WS_VISIBLE | WS_CHILD, 0, 0, rcClient.Width() - 3, 27, m_hWnd, (HMENU)IDC_SIDEBAR_HEAD);
		m_btnSideBarHead.SetImageForButton(IDB_SIDEBAR_HEAD);
	}
	if (m_searchbar.m_hWnd == NULL)
	{
		m_searchbar.CreateEx(_T(""), WS_VISIBLE | WS_CHILD, rcClient.Width() - 155, rcClient.top + 35, 20, 20, m_hWnd, (HMENU)IDC_SEARCHBAR);
		m_searchbar.SetImageForButton(IDB_SEARCHBAR);
	}
	if (m_editSearch.m_hWnd == NULL)
	{
		CRect rcEditSearch;
		rcEditSearch = rcClient;
		rcEditSearch.left = rcClient.left + 20;
		rcEditSearch.right = rcClient.Width() - 170;
		rcEditSearch.top = rcClient.top + 35;
		rcEditSearch.bottom = rcEditSearch.top + 19;
		m_editSearch.Create(WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, rcEditSearch, this, IDC_EDITSEARCH);
	}

	if (m_imgband.IsNull())
	{
		m_imgband.LoadFromResource(AfxGetInstanceHandle(), IDB_COMBOBOX);
	}

	if (m_CamTreeCtrl.m_hWnd == NULL)
	{
		CRect rcUiTreeCtrl;
		rcUiTreeCtrl = rcClient;
		rcUiTreeCtrl.left = rcClient.left + 20;
		rcUiTreeCtrl.right = rcClient.Width();
		rcUiTreeCtrl.top = rcClient.top + 60;
		m_CamTreeCtrl.Create(WS_VISIBLE | WS_CHILD | TVS_FULLROWSELECT | TVS_SINGLEEXPAND | TVS_HASBUTTONS, rcUiTreeCtrl, this, IDC_CAM_UITREE);
		m_CamTreeCtrl.setEventHandler(GetSafeHwnd());
		m_CamTreeCtrl.SetItemHeight(20);//

		CString strItem;
		strItem.Format(_T("区域组"));

	}

	if (m_deskPanel.m_hWnd == NULL)
	{
		CRect rcDeskPanel = rcClient;
		rcDeskPanel.left = 0;
		rcDeskPanel.right = rcClient.Width();
		rcDeskPanel.top = 200;
		m_deskPanel.Create(_T(""), WS_VISIBLE | WS_CHILD, rcDeskPanel, this, IDC_DESKPANEL);
		for (int i = 0; i<2; i++)
		{
			CString strTitle;
			strTitle.Format(_T("视图-%d"), i);
			m_deskPanel.AddDesk(strTitle);
		}

		DESK* pSplitDesk = m_deskPanel.GetDesk(0);

		if (pSplitDesk)
		{
			if (m_UISplitTreeCtrl.m_hWnd == NULL)
			{
				CRect rcItem0;
				pSplitDesk->wndPanel.GetWindowRect(rcItem0);
				m_UISplitTreeCtrl.Create(WS_VISIBLE | WS_CHILD | TVS_FULLROWSELECT | TVS_SINGLEEXPAND, rcItem0, &pSplitDesk->wndPanel, IDC_SPLIT_UITREE);
				m_UISplitTreeCtrl.setEventHandler(GetSafeHwnd());
				m_UISplitTreeCtrl.SetItemHeight(20);

				//	HICON hIcon = NULL;
				HTREEITEM hRoot0 = m_UISplitTreeCtrl.InsertItem(_T("默认视图"));

				HTREEITEM hTreeItem0 = m_UISplitTreeCtrl.InsertItem(_T("1画面"), 0, 0, hRoot0);
				m_UISplitTreeCtrl.SetItemData(hTreeItem0, 1);

				hTreeItem0 = m_UISplitTreeCtrl.InsertItem(_T("4画面"), 0, 0, hRoot0);
				m_UISplitTreeCtrl.SetItemData(hTreeItem0, 4);

				hTreeItem0 = m_UISplitTreeCtrl.InsertItem(_T("8画面"), 0, 0, hRoot0);
				m_UISplitTreeCtrl.SetItemData(hTreeItem0, 8);

				hTreeItem0 = m_UISplitTreeCtrl.InsertItem(_T("9画面"), 0, 0, hRoot0);
				m_UISplitTreeCtrl.SetItemData(hTreeItem0, 9);

				hTreeItem0 = m_UISplitTreeCtrl.InsertItem(_T("12画面"), 0, 0, hRoot0);
				m_UISplitTreeCtrl.SetItemData(hTreeItem0, 12);

				hTreeItem0 = m_UISplitTreeCtrl.InsertItem(_T("16画面"), 0, 0, hRoot0);
				m_UISplitTreeCtrl.SetItemData(hTreeItem0, 16);

				m_UISplitTreeCtrl.Expand(hRoot0, TVE_EXPAND);
			}
		}

		DESK* pAreaDesk = m_deskPanel.GetDesk(1);
		if (pAreaDesk)
		{
			if (m_AreaTreeCtrl.m_hWnd == NULL)
			{
				CRect rcItem0;
				pSplitDesk->wndPanel.GetWindowRect(&rcItem0);
				m_AreaTreeCtrl.Create(WS_VISIBLE | WS_CHILD | TVS_FULLROWSELECT | TVS_SINGLEEXPAND, rcItem0, &pAreaDesk->wndPanel, IDC_AREA_UITREE);
				m_AreaTreeCtrl.setEventHandler(GetSafeHwnd());
				m_AreaTreeCtrl.SetItemHeight(20);

				HTREEITEM hRoot = m_AreaTreeCtrl.InsertItem(_T("区域划分"));
				/*TODO: 硬编码的区域数量*/
				for (int i = 0; i < 5; i++)
				{
					CString strItem;
					strItem.Format(_T("区域%d"), i+1);
					HTREEITEM hTreeItem = m_AreaTreeCtrl.InsertItem(strItem, 0, 0, hRoot);
					m_AreaTreeCtrl.SetItemData(hTreeItem, i+1);
				}
				m_AreaTreeCtrl.Expand(hRoot, TVE_EXPAND);
			}
		}

	}

	return 0;
}



LRESULT CSideBar::UpdateCamTree(WPARAM EID,LPARAM _event) {
	auto e = reinterpret_cast<CamListUpdate*>(_event);
	if (EID != e->id())
		return TRUE;

	HTREEITEM hRoot = m_CamTreeCtrl.InsertItem(_T("摄像头组"));
	for (int i = 0; i < Cams::getCamInfoNum(); i++)
	{
		CString strItem;
		strItem.Format(_T("%d路摄像头"), i + 1);
		HTREEITEM hTreeItem = m_CamTreeCtrl.InsertItem(strItem, 0, 0, hRoot, TVI_LAST);
		m_CamTreeCtrl.SetItemData(hTreeItem, i);
		m_CamTreeCtrl.Expand(hRoot, TVE_EXPAND);
	}
	return TRUE;
}

void CSideBar::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(rcClient);
	if(m_btnSideBarHead.m_hWnd)
	{
		m_btnSideBarHead.MoveWindow(0,0,rcClient.Width (),27);
	}
	if(m_deskPanel.m_hWnd)
	{
		CRect rcDeskPanel = rcClient;
		rcDeskPanel.left = 0;
		rcDeskPanel.right = rcClient.Width ();
		rcDeskPanel.top = 300;
		m_deskPanel.MoveWindow (rcDeskPanel);
	}
}

LRESULT CSideBar::OnCamTreeDoubleClk(WPARAM wPar, LPARAM lPar)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(lPar);
	if (pNMTreeView->itemOld.hItem)
	{
		HTREEITEM hItem = m_CamTreeCtrl.GetSelectedItem();
		int camIndex = m_CamTreeCtrl.GetItemData(hItem);
		if (camIndex < 0 || camIndex >= Cams::getCamInfoNum())
			return FALSE;
		CVideoPanel *pWnd = CVideoPanel::GetInstance();
		if (pWnd == NULL)
			return FALSE;
		if (CDlgPanel::isPlaying(m_VideoLabels))
		{
			pWnd->OnVideoStop(m_VideoLabels);
		}
		else
		{
			pWnd->OnVideoPlay(m_VideoLabels, camIndex);
		}
	}
	 return TRUE;
}

LRESULT CSideBar::OnSplitTreeDoubleClk(WPARAM wPar, LPARAM lPar)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(lPar);

	if (pNMTreeView->itemOld.hItem)
	{
		nSplitWindow = (pNMTreeView->itemOld).lParam;
		CVideoPanel *pWnd = CVideoPanel::GetInstance();
		if (pWnd == NULL)
			return FALSE;
		pWnd->Update();
	}
	Invalidate();
	return TRUE;
}

