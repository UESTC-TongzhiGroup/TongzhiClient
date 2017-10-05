#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "CamUtils.h"

using namespace boost::property_tree;

using std::string;

typedef unsigned int UINT;

#define JSON_INTERFACE_DECL(type)											\
friend ptree & operator<<(ptree&,	type &);								\
friend ptree & operator>>(ptree&,	type &);

#define NO_JSON_NEED(type)													\
inline friend ptree & operator<<(ptree &out,	type &){ return out;}		\
inline friend ptree & operator>>(ptree &in,		type &){ return	in;	}	

template<int TYPE>
struct _CamMsg {
	std::vector<CamInfo> cams;
	template<int T>
	friend ptree & operator<<(ptree&, _CamMsg<T> &);
	template<int T>
	friend ptree & operator>>(ptree&, _CamMsg<T> &);
};

template<int TYPE>
struct _ConnMsg {
	string user;
	string password;
	string type;
	template<int T>
	friend ptree & operator<<(ptree&, _ConnMsg<T> &);
	template<int T>
	friend ptree & operator>>(ptree&, _ConnMsg<T> &);
};

template<int TYPE>
struct _WrnLogMsg {
	string time;
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
	typedef _CamMsg<3> DelCamMsg;
	typedef _CamMsg<4> FocusCamMsg;

	struct GetCamListMsg{
		NO_JSON_NEED(GetCamListMsg)
	};

	
	typedef _ConnMsg<1> LoginMsg;
	typedef _ConnMsg<2> RegisterMsg;


	struct WarnMsg{
		string time;
		string route;
		string cam_name;
		string warn_type;
		string producer;
		JSON_INTERFACE_DECL(WarnMsg)
	};

	typedef _WrnLogMsg<0> GetWarnLogMsg;
	typedef _WrnLogMsg<1> DelWarnLogMsg;

	struct WarnLogReply{
		std::vector<WarnMsg> warn_log;
		JSON_INTERFACE_DECL(WarnLogReply)
	};


	struct AreaMsg{
		UINT ID;
		int left_up;
		int right_down;
		JSON_INTERFACE_DECL(AreaMsg)
	};


	struct MsgReply{
		string status;
		string meta;
		JSON_INTERFACE_DECL(MsgReply)
	};

	struct HeartBeat {
		string status = "alive";
		friend ptree & operator<<(ptree&, HeartBeat &);
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
	struct Type2ID<type> { const static UINT ID=n; }; \
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