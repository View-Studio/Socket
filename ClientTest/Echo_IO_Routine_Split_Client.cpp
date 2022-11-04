// Linux Code

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message, pid_t pid = 0);
void WriteRoutine(int SockFd, char* SendBuf);
void ReadRoutine(int SockFd, char* RecvBuf);

int main(int argc, char* argv[])
{
	pid_t pid;
	int MySock = 0;
	sockaddr_in ServAddr;
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	if (connect(MySock, (sockaddr*)&ServAddr, sizeof(ServAddr)) == -1)
	{
		ErrorHandling("Connect Error");
	}

	pid = fork();

	if (pid == 0)
	{
		WriteRoutine(MySock, SendBuffer);
	} 
	else
	{
		ReadRoutine(MySock, RecvBuffer);
	}

	close(MySock);

	return 0;
}

void ErrorHandling(const char* message, pid_t pid)
{
	if (pid != 0)
	{
		cout << "Error Process ID : " << pid << ", Error Message : " << message << endl;
		exit(1);
	}
	else
	{
		cout << "Error Message : " << message << endl;
		exit(1);
	}
}

void WriteRoutine(int SockFd, char* SendBuf)
{
	while (true)
	{
		cout << "> ";
		cin >> SendBuf;

		if (SendBuf == "Q" || SendBuf == "q")
		{
			shutdown(SockFd, SHUT_WR);
			return;
		}

		if (write(SockFd, SendBuf, sizeof(SendBuf)) == -1)
		{
			ErrorHandling("write Error");
		}
	}
}

void ReadRoutine(int SockFd, char* RecvBuf)
{
	int ReadByte = 0;
	while (true)
	{
		if ((ReadByte = read(SockFd, RecvBuf, MAX_BUF_SIZE - 1)) == -1)
		{
			ErrorHandling("read Error");
		}
		if (ReadByte == 0)
		{
			return;
		}
		cout << RecvBuf << endl;
	}
}