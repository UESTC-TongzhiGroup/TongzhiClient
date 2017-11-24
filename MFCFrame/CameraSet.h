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
	int selIndex = 0;		//��ǰѡ�������ͷ������

	std::set<CamID> add, mod, del, focus;

	//�ؼ���Ա
	CComboBox modeComboList;
	CComboBox camComboList;
	CListCtrl camListCtrl;
	CButton timeCheck;
	CComboBox timeStart;
	CComboBox timeEnd;
public:
	static CCameraSet * GetInstance();

private:
	void pullCamInfo();
	void updateSel();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnUpdateAllCam();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCamComboList();
	afx_msg void OnSelchangeCamList(NMHDR*, LRESULT*);
	afx_msg void OnSelchangeMode();
	afx_msg void OnCancel();
};