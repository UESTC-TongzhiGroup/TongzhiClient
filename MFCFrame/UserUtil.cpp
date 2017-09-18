#include "stdafx.h"
#include "UserUtil.h"

UserType User::getType(int t)
{
	switch (t) {
	case UserType::Admin:
		return UserType::Admin;
	//
	case UserType::User:
	default:
		return UserType::User;
	}
}
