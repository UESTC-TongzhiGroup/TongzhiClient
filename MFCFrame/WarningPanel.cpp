// WarningPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Frame.h"
#include "WarningPanel.h"
#include "resource.h"

#define IDC_WARNINGLIST 10001
// CWarningPanel �Ի���

IMPLEMENT_DYNAMIC(CWarningPanel, CDialogEx)

CWarningPanel::CWarningPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWarningPanel::IDD, pParent)
{
	m_WarningList.m_hWnd = NULL;
	m_pThis = this;
}

CWarningPanel::~CWarningPanel()
{
}

void CWarningPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarningPanel, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWarningPanel ��Ϣ��������


void CWarningPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

    CRect rcClient;
	GetClientRect(&rcClient);

	if(m_WarningList.m_hWnd  != NULL) 
	{
		CRect rcListWarining = rcClient;
		rcListWarining.left =rcClient.left +5;
		rcListWarining.bottom = rcClient.bottom -5;
		rcListWarining.right = rcClient.right -5;
		m_WarningList.MoveWindow(&rcListWarining);
	}
	
}


int CWarningPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_WarningList.m_hWnd == NULL)
	{
		//m_WarningList.GetExtendedStyle()|
		m_WarningList.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, rcClient, this, IDC_WARNINGLIST);
	}
	m_WarningList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 
	m_WarningList.InsertColumn(0, _T("���"), LVCFMT_CENTER,100, 0);   
	m_WarningList.InsertColumn(1, _T("����ʱ��"), LVCFMT_CENTER, 200, 1);   
	m_WarningList.InsertColumn(2, _T("·��"), LVCFMT_CENTER, 120, 2); 
	m_WarningList.InsertColumn(3, _T("����Դ����"), LVCFMT_CENTER,260, 3);   
	m_WarningList.InsertColumn(4, _T("��������"), LVCFMT_CENTER, 250, 4);   
	m_WarningList.InsertColumn(5, _T("������"), LVCFMT_CENTER, 180, 5); 
	m_WarningList.InsertColumn(6, _T("�ط�"), LVCFMT_CENTER, 120, 6);   
	m_WarningList.InsertColumn(7, _T("��ע"), LVCFMT_CENTER, 120, 7); 

	return 0;
}
CWarningPanel * CWarningPanel::GetInstance()
{
	return m_pThis;
}
CWarningPanel *CWarningPanel::m_pThis = NULL;

void CWarningPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	int nCount = m_WarningList.GetItemCount();
	if (nCount > 0)
		m_WarningList.EnsureVisible(nCount - 1, FALSE);
}