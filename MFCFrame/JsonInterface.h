#pragma once

#define IMPL_OUT(TYPE)		inline ptree & operator<<(ptree &out,	TYPE &obj)
#define PUT_OUT(field)		out.put(#field,obj.##field)
#define IMPL_IN(TYPE)		inline ptree & operator>>(ptree &in ,	TYPE &obj)
#define GET_IN(type,field)	obj.##field = in.get<type>(#field)

template<int TYPE>
ptree & operator<<(ptree &out, _ConnMsg<TYPE> &obj) {
	PUT_OUT(user);
	PUT_OUT(password);
	PUT_OUT(type);
	return out;
}

template<int TYPE>
ptree & operator>>(ptree &in, _ConnMsg<TYPE> &obj) {
	GET_IN(string, user);
	GET_IN(string, password);
	GET_IN(string, type);
	return in;
}

template<int TYPE>
ptree & operator<<(ptree &out, _WrnLogMsg<TYPE> &obj) {
	PUT_OUT(time);
}

template<int TYPE>
ptree & operator>>(ptree &in, _WrnLogMsg<TYPE> &obj) {
	GET_IN(string, time);
}

template<int TYPE>
ptree & operator<<(ptree &out, _CamMsg<TYPE> &obj) {
	for (auto cam : obj.cams) {
		ptree subt;
		subt << cam;
		out.push_back(std::make_pair("", subt));
	}
	return out;
}

template<int TYPE>
ptree & operator>>(ptree &in, _CamMsg<TYPE> &obj) {
	obj.cams.clear();
	for (auto &itor : in) {
		CamInfo cam;
		itor.second >> cam;
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
		GET_IN(string, time);
		GET_IN(string, route);
		GET_IN(string, cam_name);
		GET_IN(string, warn_type);
		GET_IN(string, producer);
		return in;
	}

	IMPL_OUT(MsgReply) {
		PUT_OUT(status);
		PUT_OUT(meta);
		return out;
	}

	IMPL_IN(MsgReply) {
		GET_IN(string, status);
		GET_IN(string, meta);
		return in;
	}

	IMPL_OUT(AreaMsg) {
		PUT_OUT(ID);
		PUT_OUT(left_up);
		PUT_OUT(right_down);
		return out;
	}

	IMPL_IN(AreaMsg) {
		GET_IN(UINT, ID);
		GET_IN(int, left_up);
		GET_IN(int, right_down);
		return in;
	}

	IMPL_OUT(WarnLogReply) {
		for (auto wrn : obj.warn_log) {
			ptree subt;
			subt << wrn;
			out.push_back(std::make_pair("", subt));
		}
		return out;
	}

	IMPL_IN(WarnLogReply) {
		obj.warn_log.clear();
		for (auto &itor : in) {
			WarnMsg wrn;
			itor.second >> wrn;
			obj.warn_log.push_back(wrn);
		}
		return in;
	}

	IMPL_OUT(HeartBeat) {
		PUT_OUT(status);
		return out;
	}
}