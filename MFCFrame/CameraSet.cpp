#include "stdafx.h"
#include "CameraSet.h"
#include "Frame.h"
#include "Events.h"
#include "EventBus.h"
#include "NetHandler.h"
#include "CamUtils.h"
#include "StrUtil.h"
#include <thread>
// CCameraSet �Ի���

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
			time.Format(_T("%sʱ �� %sʱ"),
				StrUtil::std2CStr(cam.time_start),
				StrUtil::std2CStr(cam.time_end)) :
			time = "ȫ��";
		camListCtrl.SetItemText(i, 4, time);
		i++;
	}
	tmpInfo = camInfo;
}

void CCameraSet::pullCamInfo()
{
	//����һ�߳�ȥ��ȡ����ͷ����
	std::thread camPuller([this]()->void {
		const auto &camInfo = getCamInfo();
		CString text;
		// TODO:  �ڴ��������ͷ��ĳ�ʼ��
		try {
			Cams::pullCamInfoList();
		}
		catch (Json::LogicError &e) {
			MessageBox(_T("����ʧ��: ���ĸ�ʽ����"), _T("�߼�����"));
			TRACE(CString(e.what()));
		}
		catch (...) {
			MessageBox(_T("����ʧ��: ��ַ������߷����δ����"), _T("���Ӵ���"));
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

// CCameraSet ��Ϣ�������
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
	camListCtrl.InsertColumn(n++, _T("��ע"), LVCFMT_CENTER, 120);
	width -= 120;
	int col_width = width / 3;
	camListCtrl.InsertColumn(n++, _T("IP��ַ"), LVCFMT_CENTER, col_width);
	width -= col_width;
	camListCtrl.InsertColumn(n++, _T("�û���"), LVCFMT_CENTER, col_width / 2);
	camListCtrl.InsertColumn(n++, _T("����ģʽ"), LVCFMT_CENTER, col_width / 2);
	width -= col_width;
	camListCtrl.InsertColumn(n++, _T("���ʱ��"), LVCFMT_CENTER, width);

	CString text;
	for (int i = 0; i < 24; i++) {
		text.Format(_T("%02d:00"), i);
		timeStart.AddString(text);
		timeEnd.AddString(text);
	}

	pullCamInfo();
	return TRUE;  // return TRUE unless you set the focus to a control// �쳣:  OCX ����ҳӦ���� FALSE
}

void CCameraSet::OnDestroy()
{
	CDialogEx::OnDestroy();
	EventBus::unregistAll(this);
	// TODO: �ڴ˴������Ϣ����������
}

void CCameraSet::OnSelchangeCamComboList()
{
	selIndex = camComboList.GetCurSel();
	//TODO: ����ѡ����
	updateSel();
}

void CCameraSet::OnSelchangeCamList(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	selIndex = pNMLV->iItem;
	camComboList.SetCurSel(selIndex);		//SetCulSel��Ȼ������õ���Ӧ�ؼ���ON_CBN_SELCHANGE��Ӧ���¼�����
	//TODO: ����ѡ����
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
		//TODO: ������
	}
	EventBus::dispatch(Events::CamListUpdate());
	CDialogEx::OnOK();
}

void CCameraSet::OnCancel()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	tmpInfo.swap(Cams::getCamInfo());
	EventBus::dispatch(Events::CamListUpdate());
	CDialogEx::OnCancel();
}
