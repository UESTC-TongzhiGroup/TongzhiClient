#pragma once
#include <string>
#include <vector>
#ifndef _AFX
	#include "atlstr.h"
#else
	#include "afxstr.h"
#endif
using std::string;
using std::vector;

namespace StrUtil {
	inline string CStr2std(CString cs) {
		return (CStringA)cs;
	}
	
	inline CString std2CStr(string s) {
		return CString(s.c_str());
	}
	
	vector<CString> splitCString(CString strSource, const char *ch);

	string format(const char *fmt, ...);

	string stdString_To_UTF8(const std::string & str);
}