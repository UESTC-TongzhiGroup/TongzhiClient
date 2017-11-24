#include "stdafx.h"
#include "CamUtils.h"
#include "NetHandler.h"
#include "NetMessage.h"
#include "UserUtil.h"
#include "EventBus.h"
#include "Events.h"
#include "JsonMacro.h"

CamList camInfo;

using namespace Cams;

CamInfo::CamInfo(CamID _ID, string _name, string _url, string _user, string _password) :
	url(_url),
	ID(_ID),					name(_name),
	cam_user(_user),			cam_password(_password),
	mode(CamMode::Inactive),	mode_name(ModeName[mode]) {}

CamList & Cams::getCamInfo()
{
	return camInfo;
}

void Cams::pullCamInfoList() {
	camInfo.clear();
	Message::GetCamListMsg msg;
	auto reply = MsgHandler::sendReqMsg(msg);
	for (const CamInfo& cam : reply.list) {
		camInfo += cam;
	}

	EventBus::dispatch(Events::CamListUpdate());
}

void CamList::addCam(const CamInfo &cam) {
	insert(std::make_pair(cam.ID, cam));
}

void CamList::operator+=(const CamInfo &cam) {
	addCam(cam);
}

void CamInfo::toJsonObj(value& out) {
	OUT_TO_JSON(ID);
	OUT_TO_JSON(name);
	OUT_TO_JSON(url);
	OUT_TO_JSON(cam_user);
	OUT_TO_JSON(cam_password);
	OUT_TO_JSON(time_start);
	OUT_TO_JSON(time_end);
	OUT_TO_JSON(mode_name);
	OUT_TO_JSON(mode);
	OUT_TO_JSON(active);
}

void CamInfo::fromJsonObj(value& in) {
	IN_FROM_JSON(String, ID);
	IN_FROM_JSON(String, name);
	IN_FROM_JSON(String, url);
	IN_FROM_JSON(String, cam_user);
	IN_FROM_JSON(String, cam_password);
	IN_FROM_JSON(String, time_start);
	IN_FROM_JSON(String, time_end);
	IN_FROM_JSON(String, mode_name);
	GET_IN_BY(String, mode, getCamMode);
	IN_FROM_JSON(Bool, active);
}