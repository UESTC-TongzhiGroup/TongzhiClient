// CameraSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Frame.h"
#include "CameraSet.h"
#include "SideBar.h"
#include "locale.h"
#include "MenuBarPanel.h"
#include "Events.h"
#include "NetHandler.h"

using namespace Cams;
using Cams::getCamMode;

// CCameraSet �Ի���

IMPLEMENT_DYNAMIC(CCameraSet, CDialogEx)

CCameraSet::CCameraSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraSet::IDD, pParent)
	, ip_addr(0)
	, userName(_T(""))
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
	DDX_IPAddress(pDX, IDC_IPADDRESS1, ip_addr);
	DDX_Text(pDX, IDC_EDIT2, userName);
	DDX_Text(pDX, IDC_EDIT4, passwd);
	DDX_Text(pDX, IDC_ADDRESS, cam_address);
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

CamMode CCameraSet::getMode() {
	//ͨ��ѡ��������ȡ��Ӧ��ģʽ
	int mode = m_CameraModeList.GetCurSel();
	return getCamMode(mode);
}

void CCameraSet::updateSel() {
	timeCheck.EnableWindow(true);
	timeCheck.SetCheck(true);
	const auto &cam = getCamInfo()[m_CurrentCamIndex];
	int start = 0, end = 0, modeSel = cam.mode;
	bool timed = cam.isTimed();

	cam_address = StrUtil::stdString2CString(cam.url);
	userName = StrUtil::stdString2CString(cam.cam_user);
	passwd = StrUtil::stdString2CString(cam.cam_password);
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
	timeEnd.EnableWindow(timed);
}

CString getCamModeName(int mode) {
	return StrUtil::stdString2CString(Cams::ModeName_ZH[getCamMode(mode)]);
}

void CCameraSet::updateCamItem(int index, const CamInfo& cam) {
	CString ip(cam.url.c_str()), user(cam.cam_user.c_str()), dua;
	m_EventList.SetItemText(index, 1, ip);
	m_EventList.SetItemText(index, 2, user);
	m_EventList.SetItemText(index, 3, getCamModeName(getCamMode(cam.mode)));
	if (cam.isTimed())
		dua.Format(_T("%sʱ �� %sʱ"),
			StrUtil::stdString2CString(cam.time_start),
			StrUtil::stdString2CString(cam.time_end));
	else
		dua = _T("ȫ��");
	m_EventList.SetItemText(index, 4, dua);
	auto itor = change.find(cam);
	if (itor != change.end())
		change.erase(itor);
	change.insert(cam);
}

// CCameraSet ��Ϣ�������
BOOL CCameraSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ȡ����ͷ�����ļ�
	
	for (int i = 0; i < CamMode::TOTAL_NUM; i++) {
		m_CameraModeList.AddString(getCamModeName(i));
	}

	CRect rect;
	m_EventList.GetClientRect(&rect);

	m_EventList.SetExtendedStyle(m_EventList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
	int n=0, width = rect.Width();
	m_EventList.InsertColumn(n++, _T("��ע"),			LVCFMT_CENTER, 120);
	width -= 120;
	int col_width = width / 3;
	m_EventList.InsertColumn(n++, _T("IP��ַ"),			LVCFMT_CENTER, col_width);
	width -= col_width;
	m_EventList.InsertColumn(n++, _T("�û���"),			LVCFMT_CENTER, col_width / 2);
	m_EventList.InsertColumn(n++, _T("����ģʽ"),		LVCFMT_CENTER, col_width / 2);
	width -= col_width;
	m_EventList.InsertColumn(n++, _T("���ʱ��"),		LVCFMT_CENTER, width);

	CString text;
	m_CameraList.SetCurSel(0);
	m_CameraModeList.SetCurSel(0);

	for (int i = 0; i < 24; i++) {
		text.Format(_T("%02d:00"), i);
		timeStart.AddString(text);
		timeEnd.AddString(text);
	}
	std::thread camPuller([this]()->void{
		const auto &camInfo = getCamInfo();
		CString text;
		// TODO:  �ڴ��������ͷ��ĳ�ʼ��
		try{
			Cams::updateCamList();
		}
		catch (Json::LogicError &e) {
			MessageBox(_T("����ʧ��: ���ĸ�ʽ����"), _T("�߼�����"));
			TRACE(CString(e.what()));
		}
		catch (...) {
			MessageBox(_T("����ʧ��: ��ַ������߷����δ����"), _T("���Ӵ���"));
		}
		tmpInfo.assign(camInfo.begin(), camInfo.end());
		for (int i = 0; i < getCamInfoNum(); i++) {
			const auto &cam = getCamInfo()[i];
			text.Format(_T("%02d·����ͷ"), i);
			m_CameraList.AddString(text);
			m_EventList.InsertItem(i, text);
			m_EventList.SetItemText(i, 1, StrUtil::stdString2CString(cam.url));
			m_EventList.SetItemText(i, 2, StrUtil::stdString2CString(cam.cam_user));
			m_EventList.SetItemText(i, 3, StrUtil::stdString2CString(Cams::ModeName_ZH[cam.mode]));
			CString time;
			cam.isTimed() ?
				time.Format(_T("%sʱ �� %sʱ"),
					StrUtil::stdString2CString(cam.time_start),
					StrUtil::stdString2CString(cam.time_end)) :
				time = "ȫ��";
			m_EventList.SetItemText(i, 4, time);

			//LV_ITEM item;
			//item.iItem = i;
			//item.mask = LVFIF_TEXT;
		}
	});
	camPuller.detach();
	return TRUE;  // return TRUE unless you set the focus to a control// �쳣:  OCX ����ҳӦ���� FALSE
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
	//�޸�����ͷ��Ϣ
	Message::ModifyCamMsg msg{ change };
	auto reply = MsgHandler::sendReqMsg(msg);
	if (!reply.isSuccess()) {
		MessageBox(StrUtil::stdString2CString(reply.meta), _T("����"));
	}
	//�����ע������ͷ
	std::set<string> id_set;
	auto &cams = getCamInfo();
	for (int i = 0; i < getCamInfoNum(); i++) {
		if (m_EventList.GetCheck(i)) {
			id_set.insert(cams[i].ID);
		}
	}
	Message::FocusCamMsg focus{ id_set };
	//������Ϣ��reply��ͬ������
	reply = MsgHandler::sendReqMsg(focus);
	if (!reply.isSuccess()) {
		MessageBox(StrUtil::stdString2CString(reply.meta), _T("����"));
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
	//��������ͷ����ģʽ
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	cam.mode = getMode();
	cam.active = cam.mode != CamMode::Inactive;

	updateSel();
	updateCamItem(m_CurrentCamIndex, cam);
}

void CCameraSet::OnBnClickedCheck()
{
	bool check = timeCheck.GetCheck() == BST_CHECKED;
	auto &cam = getCamInfo()[m_CurrentCamIndex];

	timeStart.EnableWindow(check);
	timeEnd.EnableWindow(check);
}

void CCameraSet::OnCbnChangeTimeStart()
{
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	int mode = m_CameraModeList.GetCurSel();
	//auto& dua = cam.timeSel;
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_TIMESTART);
	if (pCombox) {
		int sel = pCombox->GetCurSel();
		cam.time_start = StrUtil::format("%d", sel);

		updateCamItem(m_CurrentCamIndex, cam);
		updateSel();
	}
}

void CCameraSet::OnCbnSelchangeTimeEnd()
{
	auto &cam = getCamInfo()[m_CurrentCamIndex];
	int mode = m_CameraModeList.GetCurSel();
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_TIMEEND);
	if (pCombox) {
		int sel = pCombox->GetCurSel();
		cam.time_end = StrUtil::format("%d", sel);

		updateCamItem(m_CurrentCamIndex, cam);
		updateSel();
	}
}

void CCameraSet::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	auto &camInfo = getCamInfo();
	camInfo.swap(tmpInfo);
	CDialogEx::OnCancel();
}
