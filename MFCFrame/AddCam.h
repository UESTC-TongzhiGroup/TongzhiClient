#pragma once
#include "resource.h"
#include "ImageButton.h"
#include "afxwin.h"
#include "afxcmn.h"

class CAddCam : public CDialogEx
{
	DECLARE_DYNAMIC(CAddCam)
public:
	CAddCam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddCam();

	// 对话框数据
	enum { IDD = IDD_ADD_CAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CString ID, name, url;
	CString cam_user, cam_password;
	afx_msg void OnBnClickedCamAdd();
	CListCtrl add_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelSelectedCam();
};