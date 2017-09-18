#pragma once
// CPanel

class CPanel : public CWnd
{
	DECLARE_DYNAMIC(CPanel)

public:
	CPanel();
	virtual ~CPanel();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL Create(LPCTSTR lpszWindowName,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID);
protected:
	DECLARE_MESSAGE_MAP()
};


