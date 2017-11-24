#pragma once
#include <stdexcept>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "CamUtils.h"

#include "StrUtil.h"

#define SERVER_CONFIG_PATH "./server_config"
#define LICENSE_PATH "./license"


using std::string;
using std::map;
using std::vector;

typedef unsigned short UINT16;

struct ServerConfig
{
	bool load = false;
	string url;
	UINT16 reqport;
	string loginUser = "", loginPass = "";
};

typedef std::map<string, string> StrDirc;

const static CString ModeName[CamMode::TOTAL_NUM] = {
	_T("δ����"),
	_T("��"),
	_T("����"),
	_T("Խ��"),
	_T("�ǻ�")
};

inline CString getCamModeName(CamMode mode) {
	return ModeName[mode];
}

namespace Config {
	bool isServerCfgLoaded();
	void loadLicense();
	void saveLicense();
	void loadServerCfg();
	void saveServerCfg();
	void clearServerCfg();

	ServerConfig& getServerInfo();

	StrDirc& getLicensesMap();
}
