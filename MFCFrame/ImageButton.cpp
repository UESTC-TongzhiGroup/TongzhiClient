// ImageButton.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "ImageButton.h"

IMPLEMENT_DYNAMIC(CImageButton, CWnd)

CImageButton::CImageButton()
{
  m_bIsHover=FALSE;
  m_bIsPressed=FALSE;
}

CImageButton::~CImageButton()
{
}


BEGIN_MESSAGE_MAP(CImageButton, CWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CImageButton::PreCreateWindow( CREATESTRUCT& cs )
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

BOOL CImageButton::CreateEx( LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu )
{
	return CWnd::CreateEx(0,_T("IMAGEBUTTON"),lpszWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,nIDorHMenu,NULL);
}

void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
	TRACKMOUSEEVENT tme;
	tme.cbSize=sizeof(TRACKMOUSEEVENT);
	tme.dwFlags=TME_HOVER|TME_LEAVE;
	tme.dwHoverTime=0;
	tme.hwndTrack=m_hWnd;
	TrackMouseEvent(&tme);
}

void CImageButton::OnMouseHover(UINT nFlags, CPoint point)
{
	CWnd::OnMouseHover(nFlags, point);
	if(!m_bIsHover)
	{
		m_bIsHover=TRUE;
		Invalidate();
	}
}

void CImageButton::OnMouseLeave()
{
	CWnd::OnMouseLeave();
    m_bIsHover=FALSE;
	m_bIsPressed=FALSE;
	Invalidate();
}

void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonDown(nFlags, point);
	if(m_bIsHover)
	{
		if(!m_bIsPressed)
		{
			m_bIsPressed=TRUE;
			Invalidate();
		}
	}
}

void CImageButton::OnPaint()
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
	if(m_bIsHover)
	{
		if(m_bIsPressed)
		{
			CRect rcPressed;
			rcPressed.top=0;
			rcPressed.bottom=m_imgbtn.GetHeight();
			rcPressed.left=m_imgbtn.GetWidth()*2/4;
			rcPressed.right=m_imgbtn.GetWidth()*3/4;
         m_imgbtn.Draw(dc.GetSafeHdc(),rt,rcPressed);
		}
		else
		{
			CRect rcHover;
			rcHover.top=0;
			rcHover.bottom=m_imgbtn.GetHeight();
			rcHover.left=m_imgbtn.GetWidth()*1/4;
			rcHover.right=m_imgbtn.GetWidth()*2/4;
         m_imgbtn.Draw(dc.GetSafeHdc(),rt,rcHover);
		}
	}
	else
	{
		CRect rcNormal;
		rcNormal.top=0;
		rcNormal.bottom=m_imgbtn.GetHeight();
		rcNormal.left=m_imgbtn.GetWidth()*0/4;
		rcNormal.right=m_imgbtn.GetWidth()*1/4;
       m_imgbtn.Draw(dc.GetSafeHdc(),rt,rcNormal);
	}

	int n0ldBkMode = dc.SetBkMode(TRANSPARENT);
	CString strText;
	GetWindowText(strText);
	if(!strText.IsEmpty ())
	{
		dc.DrawText (strText,rt,DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	}
	dc.SetBkMode(n0ldBkMode);
}

int CImageButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CImageButton::SetImageForButton( UINT uIDResource )
{
   if(!m_imgbtn.IsNull())
   {
	   m_imgbtn.Destroy();
   }
   m_imgbtn.LoadFromResource(AfxGetInstanceHandle(),uIDResource);
   Invalidate();
}

void CImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
   CWnd::OnLButtonUp(nFlags, point);
   if(m_bIsHover)
   {
	   if(m_bIsPressed)
	   {
		   m_bIsPressed=FALSE;
		   Invalidate();
	   }
   }
   DWORD dwID=GetDlgCtrlID();
   if(GetParent())
   {
	   GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(dwID,BN_CLICKED),(LPARAM)m_hWnd);
   }
}

BOOL CImageButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

