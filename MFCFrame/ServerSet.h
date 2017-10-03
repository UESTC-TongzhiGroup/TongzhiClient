#pragma once
#include "resource.h"
#include "afxwin.h"
#include "StrUtil.h"

//TODO: #include "DBUtil.h"

// CServerSet 对话框

using namespace StrUtil;

class CServerSet : public CDialogEx
{
	DECLARE_DYNAMIC(CServerSet)

public:
	CServerSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServerSet();

	// 对话框数据
	enum { IDD = IDD_SERVER_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	static CServerSet * GetInstance();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
private:
	//界面指针
	static CServerSet * m_pThis;
private:
	//数据成员
	CString m_url;
	CString m_user;
	CString m_pass;
	UINT m_sendport;
public:
	BOOL notAdmin;
};
