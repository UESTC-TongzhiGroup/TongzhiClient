#pragma once
#include "stdafx.h";
#include <map>
#include <set>

using std::string;

typedef UINT EID;

class GlobalEvent {
private:
	static EID NEXT_AVAILABLE;
public:
	static EID getNextAvailableID();

	virtual EID id()=0;
	virtual string name()=0;
};

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
	static void dispatch(GlobalEvent*);
};