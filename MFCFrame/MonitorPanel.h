#pragma once
#include "resource.h"
#include "SideBar.h"
#include "VideoPanel.h"
#include "ImageButton.h"

// CMonitorPanel 对话框

class CMonitorPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorPanel)

public:
	CMonitorPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMonitorPanel();

// 对话框数据
	enum { IDD = IDD_MONITORPANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
private:
	CSideBar m_sidebar;
	CVideoPanel m_videoPanel;
	CImageButton m_expand;
	BOOL m_expandSidebar;
	CImage m_imgRight;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
