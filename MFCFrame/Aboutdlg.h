#pragma once
#include "resource.h"
#include "ImageButton.h"
#include "CptionBar.h"

// CAboutdlg �Ի���

class CAboutdlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutdlg)

public:
	CAboutdlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAboutdlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	CCptionBar  m_caption;
	CImageButton  m_CloseDlg;
	CImageButton  m_Abouttext;
	//CImageButton  m_login;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
