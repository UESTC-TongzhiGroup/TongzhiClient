
#include "stdafx.h"
#include "ULang.h"

#define AUTO_CHINA	(0)

inline static void MY_MEMCPY(void *dest, const void *src, size_t count) 
{
#if (_MSC_VER > 1310) // VS2005
	memcpy_s(dest, count, src, count);
#else
	memcpy(dest, src, count);
#endif
}

HINSTANCE ULang::s_hInstance = NULL;
HINSTANCE ULang::s_hInstance2 = NULL;

void ULang::SwitchLanguage(BOOL bChina)
{
	OSVERSIONINFO ovs; 
	ovs.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
	if (!::GetVersionExW(&ovs)) 
	{
		return;
	}
	if (bChina)
	{
		if (ovs.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT)); 

			if (ovs.dwMajorVersion >= 6 && ovs.dwMinorVersion >= 0)
			{
				SetThreadUILanguage(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED));  
			}
		}
	}
	else
	{
		if (ovs.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US), SORT_DEFAULT)); 

			if (ovs.dwMajorVersion >= 6 && ovs.dwMinorVersion >= 0)
			{
				SetThreadUILanguage(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US));  
			}
		}
	}
}

BOOL ULang::IsVistaOrLater()
{
	BOOL bret = FALSE;
	OSVERSIONINFO osVerInfo={0};
	DWORD majorVer,minorVer;
	osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osVerInfo)) 
	{
		majorVer = osVerInfo.dwMajorVersion;
		minorVer = osVerInfo.dwMinorVersion;

		//ÅÐ¶ÏÊÇserver2008,vista,windows7
		if ( (osVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && 
			(majorVer >= 6) )
		{
			bret = TRUE;
		}
	}

	return bret;
}

LANGID ULang::GetUserUILanguage()
{
	if (!IsVistaOrLater())
	{
		//return GetUserDefaultUILanguage();
		LCID dwLCID = GetThreadLocale();
		return (LANGIDFROMLCID(dwLCID));
	}
	else
	{
		typedef LANGID  (WINAPI *pFunGetThreadUILanguage)(void);
		static pFunGetThreadUILanguage s_pFunc = NULL;
		if (!s_pFunc)
		{
			HINSTANCE hDLL = ::LoadLibrary(_T("Kernel32.dll"));
			if (hDLL == NULL)
			{
				return 0;
			}

			if (hDLL)
			{
				s_pFunc = (pFunGetThreadUILanguage)::GetProcAddress(hDLL,
					"GetThreadUILanguage");
				::FreeLibrary(hDLL);
			}
		}

		LANGID nLangID = 0;
		if (s_pFunc)
		{
			nLangID =  s_pFunc();
		}

		return nLangID;
	}
	
	return 0;
}


BOOL ULang::LangIsChina(BOOL bDefaultConfig)
{
	LANGID LangID = 0; 

	if (bDefaultConfig)
	{
		LangID = GetUserDefaultUILanguage();
	}
	else
	{
		LangID = ULang::GetUserUILanguage();
	}

	if (PRIMARYLANGID(LangID) == LANG_CHINESE && 
		SUBLANGID(LangID) == SUBLANG_CHINESE_SIMPLIFIED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

CString ULang::LoadString(UINT nID,
						  BOOL bDefaultConfig)
{
	CString csText;

	if (LangIsChina(bDefaultConfig))
	{
		BOOL bLoadOK = csText.LoadString(s_hInstance, nID);

		if (!bLoadOK && s_hInstance2)
		{
			bLoadOK = csText.LoadString(s_hInstance2, nID);
		}


		if (!bLoadOK)
		{
			bLoadOK = csText.LoadString(nID);
		}
	}
	else
	{
		BOOL bLoadOK = csText.LoadString(s_hInstance,  nID, 
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		if (!bLoadOK)
		{
#if AUTO_CHINA
			SwitchLanguage(TRUE);
#endif
			bLoadOK = csText.LoadString(s_hInstance, nID, 
				MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
#if AUTO_CHINA
			SwitchLanguage(FALSE);
#endif
		}

		if (!bLoadOK && s_hInstance2)
		{
			bLoadOK = csText.LoadString(s_hInstance2, nID, 
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		}

		if (!bLoadOK && s_hInstance2)
		{
#if AUTO_CHINA
			SwitchLanguage(TRUE);
#endif
			bLoadOK = csText.LoadString(s_hInstance2, nID, 
				MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
#if AUTO_CHINA
			SwitchLanguage(FALSE);
#endif
		}

		if (!bLoadOK)
		{
			bLoadOK = csText.LoadString(nID);
		}

	}
	return csText;
}

#ifdef __AFX_H__
BOOL ULang::LoadMenu(CMenu& menu, 
					 UINT uResourceID)
{
	BOOL bResult = FALSE;
	if (LangIsChina())
	{
 		HMENU hMenu = ::LoadMenu(s_hInstance, MAKEINTRESOURCE(uResourceID));

		if (!hMenu && s_hInstance2)
		{
 			hMenu = ::LoadMenu(s_hInstance2, MAKEINTRESOURCE(uResourceID));
		}

		if (!menu.Attach(hMenu))
		{
			bResult = menu.LoadMenu(uResourceID);
		}
		else
		{
			bResult = TRUE;
		}
	}
	else 
	{
		HRSRC hRes = NULL;
		HMODULE hResModule = NULL;
		{
			hResModule = s_hInstance;
			hRes = ::FindResourceEx(hResModule, 
				RT_MENU, MAKEINTRESOURCE(uResourceID), 
				MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US));
			if (hRes == NULL)
			{
				hRes = ::FindResourceEx(hResModule, 
					RT_MENU, MAKEINTRESOURCE(uResourceID), 
					MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
			}
		}

		if (hRes == NULL && s_hInstance2)
		{
			hResModule = s_hInstance2;
			hRes = ::FindResourceEx(hResModule, 
				RT_MENU, MAKEINTRESOURCE(uResourceID), 
				MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US));
			if (hRes == NULL)
			{
				hRes = ::FindResourceEx(hResModule, 
					RT_MENU, MAKEINTRESOURCE(uResourceID), 
					MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
			}
		}

		if (hRes)
		{
			HGLOBAL hTemplate = ::LoadResource(hResModule, hRes);
			if (hTemplate)
			{
				bResult = menu.LoadMenuIndirect(hTemplate);
				::FreeResource(hTemplate);
			}
		}
		else
		{
			bResult = menu.LoadMenu(uResourceID);
		}
	}

	return bResult;
}




HBITMAP ULang::LoadBitmap(HINSTANCE hInstance,
						  LPCWSTR lpBitmapName,
						  WORD wLanguageID)
{
	if (wLanguageID == LANG_CHINESE)
	{
		return ::LoadBitmap(hInstance,lpBitmapName);
	}
	else 
	{
		WORD w = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US); 
		HRSRC hRes;         // handle/ptr. to res. info. in hExe
		HGLOBAL hResLoad;   // handle to loaded resource

		hRes = FindResourceEx(hInstance, RT_BITMAP, lpBitmapName, w);
		if (hRes == NULL)
		{
			return ::LoadBitmap(hInstance,lpBitmapName);
		}

		hResLoad = LoadResource(hInstance, hRes);
		if (hResLoad == NULL)
		{
			return ::LoadBitmap(hInstance,lpBitmapName);
		}

		LPBITMAPINFO pResourceInfo = (LPBITMAPINFO)::LockResource(hResLoad);

		if (pResourceInfo == NULL)
		{
			::GlobalFree(hResLoad);
			return ::LoadBitmap(hInstance,lpBitmapName);
		}

		int biSizeImage = pResourceInfo->bmiHeader.biHeight * ((pResourceInfo->bmiHeader.biWidth * 3 + 3) & ~3);

		HBITMAP hbmResult = FALSE;

		if (SizeofResource(hInstance, hRes) >= int(biSizeImage + sizeof(BITMAPINFOHEADER)))
		{
			CDC dcSrc;
			dcSrc.CreateCompatibleDC(NULL);

			PBITMAPINFO pBitmapInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * 3);
			//ASSERT(pBitmapInfo != NULL);

			MY_MEMCPY(pBitmapInfo, &pResourceInfo->bmiHeader, sizeof(BITMAPINFOHEADER));
			pBitmapInfo->bmiHeader.biSizeImage = biSizeImage;

			BYTE* pDestBits = NULL;
			HBITMAP hBmp = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDestBits, NULL, 0);

			if (hBmp && pDestBits)
			{
				MY_MEMCPY(pDestBits, &pResourceInfo->bmiColors, biSizeImage);
				hbmResult = hBmp;
			}
			free(pBitmapInfo);
		}

		UnlockResource(hResLoad);
		FreeResource(hResLoad);

		return hbmResult;
	}
}


#endif

void ULang::SetModule(HINSTANCE hInstance)
{
	s_hInstance = hInstance;
}

void ULang::SetModule2(HINSTANCE hInstance)
{
	s_hInstance2 = hInstance;

}




UAutoLang::UAutoLang()
	: m_bIsChina(TRUE)
{
	m_bIsChina = ULang::LangIsChina();
	if (!m_bIsChina)
	{
		ULang::SwitchLanguage(TRUE);
	}
}

UAutoLang::~UAutoLang()
{
	if (!m_bIsChina)
	{
		ULang::SwitchLanguage(FALSE);
	}
}

