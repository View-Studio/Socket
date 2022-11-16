// Linux Code

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int Sock;
	sockaddr_in RecvAddr;

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Sock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&RecvAddr, 0, sizeof(RecvAddr));
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_addr.s_addr = inet_addr(argv[1]);
	RecvAddr.sin_port = htons(atoi(argv[2]));

	if (connect(Sock, (sockaddr*)&RecvAddr, sizeof(RecvAddr)) == -1)
	{
		ErrorHandling("connect Error");
	}

	write(Sock, "HAHA", strlen("HAHA") + 1);
	send(Sock, " I'm a King!", strlen(" I'm a King!") + 1, 0);
	send(Sock, " Wow!!", strlen(" Wow!!") + 1, MSG_OOB);
	write(Sock, " HAHAHAHA", strlen(" HAHAHAHA") + 1);
	send(Sock, " Wow!!", strlen(" Wow!!") + 1, MSG_OOB);

	close(Sock);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}