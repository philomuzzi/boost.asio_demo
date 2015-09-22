#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;
size_t read_complete(char * buf, const boost::system::error_code &err, size_t bytes) {
	if (err) return 0;
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

void handle_connections() {
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
	char buff[1024];
	while (true) {
		ip::tcp::socket sock(service);
		acceptor.accept(sock);
		auto bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
		std::string msg(buff, bytes);
		std::cout << msg.c_str();
		sock.write_some(buffer(msg));
		sock.close();
	}
}

int main() {
	handle_connections();
}