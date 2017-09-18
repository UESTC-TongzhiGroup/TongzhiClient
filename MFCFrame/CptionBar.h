#pragma once
#include "panel.h"
#include <atlimage.h>

class CCptionBar :
	public CPanel
{
public:
	CCptionBar(void);
	~CCptionBar(void);
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	CImage m_imgCapLeft;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
