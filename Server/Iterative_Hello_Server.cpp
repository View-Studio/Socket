#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock, ClntSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	int MessageLength = 0, szClntAddr = 0;
	const char* Message = "Hello World!! And Iterative Server!!";
	char buffer[BUF_SIZE];

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup Error");
	}

	ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(atoi(argv[1]));
	//ServAddr.sin_port = htons(9999);

	if (bind(ServSock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	szClntAddr = sizeof(ClntAddr);
	MessageLength = (int)strlen(Message);
	buffer[0] = (char)MessageLength;
	// buffer + 1 부터 Message 변수의 데이터 복사본을 저장하려고 하기 때문에 sizeof(buffer)에 -1을 해줘야 한다.
	// 그러지 않으면 Run-Time Check Failure #2 - Stack around the variable '변수이름' was corrupted 런타임 에러가 발생한다.
	strcpy_s((buffer + 1), sizeof(buffer)-1, Message);

	for (int i = 0; i < 5; ++i)
	{
		ClntSock = accept(ServSock, (SOCKADDR*)&ClntAddr, &szClntAddr);
		if (ClntSock == INVALID_SOCKET)
		{
			ErrorHandling("accept Error");
		}
		else
		{
			cout << "Client : " << i << endl;
		}

		send(ClntSock, buffer, BUF_SIZE, 0);
		
		closesocket(ClntSock);
	}

	closesocket(ServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}

