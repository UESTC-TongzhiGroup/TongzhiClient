#pragma once

#include <atlimage.h>
#include <vector>
#include "resource.h"
#include "TabButton.h"
#include "ImageButton.h"
#include "AlarmMonitor.h"
#include "AlarmLog.h"
#include "AlarmSet.h"
#include "MenuBarPanel.h"
#include "UserManagement.h"
#include "PeopleManagement.h"

// CMainFrame
using std::vector;

class CMainFrame : public CWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
public:
	int AddItem(CString strText);
	static CMainFrame * GetInstance();
private:
	vector<CTabButton*> m_vecTabButton;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void OnTabPanel(int M_TabLabels);

	CImage				m_imgTabPanel;
	CImage              m_imgCaption;
	CImage				m_imgBorder;
	CImageButton        m_btnClose;
	CImageButton        m_btnMax;
	CImageButton        m_btnMin;

	CAlarmSet			m_AlarmSet;
	CAlarmMonitor		m_AlarmMonitor;
	CAlarmLog			m_AlarmLog;
	CMenuBarPanel       m_MenuBarPanel;
	CUserManagement		m_UserManagement;
	CPeopleManagement	m_PeopleManagement;
	static CMainFrame * m_pThis;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};



