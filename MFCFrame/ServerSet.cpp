// ServerSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "ServerSet.h"
#include "MenuBarPanel.h"
#include "Config.h"


// CServerSet 对话框

using namespace Config;

IMPLEMENT_DYNAMIC(CServerSet, CDialogEx)

CServerSet::CServerSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerSet::IDD, pParent)
	, m_user(_T(""))
	, m_pass(_T(""))
	, m_url(_T(""))
	, m_sendport(0)
	, m_dbport(0)
{
	m_pThis = this;
}

CServerSet::~CServerSet()
{
}

void CServerSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDX_Text(pDX, IDC_EDIT_PASS, m_pass);

	DDX_Text(pDX, IDC_SERVER_PORT_SEND, m_sendport);
	DDX_Text(pDX, IDC_SERVER_PORT_DB, m_dbport);

	DDV_MinMaxUInt(pDX, m_sendport, 0, 65535);
	DDV_MinMaxUInt(pDX, m_dbport, 0, 65535);
	DDX_Text(pDX, IDC_SERVER_IP, m_url);
}


BEGIN_MESSAGE_MAP(CServerSet, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CServerSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerSet 消息处理程序
BOOL CServerSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	auto serverInfo = getServerInfo();
	if (!serverInfo.load)
		return TRUE;
	m_url = stdString2CString(serverInfo.url);
	m_sendport = serverInfo.textport;
	m_dbport = serverInfo.dbport;

	m_user = stdString2CString(serverInfo.loginUser);
	m_pass = stdString2CString(serverInfo.loginPass);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CServerSet * CServerSet::GetInstance()
{
	return m_pThis;
}

CServerSet * CServerSet::m_pThis = NULL;

int CServerSet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CServerSet::OnBnClickedOk()
{
	UpdateData();
	auto &serverInfo = getServerInfo();
	serverInfo.url = CString2stdString(m_url);

	serverInfo.dbport = m_dbport;
	serverInfo.textport = m_sendport;

	serverInfo.loginUser = CString2stdString(m_user);
	serverInfo.loginPass = CString2stdString(m_pass);

	serverInfo.load = true;

	//TODO: 登录后返回用户类型传递
	DBHelper helper;
	UserType type = helper.login();

	//SendMessage

	saveServerCfg();
}

