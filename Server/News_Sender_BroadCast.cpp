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
	int SendSock;
	struct sockaddr_in BroadAddr;
	int SO_BRD = 1;
	char SendBuffer[MAX_BUF_SIZE];
	ifstream ReadStream;

	if (argc != 3)
	{
		ErrorHandling("<Broadcast IP> <PORT>");
	}

	SendSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSock == -1)
	{
		ErrorHandling("socket Error");
	}

	memset(&BroadAddr, 0, sizeof(BroadAddr));
	BroadAddr.sin_family = AF_INET;
	BroadAddr.sin_addr.s_addr = inet_addr(argv[1]);
	BroadAddr.sin_port = htons(atoi(argv[2]));

	setsockopt(SendSock, SOL_SOCKET, SO_BROADCAST, (void*)&SO_BRD, sizeof(SO_BRD));

	ReadStream.open("/home/rudals/πŸ≈¡»≠∏È/Socket/Assignment/Chapter14/NEWS/News.txt", ios::in);

	while (!(ReadStream.eof()))
	{
		ReadStream.read(SendBuffer, sizeof(SendBuffer));

		sendto(SendSock, SendBuffer, sizeof(SendBuffer), 0, (sockaddr*)&BroadAddr, sizeof(BroadAddr));

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