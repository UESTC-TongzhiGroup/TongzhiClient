#pragma once
#include <string>
#include <json\json.h>
#ifdef JSON_JSON_H_INCLUDED
	#ifndef JSON_MACROS
		#define JSON_MACROS
		#define IMPL_OUT(TYPE)							inline void TYPE::toJsonObj(value &out)
		#define OUT_TO_JSON(field)						out[#field] = field
		#define IMPL_IN(TYPE)							inline void TYPE::fromJsonObj(value &in)
		#define IN_FROM_JSON(jsonType,field)			field = in[#field].as##jsonType()
		#define GET_IN_BY(jsonType,field,func)			field = func(in[#field].as##jsonType())
	#endif
	typedef bool Bool;
	typedef float Float;
	typedef double Double;
	typedef int Int;
	typedef std::string String;
#endif
