#pragma once
#include"resource.h"
#include "ImageButton.h"
#include "StrUtil.h"

// CUserLogin 对话框

class CUserLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CUserLogin)

public:
	CUserLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserLogin();

// 对话框数据
	enum { IDD = IDD_USERLOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	bool checkLicense(CString, CString);
private:
	CImageButton  m_SUserName; 
	CImageButton  m_Spassword;
	CImageButton  m_login;
public:
	CEdit m_Name;
	CEdit m_Password;
};
