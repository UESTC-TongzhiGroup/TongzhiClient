#pragma once
#include <string>

#define IMPL_OUT(TYPE)					inline void TYPE::toJsonObj(value &out)
#define PUT_OUT(field)					out[#field] = field
#define IMPL_IN(TYPE)					inline void TYPE::fromJsonObj(value &in)
#define GET_IN(jsonType,field)			field = in[#field].as##jsonType()
#define GET_IN_BY(jsonType,field,func)	field = func(in[#field].as##jsonType())

using namespace Json;

typedef bool Bool;
typedef float Float;
typedef double Double;
typedef std::string String;