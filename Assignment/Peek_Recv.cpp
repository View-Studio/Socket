#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int ListenSock, SendSock;
	sockaddr_in ListenAddr, SendAddr;
	socklen_t SendAddrSize;
	char RecvBuffer[MAX_BUF_SIZE];
	int RecvLen;

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	ListenSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&ListenAddr, 0, sizeof(ListenAddr));
	ListenAddr.sin_family = AF_INET;
	ListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ListenAddr.sin_port = htons(atoi(argv[1]));

	if (bind(ListenSock, (sockaddr*)&ListenAddr, sizeof(ListenAddr)) == -1)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ListenSock, 5) == -1)
	{
		ErrorHandling("listen Error");
	}

	SendAddrSize = sizeof(SendAddr);
	SendSock = accept(ListenSock, (sockaddr*)&SendAddr, &SendAddrSize);
	if (SendSock == -1)
	{
		ErrorHandling("accept Error");
	}

	while (true)
	{
		RecvLen = recv(SendSock, RecvBuffer, MAX_BUF_SIZE, MSG_PEEK | MSG_DONTWAIT);
		cout << "[Len] : " << RecvLen << " [Str] : " << RecvBuffer << 1 << endl;
		if (RecvLen > 0)
		{
			break;
		}
	}

	RecvLen = recv(SendSock, RecvBuffer, MAX_BUF_SIZE, 0);
	cout << "[Len] : " << RecvLen << " [Str] : " << RecvBuffer << 2 << endl;

	close(SendSock);
	close(ListenSock);
	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}
