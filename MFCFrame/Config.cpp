#include "stdafx.h"
#include <fstream>
#include <json\json.h>

#include "Config.h"

typedef Json::Value value;
using Json::Reader;
using std::ifstream;
using std::ofstream;

ServerConfig serverInfo;

StrDirc userLicense;

namespace Config {
	bool isServerCfgLoaded() {
		return serverInfo.load;
	}

	ServerConfig& getServerInfo()
	{
		return serverInfo;
	}

	StrDirc& getLicensesMap()
	{
		return userLicense;
	}

	void loadLicense() {
		userLicense.clear();
		value root;
		Reader reader;
		ifstream file(ifstream(LICENSE_PATH));
		reader.parse(file, root);
		for (auto &child : root["info"]) {
			userLicense.insert(make_pair(child["user"].asString(), child["pass"].asString()));
		}
		file.close();
	}

	void loadServerCfg() {
		clearServerCfg();
		auto& info = getServerInfo();
		value root;
		Reader reader;
		try {
			ifstream file(ifstream(SERVER_CONFIG_PATH));
			reader.parse(file, root);
			info.url = root["url"].asString();
			info.reqport = root["reqport"].asUInt();

			info.loginUser = root["user"].asString();
			info.loginPass = root["passwd"].asString();

			getServerInfo().load = true;
		}
		catch (std::exception e) {
			TRACE(e.what());
		}
	}

	void saveServerCfg() {
		value root;
		auto& info = Config::getServerInfo();
		root["url"] = info.url;
		root["reqport"] = info.reqport;

		root["user"] = info.loginUser;
		root["passwd"] = info.loginPass;
		ofstream file(SERVER_CONFIG_PATH, ofstream::trunc);
		file << root.toStyledString();
		file.close();
	}

	void clearServerCfg()
	{
		serverInfo.loginPass.clear();
		serverInfo.loginUser.clear();
		serverInfo.reqport = 0;
		serverInfo.url.clear();
		serverInfo.load = false;
	}
}
