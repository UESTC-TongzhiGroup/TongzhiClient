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

using namespace Config;
using namespace Cams;

// CCameraSet 对话框

IMPLEMENT_DYNAMIC(CCameraSet, CDialogEx)

CCameraSet::CCameraSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraSet::IDD, pParent)
	, ip_addr(0)
	, userName(_T(""))
	, passwd(_T(""))
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
	DDX_IPAddress(pDX, IDC_IPADDRESS1, ip_addr);
	DDX_Text(pDX, IDC_EDIT2, userName);
	DDX_Text(pDX, IDC_EDIT4, passwd);
}


BEGIN_MESSAGE_MAP(CCameraSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCameraSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCameraSet::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CAMERAL_LIST, &CCameraSet::OnCbnSelchangeCameralList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EVENT_LIST, &CCameraSet::OnLvnItemchangedEventList)
	ON_CBN_SELCHANGE(IDC_CAMERAMODE, &CCameraSet::OnCbnSelchangeCameraMode)
	ON_BN_CLICKED(IDC_CHECK1, &CCameraSet::OnBnClickedCheck)
	ON_CBN_SELCHANGE(IDC_TIMESTART, &CCameraSet::OnCbnChangeTimeStart)
	ON_CBN_SELCHANGE(IDC_TIMEEND, &CCameraSet::OnCbnSelchangeTimeEnd)
END_MESSAGE_MAP()

CamMode CCameraSet::getMode(int sel) {
	switch (sel) {
	case 0:
		return CamMode::Inactive;
	case 1:
		return CamMode::OnCross;
	case 2:
		return CamMode::OnInvade;
	case 3:
		return CamMode::OnWander;
	case 4:
		return CamMode::OnFight;
	default:
		throw std::out_of_range("sel out of range");
		return CamMode::Inactive;
	}
}

CamMode CCameraSet::getMode() {
	//通过选项索引获取对应的模式
	int mode = m_CameraModeList.GetCurSel();
	return getMode(mode);
}

void CCameraSet::updateSel() {
	timeCheck.EnableWindow(true);
	timeCheck.SetCheck(true);
	const auto &cam = getCamInfo()[m_CurrentCamIndex];
	int start = 0, end = 0, modeSel = cam.mode;

	m_CameraList.SetCurSel(m_CurrentCamIndex);
	m_CameraModeList.SetCurSel(modeSel);
	/*TODO: 
	switch (modeSel) {
	case CamMode::OnCross:
	case CamMode::OnInvade:
	case CamMode::OnWander:
	case CamMode::OnFight:
		start = dua.start;
		end = dua.end;
		break;
	default:
		timeCheck.SetCheck(false);
		timeCheck.EnableWindow(false);
	}
	if (!dua.timeing){
		timeCheck.SetCheck(false);
		timeStart.SetCurSel(-1);
		timeEnd.SetCurSel(-1);
	}else{
		timeStart.SetCurSel(start);
		timeEnd.SetCurSel(end);
	}
	timeStart.EnableWindow(dua.timeing);
	timeEnd.EnableWindow(dua.timeing);*/
}
/*TODO: 
void CCameraSet::updateEventListItem(int index, const CameraConfig& cam) {
	CString ip(cam.url.c_str()), user(cam.cam_user.c_str()), dua;
	m_EventList.SetItemText(index, 0, ip);
	m_EventList.SetItemText(index, 1, user);
	m_EventList.SetItemText(index, 2, getCamModeName(getMode(cam.mode)));
	if (cam.timeSel.timeing)
		dua.Format(_T("%02d:00-%02d:00"), cam.timeSel.start, cam.timeSel.end);
	else
		dua = _T("全天");
	m_EventList.SetItemText(index, 3, dua);
}
*/
// CCameraSet 消息处理程序
BOOL CCameraSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//读取摄像头配置文件
	
	for (int i = 0; i < CamMode::TOTAL_NUM; i++) {
		m_CameraModeList.AddString(getCamModeName(getMode(i)));
	}

	CRect rect;
	m_EventList.GetClientRect(&rect);

	m_EventList.SetExtendedStyle(m_EventList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	int n = 0;
	m_EventList.InsertColumn(n++, _T("IP地址"),			LVCFMT_CENTER, rect.Width() / 3);
	m_EventList.InsertColumn(n++, _T("用户名"),			LVCFMT_CENTER, rect.Width() / 6);
	m_EventList.InsertColumn(n++, _T("工作模式"),		LVCFMT_CENTER, rect.Width() / 6);
	m_EventList.InsertColumn(n++, _T("检测时段"),		LVCFMT_CENTER, rect.Width() / 3);

	CString text;
	// TODO:  在此添加摄像头项的初始化
	for (int i = 0; i < getCamInfoNum(); i++) {
		text.Format(_T("%02d路摄像头"), i);
		m_CameraList.AddString(text);
		m_EventList.InsertItem(i, text);
	}
	m_CameraList.SetCurSel(0);
	m_CameraModeList.SetCurSel(0);

	for (int i = 0; i < 24; i++) {
		text.Format(_T("%02d:00"), i);
		timeStart.AddString(text);
		timeEnd.AddString(text);
	}
	updateSel();
	const auto &camInfo = getCamInfo();
	tmpInfo.assign(camInfo.begin(), camInfo.end());
	n = 0;
	for (const auto &cam : camInfo) {
		LV_ITEM item;
		item.iItem = n;
		item.mask = LVFIF_TEXT;
		//updateEventListItem(n, cam);
		n++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control// 异常:  OCX 属性页应返回 FALSE
}

CCameraSet * CCameraSet::m_pThis = NULL;
CCameraSet * CCameraSet::GetInstance()
{
	return m_pThis;
}

void CCameraSet::OnBnClickedOk() 
{
	CString queryroute;
	CSideBar *pWnd = CSideBar::GetInstance();
	if (pWnd == nullptr)
		return;
	//helper.saveCamInfo();
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
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	cam.mode = getMode();
	cam.active = cam.mode != CamMode::Inactive;

	updateSel();
	//updateEventListItem(m_CurrentCamIndex, cam);
}

void CCameraSet::OnBnClickedCheck()
{
	bool check = timeCheck.GetCheck() == BST_CHECKED;
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	//cam.timeSel.timeing = check;

	timeStart.EnableWindow(check);
	timeEnd.EnableWindow(check);
	//updateEventListItem(m_CurrentCamIndex, cam);
}

void CCameraSet::OnCbnChangeTimeStart()
{
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	int mode = m_CameraModeList.GetCurSel();
	//auto& dua = cam.timeSel;
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_TIMESTART);
	if (pCombox) {
		int sel = pCombox->GetCurSel();
		//dua.start = sel;

		//updateEventListItem(m_CurrentCamIndex, cam);
	}
}

void CCameraSet::OnCbnSelchangeTimeEnd()
{
	//auto &cam = getCamInfo()[m_CurrentCamIndex];
	int mode = m_CameraModeList.GetCurSel();
	//auto& dua = cam.timeSel;
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_TIMEEND);
	if (pCombox) {
		int sel = pCombox->GetCurSel();
		//dua.end = sel;

		//updateEventListItem(m_CurrentCamIndex, cam);
	}
}

void CCameraSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	auto &camInfo = getCamInfo();
	camInfo.swap(tmpInfo);
	CDialogEx::OnCancel();
}
