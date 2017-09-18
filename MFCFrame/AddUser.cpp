// AddUser.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "AddUser.h"
#include"UserManagement.h"


// CAddUser 对话框
#define IDC_ABOUTTEXT   10000
#define IDC_CLOSEDLG    10001
#define IDC_USERNAME    10002
#define IDC_PASSWORD    10003
#define IDC_TYPE        10004
IMPLEMENT_DYNAMIC(CAddUser, CDialogEx)

CAddUser::CAddUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddUser::IDD, pParent)
{
	m_caption.m_hWnd = NULL;
	m_AddUserText.m_hWnd = NULL;
	m_CloseDlg.m_hWnd = NULL;
	m_SUserName.m_hWnd = NULL;
	m_Spassword.m_hWnd = NULL;
	m_type.m_hWnd = NULL;
	m_typedata = 0;
	strvalue = -1;
}

CAddUser::~CAddUser()
{
}

void CAddUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_username);
	DDX_Control(pDX, IDC_EDIT2, m_password);
	DDX_Control(pDX, IDC_COMBO1, m_comboxtype);
}


BEGIN_MESSAGE_MAP(CAddUser, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddUser::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAddUser::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAddUser 消息处理程序
BOOL CAddUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboxtype.AddString(_T("超级管理员"));
	m_comboxtype.AddString(_T("普通用户"));
	m_comboxtype.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CAddUser::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, RGB(240, 248, 251));
}


int CAddUser::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rcClient;
	GetClientRect(&rcClient);
	if (m_caption.m_hWnd == NULL)
	{
		m_caption.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, rcClient.Width() - 27, 27), this, IDC_ABOUTTEXT);
	}
	if (m_AddUserText.m_hWnd == NULL)
	{
		m_AddUserText.CreateEx(_T("添加用户"), WS_VISIBLE | WS_CHILD, 0, 0, rcClient.Width() - 27, 27, m_hWnd, (HMENU)IDC_ABOUTTEXT);
		m_AddUserText.SetImageForButton(IDB_SIDEBAR_HEAD);
	}
	if (m_CloseDlg.m_hWnd == NULL)
	{
		m_CloseDlg.CreateEx(_T(""), WS_VISIBLE | WS_CHILD, rcClient.Width() - 27, 0, 27, 27, m_hWnd, (HMENU)IDC_CLOSEDLG);
		m_CloseDlg.SetImageForButton(IDB_SIDEBAR_CLOSE);
	}
	if (m_SUserName.m_hWnd == NULL)
	{
		m_SUserName.CreateEx(_T("用户名"), WS_VISIBLE | WS_CHILD, rcClient.left + 60, rcClient.top + 76, 60, 20, m_hWnd, (HMENU)IDC_USERNAME);
		m_SUserName.SetImageForButton(IDB_BACK);
	}
	if (m_Spassword.m_hWnd == NULL)
	{
		m_Spassword.CreateEx(_T("密码"), WS_VISIBLE | WS_CHILD, rcClient.left + 60, rcClient.top + 118, 60, 20, m_hWnd, (HMENU)IDC_PASSWORD);
		m_Spassword.SetImageForButton(IDB_BACK);
	}
	if (m_type.m_hWnd == NULL)
	{
		m_type.CreateEx(_T("类型"), WS_VISIBLE | WS_CHILD, rcClient.left + 60, rcClient.top + 160, 60, 20, m_hWnd, (HMENU)IDC_TYPE);
		m_type.SetImageForButton(IDB_BACK);
	}
	return 0;
}
BOOL CAddUser::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);

	if (wNotifyCode == BN_CLICKED)
	{
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);
		if (m_CloseDlg.m_hWnd == hWndButton)
		{
			CAddUser::OnOK();
		}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}




void CAddUser::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
	CComboBox* pComboxWndMode = (CComboBox*)GetDlgItem(IDC_CAMERADISPOSE);
	if (pComboxWndMode)
	{
		int nIdx = pComboxWndMode->GetCurSel();
		if (nIdx == 0) m_typedata = 0;
		else if (nIdx == 1)	m_typedata = 1;
	}
}


void CAddUser::OnBnClickedButton1()//确定
{
	CUserManagement *pWnd = CUserManagement::GetInstance();
	m_username.GetWindowTextW(m_UsernameText);
	m_password.GetWindowTextW(m_passwordText);
	if (m_typedata == 0)
	{
		strvalue += 1;
		m_passwordText =  _T("超级管理员");
		pWnd->InsertAlarm(strvalue, m_UsernameText, m_passwordText);
	}
	if (m_typedata == 1)
	{
		strvalue += 1;
		m_passwordText = _T("普通用户");
		pWnd->InsertAlarm(strvalue, m_UsernameText, m_passwordText);
	}
}


void CAddUser::OnBnClickedButton2()
{
	CAddUser::OnOK();
}
