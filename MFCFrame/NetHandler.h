#pragma once
#include "NetMessage.h"
#include <boost\asio.hpp>
#include <boost\asio\streambuf.hpp>
#include <boost\system\error_code.hpp>
#include <json\json.h>
#include <condition_variable>
#include <queue>

using std::string;
using std::queue;
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
typedef boost::asio::streambuf sock_buf;
typedef boost::system::error_code err_code;

#define CHECK_TYPE(root) root["msg_type"].asInt()

class MsgHandler
{
protected:
	void handleReadData(sock_buf&, err_code, size_t);
	MsgHandler();
	bool active;
private:
	class BlockMsgQueue{
		queue<string> que;
		std::condition_variable_any not_emp;
		std::mutex m_mutex;
	public:
		string pop();
		void push(string);
	} responses;
	io_service service;
	socket_ptr msg_sock;
	static MsgHandler& get(bool);
public:
	static string request(string);
	static void stop();

	template<typename MSG>
	static string getJson(MSG msg) {
		Json::Value root;
		root["msg_type"] = ID_OF_MSG_TYPE(MSG);
		auto &sub = root["content"];
		msg.toJsonObj(sub);
		if(sub.type()==Json::nullValue)
			sub = Json::Value(Json::objectValue);
		Json::FastWriter wr;
		return wr.write(root);
	}


	template<typename MSG>
	static auto sendReqMsg(const MSG & msg_data){
		typedef REPLY_OF_MSG(MSG) Reply_t;
		string response(request(getJson(msg_data)));
		Json::Reader reader;
		Json::Value root;
		reader.parse(response, root);
		BOOST_ASSERT(CHECK_TYPE(root) == ID_OF_MSG_TYPE(Reply_t));
		Reply_t reply;
		reply.fromJsonObj(root["content"]);
		return reply;
	}
};
