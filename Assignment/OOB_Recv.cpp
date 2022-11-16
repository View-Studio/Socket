// Linux Code

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;
int ListenSock, SendSock;

void ErrorHandling(const char* message);
void SIGURG_Handler(int sig);

int main(int argc, char* argv[])
{
	sockaddr_in ListenAddr, SendAddr;
	socklen_t SendAddrSize;
	struct sigaction SigAct;
	char RecvBuffer[MAX_BUF_SIZE];
	int RecvLen = 0;

	SigAct.sa_handler = SIGURG_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGURG, &SigAct, 0);

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
	else
	{
		fcntl(SendSock, F_SETOWN, getpid());
	}
	
	while (true)
	{
		RecvLen = recv(SendSock, RecvBuffer, sizeof(RecvBuffer), 0);
		if (RecvLen == 0)
		{
			break;
		}
		else if (RecvLen == -1)
			continue;
		cout << RecvBuffer << endl;
	}

	close(SendSock);
	close(ListenSock);
	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}

void SIGURG_Handler(int sig)
{
	if (sig == SIGURG)
	{
		char RecvBuffer[MAX_BUF_SIZE];
		int RecvLen = 0;
		RecvLen = recv(SendSock, RecvBuffer, sizeof(RecvBuffer), MSG_OOB);
		cout << "[SIGURG] : " << RecvBuffer << endl;
	}
}