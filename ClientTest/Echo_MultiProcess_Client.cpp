// Linux Code

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int MySock;
	sockaddr_in ServAddr;
	const char* SendMessage[3] = { "Wow! Hi! I'm Client!", "My name is Code!", "I love you too!!" };
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];
	int MessageLength = 0, RecvLen = 0, RecvTmp = 0;

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	if (connect(MySock, (sockaddr*)&ServAddr, sizeof(ServAddr)) == -1)
	{
		ErrorHandling("connect Error");
	}

	while (true)
	{
		cout << "> ";
		cin >> SendBuffer;

		if (write(MySock, SendBuffer, sizeof(SendBuffer)) == -1)
		{
			ErrorHandling("send Error");
		}

		RecvLen = 0;
		while (RecvLen < sizeof(SendBuffer))
		{
			if ((RecvTmp = read(MySock, &RecvBuffer[RecvLen], MAX_BUF_SIZE - 1)) == -1)
			{
				ErrorHandling("recv Error 2");
			}
			RecvLen += RecvTmp;
		}
		cout << RecvBuffer << endl;
	}

	close(MySock);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}