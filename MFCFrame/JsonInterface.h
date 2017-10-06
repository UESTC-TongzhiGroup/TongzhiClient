#pragma once

#define IMPL_OUT(TYPE)			inline value & TYPE::toJsonObj(value &out)
#define PUT_OUT(field)			out[#field] = field
#define IMPL_IN(TYPE)			inline value & TYPE::fromJsonObj(value &in)
#define GET_IN(jsonType,field)	field = in[#field].as##jsonType()

typedef bool Bool;
typedef float Float;
typedef double Double;
typedef string String;

template<UInt TYPE>
IMPL_OUT(_ConnMsg<TYPE>) {
	PUT_OUT(user);
	PUT_OUT(password);
	PUT_OUT(type);
	return out;
}

template<UInt TYPE>
IMPL_IN(_ConnMsg<TYPE>) {
	GET_IN(String, user);
	GET_IN(String, password);
	GET_IN(String, type);
	return in;
}

template<UInt TYPE>
IMPL_OUT(_WrnLogMsg<TYPE>) {
	PUT_OUT(time);
}

template<UInt TYPE>
IMPL_IN(_WrnLogMsg<TYPE>) {
	GET_IN(String, time);
}

template<UInt TYPE>
IMPL_OUT(_CamMsg<TYPE>) {
	for (auto cam : obj.cams) {
		value sub;
		sub << cam;
		out.append(subt);
	}
	return out;
}

template<UInt TYPE>
IMPL_IN(_CamMsg<TYPE>) {
	obj.cams.clear();
	for (auto &itor : in) {
		CamInfo cam;
		itor >> cam;
		obj.cams.push_back(cam);
	}
	return in;
}

namespace Message {

	IMPL_OUT(WarnMsg) {
		PUT_OUT(time);
		PUT_OUT(route);
		PUT_OUT(cam_name);
		PUT_OUT(warn_type);
		PUT_OUT(producer);
		return out;
	}

	IMPL_IN(WarnMsg) {
		GET_IN(String, time);
		GET_IN(String, route);
		GET_IN(String, cam_name);
		GET_IN(String, warn_type);
		GET_IN(String, producer);
		return in;
	}

	IMPL_OUT(MsgReply) {
		PUT_OUT(status);
		PUT_OUT(meta);
		return out;
	}

	IMPL_IN(MsgReply) {
		GET_IN(String, status);
		GET_IN(String, meta);
		return in;
	}

	IMPL_OUT(AreaMsg) {
		PUT_OUT(ID);
		PUT_OUT(left_up);
		PUT_OUT(right_down);
		return out;
	}

	IMPL_IN(AreaMsg) {
		GET_IN(UInt, ID);
		GET_IN(Int, left_up);
		GET_IN(Int, right_down);
		return in;
	}

	IMPL_OUT(WarnLogReply) {
		for (auto wrn : warn_log) {
			value sub;
			;
			out.append(sub);
		}
		return out;
	}

	IMPL_IN(WarnLogReply) {
		warn_log.clear();
		for (auto &itor : in) {
			WarnMsg wrn;
			;
			warn_log.push_back(wrn);
		}
		return in;
	}

	IMPL_OUT(DelCamMsg) {
		for (auto id : list) {
			out.append(id);
		}
		return out;
	}

	IMPL_IN(DelCamMsg) {
		list.clear();
		for (auto &itor : in) {
			list.push_back(itor.asInt());
		}
		return in;
	}

	IMPL_OUT(HeartBeat) {
		PUT_OUT(status);
		return out;
	}
}