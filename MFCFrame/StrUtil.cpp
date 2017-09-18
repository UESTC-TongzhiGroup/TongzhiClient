#include "stdafx.h"
#include "StrUtil.h"

namespace StrUtil {
	vector<CString> splitCString(CString strSource, const char *ch)
	{
		char *buf;
		vector<CString> vecString;
		CString strTmp(strtok_s((LPSTR)(LPCTSTR)strSource, ch, &buf));//(LPSTR)(LPCTSTR)½«CString×ªchar*
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
}