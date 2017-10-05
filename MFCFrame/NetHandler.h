#pragma once
#include "NetMessage.h"
#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/error_code.hpp>
#include <condition_variable>
#include <queue>

using std::string;
using std::queue;
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
typedef boost::asio::streambuf sock_buf;
typedef boost::system::error_code err_code;

#define CHECK_TYPE(Ptree) Ptree.get<int>("msg_type")

class MsgHandler
{
protected:
	void handleReadData(sock_buf&, err_code, size_t);
	MsgHandler();
	bool active;
	socket_ptr msg_sock;
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
	static MsgHandler& get();
public:
	static string request(string);
	static void start();
	static void stop();

	template<typename MSG>
	static string getJson(MSG msg) {
		ptree pt, sub;
		pt.put("msg_type", ID_OF_MSG_TYPE(MSG));
		sub << msg_data;
		pt.put_child("content", sub);
		stringstream ss;
		write_json(ss, pt, false);

		return ss.str();
	}


	template<typename MSG>
	static auto sendReqMsg(MSG msg_data){
		typedef REPLY_OF_MSG(MSG) Reply_t;
		string response(getJson(msg_data));
		ptree pt;
		read_json(stringstream(response), pt);
		BOOST_ASSERT(CHECK_TYPE(pt) == ID_OF_MSG_TYPE(Reply_t));
		Reply_t reply;
		pt.get_child("content") >> reply;
		return reply;
	}
};
