#pragma once
#include "stdafx.h";
#include "StrUtil.h"
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
	EventBus(EID ID) :EVENT_ID(ID){}
public:
	static EventBus& getEventBus(EID);
	static void regist(EID, HWND);

	template<typename GlobalEvent>
	static void dispatch(GlobalEvent& _event)
	{
		string event_name = _event.name();
		TRACE(_T("分发事件 %s\n"), StrUtil::std2CStr(event_name));
		auto ptr = std::make_shared<GlobalEvent>(_event);
		getEventBus(_event.ID()).dispatch(
			_event.ID(),
			(LPARAM)ptr.get()
		);
	}
};