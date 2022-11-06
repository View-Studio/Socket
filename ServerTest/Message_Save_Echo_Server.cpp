#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

const int M_BUF_SIZE = 1024;

void ErrorHandling(const char* message);
void SIGCHLD_Handler(int sig);

int main(int argc, char* argv[])
{
	int ServSock, ClntSock;
	sockaddr_in ServAddr, ClntAddr;
	struct sigaction SigAct;
	pid_t pid;
	int ParentSendPipeFds[2], ChildSendPipeFds[2];
	char SendBuffer[M_BUF_SIZE], RecvBuffer[M_BUF_SIZE];
	int RecvLen = 0;
	socklen_t ClntAddrSize = sizeof(ClntAddr);

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if ((ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(atoi(argv[1]));

	if (bind(ServSock, (sockaddr*)&ServAddr, sizeof(ServAddr)) == -1)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == -1)
	{
		ErrorHandling("listen Error");
	}

	SigAct.sa_handler = SIGCHLD_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGCHLD, &SigAct, 0);

	pipe(ParentSendPipeFds);
	pipe(ChildSendPipeFds);

	pid = fork();
	if (pid == 0) // ���� ���� ���μ���
	{
		close(ServSock);
		int OutputFileFd = open("/home/rudals/����ȭ��/Socket/Assignment/Chapter11/EchoClientMessage.txt", O_RDWR | O_TRUNC);

		for (int i = 0; i < 10; ++i)
		{
			RecvLen = read(ParentSendPipeFds[0], RecvBuffer, M_BUF_SIZE); // read ��ȯ���� ���� ����Ʈ ���̰�, �ִ� ���� 3��° �μ��� �� ����ŭ �̴�.
			// ��, ����° �μ� ���� 100�ε� Ŭ���̾�Ʈ�� 1024����Ʈ�� �����͸� ���´ٸ� 100�� 10�� ��ȯ�ϰ�, 24�� 1�� ��ȯ�Ѵ�.
			
			write(OutputFileFd, RecvBuffer, strlen(RecvBuffer)); // ���� ������ ����° �μ��� read ��ȯ���� �ִ� ���� �����ϱ� ������, read ��ȯ�� ��ſ� ���� ���ڿ� strlen �� �ֱ�.
		}
		close(OutputFileFd);
		return 0;
	} 
	else
	{
		while (true)
		{
			if ((ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &ClntAddrSize)) == -1)
			{
				continue;
			}
			else
			{
				cout << "New Client Connected..." << endl;
			}

			pid = fork();
			if (pid == -1)
			{
				close(ClntSock);
				continue;
			}
			else if (pid == 0)
			{
				close(ServSock);

				while ((RecvLen = read(ClntSock, RecvBuffer, M_BUF_SIZE)) != -1)
				{
					if (RecvLen == 0)
					{
						break;
					}
					else
					{
						cout << RecvBuffer << endl;

						write(ClntSock, RecvBuffer, sizeof(RecvBuffer));
						write(ParentSendPipeFds[1], RecvBuffer, sizeof(RecvBuffer));
					}
				}
				close(ClntSock);
				return 0;
			}
			else
			{
				close(ClntSock);
				continue;
			}
		}
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}

void SIGCHLD_Handler(int sig)
{
	int status = 0;
	if (sig == SIGCHLD)
	{
		waitpid(-1, &status, WNOHANG);
		if (WIFEXITED(status))
		{
			cout << WEXITSTATUS(status) << endl;
		}
	}
}