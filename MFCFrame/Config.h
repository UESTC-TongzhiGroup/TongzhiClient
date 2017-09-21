#pragma once
#include <stdexcept>
#include <map>
#include <vector>
#include <string>
#include <sstream>

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
	UINT16 textport;
	string loginUser = "", loginPass = "";
};

typedef enum {
	Inactive = 0,
		OnFight,
		OnInvade,
		OnCross,
		OnWander,

		TOTAL_NUM
} CamMode;

typedef std::map<string, string> StrDirc;

const static CString ModeName[CamMode::TOTAL_NUM] = {
	_T("未激活"),
	_T("打斗"),
	_T("入侵"),
	_T("越界"),
	_T("徘徊")
};

inline CString getCamModeName(CamMode mode) {
	return ModeName[mode];
}

class CameraConfig
{
public:
	bool active;
	short mode = Inactive;
	string url;
	string name;
	string cam_user;
	string cam_passwd;
	struct Duration {
		bool timeing;
		int start, end;
	} timeSel;

	inline string getFullURL() {
		std::stringstream ss;
		ss << "rtsp://" << cam_user
			<< ':' << cam_passwd
			<< '@' << url
			<< ":554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";
		return ss.str();
	}
};

typedef std::vector<CameraConfig> CamList;

namespace Config {
	void loadLicense();
	void loadServerCfg();
	void saveServerCfg();
	void clearServerCfg();

	CameraConfig& getCamByIndex(int);

	CamList& getCamInfo();

	size_t getCamInfoNum();

	ServerConfig& getServerInfo();

	StrDirc& getLicensesMap();

	inline size_t getCamInfoNum() {
		return getCamInfo().size();
	}

	inline CameraConfig& getCamByIndex(int index) {
		if (index > getCamInfoNum() || index < 0) {
			throw std::out_of_range("camIndex out of range");
		}
		return getCamInfo()[index];
	}

}
