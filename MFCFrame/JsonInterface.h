#pragma once

#include "JsonMacro.h"

template<UInt TYPE>
IMPL_OUT(_ConnMsg<TYPE>) {
	PUT_OUT(user);
	PUT_OUT(password);
	PUT_OUT(type);
}

template<UInt TYPE>
IMPL_IN(_ConnMsg<TYPE>) {
	GET_IN(String, user);
	GET_IN(String, password);
	GET_IN(String, type);
}

template<UInt TYPE>
IMPL_OUT(_CamMsg<TYPE>) {
	for (auto cam : list) {
		value sub;
		cam.toJsonObj(sub);
		//out["list"].append(sub);
		out.append(sub);
	}
}

template<UInt TYPE>
IMPL_IN(_CamMsg<TYPE>) {
	list.clear();
	//for (auto &itor : in["list"]) {
	for (auto &itor : in) {
		CamInfo cam;
		cam.fromJsonObj(itor);
		list.emplace(cam);
	}
}

namespace Message {

	IMPL_OUT(GetWarnLogMsg) {
		PUT_OUT(time);
		PUT_OUT(meta);
	}

	IMPL_IN(GetWarnLogMsg) {
		GET_IN(String, time);
		GET_IN(String, meta);
	}

	IMPL_OUT(WarnMsg) {
		PUT_OUT(time);
		PUT_OUT(route);
		PUT_OUT(cameraName);
		PUT_OUT(context);
		PUT_OUT(producer);
	}

	IMPL_IN(WarnMsg) {
		GET_IN(String, time);
		GET_IN(String, route);
		GET_IN(String, cameraName);
		GET_IN(String, context);
		GET_IN(String, producer);
	}

	IMPL_OUT(MsgReply) {
		PUT_OUT(status);
		PUT_OUT(meta);
	}

	IMPL_IN(MsgReply) {
		GET_IN(String, status);
		GET_IN(String, meta);
	}

	IMPL_OUT(AreaMsg) {
		PUT_OUT(ID);
		PUT_OUT(left_up);
		PUT_OUT(right_down);
	}

	IMPL_IN(AreaMsg) {
		GET_IN(UInt, ID);
		GET_IN(Int, left_up);
		GET_IN(Int, right_down);
	}

	IMPL_OUT(WarnLogReply) {
		for (auto wrn : list) {
			value sub;
			wrn.toJsonObj(sub);
			out["list"].append(sub);
		}
		PUT_OUT(meta);
	}

	IMPL_IN(WarnLogReply) {
		list.clear();
		for (auto &itor : in["list"]) {
			WarnMsg wrn;
			wrn.fromJsonObj(itor);
			list.push_back(wrn);
		}
		GET_IN(String, meta);
	}

	IMPL_OUT(DelCamMsg) {
		for (auto id : list) {
			out["list"].append(id);
		}
	}

	IMPL_IN(DelCamMsg) {
		list.clear();
		for (auto &itor : in["list"]) {
			list.emplace(itor.asString());
		}
	}

	IMPL_OUT(FocusCamMsg) {
		for (auto id : list) {
			//out["list"].append(id);
			out.append(id);
		}
	}

	IMPL_IN(FocusCamMsg) {
		list.clear();
		//for (auto &itor : in["list"]) 
		for (auto &itor : in) {
			list.emplace(itor.asString());
		}
	}

	IMPL_OUT(HeartBeat) {
		PUT_OUT(status);
	}
}