// DeskButton.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "DeskButton.h"

IMPLEMENT_DYNAMIC(CDeskButton, CWnd)

CDeskButton::CDeskButton()
{
  m_bIsSelected = FALSE;
}

CDeskButton::~CDeskButton()
{
}


BEGIN_MESSAGE_MAP(CDeskButton, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CDeskButton::PreCreateWindow( CREATESTRUCT& cs )
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
	wex.hbrBackground=(HBRUSH)::GetStockObject(NULL);
	wex.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wex.hIcon=wex.hIconSm=::LoadIcon(hInstance,MAKEINTRESOURCE(IDB_EXPAND));
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

BOOL CDeskButton::Create(LPCTSTR lpszWindowName,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID)
{
	return CWnd::CreateEx(0,_T("DeskButton"),lpszWindowName,dwStyle,rect,pParentWnd,nID);
}

void CDeskButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
	TRACKMOUSEEVENT tme;
	tme.cbSize=sizeof(TRACKMOUSEEVENT);
	tme.dwFlags=TME_HOVER|TME_LEAVE;
	tme.dwHoverTime=0;
	tme.hwndTrack=m_hWnd;
	TrackMouseEvent(&tme);
}

void CDeskButton::OnMouseHover(UINT nFlags, CPoint point)
{
	CWnd::OnMouseHover(nFlags, point);
}

void CDeskButton::OnMouseLeave()
{
	CWnd::OnMouseLeave();
}

void CDeskButton::OnLButtonDown(UINT nFlags, CPoint point)
{
   
	if(!m_bIsSelected)
	{
		m_bIsSelected = TRUE;
		Invalidate();
	}

	CWnd * pWndParent = GetParent();
	ASSERT(pWndParent);
	WORD wID = GetDlgCtrlID();
	pWndParent->SendMessage (WM_COMMAND,MAKEWPARAM(wID,BN_CLICKED),(LPARAM)m_hWnd);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CDeskButton::OnPaint()
{
	CPaintDC dc(this); 
    if(m_imgbtn.IsNull())
		return ;
	CRect rt;
	GetClientRect(rt);
	BOOL bEnable=IsWindowEnabled();
	if(!bEnable)
	{
		CRect rcEnable;
		rcEnable.top=0;
		rcEnable.bottom=m_imgbtn.GetHeight();
		rcEnable.left=m_imgbtn.GetWidth()*3/4;
		rcEnable.right=m_imgbtn.GetWidth();
		m_imgbtn.Draw(dc.GetSafeHdc(),rt,rcEnable);
		return ;
	}
	if(m_bIsSelected)
	{
		CRect rcItemLeft = rt;
		rcItemLeft.right=m_imgbtn.GetWidth()/4/2-1;
        m_imgbtn.Draw(dc.GetSafeHdc(),rcItemLeft,CRect(m_imgbtn.GetWidth()*2/4,0,m_imgbtn.GetWidth()*2/4+m_imgbtn.GetWidth()/4/2-1,m_imgbtn.GetHeight()));

		CRect rcItemCenter = rt;
		rcItemLeft.left=m_imgbtn.GetWidth()/4/2-1;
		rcItemLeft.right=m_imgbtn.GetWidth()-(m_imgbtn.GetWidth()/4/2-1);
        m_imgbtn.Draw(dc.GetSafeHdc(),rcItemCenter,CRect(m_imgbtn.GetWidth()*2/4+m_imgbtn.GetWidth()/4/2-1,0,m_imgbtn.GetWidth()*2/4+m_imgbtn.GetWidth()/4/2,m_imgbtn.GetHeight()));

		CRect rcItemRight = rt;
		rcItemRight.left=m_imgbtn.GetWidth()-(m_imgbtn.GetWidth()/4/2-1);
        m_imgbtn.Draw(dc.GetSafeHdc(),rcItemRight,CRect(m_imgbtn.GetWidth()*2/4+m_imgbtn.GetWidth()/4/2+1,0,m_imgbtn.GetWidth()*3/4,m_imgbtn.GetHeight()));
	}
	else
	{
		CRect rcItemLeft = rt;
		rcItemLeft.right=m_imgbtn.GetWidth()/4/2-1;
        m_imgbtn.Draw(dc.GetSafeHdc(),rcItemLeft,CRect(m_imgbtn.GetWidth()*0/4,0,m_imgbtn.GetWidth()*0/4+m_imgbtn.GetWidth()/4/2-1,m_imgbtn.GetHeight()));

		CRect rcItemCenter = rt;
		rcItemLeft.left=m_imgbtn.GetWidth()/4/2-1;
		rcItemLeft.right=m_imgbtn.GetWidth()-(m_imgbtn.GetWidth()/4/2-1);
        m_imgbtn.Draw(dc.GetSafeHdc(),rcItemCenter,CRect(m_imgbtn.GetWidth()*0/4+m_imgbtn.GetWidth()/4/2-1,0,m_imgbtn.GetWidth()*0/4+m_imgbtn.GetWidth()/4/2,m_imgbtn.GetHeight()));

		CRect rcItemRight = rt;
		rcItemRight.left=m_imgbtn.GetWidth()-(m_imgbtn.GetWidth()/4/2-1);
        m_imgbtn.Draw(dc.GetSafeHdc(),rcItemRight,CRect(m_imgbtn.GetWidth()*0/4+m_imgbtn.GetWidth()/4/2+1,0,m_imgbtn.GetWidth()*1/4,m_imgbtn.GetHeight()));
	}
	
	int n0ldBkMode = dc.SetBkMode(TRANSPARENT);
	{
		CString strText;
		GetWindowText(strText);
		if(!strText.IsEmpty ())
		{
			dc.DrawText (strText,rt,DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
		}
	}
	dc.SetBkMode(n0ldBkMode);

}

int CDeskButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDeskButton::SetTabForButton( UINT uIDResource )
{
   if(!m_imgbtn.IsNull())
   {
	   m_imgbtn.Destroy();
   }
   m_imgbtn.LoadFromResource(AfxGetInstanceHandle(),uIDResource);
  
   Invalidate();
}
void CDeskButton::OnLButtonUp(UINT nFlags, CPoint point)
{
   CWnd::OnLButtonUp(nFlags, point);
   
}

BOOL CDeskButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CDeskButton::SetSelected(BOOL bIsSelected)
{
	if(bIsSelected!=m_bIsSelected)
	{
		m_bIsSelected = bIsSelected;
		Invalidate();
	}
}

BOOL CDeskButton::GetSelected()
{
	return m_bIsSelected;
}

