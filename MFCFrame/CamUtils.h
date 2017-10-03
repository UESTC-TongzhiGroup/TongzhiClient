#pragma once
#include <string>
#include <sstream>
#include <vector>

using std::string;

typedef enum {
	Inactive = 0,
	OnFight,
	OnInvade,
	OnCross,
	OnWander,

	TOTAL_NUM
} CamMode;

struct CamInfo {
	string ID;
	string name;
	string url;
	string cam_user;
	string cam_passwd;
	int time_start, time_end;
	string mode_name;
	CamMode mode;
	bool active;

	inline string getFullURL() {
		std::stringstream ss;
		ss << "rtsp://" << cam_user
			<< ':' << cam_passwd
			<< '@' << url
			<< ":554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";
		return ss.str();
	}
};

typedef std::vector<CamInfo> CamList;

namespace Cams {

	const static string ModeName[CamMode::TOTAL_NUM] = {
		"Inactive",
		"Fight",
		"Invade",
		"Cross",
		"Wander"
	};

	CamList& getCamInfo();

	inline size_t getCamInfoNum() {
		return getCamInfo().size();
	}
}