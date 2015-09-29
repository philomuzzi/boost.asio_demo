#include <iostream>
#include <boost/asio.hpp>
#include <chrono>

typedef std::function<void(const boost::system::error_code&)> timer_callback;
using namespace std;

int main() {
	boost::asio::io_service io;
	boost::asio::deadline_timer timer_one_minute(io, boost::posix_time::minutes(1));
	boost::asio::deadline_timer timer_seconds(io, boost::posix_time::seconds(2));
	boost::asio::deadline_timer timer_one_hour(io, boost::posix_time::hours(1));

	timer_callback f = [&timer_seconds, &f](const boost::system::error_code&)
	{
		auto tp = std::chrono::system_clock::now();
		std::time_t cur_time = std::chrono::system_clock::to_time_t(tp);
		std::string str_time = std::ctime(&cur_time);
		std::cout << "what output? now time is: " << str_time << std::endl;

		timer_seconds.expires_at(timer_seconds.expires_at() + boost::posix_time::seconds(1));
		timer_seconds.async_wait(f);
	};

//	timer_one_minute.async_wait(f);
//	timer_one_hour.async_wait(f);
	timer_seconds.async_wait(f);

	auto tp = std::chrono::system_clock::now();
	std::time_t cur_time = std::chrono::system_clock::to_time_t(tp);
	std::string str_time = std::ctime(&cur_time);
	std::cout << "begin, now time is: " << str_time << std::endl;

	io.run();

	getchar();
	return 0;
}