#include "stdafx.h"
#include "udp_server.hpp"
#include <cstdio>
#include <windows.h>
#include <winsock.h>
#include "image_destructor.hpp"

#pragma comment(lib,"ws2_32.lib")

#define UDP_PORT 4554

udp_server::udp_server()
{
	char buff[1024];

	// Открытие и закрытие сокета
	auto my_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_sock == INVALID_SOCKET)
	{
		printf("Socket error: %d\n", WSAGetLastError());
		WSACleanup();
	}

	// Связывание сокета 
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(u_short(UDP_PORT));

	if (::bind(my_sock, (sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error: %d\n", WSAGetLastError());
		closesocket(my_sock);
	}


	// Обработка присланных пактов
	while (1)
	{
		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);
		int bsize = recvfrom(my_sock, buff, 1024, 0, (sockaddr*)&client_addr, &client_addr_size);
		if (bsize == SOCKET_ERROR)
		{
			auto wsa_get_last_error = WSAGetLastError();
			printf("recvfrom() error: %d\n", wsa_get_last_error);
			continue;
		}


		// Определяем IP-адрес клиента и прочие атрибуты
		HOSTENT* hostent = gethostbyaddr((char *)&client_addr.sin_addr, 4, AF_INET);
		printf("accepted connection from %s, port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		// Добавление завершающего нуля
		buff[bsize] = 0;

		// Вывод на экран 
		printf("C=>S:%s\n", &buff[0]);

		// посылка датаграммы клиенту
		sendto(my_sock, &buff[0], bsize, 0, (sockaddr *)&client_addr, sizeof(client_addr));
	}
}


udp_server::~udp_server()
{
}
