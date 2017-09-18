// Panel.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "Panel.h"


// CPanel

IMPLEMENT_DYNAMIC(CPanel, CWnd)

CPanel::CPanel()
{
}

CPanel::~CPanel()
{
}


BEGIN_MESSAGE_MAP(CPanel, CWnd)
END_MESSAGE_MAP()



// CPanel 消息处理程序


BOOL CPanel::PreCreateWindow( CREATESTRUCT& cs )
{
	HINSTANCE hInstance=cs.hInstance;
	ASSERT(hInstance);
	WNDCLASSEX wex;
	wex.cbSize=sizeof(WNDCLASSEX);
	BOOL bRet=GetClassInfoEx(hInstance,cs.lpszClass,&wex);
	if(bRet)
	{
		return TRUE;
	}
	wex.cbClsExtra=0;
	wex.cbWndExtra=0;
	wex.hbrBackground=(HBRUSH)::GetStockObject(DKGRAY_BRUSH);
	wex.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wex.hIcon=wex.hIconSm=::LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));
	wex.hInstance=hInstance;
	wex.lpfnWndProc=AfxWndProc;
	wex.lpszClassName=cs.lpszClass;
	wex.lpszMenuName=NULL;
	wex.style=CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;

	bRet=RegisterClassEx(&wex);
	if(!bRet)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CPanel::Create(LPCTSTR lpszWindowName,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID)
{
  return CWnd::CreateEx(0,_T("PANEL"),lpszWindowName,dwStyle,rect,pParentWnd,nID);
}