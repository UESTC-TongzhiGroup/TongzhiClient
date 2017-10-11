// ServerSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Frame.h"
#include "ServerSet.h"
#include "MenuBarPanel.h"
#include "NetHandler.h"
#include "EventBus.h"
#include "Events.h"
#include "Config.h"


// CServerSet �Ի���

IMPLEMENT_DYNAMIC(CServerSet, CDialogEx)

CServerSet::CServerSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerSet::IDD, pParent)
	, m_user(_T(""))
	, m_pass(_T(""))
	, m_url(_T(""))
	, m_sendport(0)
	, notAdmin(FALSE)
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

	DDV_MinMaxUInt(pDX, m_sendport, 0, 65535);
	DDX_Text(pDX, IDC_SERVER_IP, m_url);
	DDX_Radio(pDX, IDC_ADMIN, notAdmin);
}


BEGIN_MESSAGE_MAP(CServerSet, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CServerSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerSet ��Ϣ�������
BOOL CServerSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	auto serverInfo = Config::getServerInfo();
	if (!serverInfo.load)
		return TRUE;
	m_url = stdString2CString(serverInfo.url);
	m_sendport = serverInfo.reqport;

	m_user = stdString2CString(serverInfo.loginUser);
	m_pass = stdString2CString(serverInfo.loginPass);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	auto &serverInfo = Config::getServerInfo();
	serverInfo.url = CString2stdString(m_url);

	serverInfo.reqport = m_sendport;

	serverInfo.loginUser = CString2stdString(m_user);
	serverInfo.loginPass = CString2stdString(m_pass);

	serverInfo.load = true;

	//TODO: ��¼�󷵻��û����ʹ���
	string userType = notAdmin ? "User" : "Admin";
	Message::LoginMsg msg{ serverInfo.loginUser,serverInfo.loginPass,userType };
	try {
		MsgHandler::stop();
		auto reply = MsgHandler::sendReqMsg<Message::LoginMsg>(msg);

		if (reply.status == "success") {
			Events::UserLogin e(User::getTypeByStr(userType));
			EventBus::dispatch(e);
			Config::saveServerCfg();

			CDialogEx::OnOK();
		}
		else {
			auto reason = StrUtil::stdString2CString(reply.meta);
			MessageBox(reason, _T("��½ʧ��"));
		}
	}
	catch (boost::system::system_error e) {
		MessageBox(_T("����ʧ��: ��ַ������߷����δ����"), _T("���Ӵ���"));
	}
}
