#include "stdafx.h"
#include "StrUtil.h"

namespace StrUtil {
	vector<CString> splitCString(CString strSource, const char *ch)
	{
		char *buf;
		vector<CString> vecString;
		CString strTmp(strtok_s((LPSTR)(LPCTSTR)strSource, ch, &buf));//(LPSTR)(LPCTSTR)将CString转char*
		for(;;)
		{
			strTmp = strtok_s(NULL, ch, &buf);
			if (strTmp == _T(""))
				break;
			strTmp.TrimLeft();
			vecString.push_back(strTmp);
		}
		return vecString;
	}

	std::string format(const char *fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);
		int size = vsnprintf(nullptr, 0, fmt, ap) + 1;
		va_end(ap);
		char *buf = new char[size];
		va_start(ap, fmt);
		vsnprintf(buf, size, fmt, ap);
		va_end(ap);
		std::string fs(buf);
		delete[] buf;
		return fs;
	}

	string stdString_To_UTF8(const std::string & str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

		wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
		ZeroMemory(pwBuf, nwLen * 2 + 2);

		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

		char * pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

		std::string retStr(pBuf);

		delete[]pwBuf;
		delete[]pBuf;

		pwBuf = nullptr;
		pBuf = nullptr;

		return retStr;
	}
}