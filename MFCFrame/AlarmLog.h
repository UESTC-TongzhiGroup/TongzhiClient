#pragma once
#include"resource.h"
#include "afxcmn.h"
#include "ImageButton.h"
#include "Events.h"

// CAlarmLog 对话框

using namespace Events;

class CAlarmLog : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmLog)

public:
	CAlarmLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlarmLog();

// 对话框数据
	enum { IDD = IDD_ALARMLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnWarnMsg(Warn&);
	void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CImageButton  m_alarmcount;CImageButton  m_nowyear;CImageButton  m_nowmouth;
	CImageButton  m_nowday;CImageButton  m_foryear;CImageButton  m_formouth;
	CImageButton  m_forday;CImageButton  m_from;CImageButton  m_last;
	CImageButton  m_camera;CImageButton  m_people;CImageButton  m_alarm;
	CEdit m_edit0;CEdit m_edit1;CEdit m_edit2;CEdit m_edit3;CEdit m_edit4;
	CEdit m_edit5;CEdit m_edit6;CEdit m_edit7;CEdit m_edit8;CEdit m_edit9;
	CEdit m_edit10;CEdit m_edit11;
	CComboBox m_comboBox0;
	CComboBox m_comboBox1;
	static CAlarmLog * m_pThis;
public:
	CListCtrl m_AlarmLogList;
public:
	static CAlarmLog * GetInstance();
};
