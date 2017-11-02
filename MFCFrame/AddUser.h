#pragma once
#include "resource.h"
#include "ImageButton.h"
#include "CptionBar.h"
#include "afxwin.h"
// CAddUser 对话框

class CAddUser : public CDialogEx
{
	DECLARE_DYNAMIC(CAddUser)

public:
	CAddUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddUser();

// 对话框数据
	enum { IDD = IDD_ADD_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	CCptionBar  m_caption;
	CImageButton  m_CloseDlg;
	CImageButton  m_AddUserText;
	CImageButton  m_SUserName;
	CImageButton  m_Spassword;
	CImageButton  m_type;
	int m_typedata;
	int strvalue;
	CEdit m_username;
	CEdit m_password;
	CComboBox m_comboxtype;
	CString m_UsernameText;
	CString m_passwordText;
	CString m_typeText;
private:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
