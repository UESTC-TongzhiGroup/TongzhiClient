#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <json/json.h>

using std::string;

typedef Json::Value value;

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
	string cam_password;
	string time_start, time_end;
	string mode_name;
	CamMode mode;
	bool active;

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

	inline bool isTimed() const{
		return time_start != "0" || time_end != "0";
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

typedef std::vector<CamInfo> CamList;

namespace Cams {

	const static string ModeName[CamMode::TOTAL_NUM] = {
		//�����ִ�Сд����һ��
		//�����˱���һ��,�����ҵĹ�
		"Inactive",
		"fight",
		"Invasion",
		//���û����
		"Cross(UB)",
		"Hovering"
	};

	const static string ModeName_ZH[CamMode::TOTAL_NUM] = {
		"δ����",
		"��",
		"����",
		"Խ��",
		"�ǻ�"
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
			RETURN_CASE(CamMode::Inactive)
			RETURN_CASE(CamMode::OnCross)
			RETURN_CASE(CamMode::OnFight)
			RETURN_CASE(CamMode::OnInvade)
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