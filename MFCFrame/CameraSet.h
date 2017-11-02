#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "CamUtils.h"
#include <set>

// CCameraSet 对话框

class CCameraSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraSet)

public:
	CCameraSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraSet();

// 对话框数据
	enum { IDD = IDD_CAMERA_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	void updateSel();
	void updateCamItem(int, const CamInfo&);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
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
	std::set<CamInfo> cam_change;
	std::set<string> cam_delete;
	int m_CurrentCamIndex;
	CString user_name;
	CString passwd;
	CString cam_address;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCameralList();
	afx_msg void OnCbnSelchangeCameraMode();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnCbnChangeTimeStart();
	afx_msg void OnCbnSelchangeTimeEnd();
	afx_msg void OnLvnItemchangedEventList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChange();
	afx_msg void OnStnClickedAddCam();
	afx_msg LRESULT updateAllCam(WPARAM, LPARAM);
	afx_msg void OnBnClickedDelSelectedCam();
};
