#pragma once
#include "EventBus.h"

#define DEF_EVENT(EVENT_CLASS, _NAME) \
	const EID EVENT_CLASS::ID = GlobalEvent::getNextAvailableID();\
	const string EVENT_CLASS::NAME = _NAME;

namespace Events {
	class CamListUpdate : public GlobalEvent {
		static EID ID;
	public:
		virtual EID id() {
			return ID;
		}

		virtual string name() {
			return "CamListUpdate";
		}
	};

}