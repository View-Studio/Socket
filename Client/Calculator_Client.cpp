#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET MySock;
	SOCKADDR_IN ServAddr;
	string Num = "";
	string NumLen = "";
	string Operator = "";
	string Result = "";

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
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

	cout << "Operand Count : ";
	cin >> NumLen;
	if (send(MySock, &NumLen[0], (int)NumLen.size(), 0) == SOCKET_ERROR)
	{
		ErrorHandling("send1 Error");
	}

	for (int i = 0; i < stoi(NumLen); ++i)
	{
		cout << "Operand " << (i + 1) << " : ";
		cin >> Num;
		if (send(MySock, &Num[0], (int)Num.size(), 0) == SOCKET_ERROR)
		{
			ErrorHandling("send2 Error");
		}
	}

	cout << "Operator : ";
	cin >> Operator;
	if (send(MySock, &Operator[0], (int)Operator.size(), 0) == SOCKET_ERROR)
	{
		ErrorHandling("send3 Error");
	}

	while (true)
	{
		recv(MySock, &Result[0], BUF_SIZE, 0);
		if (sizeof(Result) > 0)
		{
			break;
		}
	}

	cout << "Result Num : " << Result << endl;

	closesocket(MySock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}