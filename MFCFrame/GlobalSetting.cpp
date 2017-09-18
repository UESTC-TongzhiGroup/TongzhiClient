#include "StdAfx.h"
#include "GlobalSetting.h"
#include "ULang.h"
CGlobalSetting::CGlobalSetting(void)
{
}

CGlobalSetting::~CGlobalSetting(void)
{

}

CFont* CGlobalSetting::m_pFont=NULL;

void CGlobalSetting::CreateMyToolTip(HWND hWnd,const char* csToolTip )
{
    static HWND s_hWndTT=0;
	static HWND s_hGlobalWnd=AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	_map_hwnd  s_tipwnd_map;
	                //只创建一个窗口
	if(s_hWndTT==0)
	{
		s_hWndTT = ::CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,0,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP /*| TTS_BALLOON*/,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hWnd,
			NULL,
			::AfxGetInstanceHandle(),
			NULL);
	} 
	_map_hwnd::iterator iter_find=s_tipwnd_map.find(hWnd);
	if(iter_find==s_tipwnd_map.end())
	{
        s_tipwnd_map[hWnd]=hWnd;
		TOOLINFO toolinfo={0};

		toolinfo.cbSize = sizeof(TOOLINFO);
		toolinfo.uFlags = TTF_IDISHWND|TTF_SUBCLASS;
		toolinfo.hwnd   = s_hGlobalWnd;
		toolinfo.hinst  = ::AfxGetInstanceHandle();
		toolinfo.uId    = (UINT)hWnd;
		toolinfo.lpszText = (LPTSTR)csToolTip;
		toolinfo.lParam = 0;
		SendMessage(s_hWndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &toolinfo);
		SendMessage(s_hWndTT, TTM_SETMAXTIPWIDTH, 0, 300);
     }
	else
	{
		TOOLINFO ti;
		memset(&ti, 0, sizeof(ti));
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		ti.hwnd = s_hGlobalWnd;
		ti.uId = (UINT)hWnd;
		ti.hinst = AfxGetInstanceHandle();
		ti.lpszText = (LPTSTR)csToolTip;
		SendMessage(s_hWndTT, TTM_UPDATETIPTEXT, 0, (LPARAM)(LPTOOLINFO) &ti);
	}
}

void CGlobalSetting::CreateMyToolTip(HWND hWnd,UINT uID )
{
   CString strInfo;
   strInfo=CGlobalSetting::LoadString(uID);
   CreateMyToolTip(hWnd,(const char*)LPCTSTR(strInfo));
}

CString CGlobalSetting::LoadString( UINT uIDResource )
{
	return ULang::LoadString(uIDResource);
}

CFont* CGlobalSetting::CreateFont( LOGFONT* lpLogFont, BOOL bUseYaHei/*=TRUE*/ )
{
  m_pFont=NULL;
  if(!lpLogFont)
  {
	  return m_pFont;
  }
  m_pFont=new CFont;
  VPCreateFont(lpLogFont,*m_pFont,bUseYaHei);
  return m_pFont;
}

BOOL CGlobalSetting::VPCreateFont( LOGFONT* lpLogFont, CFont& font, BOOL bUseYaHei/*=TRUE*/ )
{
	if (!lpLogFont)
	{
		return FALSE;
	}
    //lfHeight 为外界提供
	//lpLogFont->lfHeight;
	lpLogFont->lfHeight=15;
	lpLogFont->lfWidth = 0;
	lpLogFont->lfEscapement = 0;
	lpLogFont->lfOrientation = 0;
	lpLogFont->lfWeight = FW_NORMAL;
	lpLogFont->lfItalic = FALSE;
	lpLogFont->lfUnderline = FALSE;
	lpLogFont->lfStrikeOut = 0;
	lpLogFont->lfCharSet = ANSI_CHARSET;
	lpLogFont->lfOutPrecision = OUT_DEFAULT_PRECIS;
	lpLogFont->lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lpLogFont->lfQuality = DEFAULT_QUALITY;
	lpLogFont->lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	//lfFaceName外界不提供的默认成微软雅黑
	//lpLogFont->lfFaceName[LF_FACESIZE];

	if(lpLogFont->lfFaceName[0] == 0)
	{
		if(bUseYaHei)
		{
			StrCpy(lpLogFont->lfFaceName, _T("微软雅黑"));
		}
		else
		{
			StrCpy(lpLogFont->lfFaceName, _T("Arial"));
		}
	}
	BOOL bSucessed = font.CreateFontIndirect(lpLogFont);
	return TRUE;
}

CMainFrame* CGlobalSetting::GetMainFrame()
{
	if(m_pMainFrame==NULL)
	{
      m_pMainFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	}
    return m_pMainFrame;
}

CMainFrame* CGlobalSetting::m_pMainFrame=NULL;