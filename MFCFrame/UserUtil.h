#pragma once
#include <string>

using std::string;

typedef enum {
	User, Admin
} UserType;

class User {
	UserType type;
	string name;
public:
	static UserType getType(int t);
	static UserType getTypeByStr(string str);
	static string getTypeName(int);
};
