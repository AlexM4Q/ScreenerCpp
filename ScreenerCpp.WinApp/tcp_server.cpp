#include "stdafx.h"
#include "tcp_server.hpp"
#include <cstdio>
#include <winsock.h>

#define TCP_PORT 4555
#define UDP_PORT 4553

namespace screener::winapp::server
{
	tcp_server::tcp_server(const SOCKET sever)
	{
		server_ = sever;
		clients_ = vector<client*>();
		process_ = new thread([this] { run(); });
	}

	tcp_server::~tcp_server()
	{
		closesocket(server_);
		delete process_;
	}

	void tcp_server::run()
	{
		int client_address_size = sizeof(SOCKADDR_IN);
		while (true)
		{
			SOCKADDR_IN client_address;
			const int socket = accept(server_, (SOCKADDR*)&client_address, &client_address_size);
			if (SOCKET_ERROR == socket)
			{
				continue;
			}

			client_address.sin_port = htons(UDP_PORT);

			const auto client = client::create(socket, client_address);
			if (nullptr == client)
			{
				continue;
			}

			clients_.push_back(client);
		}
	}

	tcp_server* tcp_server::create()
	{
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data))
		{
			WSACleanup();
			return nullptr;
		}

		const auto server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (INVALID_SOCKET == server)
		{
			WSACleanup();
			return nullptr;
		}

		const auto opt = true;
		if (SOCKET_ERROR == setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof opt))
		{
			WSACleanup();
			return nullptr;
		}

		SOCKADDR_IN server_addr;
		server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
		server_addr.sin_port = htons(TCP_PORT);
		server_addr.sin_family = AF_INET;

		if (SOCKET_ERROR == ::bind(server, (SOCKADDR*)&server_addr, sizeof server_addr))
		{
			WSACleanup();
			return nullptr;
		}

		if (SOCKET_ERROR == listen(server, SOMAXCONN))
		{
			WSACleanup();
			return nullptr;
		}

		return new tcp_server(server);
	}
}
