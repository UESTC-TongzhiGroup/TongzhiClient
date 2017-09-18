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
	inline string CString2stdString(CString cs) {
		return (CStringA)cs;
	}
	
	inline CString stdString2CString(string s) {
		return CString(s.c_str());
	}
	
	vector<CString> splitCString(CString strSource, const char *ch);

	std::string format(const char *fmt, ...);
}