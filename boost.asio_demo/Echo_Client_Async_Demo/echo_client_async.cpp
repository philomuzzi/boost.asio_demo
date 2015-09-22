#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::posix_time;

io_service service;

#define MEM_FUN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y) boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x,y,z) boost::bind(&self_type::x, shared_from_this(), y, z)


class talk_to_srv : public boost::enable_shared_from_this<talk_to_srv>, boost::noncopyable {
	typedef talk_to_srv self_type;
	talk_to_srv(const std::string & message) : sock_(service), message_(message) {}

	void start(ip::tcp::endpoint ep) {
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
	}

public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_srv> ptr;

	static ptr start(ip::tcp::endpoint ep, const std::string & message) {
		ptr new_(new talk_to_srv(message));
		new_->start(ep);
		return new_;
	}

	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();
	}

	bool started() { return started_; }
private:
	void on_connect(const error_code & err) {
		if (!err) do_write(message_ + "\n");
		else stop();
	}

	void on_read(const error_code & err, size_t bytes) {
		if (!err) {
			std::string copy(read_buffer_, bytes - 1);
			std::cout << "server echoed our " << message_ << ": "
				<< (copy == message_ ? "OK" : "FAIL") << std::endl;
		}

		stop();
	}

	void on_write(const error_code& err, size_t bytes) {
		do_read();
	}

	void do_read() {
		async_read(sock_, buffer(read_buffer_),
			MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
	}

	void do_write(const std::string& msg) {
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(buffer(write_buffer_, msg.size()), MEM_FN2(on_write, _1, _2));
	}

	size_t read_complete(const error_code& err, size_t bytes) {
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}

private:
	ip::tcp::socket sock_;
	char read_buffer_[1024];
	char write_buffer_[1024];
	bool started_{ true };
	std::string message_;
};

int main() {
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
	char * messages[] = { "john says hi", "so does james", "lucy just got home", "ssxxxx", 0 };
	for (char ** message = messages; *message; ++message) {
		talk_to_srv::start(ep, *message);
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

	service.run();

	getchar();
}