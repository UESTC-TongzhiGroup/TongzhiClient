// AlarmSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "AlarmSet.h"

#define		IDC_PEDEESTRIANLIST 10001
#define     IDC_ADD	10002
#define	IDC_ALTER	10003
#define	IDC_DELETE	10004
// CAlarmSet 对话框

IMPLEMENT_DYNAMIC(CAlarmSet, CDialogEx)

CAlarmSet::CAlarmSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlarmSet::IDD, pParent)
{
	m_PedestrianList.m_hWnd = NULL;
	m_add.m_hWnd = NULL;
	m_alter.m_hWnd = NULL;
	m_delete.m_hWnd = NULL;
}

CAlarmSet::~CAlarmSet()
{
}

void CAlarmSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlarmSet, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAlarmSet 消息处理程序


void CAlarmSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect (rcClient,RGB(240,248,251));
}


int CAlarmSet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_PedestrianList.m_hWnd == NULL)
	{
		m_PedestrianList.Create(WS_VISIBLE | WS_CHILD |LVS_REPORT,rcClient,this,IDC_PEDEESTRIANLIST);
	}
	m_PedestrianList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PedestrianList.InsertColumn(0, _T("序号"), LVCFMT_CENTER,rcClient.Width()/8, 0);   
	m_PedestrianList.InsertColumn(1, _T("摄像头名称"), LVCFMT_CENTER, rcClient.Width()/2, 1);   
	m_PedestrianList.InsertColumn(2, _T("路数"), LVCFMT_CENTER, rcClient.Width()/4, 2); 
	m_PedestrianList.InsertColumn(3, _T("报警日程表"), LVCFMT_CENTER,rcClient.Width()*3/4, 3);   
	

	if(m_add.m_hWnd == NULL)
	{
		m_add.CreateEx(_T("添加"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_ADD);
		m_add.SetImageForButton(IDB_DESKBUTTON);
	}
	if(m_alter.m_hWnd == NULL)
	{
		m_alter.CreateEx(_T("修改"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_ALTER);
		m_alter.SetImageForButton(IDB_DESKBUTTON);
	}
	if(m_delete.m_hWnd == NULL)
	{
		m_delete.CreateEx(_T("删除"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_DELETE);
		m_delete.SetImageForButton(IDB_DESKBUTTON);
	}


	return 0;
}


void CAlarmSet::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_PedestrianList.m_hWnd  != NULL)
	{
		CRect rcPedestrianList = rcClient;
		rcPedestrianList.left =rcClient.left +5;
		rcPedestrianList.bottom = rcClient.bottom -5;
		rcPedestrianList.top =rcClient.top +30;
		rcPedestrianList.right = rcClient.right -5;
		m_PedestrianList.MoveWindow(&rcPedestrianList);  
	}
	if(m_add.m_hWnd != NULL)
	{
		m_add.MoveWindow(rcClient.left +5,rcClient.top,50,30); 
	}
	if(m_alter.m_hWnd != NULL)
	{
		m_alter.MoveWindow(rcClient.left +55,rcClient.top,50,30);
	}
	if(m_delete.m_hWnd != NULL)
	{
		m_delete.MoveWindow(rcClient.left+105,rcClient.top,50,30);
	}
}
