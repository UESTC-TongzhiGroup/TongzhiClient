#include "stdafx.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Config.h"

using namespace boost::property_tree;
using namespace bsoncxx::builder::stream;

using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;


ServerConfig serverInfo;

CamList camInfo;
StrDirc userLicense;

namespace Config {

	ServerConfig& getServerInfo()
	{
		return serverInfo;
	}

	CamList & getCamInfo()
	{
		return camInfo;
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
			info.dbport = pt.get<UINT16>("dbport");
			info.textport = pt.get<UINT16>("textport");

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
		pt.put<UINT16>("dbport", info.dbport);
		pt.put<UINT16>("textport", info.textport);

		pt.put<string>("user", info.loginUser);
		pt.put<string>("passwd", info.loginPass);
		write_json(SERVER_CONFIG_PATH, pt);
	}

	void clearServerCfg()
	{
		serverInfo.loginPass.clear();
		serverInfo.loginUser.clear();
		serverInfo.textport = 0;
		serverInfo.dbport = 0;
		serverInfo.url.clear();
		serverInfo.load = false;
	}
}
