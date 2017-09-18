// Aboutdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Aboutdlg.h"

// CAboutdlg 对话框

#define IDC_ABOUTTEXT   10000
#define IDC_CLOSEDLG    10001
IMPLEMENT_DYNAMIC(CAboutdlg, CDialogEx)

CAboutdlg::CAboutdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutdlg::IDD, pParent)
{
	m_caption.m_hWnd = NULL;
	m_Abouttext.m_hWnd = NULL;
	m_CloseDlg.m_hWnd = NULL;
}

CAboutdlg::~CAboutdlg()
{
}

void CAboutdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutdlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()

END_MESSAGE_MAP()


// CAboutdlg 消息处理程序


void CAboutdlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, RGB(240, 248, 251));
}


int CAboutdlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rcClient;
	GetClientRect(&rcClient);
	if (m_caption.m_hWnd == NULL)
	{
		m_caption.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, rcClient.Width() - 30, 29), this, IDC_ABOUTTEXT);
	}
	if (m_Abouttext.m_hWnd == NULL)
	{
		m_Abouttext.CreateEx(_T("关于"), WS_VISIBLE | WS_CHILD, 0, 0, rcClient.Width()-30, 29, m_hWnd, (HMENU)IDC_ABOUTTEXT);
		m_Abouttext.SetImageForButton(IDB_ABOUT_HEAD);
	}
	if (m_CloseDlg.m_hWnd == NULL)
	{
		m_CloseDlg.CreateEx(_T(""), WS_VISIBLE | WS_CHILD, rcClient.Width() - 30, 0, 30, 29, m_hWnd, (HMENU)IDC_CLOSEDLG);
		m_CloseDlg.SetImageForButton(IDB_CLOSEDLG);
	}
	
	return 0;
}
BOOL CAboutdlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);

	if (wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);
		if (m_CloseDlg.m_hWnd == hWndButton)
		{
			CAboutdlg::OnOK();
		}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}