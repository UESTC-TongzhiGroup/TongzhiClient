#pragma once
#include "stdafx.h";
#include <map>
#include <set>

#define GLOBAL_EVENT (WM_USER+400)

using std::string;

typedef UINT EID;

class EventBus {
private:
	static std::map<EID, EventBus> BUS_MAP;
private:
	const EID EVENT_ID;
	const string EVENT_NAME;
	std::set<HWND> handlerSet;
	void regist(HWND);
	void dispatch(WPARAM, LPARAM);
	EventBus(EID id) :EVENT_ID(id){}
public:
	static EventBus& getEventBus(EID);
	static void regist(EID, HWND);

	template<typename GlobalEvent>
	static void dispatch(GlobalEvent& _event)
	{
		TRACE("分发事件 %s", _event.name());
		auto ptr = std::make_shared<GlobalEvent>(_event);
		getEventBus(_event.id()).dispatch(
			_event.id(),
			(LPARAM)ptr.get()
		);
	}
};