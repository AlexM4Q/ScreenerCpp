#pragma once
#include <vector>
#include <winsock.h>
#include "client.hpp"

namespace screener::winapp::server
{
	class tcp_server
	{
		SOCKET server_;
		vector<client*> clients_;
		thread* process_;

		explicit tcp_server(SOCKET);
		void run();

	public:
		~tcp_server();

		static tcp_server* create();
	};
}
