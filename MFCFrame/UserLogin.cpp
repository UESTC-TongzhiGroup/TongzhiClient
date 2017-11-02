// UserLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "UserLogin.h"
#include "Config.h"
#define IDC_LOGIN    10000
#define IDC_USERNAME 10001
#define IDC_PASSWORD 10002

// CUserLogin 对话框

IMPLEMENT_DYNAMIC(CUserLogin, CDialogEx)

CUserLogin::CUserLogin(CWnd* pParent)
	: CDialogEx(CUserLogin::IDD, pParent)
{
	m_SUserName.m_hWnd = NULL;
	m_Spassword.m_hWnd = NULL;
	m_login.m_hWnd = NULL;
}

CUserLogin::~CUserLogin()
{
}

void CUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Name);
	DDX_Control(pDX, IDC_EDIT2, m_Password);
}


BEGIN_MESSAGE_MAP(CUserLogin, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CUserLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserLogin 消息处理程序


void CUserLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient, RGB(240, 248, 251));
}


int CUserLogin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rcClient;
	GetClientRect(&rcClient);
	
	if (m_login.m_hWnd == NULL)
	{
		m_login.CreateEx(_T("多功能智能监控报警软件"), WS_VISIBLE | WS_CHILD, 0, 0, rcClient.Width(), 36, m_hWnd, (HMENU)IDC_LOGIN);
		m_login.SetImageForButton(IDB_SIDEBAR_HEAD);
	}
	if (m_SUserName.m_hWnd == NULL)
	{
		m_SUserName.CreateEx(_T("用户名"), WS_VISIBLE | WS_CHILD, rcClient.left+60, rcClient.top+76, 60, 20, m_hWnd, (HMENU)IDC_USERNAME);
		m_SUserName.SetImageForButton(IDB_BACK);
	}
	if (m_Spassword.m_hWnd == NULL)
	{
		m_Spassword.CreateEx(_T("密码"), WS_VISIBLE | WS_CHILD, rcClient.left + 60, rcClient.top + 118, 60, 20, m_hWnd, (HMENU)IDC_PASSWORD);
		m_Spassword.SetImageForButton(IDB_BACK);
	}
	
	return 0;
}

using namespace StrUtil;
bool  CUserLogin::checkLicense(CString usr, CString passwd) {
	auto lmap = Config::getLicensesMap();
	string usr_str=CStr2std(usr);
	auto pair = lmap.find(usr_str);
	return pair != lmap.end() && passwd == std2CStr(pair->second);
}


void CUserLogin::OnBnClickedOk()
{
	CString c, b;
	m_Name.GetWindowText(c);
	m_Password.GetWindowText(b);
	if (c.IsEmpty() || b.IsEmpty())
	{
		MessageBox(_T("用户名或密码不能为空！"),_T("提示"));
		return;
	}
	//TODO:修改登录逻辑
	//==只能用来与LPTSTR进行相等判断,两个CString之间使用CString::Compare
	else if(checkLicense(c,b))
	{
		//MessageBox(_T("登陆成功！"), _T("登陆"));//这句可有可无        
	}
	else
	{
		MessageBox(_T("用户名或密码不正确！"), _T("提示"));
		return;
	}
	CDialogEx::OnOK();
}
