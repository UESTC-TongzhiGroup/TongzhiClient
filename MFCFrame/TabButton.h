#pragma once
#include<atlimage.h>
class CTabButton : public CWnd
{
	DECLARE_DYNAMIC(CTabButton)

public:
	CTabButton();
	virtual ~CTabButton();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL Create(LPCTSTR lpszWindowName,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID);
public:
	void SetTabForButton(UINT uIDResource);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetSelected(BOOL bIsSelected);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	BOOL m_bIsSelected;
	CImage m_imgbtn;
	
};


