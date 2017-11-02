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
		PUT_OUT(top);
		PUT_OUT(bottom);
		PUT_OUT(left);
		PUT_OUT(right);
	}

	IMPL_IN(AreaMsg) {
		GET_IN(String, ID);
		GET_IN(Int, top);
		GET_IN(Int, bottom);
		GET_IN(Int, left);
		GET_IN(Int, right);
	}

	IMPL_OUT(WarnLogReply) {
		for (auto wrn : list) {
			value sub;
			wrn.toJsonObj(sub);
			//out["list"].append(sub);
			out.append(sub);
		}
		PUT_OUT(meta);
	}

	IMPL_IN(WarnLogReply) {
		list.clear();
		//for (auto &itor : in["list"]) {
		for (auto &itor : in) {
			WarnMsg wrn;
			wrn.fromJsonObj(itor);
			list.push_back(wrn);
		}
		GET_IN(String, meta);
	}

	IMPL_OUT(DelCamMsg) {
		for (auto ID : list) {
			//out["list"].append(ID);
			out.append(ID);
		}
	}

	IMPL_IN(DelCamMsg) {
		list.clear();
		//for (auto &itor : in["list"]) {
		for (auto &itor : in) {
			list.emplace(itor.asString());
		}
	}

	IMPL_OUT(FocusCamMsg) {
		for (auto ID : list) {
			//out["list"].append(id);
			out.append(ID);
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