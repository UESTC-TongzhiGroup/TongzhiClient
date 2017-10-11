#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <set>
#include <json\json.h>
#include "CamUtils.h"


using std::string;

typedef Json::Value value;
typedef unsigned int UInt;

#define JSON_INTERFACE_DECL										\
void toJsonObj(value&);											\
void fromJsonObj(value&);

#define NO_JSON_NEED											\
inline void toJsonObj(value &out)	{}							\
inline void fromJsonObj(value &in)	{}	

template<UInt TYPE>
struct _CamMsg {
	std::set<CamInfo> list;
	JSON_INTERFACE_DECL
};

template<UInt TYPE>
struct _ConnMsg {
	string user;
	string password;
	string type;
	JSON_INTERFACE_DECL
};

namespace Message {

	const static string SPLIT_SIGN = "\r\n";

	//=====================================================================================================

	/**
	* MsgType Defination
	* Block start
	*/

	typedef _CamMsg<1> AddCamMsg;
	typedef _CamMsg<2> ModifyCamMsg;

	typedef struct {
		ModifyCamMsg msg;
		string meta;
	}GetCamListReply;

	typedef struct {
		std::set<string> list;
		JSON_INTERFACE_DECL
	} DelCamMsg;

	struct GetCamListMsg{
		NO_JSON_NEED
	};

	typedef struct {
		std::set<string> list;
		JSON_INTERFACE_DECL
	} FocusCamMsg;

	
	typedef _ConnMsg<1> LoginMsg;
	typedef _ConnMsg<2> RegisterMsg;


	struct WarnMsg{
		string time;
		string route;
		string cameraName;
		string context;
		string producer;
		JSON_INTERFACE_DECL
	};

	typedef struct {
		string time;
		string meta;
		JSON_INTERFACE_DECL
	}GetWarnLogMsg;

	typedef struct {
		std::set<string> list;
		string meta;
		JSON_INTERFACE_DECL
	}DelWarnLogMsg;

	struct WarnLogReply{
		std::vector<WarnMsg> list;
		string meta;
		JSON_INTERFACE_DECL
	};


	struct AreaMsg{
		UINT ID;
		int left_up;
		int right_down;
		JSON_INTERFACE_DECL
	};


	struct MsgReply{
		string status;
		string meta;
		JSON_INTERFACE_DECL

		inline bool isSuccess() {
			return status == "success";
		}
	};

	struct HeartBeat {
		string status = "alive";
		void toJsonObj(value&);
	};

	/**
	* MsgType Defination
	* Block end
	*/
}
#include "JsonInterface.h"

/**
* MsgType Registration
* Block start
*/

template <typename T>
struct Type2ID;
template <typename T>
struct Reply;

/*
对MSG_TYPE_ID进行判断
将判断条件的确认固定在编译期的包装
*/
#define ID_OF_MSG_TYPE(MSG_TYPE) (Type2ID<MSG_TYPE>::ID)

#define REPLY_OF_MSG(MSG_TYPE) Reply<MSG_TYPE>::type_t

#define REG_MSG_TYPE(type, n) \
	template<>\
	struct Type2ID<type> { const static Int ID=n; }; \
//	template<>\
//	struct ID2Type<n> { typedef type type_t; };

#define REG_MSG_WITH_REPLY(type, n, reply)\
	REG_MSG_TYPE(type, n);\
	template<>\
	struct Reply<type> { typedef reply type_t; };

#define REG_MSG_REPLY_DEFAULT(type, n) REG_MSG_WITH_REPLY(type, n, Message::MsgReply)

REG_MSG_TYPE(Message::HeartBeat, -1);

REG_MSG_REPLY_DEFAULT(Message::LoginMsg, 0);
REG_MSG_REPLY_DEFAULT(Message::RegisterMsg, 1);

REG_MSG_REPLY_DEFAULT(Message::AddCamMsg, 2);
REG_MSG_REPLY_DEFAULT(Message::ModifyCamMsg, 3);
REG_MSG_REPLY_DEFAULT(Message::DelCamMsg, 4);

REG_MSG_WITH_REPLY(Message::GetCamListMsg, 5, Message::ModifyCamMsg);

REG_MSG_TYPE(Message::WarnMsg, 6);

REG_MSG_WITH_REPLY(Message::GetWarnLogMsg, 7, Message::WarnLogReply);
REG_MSG_TYPE(Message::WarnLogReply, 8);

REG_MSG_REPLY_DEFAULT(Message::AreaMsg, 9);

REG_MSG_TYPE(Message::MsgReply, 10);

REG_MSG_REPLY_DEFAULT(Message::FocusCamMsg, 11);

REG_MSG_REPLY_DEFAULT(Message::DelWarnLogMsg, 12);


/**
* MsgType Registration
* Block end
*/