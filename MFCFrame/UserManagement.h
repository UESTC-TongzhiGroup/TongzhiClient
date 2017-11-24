#pragma once
#include"resource.h"
#include"AddUser.h"
#include"ModUser.h"

// CUserManagement �Ի���

class CUserManagement : public CDialogEx
{
	DECLARE_DYNAMIC(CUserManagement)

public:
	CUserManagement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserManagement();

// �Ի�������
	enum { IDD = IDD_USER_MANGEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
private:
	CListCtrl m_PedestrianList;
	CImageButton  m_add;
	CImageButton  m_alter;
	CImageButton  m_delete;

	CAddUser      userAddDlg;
	CModUser      userModDlg;
	static CUserManagement * m_pThis;
public:
	static CUserManagement * GetInstance();
	void InsertUser(int strvalue, CString strname, CString strtype);
};
