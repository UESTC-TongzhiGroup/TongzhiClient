#pragma once
#include"resource.h"
#include "afxcmn.h"

// CWarningPanel �Ի���

class CWarningPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CWarningPanel)

public:
	CWarningPanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWarningPanel();

// �Ի�������
	enum { IDD = IDD_WARNINGPANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnWarnMsg(WPARAM EID, LPARAM _event);
	void OnSize(UINT nType, int cx, int cy);
private:
	
	BOOL m_new;
	static CWarningPanel * m_pThis;
public:
	CListCtrl m_WarningList;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	static CWarningPanel * GetInstance();
	afx_msg void OnPaint();
};
