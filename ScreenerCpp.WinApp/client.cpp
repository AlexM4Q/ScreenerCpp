#include "stdafx.h"
#include <thread>
#include "client.hpp"
#include "screen_util.hpp"
#include "image_destructor.hpp"

using namespace std;

namespace screener::winapp::server
{
	using namespace utils;
	using namespace destructors;

	client::client(const SOCKET tcp, const SOCKET udp, const SOCKADDR_IN address)
	{
		tcp_ = tcp;
		udp_ = udp;
		address_ = address;
		process_ = new thread([this] { run(); });
	}

	client::~client()
	{
		closesocket(tcp_);
		delete process_;
	}

	void client::run() const
	{
		while (true)
		{
			const auto image = screen_util::get_window_screen();
			image_destructor::destruct(*image, [this](const vector<char> bytes)
			{
				sendto(udp_, bytes.data(), bytes.size(), 0, (SOCKADDR*)&address_, sizeof address_);
			});

			delete image;

			this_thread::sleep_for(50ms);
		}
	}

	client* client::create(const SOCKET tcp, const SOCKADDR_IN address)
	{
		const auto udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (INVALID_SOCKET == udp)
		{
			WSACleanup();
			return nullptr;
		}

		return new client(tcp, udp, address);
	}
}
