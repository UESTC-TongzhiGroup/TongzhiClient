#pragma once
#include"resource.h"
#include "ImageButton.h"
#include "StrUtil.h"
#include "afxwin.h"

// CUserLogin �Ի���

class CUserLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CUserLogin)

public:
	CUserLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserLogin();

// �Ի�������
	enum { IDD = IDD_USERLOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	bool checkLicense(CString, CString);
private:
	bool registerMode = false;
	CImageButton  m_SUserName; 
	CImageButton  m_Spassword;
	CImageButton  m_login;
public:
	CEdit m_Name;
	CEdit m_Password;
	virtual BOOL OnInitDialog();
};
