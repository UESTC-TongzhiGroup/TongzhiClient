#pragma once
#include <atlimage.h>

// CUITreeCtrl

class CUITreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CUITreeCtrl)

public:
	CUITreeCtrl();
	virtual ~CUITreeCtrl();
public:
	void DrawBackground(CDC* pDC);
	void DrawItem(CDC* pDC, HTREEITEM hTreeItem);
	BOOL SetItemHeightEx(HTREEITEM hTreeItem,UINT cyItemHeight);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnTreeSelChange(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnSelDblClk(NMHDR * pNMHDR, LRESULT * pResult);
	HWND eventHandler;
public:
	void setEventHandler(HWND);
	CImage m_imgCollapse;
	CImage m_imgExpand;
};

inline void CUITreeCtrl::setEventHandler(HWND handler) {
	eventHandler = handler;
}

