// Linux Code

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);
void InputRoutine(int sock, char Buffer[]);
void OutputRoutine(int sock, char Buffer[]);

int main(int argc, char* argv[])
{
	int MySock;
	sockaddr_in ServAddr;
	pid_t pid;
	char SendBuffer[MAX_BUF_SIZE], RecvBuffer[MAX_BUF_SIZE];

	if (argc != 3)
	{
		ErrorHandling("main function parmeter Error");
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

	if (connect(MySock, (sockaddr*)&ServAddr, sizeof(ServAddr)))
	{
		ErrorHandling("connect Error");
	}

	pid = fork();
	if (pid == 0) // child process
	{
		InputRoutine(MySock, RecvBuffer); // 입력 루틴
	}
	else // parent process
	{
		OutputRoutine(MySock, SendBuffer); // 출력 루틴
	}

	close(MySock);
	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}

void InputRoutine(int sock, char RecvBuffer[])
{
	int TempLen = 0;
	while (true)
	{
		TempLen = read(sock, RecvBuffer, MAX_BUF_SIZE - 1);
		if (TempLen == 0)
		{
			shutdown(sock, SHUT_RD);
			return;
		}
		cout << RecvBuffer << endl;
	}	
}

void OutputRoutine(int sock, char SendBuffer[])
{
	unsigned char SendBufferLength = 0;
	while (true)
	{
		cin >> SendBuffer;

		if (strlen(SendBuffer) == 1)
		{
			if (SendBuffer[0] == 'Q' || SendBuffer[0] == 'q')
			{
				shutdown(sock, SHUT_WR);
				return;
			}
		}

		write(sock, SendBuffer, strlen(SendBuffer) + 1);
	}
}