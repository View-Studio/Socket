#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 30;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int RecvSock, StrLen;
	struct sockaddr_in MyAddr;
	char RecvBuffer[MAX_BUF_SIZE];

	if (argc != 2)
	{
		ErrorHandling("<Broadcast IP> <PORT>");
	}

	RecvSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (RecvSock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&MyAddr, 0, sizeof(MyAddr));
	MyAddr.sin_family = AF_INET;
	MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	MyAddr.sin_port = htons(atoi(argv[1]));

	if (bind(RecvSock, (sockaddr*)&MyAddr, sizeof(MyAddr)) == -1)
	{
		ErrorHandling("bind Error");
	}

	while (true)
	{
		StrLen = recvfrom(RecvSock, RecvBuffer, MAX_BUF_SIZE - 1, 0, NULL, 0);
		if (StrLen < 0)
		{
			break;
		}
		RecvBuffer[StrLen] = '\0';
		cout << RecvBuffer << endl;
	}
	close(RecvSock);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}