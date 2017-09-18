#pragma once

#include "atlstr.h"

class ULang
{
public:
	static void SetModule(HINSTANCE hInstance);
	static void SetModule2(HINSTANCE hInstance);

	static void SwitchLanguage(BOOL bChina);
	static BOOL LangIsChina(BOOL bDefaultConfig=FALSE);
	static CString LoadString(UINT nID, BOOL bDefaultConfig=FALSE);

#ifdef __AFX_H__
	static BOOL LoadMenu(CMenu& menu, UINT uResourceID);
	static HBITMAP LoadBitmap(HINSTANCE hInstance, LPCWSTR lpBitmapName, WORD wLanguageID);
#endif

	static BOOL IsVistaOrLater();
	static LANGID GetUserUILanguage();

protected:
	static HINSTANCE s_hInstance;
	static HINSTANCE s_hInstance2;
};


class UAutoLang
{
public:
	UAutoLang();
	~UAutoLang();

protected:
	BOOL m_bIsChina;
};