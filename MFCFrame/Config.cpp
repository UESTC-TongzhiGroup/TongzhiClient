#include "stdafx.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Config.h"

using namespace boost::property_tree;

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
		ptree pt;
		read_json<ptree>(LICENSE_PATH, pt);
		for (auto child : pt.get_child("info")) {
			ptree subt = child.second;
			userLicense.insert(make_pair(subt.get<string>("user"), subt.get<string>("pass")));
		}
	}

	void loadServerCfg() {
		ptree pt;
		auto& info = getServerInfo();
		try {
			read_json<ptree>(SERVER_CONFIG_PATH, pt);
			info.url = pt.get<string>("url");
			info.reqport = pt.get<UINT16>("reqport");

			info.loginUser = pt.get<string>("user");
			info.loginPass = pt.get<string>("passwd");

			getServerInfo().load = true;
		}
		catch (std::exception e) {}
	}

	void saveServerCfg() {
		ptree pt;
		auto& info = Config::getServerInfo();
		pt.put<string>("url", info.url);
		pt.put<UINT16>("reqport", info.reqport);

		pt.put<string>("user", info.loginUser);
		pt.put<string>("passwd", info.loginPass);
		write_json(SERVER_CONFIG_PATH, pt);
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
