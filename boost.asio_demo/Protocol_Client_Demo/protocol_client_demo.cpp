#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <functional>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;

io_service service;

size_t read_complete(char * buf, const boost::system::error_code &err, size_t bytes) {
	if (err) return 0;
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);

void sync_echo(std::string msg) {
	msg += "\n";	
	int head = (1 << 16) | msg.size() & 0x0000ffff;
	std::array<char, sizeof(int)> arrHead;
	memcpy(arrHead.data(), &head, sizeof(head));
	std::string strHead(arrHead.data(), sizeof(int));
	msg = strHead + msg;

	ip::tcp::socket sock(service);
	sock.connect(ep);
	sock.write_some(buffer(msg));


	char buf[1024];
	memset(buf, 0, 1024);
	int bytes = read(sock, buffer(buf), boost::bind(read_complete, buf, _1, _2));
	std::string copy(buf, bytes - 1);
	msg = msg.substr(sizeof(int), msg.size() - sizeof(int) - 1);
	std::cout << "server echoed our " << msg << ": " << (copy == msg ? "OK" : "FAIL") << std::endl;
	sock.close();
}

int main() {
	char * messages[] = { "john says hi", "so does james", "lucy just got home", "ssxxxx", 0 };
	boost::thread_group threads;
	for (char ** message = messages; *message; ++message) {
		threads.create_thread(std::bind(sync_echo, *message));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

	threads.join_all();

	getchar();
}