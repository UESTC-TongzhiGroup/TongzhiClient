#pragma once
#include <atlimage.h>
#include "resource.h"
#include"ImageButton.h"
#include"WarningPanel.h"
#include"WarningBtn.h"
#include"MonitorPanel.h"
// CAlarmMonitor 对话框

class CAlarmMonitor : public CDialogEx
{
	DECLARE_DYNAMIC(CAlarmMonitor)

public:
	CAlarmMonitor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlarmMonitor();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALARMMONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
private:
	CImageButton m_expandState;
	CWarningBtn m_waringBtn;
	CWarningPanel m_warningPanel;
	CMonitorPanel m_monitorPanel;
	BOOL m_expandSidebar;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
