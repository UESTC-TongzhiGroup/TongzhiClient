#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "Config.h"

// CCameraSet �Ի���

class CCameraSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraSet)

public:
	CCameraSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraSet();

// �Ի�������
	enum { IDD = IDD_CAMERA_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	void updateSel();
	//void updateEventListItem(int, const CameraConfig&);
	virtual BOOL OnInitDialog();
	CamMode getMode(int sel);
	CamMode getMode();
public:
	static CCameraSet * GetInstance();
private:
	CComboBox m_CameraList;
	CComboBox m_CameraModeList;
	CListCtrl m_EventList;
	CButton timeCheck;
	CComboBox timeStart;
	CComboBox timeEnd;

	//DBHelper helper;

	static CCameraSet * m_pThis;
	CamList tmpInfo;
	int m_CurrentCamIndex;
	DWORD ip_addr;
	CString userName;
	CString passwd;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCameralList();
	afx_msg void OnCbnSelchangeCameraMode();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnCbnChangeTimeStart();
	afx_msg void OnCbnSelchangeTimeEnd();
	afx_msg void OnLvnItemchangedEventList(NMHDR *pNMHDR, LRESULT *pResult);
};
