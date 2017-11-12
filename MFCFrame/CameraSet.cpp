#include "stdafx.h"
#include "CameraSet.h"
#include "Frame.h"
#include "Events.h"
#include "EventBus.h"
#include "CamUtils.h"
#include <thread>
// CCameraSet 对话框

IMPLEMENT_DYNAMIC(CCameraSet, CDialogEx)

using namespace Cams;
using namespace Events;

CCameraSet::CCameraSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraSet::IDD, pParent)
//	, user_name(_T(""))
//	, passwd(_T(""))
//	, cam_address(_T(""))
{
//	m_CurrentCamIndex = 0;
	m_pThis = this;
}

CCameraSet * CCameraSet::m_pThis = NULL;

CCameraSet * CCameraSet::GetInstance()
{
	return m_pThis;
}

CCameraSet::~CCameraSet()
{
	EventBus::unregistAll(GetSafeHwnd());
}

void CCameraSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERAL_LIST,	camComboList);
	DDX_Control(pDX, IDC_EVENT_LIST,	camListCtrl);
	/*
	DDX_Control(pDX, IDC_CAMERAMODE, m_CameraModeList);
	DDX_Control(pDX, IDC_TIMESTART, timeStart);
	DDX_Control(pDX, IDC_TIMEEND, timeEnd);
	DDX_Control(pDX, IDC_CHECK1, timeCheck);
	DDX_Text(pDX, IDC_USER_NAME, user_name);
	DDX_Text(pDX, IDC_PASSWD, passwd);
	DDX_Text(pDX, IDC_ADDRESS, cam_address);
	*/
}

BEGIN_MESSAGE_MAP(CCameraSet, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CCameraSet::OnUpdateAllCam(CamListUpdate&) {
	auto &camInfo = getCamInfo();
	camComboList.Clear();
	camListCtrl.DeleteAllItems();
	int i = 0;
	for (auto &itor : getCamInfo()) {
		const auto &cam = itor.second;
		CString name(cam.ID.c_str());
		camComboList.AddString(name);
		camListCtrl.InsertItem(i, name);
		camListCtrl.SetCheck(i, TRUE);
		camListCtrl.SetItemText(i, 1, StrUtil::std2CStr(cam.url));
		camListCtrl.SetItemText(i, 2, StrUtil::std2CStr(cam.cam_user));
		camListCtrl.SetItemText(i, 3, StrUtil::std2CStr(Cams::ModeName_ZH[cam.mode]));
		CString time;
		cam.isTimed() ?
			time.Format(_T("%s时 至 %s时"),
				StrUtil::std2CStr(cam.time_start),
				StrUtil::std2CStr(cam.time_end)) :
			time = "全天";
		camListCtrl.SetItemText(i, 4, time);
		i++;
	}
	tmpInfo = camInfo;
}

void CCameraSet::pullCamInfo()
{
	//另创建一线程去拉取摄像头配置
	std::thread camPuller([this]()->void {
		const auto &camInfo = getCamInfo();
		CString text;
		// TODO:  在此添加摄像头项的初始化
		try {
			Cams::updateCamList();
		}
		catch (Json::LogicError &e) {
			MessageBox(_T("解析失败: 报文格式错误"), _T("逻辑错误"));
			TRACE(CString(e.what()));
		}
		catch (...) {
			MessageBox(_T("连接失败: 地址错误或者服务端未开启"), _T("连接错误"));
		}
		tmpInfo = camInfo;
	});
	camPuller.detach();
}

// CCameraSet 消息处理程序
int CCameraSet::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	EventBus::regist<CamListUpdate>(CREAT_HANDLER(
		[this](BaseEvent &e)->void {
			OnUpdateAllCam(static_cast<CamListUpdate&>(e));
		}
	));
	return 0;
}

BOOL CCameraSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	/*TODO: 工作模式列表初始化
	for (int i = 0; i < CamMode::TOTAL_NUM; i++) {
		m_CameraModeList.AddString(getCamModeName(i));
	}
	*/
	CRect rect;
	camListCtrl.GetClientRect(&rect);

	camListCtrl.SetExtendedStyle(camListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	int n = 0, width = rect.Width();
	camListCtrl.InsertColumn(n++, _T("关注"), LVCFMT_CENTER, 120);
	width -= 120;
	int col_width = width / 3;
	camListCtrl.InsertColumn(n++, _T("IP地址"), LVCFMT_CENTER, col_width);
	width -= col_width;
	camListCtrl.InsertColumn(n++, _T("用户名"), LVCFMT_CENTER, col_width / 2);
	camListCtrl.InsertColumn(n++, _T("工作模式"), LVCFMT_CENTER, col_width / 2);
	width -= col_width;
	camListCtrl.InsertColumn(n++, _T("检测时段"), LVCFMT_CENTER, width);

	/*TODO: 初始化时间选择列表
	CString text;
	for (int i = 0; i < 24; i++) {
		text.Format(_T("%02d:00"), i);
		timeStart.AddString(text);
		timeEnd.AddString(text);
	}
	*/

	pullCamInfo();
	return TRUE;  // return TRUE unless you set the focus to a control// 异常:  OCX 属性页应返回 FALSE
}