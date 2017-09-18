#pragma once
#include "panel.h"

class CDeskContant :
	public CPanel
{
public:
	CDeskContant(void);
	~CDeskContant(void);
	DECLARE_MESSAGE_MAP()
	void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};

