#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

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

	const char* SendStr = "I'm a King!";

	send(Sock, SendStr, strlen(SendStr) + 1, 0);

	close(Sock);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}