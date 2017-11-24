#include "stdafx.h"
#include "CameraSet.h"
#include "Frame.h"
#include "Events.h"
#include "EventBus.h"
#include "NetHandler.h"
#include "CamUtils.h"
#include "StrUtil.h"
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
}

void CCameraSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERAL_LIST,	camComboList);
	DDX_Control(pDX, IDC_EVENT_LIST,	camListCtrl);
	DDX_Control(pDX, IDC_CAMERAMODE, modeComboList);
	DDX_Control(pDX, IDC_TIMESTART, timeStart);
	DDX_Control(pDX, IDC_TIMEEND, timeEnd);
	DDX_Control(pDX, IDC_CHECK1, timeCheck);
	/*
	DDX_Text(pDX, IDC_USER_NAME, user_name);
	DDX_Text(pDX, IDC_PASSWD, passwd);
	DDX_Text(pDX, IDC_ADDRESS, cam_address);
	*/
}

BEGIN_MESSAGE_MAP(CCameraSet, CDialogEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CAMERAL_LIST, &CCameraSet::OnSelchangeCamComboList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EVENT_LIST, &CCameraSet::OnSelchangeCamList)
	ON_CBN_SELCHANGE(IDC_CAMERAMODE, &CCameraSet::OnSelchangeMode)

	ON_BN_CLICKED(IDOK, &CCameraSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCameraSet::OnCancel)
END_MESSAGE_MAP()

void CCameraSet::OnUpdateAllCam() {
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
			Cams::pullCamInfoList();
		}
		catch (Json::LogicError &e) {
			MessageBox(_T("解析失败: 报文格式错误"), _T("逻辑错误"));
			TRACE(CString(e.what()));
		}
		catch (...) {
			MessageBox(_T("连接失败: 地址错误或者服务端未开启"), _T("连接错误"));
		}
		tmpInfo = camInfo;
		selIndex = 0;
		updateSel();
	});
	camPuller.detach();
}

void CCameraSet::updateSel()
{
	camComboList.SetCurSel(selIndex);
	std::string name = StrUtil::CStr2std(
		camListCtrl.GetItemText(selIndex, 0)
	);
	const CamInfo &cam = getCamInfo()[name];
	modeComboList.SetCurSel(cam.mode);
}

// CCameraSet 消息处理程序
int CCameraSet::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	EventBus::regist<CamListUpdate>(CREAT_HANDLER(
		[this](BaseEvent &e)->void {
			OnUpdateAllCam();
		}
	));
	return 0;
}

BOOL CCameraSet::OnInitDialog()
{
	mod.clear();
	CDialogEx::OnInitDialog();
	for (int i = 0; i < CamMode::TOTAL_NUM; i++) {
		modeComboList.AddString(
			StrUtil::std2CStr(Cams::ModeName_ZH[i])
		);
	}
	
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

	CString text;
	for (int i = 0; i < 24; i++) {
		text.Format(_T("%02d:00"), i);
		timeStart.AddString(text);
		timeEnd.AddString(text);
	}

	pullCamInfo();
	return TRUE;  // return TRUE unless you set the focus to a control// 异常:  OCX 属性页应返回 FALSE
}

void CCameraSet::OnDestroy()
{
	CDialogEx::OnDestroy();
	EventBus::unregistAll(this);
	// TODO: 在此处添加消息处理程序代码
}

void CCameraSet::OnSelchangeCamComboList()
{
	selIndex = camComboList.GetCurSel();
	//TODO: 更新选择项
	updateSel();
}

void CCameraSet::OnSelchangeCamList(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	selIndex = pNMLV->iItem;
	camComboList.SetCurSel(selIndex);		//SetCulSel竟然不会调用到对应控件的ON_CBN_SELCHANGE对应的事件处理
	//TODO: 更新选择项
	updateSel();
}

void CCameraSet::OnSelchangeMode()
{
	std::string name = StrUtil::CStr2std(
		camListCtrl.GetItemText(selIndex, 0)
	);
	CamInfo &cam = getCamInfo()[name];
	cam.mode = Cams::getCamMode(modeComboList.GetCurSel());
	cam.mode_name = Cams::ModeName[cam.mode];
	mod.emplace(cam.ID);

	OnUpdateAllCam();
}


void CCameraSet::OnBnClickedOk()
{
	Message::ModifyCamMsg msg;
	for (auto &itor : mod) {
		msg.list.insert(Cams::getCamInfo()[itor]);
	}
	auto reply = MsgHandler::sendReqMsg(msg);
	if (!reply.isSuccess()) {
		//TODO: 出错处理
	}
	EventBus::dispatch(Events::CamListUpdate());
	CDialogEx::OnOK();
}

void CCameraSet::OnCancel()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	tmpInfo.swap(Cams::getCamInfo());
	EventBus::dispatch(Events::CamListUpdate());
	CDialogEx::OnCancel();
}
