#include "stdafx.h"
#include "Config.h"
#include <functional>
#include <mutex>  
#include <thread>
#include "NetHandler.h"
#include "Events.h"

using std::stringstream;

typedef ip::tcp::endpoint endpoint_t;


/**
*Util Function:
*Implementation
*/

endpoint_t getEndPoint() {
	auto server = Config::getServerInfo();
	if (!server.load)
		throw std::exception("服务器配置尚未加载/初始化");
	return endpoint_t(ip::address::from_string(server.url), server.reqport);
}

string getReadStr(sock_buf& buffer, size_t size) {
	//buffer.commit(size);
	auto buf = buffer.data();
	string s(buffers_begin(buf), buffers_end(buf));
	buffer.consume(size);
	s.resize(size - 2);
	return s;
}

bool isWrnMsg(value root) {
	return CHECK_TYPE(root) == ID_OF_MSG_TYPE(Message::WarnMsg);
}

enum { HEARTBEAT_INTERVAL = 3000 };


/**
* class MsgHandler:
* Implementation
* 这玩意根本没法从逻辑上来保证请求和回应是对应的
* 只是tcp的特性使得请求和回应的顺序是一样的罢了
*/

MsgHandler::MsgHandler():
	active(false),
	service(),
	msg_sock(new ip::tcp::socket(service))
{
	msg_sock->connect(getEndPoint());
	static sock_buf buf;
	async_read_until(*msg_sock,buf,Message::SPLIT_SIGN,
		[&](err_code ec, size_t size)->void {
			this->handleReadData(buf, ec, size);
		}
	);
}

#define NO_HEART_BEAT

MsgHandler& MsgHandler::get(bool startNow = true) {
	static MsgHandler inst;
	if (startNow && !inst.active) {
		inst.active = true;
		std::thread service([]()->void {
			try {
				io_service::work w(inst.service);
				inst.service.run();
			}
			catch (...) {
				stop();
			}
		});
		service.detach();
#ifndef NO_HEART_BEAT
		std::thread heartBeat([]()->void {
			while (inst.active) {
				try {
					string hbtext(getJson(Message::HeartBeat{}));
					inst.msg_sock->write_some(buffer(hbtext + Message::SPLIT_SIGN));
				}
				catch (...) {
					stop();
				}
				Sleep(HEARTBEAT_INTERVAL);
			}
		});
		heartBeat.detach();
#endif // HEART_BEAT
	}
	return inst;
}

void MsgHandler::handleReadData(sock_buf& buf, err_code err, size_t bytes_transferred) {
	if (err)
		throw err;
	string data = getReadStr(buf, bytes_transferred);

	value root;
	Json::Reader reader;
	reader.parse(data, root);

	if (isWrnMsg(root)) {
		Message::WarnMsg wrn;
		wrn.fromJsonObj(root["content"]);
		EventBus::dispatch(Events::Warn(wrn));
	}
	else {
		responses.push(data);
	}

	async_read_until(*msg_sock, buf, Message::SPLIT_SIGN,
		[&](err_code ec, size_t size)->void {
			this->handleReadData(buf, ec, size);
		}
	);
}

string MsgHandler::request(string req)
{
	auto &inst = get();
	auto &soc = inst.msg_sock;
	soc->write_some(buffer(req + Message::SPLIT_SIGN));
	return inst.responses.pop();
}

void MsgHandler::stop() {
	auto &inst = get(false);
	inst.active = false;
	//inst.msg_sock->c
	inst.service.stop();
	inst.service.reset();
}

string MsgHandler::BlockMsgQueue::pop() {
	std::lock_guard<std::mutex> locker(m_mutex);

	not_emp.wait(m_mutex);
	string response = que.front();
	que.pop();
	return response;
}

void MsgHandler::BlockMsgQueue::push(string response) {
	std::lock_guard<std::mutex> locker(m_mutex);

	que.push(response);
	not_emp.notify_one();
}