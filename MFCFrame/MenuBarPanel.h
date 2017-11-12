#pragma once
#include <afxinet.h>
#include <atlimage.h>
#include <vector>
#include "Panel.h"
#include "MenuEx.h"
#include "ServerSet.h"
#include "CameraSet.h"
#include "Aboutdlg.h"
#include "Events.h"

using std::vector;
using namespace Events;
class CMenuBarPanel;
class CMenuItem
{
public:
	CMenuItem();
	~CMenuItem();
	friend CMenuBarPanel;
	enum MenuItemState
	{
		Normal = 0,
		Hover,
		Pressed
	};
protected:
	void Draw(CDC* pDC);
	void UpDate(CRgn RefrshRgn);
private:
	CRect   m_rcMenu;
	CString m_csMenuText;
	HMENU   m_hPopMenu;
	MenuItemState   m_dwState;
};

class CMenuBarPanel :public CPanel
{
public:
	CMenuBarPanel(void);
	~CMenuBarPanel(void);
	DECLARE_MESSAGE_MAP()
protected:
	void OnPaint();
	afx_msg void OnUserLogin(UserLogin&);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	static LRESULT CALLBACK MessageProc(int code, WPARAM wParam, LPARAM lParam);
protected:
	void ClearMenu();
	void SwitchMenu();
public:
	void LoadMenuFromResource(UINT uIDResource);
	void HitTest(CPoint pt);
	int  GetMenuIndex(CPoint pt);
	void ShowPopupMenu(int nIndex, CPoint pt);
	CMenuItem* GetMenuItem(int nIndex);
	afx_msg void OnVideoFile();
	void InsertAlarm(CString strtime, int stroute, CString strname, CString strcontent);
	void InsertAlarmLog(CString strtime, int stroute, CString strname, CString strcontent);
private:
	CImage m_imgMenuBar;
	vector<CMenuItem*> m_vecPMenuItem;
	CMenuEx m_menu;
	BOOL m_bTracking;
	CServerSet  m_ServerSet;
	CCameraSet  m_CameraSet;
	CAboutdlg   m_Aboutdlg;
	static CMenuBarPanel * m_pThis;
public:
	afx_msg void OnQuit();
	afx_msg void OnLock();
	afx_msg void OnSwitchUser();
	afx_msg void OnBackupData();
	afx_msg void OnRestoreData();
	afx_msg void OnUserManagement();
	afx_msg void OnSet();
	afx_msg void OnPeopleCount();
	afx_msg void OnCalendar();
	afx_msg void OnServerSet();
	afx_msg void OnCamareSet();
	afx_msg void OnUserManual();
	afx_msg void OnAbout();
	static CMenuBarPanel * GetInstance();
	friend UINT taskRecieve0(LPVOID param);
	void sendData(CString SendData);
	void sendSetList(CString SendData);
	void connectServer();
public:
	int m_AlarmValue;
	//	CString strtime;
	//	int stroute;
	//	CString strname;
	//	CString strcontent;
	//	CString strproduct;
	BOOL m_ConnectState;
	CString strTextIp;
	CString strTexPort;
	CString strWebSocketPost;
	CInternetSession session;
	CHttpConnection * pConnection;
};
