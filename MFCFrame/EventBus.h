#pragma once
#include "stdafx.h";
#include "StrUtil.h"
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include "Events.h"

using std::string;
using namespace Events;

class EventBus;

typedef std::type_index EID;
typedef std::unordered_map<std::type_index, EventBus> BusMap;

class Handler {
public:
	typedef void* Invoker;
	typedef std::function<void(BaseEvent&)> Function;

	Invoker ivk;
	Function func;
	Handler(Invoker ivk, Function func) : ivk(ivk), func(func) {}

	bool operator== (Handler& another) {
		return ivk == another.ivk;
	}
};

#define CREAT_HANDLER(func) Handler((void*)this, func)

typedef std::unordered_map<Handler::Invoker, Handler> HandlerMap;

class EventBus {
private:
	static BusMap BUS_MAP;
private:
	HandlerMap handlers;
	void regist(Handler&);
	void unregist(Handler::Invoker);
	void dispatch(BaseEvent&);
	static EventBus& getEventBus(EID);
public:
	template<class EventType>
	static void regist(Handler &h) {
		getEventBus(typeid(EventType)).regist(h);
	}

	template<class EventType>
	static void unregist(Handler::Invoker ivk) {
		getEventBus(typeid(EventType)).unregist(ivk);
	}

	static void unregistAll(Handler::Invoker ivk) {
		for (auto &itor = BUS_MAP.begin(); itor != BUS_MAP.end(); itor++) {
			itor->second.unregist(ivk);
		}
	}

	template<class EventType>
	static void dispatch(EventType& _event)
	{
		TRACE(_T("分发事件 %s\n"), CString(_event.name()));
		getEventBus(typeid(EventType)).dispatch(static_cast<BaseEvent&>(_event));
	}
};