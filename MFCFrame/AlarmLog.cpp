// AlarmLog.cpp : 实现文件
//

#include "stdafx.h"
#include "Frame.h"
#include "AlarmLog.h"
#include "EventBus.h"
#include "Events.h"

#define IDC_ALARMLOGLIST 10000
#define IDC_ALARMCOUNT 10001
#define IDC_NOWYEAR		10002
#define	IDC_NOWMOUTH	10003
#define	IDC_NOWDAY		10004
#define	IDC_FORYEAR		10005
#define	IDC_FORMOUTH	10006
#define	IDC_FORDAY		10007
#define	IDC_FOROM		10008
#define	IDC_LAST		10009
#define IDC_CAMERA		10010
#define IDC_PEOPLE		10011
#define IDC_ALARM       10012

#define IDC_EDITE0		10013
#define IDC_EDITE1		10013
#define IDC_EDITE2		10013
#define IDC_EDITE3		10013
#define IDC_EDITE4		10013
#define IDC_EDITE5		10013
#define IDC_EDITE6		10013
#define IDC_EDITE7		10013
#define IDC_EDITE8		10013
#define IDC_EDITE9		10013
#define IDC_EDITE10		10013
#define IDC_EDITE11		10013
#define IDC_COMBOBOX0       10014
#define IDC_COMBOBOX1       10014
// CAlarmLog 对话框

IMPLEMENT_DYNAMIC(CAlarmLog, CDialogEx)

CAlarmLog::CAlarmLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlarmLog::IDD, pParent)
{
	m_pThis = this;
	m_AlarmLogList.m_hWnd = NULL;
	m_alarmcount.m_hWnd = NULL;
	m_nowyear.m_hWnd = NULL;
	m_nowmouth.m_hWnd = NULL;
	m_nowday.m_hWnd = NULL;
	m_foryear.m_hWnd = NULL;
	m_formouth.m_hWnd = NULL;
	m_forday.m_hWnd = NULL;
	m_from.m_hWnd = NULL;
	m_last.m_hWnd = NULL;
	m_camera.m_hWnd = NULL;
	m_people.m_hWnd = NULL;
	m_alarm.m_hWnd = NULL;
	m_edit0.m_hWnd = NULL;
	m_edit1.m_hWnd = NULL;
	m_edit2.m_hWnd = NULL;
	m_edit3.m_hWnd = NULL;
	m_edit4.m_hWnd = NULL;
	m_edit5.m_hWnd = NULL;
	m_edit6.m_hWnd = NULL;
	m_edit7.m_hWnd = NULL;
	m_edit8.m_hWnd = NULL;
	m_edit9.m_hWnd = NULL;
	m_edit10.m_hWnd = NULL;
	m_edit11.m_hWnd = NULL;
	m_comboBox0.m_hWnd = NULL;
	m_comboBox1.m_hWnd = NULL;
}

CAlarmLog::~CAlarmLog()
{
}

void CAlarmLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_ALARMLOG, m_AlarmLogList);
}


BEGIN_MESSAGE_MAP(CAlarmLog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CAlarmLog 消息处理程序

using StrUtil::std2CStr;
void CAlarmLog::OnWarnMsg(Warn &_event) {
	CString str;
	const auto &warn = _event.warn;
	int index = m_AlarmLogList.GetItemCount();
	str.Format(L"%d", index + 1);
	m_AlarmLogList.InsertItem(index, str);
	m_AlarmLogList.SetItemText(index, 1, std2CStr(warn.time));
	m_AlarmLogList.SetItemText(index, 2, std2CStr(warn.route));
	m_AlarmLogList.SetItemText(index, 3, std2CStr(warn.cameraName));
	m_AlarmLogList.SetItemText(index, 4, std2CStr(warn.context));
	m_AlarmLogList.SetItemText(index, 5, std2CStr(warn.producer));
}

void CAlarmLog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect (rcClient,RGB(240,248,251));
}


void CAlarmLog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	
	if(m_AlarmLogList.m_hWnd  != NULL)
	{
		CRect rcListWarining = rcClient;
		rcListWarining.left =rcClient.left +5;
		rcListWarining.bottom = rcClient.bottom -5;
		rcListWarining.right = rcClient.right -400;
		m_AlarmLogList.MoveWindow(&rcListWarining);  
	}
	
	if(m_alarmcount.m_hWnd != NULL)
	{
		m_alarmcount.MoveWindow(rcClient.right -220,rcClient.top +5,100,20); 
	}
	if(m_from.m_hWnd != NULL)
	{
		m_from.MoveWindow(rcClient.right -380,rcClient.top +45,20,20);
	}
	if(m_nowyear.m_hWnd != NULL)
	{
		m_nowyear.MoveWindow(rcClient.right -305,rcClient.top+45,20,20);
	}
	if(m_nowmouth.m_hWnd != NULL)
	{
		m_nowmouth.MoveWindow(rcClient.right -235,rcClient.top +45,20,20);
	}
	if(m_nowday.m_hWnd != NULL)
	{
		m_nowday.MoveWindow(rcClient.right -165,rcClient.top +45,20,20);
	}
	if(m_last.m_hWnd != NULL)
	{
		m_last.MoveWindow(rcClient.right -380,rcClient.top +70,20,20);
	}
	if(m_foryear.m_hWnd != NULL)
	{
		m_foryear.MoveWindow(rcClient.right -305,rcClient.top +70,20,20);
	}
	if(m_formouth.m_hWnd!= NULL)
	{
		m_formouth.MoveWindow(rcClient.right -235,rcClient.top +70,20,20);
	}
	if(m_forday.m_hWnd != NULL)
	{
		m_forday.MoveWindow(rcClient.right -165,rcClient.top +70,20,20);
	}
	if(m_camera.m_hWnd != NULL)
	{
		m_camera.MoveWindow(rcClient.right -380,rcClient.top +105,80,20);
	}
	if(m_people.m_hWnd != NULL)
	{
		m_people.MoveWindow(rcClient.right -180,rcClient.top +105,50,20);
	}
	if(m_alarm.m_hWnd != NULL)
	{
		m_alarm.MoveWindow(rcClient.right -380,rcClient.top +135,100,20); 
	}
	if(m_edit0.m_hWnd != NULL)
	{
		m_edit0.MoveWindow(rcClient.right -360,rcClient.top +45,50,20); 
	}
	if(m_edit1.m_hWnd != NULL)
	{
		m_edit1.MoveWindow(rcClient.right -280,rcClient.top +45,40,20); 
	}
	if(m_edit2.m_hWnd != NULL)
	{
		m_edit2.MoveWindow(rcClient.right -210,rcClient.top +45,40,20); 
	}
	if(m_edit3.m_hWnd != NULL)
	{
		m_edit3.MoveWindow(rcClient.right -140,rcClient.top +45,30,20); 
	}
	if(m_edit4.m_hWnd != NULL)
	{
		m_edit4.MoveWindow(rcClient.right -95,rcClient.top +45,30,20); 
	}
	if(m_edit5.m_hWnd != NULL)
	{
		m_edit5.MoveWindow(rcClient.right -50,rcClient.top +45,30,20); 
	}
	if(m_edit6.m_hWnd != NULL)
	{
		m_edit6.MoveWindow(rcClient.right -360,rcClient.top +70,50,20); 
	}
	if(m_edit7.m_hWnd != NULL)
	{
		m_edit7.MoveWindow(rcClient.right -280,rcClient.top +70,40,20); 
	}
	if(m_edit8.m_hWnd != NULL)
	{
		m_edit8.MoveWindow(rcClient.right -210,rcClient.top +70,40,20); 
	}
	if(m_edit9.m_hWnd != NULL)
	{
		m_edit9.MoveWindow(rcClient.right -140,rcClient.top +70,30,20); 
	}
	if(m_edit10.m_hWnd != NULL)
	{
		m_edit10.MoveWindow(rcClient.right -95,rcClient.top +70,30,20); 
	}
	if(m_edit11.m_hWnd != NULL)
	{
		m_edit11.MoveWindow(rcClient.right -50,rcClient.top +70,30,20); 
	}
	if(m_comboBox0.m_hWnd != NULL)
	{
		m_comboBox0.MoveWindow(rcClient.right -295,rcClient.top +105,100,20);
	}
	if(m_comboBox1.m_hWnd != NULL)
	{
		m_comboBox1.MoveWindow(rcClient.right -125,rcClient.top +105,80,20);
	}
}


int CAlarmLog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	EventBus::regist<Warn>(CREAT_HANDLER(
		[this](BaseEvent &e)->void{OnWarnMsg(static_cast<Warn&>(e)); }
	));
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(&rcClient);

	if(m_AlarmLogList.m_hWnd == NULL)
	{
		m_AlarmLogList.Create(WS_VISIBLE | WS_CHILD |LVS_REPORT,rcClient,this,IDC_ALARMLOGLIST);
	}
	m_AlarmLogList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_AlarmLogList.InsertColumn(0, _T("序号"), LVCFMT_CENTER,80, 0);   
	m_AlarmLogList.InsertColumn(1, _T("报警时间"), LVCFMT_CENTER, 120, 1);   
	m_AlarmLogList.InsertColumn(2, _T("路数"), LVCFMT_CENTER, 100, 2); 
	m_AlarmLogList.InsertColumn(3, _T("报警源名称"), LVCFMT_CENTER,210, 3);   
	m_AlarmLogList.InsertColumn(4, _T("报警内容"), LVCFMT_CENTER, 215, 4);   
	m_AlarmLogList.InsertColumn(5, _T("产生人"), LVCFMT_CENTER, 130, 5); 
	m_AlarmLogList.InsertColumn(6, _T("预览"), LVCFMT_CENTER, 100, 6);   
	m_AlarmLogList.InsertColumn(7, _T("备注"), LVCFMT_CENTER, 100, 7); 

	if(m_alarmcount.m_hWnd == NULL)
	{
		m_alarmcount.CreateEx(_T("报警统计"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_ALARMCOUNT);
		m_alarmcount.SetImageForButton(IDB_BACK);
	}

	if(m_from.m_hWnd == NULL)
	{
		m_from.CreateEx(_T("自"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_FOROM);
		m_from.SetImageForButton(IDB_BACK);
	}
	if(m_nowyear.m_hWnd == NULL)
	{
		m_nowyear.CreateEx(_T("年"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_NOWYEAR);
		m_nowyear.SetImageForButton(IDB_BACK);
	}
	if(m_nowmouth.m_hWnd == NULL)
	{
		m_nowmouth.CreateEx(_T("月"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_NOWMOUTH);
		m_nowmouth.SetImageForButton(IDB_BACK);
	}
	if(m_nowday.m_hWnd == NULL)
	{
		m_nowday.CreateEx(_T("日"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_NOWDAY);
		m_nowday.SetImageForButton(IDB_BACK);
	}
	if(m_last.m_hWnd == NULL)
	{
		m_last.CreateEx(_T("到"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_LAST);
		m_last.SetImageForButton(IDB_BACK);
	}
	if(m_foryear.m_hWnd == NULL)
	{
		m_foryear.CreateEx(_T("年"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_FORYEAR);
		m_foryear.SetImageForButton(IDB_BACK);
	}
	if(m_formouth.m_hWnd == NULL)
	{
		m_formouth.CreateEx(_T("月"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_FORMOUTH);
		m_formouth.SetImageForButton(IDB_BACK);
	}
	if(m_forday.m_hWnd == NULL)
	{
		m_forday.CreateEx(_T("日"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_FORDAY);
		m_forday.SetImageForButton(IDB_BACK);
	}
	if(m_camera.m_hWnd == NULL)
	{
		m_camera.CreateEx(_T("摄像头名称"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_CAMERA);
		m_camera.SetImageForButton(IDB_BACK);
	}
	if(m_people.m_hWnd == NULL)
	{
		m_people.CreateEx(_T("产生人"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_PEOPLE);
		m_people.SetImageForButton(IDB_BACK);
	}
	if(m_alarm.m_hWnd == NULL)
	{
		m_alarm.CreateEx(_T("共产生报警:"),WS_VISIBLE|WS_CHILD,NULL,NULL,NULL,NULL,m_hWnd,(HMENU)IDC_ALARM);
		m_alarm.SetImageForButton(IDB_BACK);
	}
	if(m_edit0.m_hWnd == NULL)
	{
		m_edit0.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE0);
	}
	if(m_edit1.m_hWnd == NULL)
	{
		m_edit1.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE1);
	}
	if(m_edit2.m_hWnd == NULL)
	{
		m_edit2.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE2);
	}
	if(m_edit3.m_hWnd == NULL)
	{
		m_edit3.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE3);
	}
	if(m_edit4.m_hWnd == NULL)
	{
		m_edit4.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE4);
	}
	if(m_edit5.m_hWnd == NULL)
	{
		m_edit5.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE5);
	}
	if(m_edit6.m_hWnd == NULL)
	{
		m_edit6.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE6);
	}
	if(m_edit7.m_hWnd == NULL)
	{
		m_edit7.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE7);
	}
	if(m_edit8.m_hWnd == NULL)
	{
		m_edit8.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE8);
	}
	if(m_edit9.m_hWnd == NULL)
	{
		m_edit9.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE9);
	}
	if(m_edit10.m_hWnd == NULL)
	{
		m_edit10.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE10);
	}
	if(m_edit11.m_hWnd == NULL)
	{
		m_edit11.Create(WS_VISIBLE | WS_CHILD |ES_AUTOHSCROLL,rcClient,this,IDC_EDITE11);
	}
	if(m_comboBox0.m_hWnd == NULL)
	{
		m_comboBox0.Create(WS_VISIBLE | WS_CHILD |CBS_DROPDOWN,rcClient,this,IDC_COMBOBOX0);
	}
	if(m_comboBox1.m_hWnd == NULL)
	{
		m_comboBox1.Create(WS_VISIBLE | WS_CHILD |CBS_DROPDOWN,rcClient,this,IDC_COMBOBOX1);
	}
	return 0;
}
CAlarmLog * CAlarmLog::GetInstance()
{
	return m_pThis;
}
CAlarmLog *CAlarmLog::m_pThis = NULL;