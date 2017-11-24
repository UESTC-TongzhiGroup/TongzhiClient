#include "stdafx.h"
#include "AddCam.h"
#include "Frame.h"

#include "CamUtils.h"
#include "NetHandler.h"
#include "NetMessage.h"
#include "StrUtil.h"

IMPLEMENT_DYNAMIC(CAddCam, CDialogEx)

#define Cstr2Str StrUtil::CStr2std

CAddCam::CAddCam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddCam::IDD, pParent)
	, ID(_T(""))
	, name(_T(""))
{
}

CAddCam::~CAddCam()
{
}

void CAddCam::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_CAM_ID, ID);
	DDX_Text(pDX, IDC_CAM_NAME, name);
	DDX_Text(pDX, IDC_CAM_URL, url);

	DDX_Text(pDX, IDC_CAM_USER, cam_user);
	DDX_Text(pDX, IDC_CAM_PASSWORD, cam_password);
	DDX_Control(pDX, IDC_CAM_ADD_LIST, add_list);
}

BEGIN_MESSAGE_MAP(CAddCam, CDialogEx)
	ON_BN_CLICKED(IDC_CAM_ADD, &CAddCam::OnBnClickedCamAdd)
	ON_BN_CLICKED(IDOK, &CAddCam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DEL_SELECTED_CAM, &CAddCam::OnBnClickedDelSelectedCam)
END_MESSAGE_MAP()

void CAddCam::OnBnClickedCamAdd()
{
	UpdateData();
	int num = add_list.GetItemCount();
	add_list.InsertItem(num, ID);
	add_list.SetItemText(num, 1, name);
	add_list.SetItemText(num, 2, url);
	add_list.SetItemText(num, 3, cam_user);
	add_list.SetItemText(num, 4, cam_password);
}


BOOL CAddCam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	add_list.SetExtendedStyle(add_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	CRect rect;
	add_list.GetClientRect(&rect);

	int n = 0, width = rect.Width();

	add_list.InsertColumn(n++, _T("ID"), LVCFMT_CENTER, width / 5);
	add_list.InsertColumn(n++, _T("名称"), LVCFMT_CENTER, width / 5);
	add_list.InsertColumn(n++, _T("地址"), LVCFMT_CENTER, width / 5);
	add_list.InsertColumn(n++, _T("用户名"), LVCFMT_CENTER, width / 5);
	add_list.InsertColumn(n++, _T("密码"), LVCFMT_CENTER, width / 5);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

#define GetStr(i,sub) StrUtil::CStr2std(add_list.GetItemText(i,sub))
void CAddCam::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	Message::AddCamMsg msg;
	for (int i = 0; i < add_list.GetItemCount(); i++) {
		msg.list.emplace(
			GetStr(i, 0),
			GetStr(i, 1),
			GetStr(i, 2),
			GetStr(i, 3),
			GetStr(i, 4)
		);
	}
	auto reply = MsgHandler::sendReqMsg(msg);
	if (!reply.isSuccess()) {
		//出错处理
	}
	Cams::pullCamInfoList();
	CDialogEx::OnOK();
}


void CAddCam::OnBnClickedDelSelectedCam()
{
	POSITION pos = add_list.GetFirstSelectedItemPosition();
	while (pos != nullptr) {
		int nItem = add_list.GetNextSelectedItem(pos);
		add_list.DeleteItem(nItem);
		pos = add_list.GetFirstSelectedItemPosition();
	}
}
