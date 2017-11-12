#pragma once
#include "Resource.h"
#include "CamUtils.h"
#include <set>
#include "Events.h"

using Events::CamListUpdate;

class CCameraSet : public CDialogEx {
	DECLARE_DYNAMIC(CCameraSet)
public:
	CCameraSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraSet();
	// �Ի�������
	enum { IDD = IDD_CAMERA_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	int OnCreate(LPCREATESTRUCT);
	DECLARE_MESSAGE_MAP()
private:
	static CCameraSet * m_pThis;
	CamList tmpInfo;	//����ȡ��������ָ���ǰ������

	//�ؼ���Ա
	CComboBox camComboList;
	CListCtrl camListCtrl;
public:
	static CCameraSet * GetInstance();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnUpdateAllCam(CamListUpdate&);
private:
	void pullCamInfo();
};