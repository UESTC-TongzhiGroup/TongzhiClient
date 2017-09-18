#pragma once
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#undef min
#undef max
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>

#include "UserUtil.h"
#include "Config.h"
#include "StrUtil.h"


using std::string;

using namespace boost::property_tree;
using namespace bsoncxx::builder::stream;

typedef bsoncxx::builder::stream::document docBuilder;
typedef bsoncxx::v_noabi::document::value doc;

class DBHelper {
public:
	//数据库名
	const static char* dbVideo;
	//摄像头数据集名
	const static char* collecCam;
	//监视区域数据集名
	const static char* collecArea;
	//用户数据集名
	const static char* collecUser;
	DBHelper();
public:
	void pullCamInfo();
	void saveCamInfo();
	UserType login();
	void updateCamInfo(int index);
	void updateCamArea(int index, CRect area);
private:
	const static mongocxx::instance inst;
	mongocxx::client conn;
	mongocxx::database db;
	doc parseAreaInfo(int, CRect);
	doc parseFromCamInfo(int index, const CameraConfig & info);
	void parseToCamInfo(ptree &pt, CameraConfig &cam);
};