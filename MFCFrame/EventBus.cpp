#include "stdafx.h"
#include <memory>
#include <utility>
#include "Resource.h"
#include "EventBus.h"

std::map<EID, EventBus> EventBus::BUS_MAP;

void EventBus::regist(HWND handle)
{
	handlerSet.insert(handle);
}

void EventBus::dispatch(WPARAM wParam, LPARAM lParam)
{
	for (auto itor = handlerSet.begin(); itor != handlerSet.end(); itor++) {
		HWND handle = *itor;
		::SendMessage(handle, GLOBAL_EVENT, wParam, lParam);
	}
}

EventBus& EventBus::getEventBus(EID ID)
{
	auto itor = BUS_MAP.find(ID);
	if (itor == BUS_MAP.end()) {
		return BUS_MAP.insert(std::make_pair(ID, EventBus(ID))).first->second;
	}
	return itor->second;
}

void EventBus::regist(EID ID, HWND handle)
{
	getEventBus(ID).regist(handle);
}

