#include "stdafx.h"
#include <memory>
#include <utility>
#include "Resource.h"
#include "EventBus.h"

BusMap EventBus::BUS_MAP;

void EventBus::regist(Handler &handle)
{
	handlers.insert({ handle.ivk, handle });
}

void EventBus::unregist(Handler::Invoker ivk)
{
	handlers.erase(ivk);
}

void EventBus::dispatch(BaseEvent &event)
{
	for (auto &itor = handlers.begin(); itor != handlers.end(); itor++) {
		itor->second.func(event);
	}
}

EventBus& EventBus::getEventBus(EID ID)
{
	return BUS_MAP.emplace(ID, EventBus()).first->second;
}

