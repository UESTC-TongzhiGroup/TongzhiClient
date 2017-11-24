#pragma once

#include "JsonMacro.h"

template<UInt TYPE>
IMPL_OUT(_ConnMsg<TYPE>) {
	OUT_TO_JSON(user);
	OUT_TO_JSON(password);
	OUT_TO_JSON(type);
}

template<UInt TYPE>
IMPL_IN(_ConnMsg<TYPE>) {
	IN_FROM_JSON(String, user);
	IN_FROM_JSON(String, password);
	IN_FROM_JSON(String, type);
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
		OUT_TO_JSON(time);
		OUT_TO_JSON(meta);
	}

	IMPL_IN(GetWarnLogMsg) {
		IN_FROM_JSON(String, time);
		IN_FROM_JSON(String, meta);
	}

	IMPL_OUT(WarnMsg) {
		OUT_TO_JSON(time);
		OUT_TO_JSON(route);
		OUT_TO_JSON(cameraName);
		OUT_TO_JSON(context);
		OUT_TO_JSON(producer);
	}

	IMPL_IN(WarnMsg) {
		IN_FROM_JSON(String, time);
		IN_FROM_JSON(String, route);
		IN_FROM_JSON(String, cameraName);
		IN_FROM_JSON(String, context);
		IN_FROM_JSON(String, producer);
	}

	IMPL_OUT(MsgReply) {
		OUT_TO_JSON(status);
		OUT_TO_JSON(meta);
	}

	IMPL_IN(MsgReply) {
		IN_FROM_JSON(String, status);
		IN_FROM_JSON(String, meta);
	}

	IMPL_OUT(AreaMsg) {
		OUT_TO_JSON(ID);
		OUT_TO_JSON(top);
		OUT_TO_JSON(bottom);
		OUT_TO_JSON(left);
		OUT_TO_JSON(right);
	}

	IMPL_IN(AreaMsg) {
		IN_FROM_JSON(String, ID);
		IN_FROM_JSON(Int, top);
		IN_FROM_JSON(Int, bottom);
		IN_FROM_JSON(Int, left);
		IN_FROM_JSON(Int, right);
	}

	IMPL_OUT(WarnLogReply) {
		for (auto wrn : list) {
			value sub;
			wrn.toJsonObj(sub);
			//out["list"].append(sub);
			out.append(sub);
		}
		OUT_TO_JSON(meta);
	}

	IMPL_IN(WarnLogReply) {
		list.clear();
		//for (auto &itor : in["list"]) {
		for (auto &itor : in) {
			WarnMsg wrn;
			wrn.fromJsonObj(itor);
			list.push_back(wrn);
		}
		IN_FROM_JSON(String, meta);
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
		OUT_TO_JSON(status);
	}
}