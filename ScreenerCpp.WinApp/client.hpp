#pragma once
#include <thread>
#include <winsock.h>

using namespace std;

namespace screener::winapp::server
{
	class client
	{
		SOCKET tcp_;
		SOCKET udp_;
		SOCKADDR_IN address_;
		thread* process_;

		explicit client(SOCKET, SOCKET, SOCKADDR_IN);
		void run() const;

	public:
		~client();

		static client* create(SOCKET, SOCKADDR_IN);
	};
}
