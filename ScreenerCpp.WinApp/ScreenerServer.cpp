#include "stdafx.h"
#include "ScreenerServer.h"

#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winsock.h>
#include "screen_util.h"
#include "byte_util.h"

#pragma comment(lib,"ws2_32.lib")

#define PORT 4555
#define BACKLOG 10
#define TRUE 1

ScreenerServer::ScreenerServer()
{
	int i_server_sock;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	char buf[1024];
	int nBytes = 1024, recvbytes;

	int sin_size;

	WSADATA WSAData;

	if (WSAStartup(MAKEWORD(1, 1), &WSAData))
	{
		printf("initializationing error!\n");
		WSACleanup();
		exit(0);
	}
	printf("initializationing ...\n");

	if ((i_server_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Fail to create socket！\n");
		WSACleanup();
		exit(0);
	}
	printf("create socket!\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((u_short)PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

	bind(i_server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	// if (bind(i_server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	// {
		// printf("bind fail！\n");
		// WSACleanup();
		// exit(0);
	// }
	printf("bind success!\n");

	if (listen(i_server_sock, BACKLOG) == -1)
	{
		printf("listen fail！\n");
		WSACleanup();
		exit(0);
	}
	printf("listen success\n");

	for (;;)
	{
		sin_size = sizeof(struct sockaddr_in);
		const int i_client_sock = accept(i_server_sock, (struct sockaddr *)&client_addr, &sin_size);

		if (i_client_sock == -1)
		{
			printf("accept fail\n");
			exit(0);
		}
		printf("accept success\n");

		printf("server connect to%s\n", inet_ntoa(client_addr.sin_addr));


		////Receive from Client
		//if ((recvbytes = recv(i_client_sock, buf, nBytes, 0)) == SOCKET_ERROR)
		//{
		//	printf("\nErrorcode=%d,   Recv   from   client\n", WSAGetLastError());
		//	exit(0);
		//}


		//Send Data to client


		//send(i_client_sock, buf, recvbytes, 0);

		while (true)
		{
			auto buffer = screener::winapp::util::screen_util::get_desktop_screen_shot();
			screener::winapp::util::byte_util::insert_length(buffer);
			send(i_client_sock, buffer.data(), buffer.size(), 0);
		}

		// closesocket(i_client_sock);
	}
}

ScreenerServer::~ScreenerServer()
{
}
