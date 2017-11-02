// CameraSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "CameraSet.h"
#include "SideBar.h"
#include "locale.h"
#include "MenuBarPanel.h"
#include "Events.h"
#include "NetHandler.h"
#include "AddCam.h"

using namespace Cams;
using Cams::getCamMode;

// CCameraSet 对话框

IMPLEMENT_DYNAMIC(CCameraSet, CDialogEx)

CCameraSet::CCameraSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraSet::IDD, pParent)
	, user_name(_T(""))
	, passwd(_T(""))
	, cam_address(_T(""))
{
	m_CurrentCamIndex = 0;
	m_pThis = this;
}

CCameraSet::~CCameraSet()
{
}

void CCameraSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERAL_LIST, m_CameraList);
	DDX_Control(pDX, IDC_EVENT_LIST, m_EventList);
	DDX_Control(pDX, IDC_CAMERAMODE, m_CameraModeList);
	DDX_Control(pDX, IDC_TIMESTART, timeStart);
	DDX_Control(pDX, IDC_TIMEEND, timeEnd);
	DDX_Control(pDX, IDC_CHECK1, timeCheck);
	DDX_Text(pDX, IDC_USER_NAME, user_name);
	DDX_Text(pDX, IDC_PASSWD, passwd);
	DDX_Text(pDX, IDC_ADDRESS, cam_address);
}


BEGIN_MESSAGE_MAP(CCameraSet, CDialogEx)
	ON_WM_CREATE()

	ON_BN_CLICKED(IDOK, &CCameraSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCameraSet::OnBnClickedCancel)

	ON_CBN_SELCHANGE(IDC_CAMERAL_LIST, &CCameraSet::OnCbnSelchangeCameralList)
	ON_CBN_SELCHANGE(IDC_CAMERAMODE, &CCameraSet::OnCbnSelchangeCameraMode)

	ON_BN_CLICKED(IDC_CHECK1, &CCameraSet::OnBnClickedCheck)
	ON_CBN_SELCHANGE(IDC_TIMESTART, &CCameraSet::OnCbnChangeTimeStart)
	ON_CBN_SELCHANGE(IDC_TIMEEND, &CCameraSet::OnCbnSelchangeTimeEnd)
	ON_EN_CHANGE(IDC_ADDRESS, &CCameraSet::OnEnChange)
	ON_EN_CHANGE(IDC_USER_NAME, &CCameraSet::OnEnChange)
	ON_EN_CHANGE(IDC_PASSWD, &CCameraSet::OnEnChange)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EVENT_LIST, &CCameraSet::OnLvnItemchangedEventList)
	ON_STN_CLICKED(IDC_ADD_CAM, &CCameraSet::OnStnClickedAddCam)

	ON_MESSAGE(GLOBAL_EVENT, &CCameraSet::updateAllCam)
	ON_BN_CLICKED(IDC_DEL_SELECTED_CAM, &CCameraSet::OnBnClickedDelSelectedCam)
END_MESSAGE_MAP()

CamMode CCameraSet::getMode() {
	//通过选项索引获取对应的模式
	int mode = m_CameraModeList.GetCurSel();
	return getCamMode(mode);
}

void CCameraSet::updateSel() {
	/*
	TODO: 重写更新选中摄像头的逻辑
	timeCheck.EnableWindow(true);
	timeCheck.SetCheck(true);
	const auto &cam = getCamInfo()[m_CurrentCamIndex];
	int start = 0, end = 0, modeSel = cam.mode;
	bool timed = cam.isTimed();

	cam_address = StrUtil::std2CStr(cam.url);
	user_name = StrUtil::std2CStr(cam.cam_user);
	passwd = StrUtil::std2CStr(cam.cam_password);
	UpdateData(FALSE);

	m_CameraList.SetCurSel(m_CurrentCamIndex);
	m_CameraModeList.SetCurSel(modeSel);
	switch (modeSel) {
	case CamMode::OnCross:
	case CamMode::OnInvade:
	case CamMode::OnWander:
	case CamMode::OnFight:
		stringstream(cam.time_start) >> start;
		stringstream(cam.time_end) >> end;
		break;
	default:
		timeCheck.SetCheck(false);
		timeCheck.EnableWindow(false);
	}
	if (!timed){
		timeCheck.SetCheck(false);
		timeStart.SetCurSel(-1);
		timeEnd.SetCurSel(-1);
	}else{
		timeStart.SetCurSel(start);
		timeEnd.SetCurSel(end);
	}
	timeStart.EnableWindow(timed);
	timeEnd.EnableWindow(timed);*/
}

CString getCamModeName(int mode) {
	return StrUtil::std2CStr(Cams::ModeName_ZH[getCamMode(mode)]);
}

void CCameraSet::updateCamItem(int index, const CamInfo& cam) {
	CString ip(cam.url.c_str()), user(cam.cam_user.c_str()), dua;
	m_EventList.SetItemText(index, 1, ip);
	m_EventList.SetItemText(index, 2, user);
	m_EventList.SetItemText(index, 3, getCamModeName(getCamMode(cam.mode)));
	if (cam.isTimed())
		dua.Format(_T("%s时 至 %s时"),
			StrUtil::std2CStr(cam.time_start),
			StrUtil::std2CStr(cam.time_end));
	else
		dua = _T("全天");
	m_EventList.SetItemText(index, 4, dua);
	auto itor = cam_change.find(cam);
	if (itor != cam_change.end())
		cam_change.erase(itor);
	cam_change.insert(cam);
}

LRESULT CCameraSet::updateAllCam(WPARAM, LPARAM) {
	auto &camInfo = getCamInfo();
	m_CameraList.Clear();
	m_EventList.DeleteAllItems();
	int i = 0;
	for (auto &itor:getCamInfo()) {
		const auto &cam = itor.second;
		CString name(cam.ID.c_str());
		m_CameraList.AddString(name);
		m_EventList.InsertItem(i, name);
		m_EventList.SetCheck(i, TRUE);
		m_EventList.SetItemText(i, 1, StrUtil::std2CStr(cam.url));
		m_EventList.SetItemText(i, 2, StrUtil::std2CStr(cam.cam_user));
		m_EventList.SetItemText(i, 3, StrUtil::std2CStr(Cams::ModeName_ZH[cam.mode]));
		CString time;
		cam.isTimed() ?
			time.Format(_T("%s时 至 %s时"),
				StrUtil::std2CStr(cam.time_start),
				StrUtil::std2CStr(cam.time_end)) :
			time = "全天";
		m_EventList.SetItemText(i, 4, time);
		i++;
	}
	tmpInfo = camInfo;
	return TRUE;
}

// CCameraSet 消息处理程序
int CCameraSet::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	EventBus::regist(Events::CamListUpdate::ID(), GetSafeHwnd());
	return 0;
}

BOOL CCameraSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//读取摄像头配置文件
	for (int i = 0; i < CamMode::TOTAL_NUM; i++) {
		m_CameraModeList.AddString(getCamModeName(i));
	}
	
	CRect rect;
	m_EventList.GetClientRect(&rect);

	m_EventList.SetExtendedStyle(m_EventList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
	int n=0, width = rect.Width();
	m_EventList.InsertColumn(n++, _T("关注"),			LVCFMT_CENTER, 120);
	width -= 120;
	int col_width = width / 3;
	m_EventList.InsertColumn(n++, _T("IP地址"),			LVCFMT_CENTER, col_width);
	width -= col_width;
	m_EventList.InsertColumn(n++, _T("用户名"),			LVCFMT_CENTER, col_width / 2);
	m_EventList.InsertColumn(n++, _T("工作模式"),		LVCFMT_CENTER, col_width / 2);
	width -= col_width;
	m_EventList.InsertColumn(n++, _T("检测时段"),		LVCFMT_CENTER, width);

	CString text;
	for (int i = 0; i < 24; i++) {
		text.Format(_T("%02d:00"), i);
		timeStart.AddString(text);
		timeEnd.AddString(text);
	}
	std::thread camPuller([this]()->void{
		const auto &camInfo = getCamInfo();
		CString text;
		// TODO:  在此添加摄像头项的初始化
		try{
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
	return TRUE;  // return TRUE unless you set the focus to a control// 异常:  OCX 属性页应返回 FALSE
}

CCameraSet * CCameraSet::m_pThis = NULL;
CCameraSet * CCameraSet::GetInstance()
{
	return m_pThis;
}

void CCameraSet::OnBnClickedOk() 
{
	CSideBar *pWnd = CSideBar::GetInstance();
	if (pWnd == nullptr)
		return;
	//修改摄像头信息
	if (cam_change.size() > 0) {
		Message::ModifyCamMsg msg{ cam_change };
		auto reply = MsgHandler::sendReqMsg(msg);
		if (!reply.isSuccess()) {
			MessageBox(StrUtil::std2CStr(reply.meta), _T("错误"));
		}
	}

	//处理关注的摄像头
	std::set<string> id_set;
	auto &cams = getCamInfo();
	for (int i = 0; i < getCamInfoNum(); i++) {
		if (m_EventList.GetCheck(i)) {
			id_set.insert(cams[StrUtil::CStr2std(m_EventList.GetItemText(i,0))].ID);
		}
	}

	if (id_set.size() > 0) {
		Message::FocusCamMsg focus{ id_set };
		auto reply = MsgHandler::sendReqMsg(focus);
		if (!reply.isSuccess()) {
			MessageBox(StrUtil::std2CStr(reply.meta), _T("错误"));
		}
	}

	if (cam_delete.size() > 0) {
		Message::DelCamMsg del{ cam_delete };
		auto reply = MsgHandler::sendReqMsg(del);
		if (!reply.isSuccess()) {
			MessageBox(StrUtil::std2CStr(reply.meta), _T("错误"));
		}
	}

	CDialogEx::OnOK();
}

void CCameraSet::OnCbnSelchangeCameralList()
{
	CComboBox* pComboxWndMode = (CComboBox*)GetDlgItem(IDC_CAMERAL_LIST);
	if (pComboxWndMode)
	{
		int nIdx = pComboxWndMode->GetCurSel();
		if (nIdx >= 0 && nIdx < getCamInfoNum())
			m_CurrentCamIndex = nIdx;
		updateSel();
	}
}

void CCameraSet::OnLvnItemchangedEventList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int& sel = m_CurrentCamIndex;
	sel = pNMLV->iItem;
	m_CameraList.SetCurSel(sel);
	updateSel();
	*pResult = 0;
}

void CCameraSet::OnCbnSelchangeCameraMode()
{
	//更新摄像头工作模式
	/*重写
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	cam.mode = getMode();
	cam.mode_name = Cams::ModeName[cam.mode];
	cam.active = (cam.mode != CamMode::Inactive);

	updateSel();
	updateCamItem(m_CurrentCamIndex, cam);*/
}

void CCameraSet::OnBnClickedCheck()
{
	/*重写
	bool check = timeCheck.GetCheck() == BST_CHECKED;
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	if (!check) {
		cam.setFullDayTask();
		updateCamItem(m_CurrentCamIndex, cam);
	}
	timeStart.EnableWindow(check);
	timeEnd.EnableWindow(check);*/
}

void CCameraSet::OnCbnChangeTimeStart()
{
	/*TODO: 重写
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	int mode = m_CameraModeList.GetCurSel();
	//auto& dua = cam.timeSel;
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_TIMESTART);
	if (pCombox) {
		int sel = pCombox->GetCurSel();
		cam.time_start = StrUtil::format("%d", sel);

		updateCamItem(m_CurrentCamIndex, cam);
		updateSel();
	}*/
}

void CCameraSet::OnCbnSelchangeTimeEnd()
{
	/*TODO: 重写
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	int mode = m_CameraModeList.GetCurSel();
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_TIMEEND);
	if (pCombox) {
		int sel = pCombox->GetCurSel();
		cam.time_end = StrUtil::format("%d", sel);

		updateCamItem(m_CurrentCamIndex, cam);
		updateSel();
	}*/
}

void CCameraSet::OnBnClickedCancel()
{
	auto &camInfo = getCamInfo();
	camInfo.swap(tmpInfo);
	CDialogEx::OnCancel();
}

void CCameraSet::OnEnChange()
{
	UpdateData();
	/*TODO: 重写修改 文本框内文本更新 对应摄像头更新逻辑
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	cam.url = StrUtil::CStr2std(cam_address);
	cam.cam_user = StrUtil::CStr2std(user_name);
	cam.cam_password = StrUtil::CStr2std(passwd);
	updateCamItem(m_CurrentCamIndex, cam);*/
}

void CCameraSet::OnStnClickedAddCam()
{
	CAddCam addCam = new CAddCam();
	addCam.DoModal();
	delete addCam;
}


void CCameraSet::OnBnClickedDelSelectedCam()
{
	/*
	TODO: 重写删除摄像头逻辑
	int nItem = m_CurrentCamIndex;
	cam_delete.emplace(StrUtil::CStr2std(m_EventList.GetItemText(nItem, 0)));
	m_EventList.DeleteItem(nItem);
	m_CameraList.DeleteString(nItem);
	auto &cam = getCamInfo();
	auto itor = cam_change.find(cam[nItem]);
	if (itor != cam_change.end()) {
		cam_change.erase(itor);
	}
	cam.erase(cam.begin() + nItem);*/
}
