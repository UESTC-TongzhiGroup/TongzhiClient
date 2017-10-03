#pragma once
#include "resource.h"
#include "ImageButton.h"
#include "CptionBar.h"

// CAboutdlg 对话框

class CAboutdlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutdlg)

public:
	CAboutdlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutdlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	CCptionBar  m_caption;
	CImageButton  m_CloseDlg;
	CImageButton  m_Abouttext;
	//CImageButton  m_login;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
