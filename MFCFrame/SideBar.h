#pragma once
#include "panel.h"
#include "ImageButton.h"
#include "DeskPanel.h"
#include "UITreeCtrl.h"

class CSideBar :public CPanel
{
public:
	CSideBar(void);
	~CSideBar(void);
protected:
	static CSideBar* instance;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSplitTreeDoubleClk(WPARAM,LPARAM);
	afx_msg LRESULT OnCamTreeDoubleClk(WPARAM,LPARAM);
	afx_msg LRESULT UpdateCamTree(WPARAM, LPARAM);
private:
	CImageButton m_btnSideBarHead;
	CUITreeCtrl m_CamTreeCtrl;
	CUITreeCtrl m_UISplitTreeCtrl;
	CUITreeCtrl m_AreaTreeCtrl;
	CDeskPanel m_deskPanel;
	CEdit m_editSearch;
	CImage m_imgband;
	CImageButton m_searchbar;
public:
	static CSideBar* GetInstance();
};

inline CSideBar* CSideBar::GetInstance() {
	return CSideBar::instance;
}

