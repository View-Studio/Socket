// Linux Code

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int Sock, ByteLen;
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

	const char* SendArr[5] = { "HAHA", " I'm a King!", " Wow!!", " HAHAHAHA", " Wow!!" };

	send(Sock, SendArr[0], strlen(SendArr[0]), 0);
	write(Sock, SendArr[1], strlen(SendArr[1]));
	sleep(1); 
	send(Sock, SendArr[2], strlen(SendArr[2]), MSG_OOB);
	write(Sock, SendArr[3], strlen(SendArr[3]));
	sleep(1);
	send(Sock, SendArr[4], strlen(SendArr[4]) + 1, MSG_OOB);

	close(Sock);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}