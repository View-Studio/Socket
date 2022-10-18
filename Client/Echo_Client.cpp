#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
using std::cin;

const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET MySock;
	SOCKADDR_IN ServAddr;
	int StrLen = 0;
	std::string Message = "";

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	if (connect(MySock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect Error");
	}
	else
	{
		cout << "Connected..." << endl;
	}

	while (true)
	{
		cout << "My> ";
		cin >> Message;

		if (Message == "q" || Message == "Q")
		{
			break;
		}

		send(MySock, Message.c_str(), (int)Message.size(), 0);
		// recv 함수가 cin 입력당 딱 한번 호출되기 때문에 이 코드는 데이터를 다 받지 못 할 가능성이 존재함.
		StrLen = recv(MySock, (char*)Message.c_str(), BUF_SIZE - 1, 0);
		Message[StrLen] = '\0';
		cout << "Message from server : " << Message << endl;
	}

	closesocket(MySock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}