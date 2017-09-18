#pragma once
#include "stdafx.h"
#include "Events.h"

namespace Events{
	EID CamListUpdate::ID = GlobalEvent::getNextAvailableID();
	string CamListUpdate::NAME = "CamListUpdate";
}