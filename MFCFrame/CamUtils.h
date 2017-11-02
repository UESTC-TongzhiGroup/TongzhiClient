#pragma once
#include <string>
#include <sstream>
#include <map>
#include <json/json.h>

using std::string;

typedef Json::Value value;

typedef string CamID;

typedef enum {
	Inactive = 0,
	OnFight,
	OnInvade,
	OnCross,
	OnWander,

	TOTAL_NUM
} CamMode;

struct CamInfo {
	CamID ID;
	string name;
	string url;
	string cam_user;
	string cam_password;
	string time_start, time_end;
	CamMode mode;
	string mode_name;
	bool active;
	CamInfo() = default;

	CamInfo(CamID _ID, string _name, string _url,
		string _user, string _password);

	inline string getFullURL() {
		std::stringstream ss;
		ss << "rtsp://" << cam_user
			<< ':'		<< cam_password
			<< '@'		<< url
			<< ":554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";
		return ss.str();
	}

	void toJsonObj(value&);
	void fromJsonObj(value&);

	inline void setFullDayTask() {
		time_start = "0";
		time_end = "0";
	}

	inline bool isTimed() const{
		return (time_start != "0" && time_start != "") ||
			(time_end != "0" && time_end != "");
	}

	inline bool operator == (const CamInfo &rhs) const
	{
		return rhs.ID == ID;
	}
	
	inline bool operator < (const CamInfo &rhs) const
	{
		return rhs.ID > ID;
	}
};

class CamList : public std::map<CamID, CamInfo> {
public:
	void addCam(const CamInfo& cam);
	void operator+= (const CamInfo& cam);
};

namespace Cams {

	const static string ModeName[CamMode::TOTAL_NUM] = {
		//连名字大小写都不一样
		//与服务端保持一致,不是我的锅
		"Inactive",
		"fight",
		"Invasion",
		//这个没用上
		"Cross(UB)",
		"Hovering"
	};

	const static string ModeName_ZH[CamMode::TOTAL_NUM] = {
		"未启动",
		"打斗",
		"入侵",
		"越界",
		"徘徊"
	};

	CamList& getCamInfo();

	inline size_t getCamInfoNum() {
		return getCamInfo().size();
	}

	void updateCamList();

#define RETURN_CASE(_case)	\
	case _case:				\
		return _case;		\

	inline CamMode getCamMode(int i) {
		switch (i) {
			RETURN_CASE(CamMode::OnFight)
			RETURN_CASE(CamMode::OnInvade)
			RETURN_CASE(CamMode::OnCross)
			RETURN_CASE(CamMode::OnWander)
		default:
			return CamMode::Inactive;
		}
	}

	inline CamMode getCamMode(string i){
		int type;
		std::stringstream(i) >> type;
		return getCamMode(type);
	}
}