#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/)
{
	std::cout << "Hello, world!\n";
}

int main()
{
	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(3));

	timer.async_wait([] (const boost::system::error_code&){
		std::cout << "Async Hello world!" << std::endl;
	});

	io.run();

	std::cout << "Hello, world!\n";

	getchar();
	return 0;
}