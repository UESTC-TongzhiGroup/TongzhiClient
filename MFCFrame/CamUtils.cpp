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

void Cams::updateCamList() {
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
	PUT_OUT(ID);
	PUT_OUT(name);
	PUT_OUT(url);
	PUT_OUT(cam_user);
	PUT_OUT(cam_password);
	PUT_OUT(time_start);
	PUT_OUT(time_end);
	PUT_OUT(mode_name);
	PUT_OUT(mode);
	PUT_OUT(active);
}

void CamInfo::fromJsonObj(value& in) {
	GET_IN(String, ID);
	GET_IN(String, name);
	GET_IN(String, url);
	GET_IN(String, cam_user);
	GET_IN(String, cam_password);
	GET_IN(String, time_start);
	GET_IN(String, time_end);
	GET_IN(String, mode_name);
	GET_IN_BY(String, mode, getCamMode);
	GET_IN(Bool, active);
}