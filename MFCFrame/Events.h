#pragma once
#include "NetMessage.h"
#include "UserUtil.h"
#include <typeinfo>
#include <typeindex>


#define DEF_EVENT(EVENT)										\
	public:														\
		inline static const char * name() {						\
			return #EVENT;										\
		}														\

namespace Events {
	class BaseEvent {};

	class CamListUpdate :public BaseEvent {
		DEF_EVENT(CamListUpdate)
	}; 

	class UserLogin :public BaseEvent {
		DEF_EVENT(UserLogin)
	public:
		UserType type;
		UserLogin(UserType _type) :type(_type) {}
	};

	class Warn :public BaseEvent {
		DEF_EVENT(Warn)
	public:
		typedef Message::WarnMsg msg;
		msg warn;
		Warn(msg _warn) :warn(_warn) {}
	};

	//监控画面选择更新事件
	class HMISelect :public BaseEvent {
		DEF_EVENT(Warn)
	public:
		static void* current;
		HMISelect(void *ivk) { current = ivk; }
	};
}