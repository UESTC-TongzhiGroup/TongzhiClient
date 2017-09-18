#pragma once
#include <map>
#include "MainFrame.h"
using namespace std;
typedef map<HWND,HWND> _map_hwnd;
class CGlobalSetting
{
public:
	CGlobalSetting(void);
	~CGlobalSetting(void);
public:
	 static CGlobalSetting* GetInstance()
	{
		static CGlobalSetting s_hInstance;
		return &s_hInstance;
	}
public:
	static void CreateMyToolTip(HWND hWnd,const char* csToolTip);
	static void CreateMyToolTip(HWND hWnd,UINT uID);
	static CString LoadString(UINT uIDResource);
	static CFont* CreateFont(LOGFONT* lpLogFont, BOOL bUseYaHei=TRUE);
	static BOOL VPCreateFont(LOGFONT* lpLogFont, CFont& font, BOOL bUseYaHei=TRUE);
	static CMainFrame* GetMainFrame();
private:
	static CFont* m_pFont;
	static CMainFrame* m_pMainFrame;
};
