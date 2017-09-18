#pragma once
#include "afxwin.h"

typedef struct _MENUITEM
{
 HICON hIcon;
 CString strMenu;
}MENUITEM,*PMENUITEM;

class CMenuEx :
	public CMenu
{
public:
	CMenuEx(void);
	~CMenuEx(void);
public:
	void SetMenuWidth(int nWidth);
protected:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	int m_iWidth;
};
