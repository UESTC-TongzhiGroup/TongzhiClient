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
		::PostMessage(handle, GLOBAL_EVENT, wParam, lParam);
	}
}

EventBus& EventBus::getEventBus(EID id)
{
	auto itor = BUS_MAP.find(id);
	if (itor == BUS_MAP.end()) {
		return BUS_MAP.insert(std::make_pair(id, EventBus(id))).first->second;
	}
	return itor->second;
}

void EventBus::regist(EID id, HWND handle)
{
	getEventBus(id).regist(handle);
}

