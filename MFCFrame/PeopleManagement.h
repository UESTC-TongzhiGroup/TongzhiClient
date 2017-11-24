#pragma once
#include"resource.h"
#include "ImageButton.h"

// CPeopleManagement �Ի���

class CPeopleManagement : public CDialogEx
{
	DECLARE_DYNAMIC(CPeopleManagement)

public:
	CPeopleManagement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPeopleManagement();

// �Ի�������
	enum { IDD = IDD_PEOPLE_MANAGEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	int rowSel, colSel;

	CListCtrl licenseList;
	CEdit edit;
	CImageButton save, refresh;

	afx_msg void OnEditList(NMHDR *, LRESULT *);
	afx_msg void OnEditLostFocus();
	bool check();
	afx_msg void OnSave();
	afx_msg void OnRefresh();
	inline afx_msg void CPeopleManagement::OnOK() {}
	inline afx_msg void CPeopleManagement::OnCancel() {}
public:
	virtual BOOL OnInitDialog();
};
