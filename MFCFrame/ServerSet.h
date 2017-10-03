#pragma once
#include "resource.h"
#include "afxwin.h"
#include "StrUtil.h"

//TODO: #include "DBUtil.h"

// CServerSet �Ի���

using namespace StrUtil;

class CServerSet : public CDialogEx
{
	DECLARE_DYNAMIC(CServerSet)

public:
	CServerSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServerSet();

	// �Ի�������
	enum { IDD = IDD_SERVER_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	static CServerSet * GetInstance();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
private:
	//����ָ��
	static CServerSet * m_pThis;
private:
	//���ݳ�Ա
	CString m_url;
	CString m_user;
	CString m_pass;
	UINT m_sendport;
public:
	BOOL notAdmin;
};
