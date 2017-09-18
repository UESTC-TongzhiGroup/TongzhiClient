#pragma once
#include <atlimage.h>
class CImageButton : public CWnd
{
	DECLARE_DYNAMIC(CImageButton)

public:
	CImageButton();
	virtual ~CImageButton();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL CreateEx(
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU nIDorHMenu
		);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	BOOL m_bIsHover;
	BOOL m_bIsPressed;
	CImage m_imgbtn;
public:
	void SetImageForButton(UINT uIDResource);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


