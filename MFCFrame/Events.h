#pragma once
#include "EventBus.h"
#include "NetMessage.h"
#include "UserUtil.h"


#define DEF_EVENT(EVENT,E_ID)									\
	public:														\
		inline EID id() {										\
			return E_ID;										\
		}														\
		inline string name() {									\
			return #EVENT;										\
		}														\

namespace Events {
	class CamListUpdate {
		DEF_EVENT(CamListUpdate, 0)
	}; 

	class UserLogin {
		DEF_EVENT(UserLogin, 1)
	public:
		UserType type;
		UserLogin(UserType _type) :type(_type) {}
	};

	class Warn {
		DEF_EVENT(Warn, 2)
	public:
		typedef Message::WarnMsg msg;
		msg warn;
		Warn(msg _warn) :warn(_warn) {}
	};
}