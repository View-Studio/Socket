#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

const int TTL = 64, MAX_BUF_SIZE = 30;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int SendSock;
	struct sockaddr_in MultiAddr;
	int TimeLive = TTL;
	char SendBuffer[MAX_BUF_SIZE];
	ifstream ReadStream;

	if (argc != 3)
	{
		ErrorHandling("<GroupIP> <PORT>");
	}

	SendSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&MultiAddr, 0, sizeof(MultiAddr));
	MultiAddr.sin_family = AF_INET;
	MultiAddr.sin_addr.s_addr = inet_addr(argv[1]);
	MultiAddr.sin_port = htons(atoi(argv[2]));

	setsockopt(SendSock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&TimeLive, sizeof(TimeLive));

	ReadStream.open("/home/rudals/πŸ≈¡»≠∏È/Socket/Assignment/Chapter14/NEWS/News.txt", ios::in);

	while (!(ReadStream.eof()))
	{
		ReadStream.read(SendBuffer, sizeof(SendBuffer));

		sendto(SendSock, SendBuffer, sizeof(SendBuffer), 0, (sockaddr*)&MultiAddr, sizeof(MultiAddr));

		sleep(2);
	}

	ReadStream.close();
	close(SendSock);

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}