#pragma once
#include<atlimage.h>
#include "panel.h"
#include"resource.h"
#include"ImageButton.h"

class CWarningBtn :public CPanel
{
public:
	CWarningBtn(void);
	~CWarningBtn(void);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CImage  m_imgWarningPanel;
	CImageButton m_warning;
	CImageButton m_tanhao;
};

