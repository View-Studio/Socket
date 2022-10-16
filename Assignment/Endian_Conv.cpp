#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	unsigned short HostPort = 0x1234;
	unsigned short NetPort = 0;
	unsigned long HostAddr = 0x12345678;
	unsigned long NetAddr = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	NetPort = htons(HostPort);
	NetAddr = htonl(HostAddr);

	cout << "Host ordered port : " << std::hex << HostPort << endl;
	cout << "Network ordered port : " << std::hex << NetPort << endl;
	cout << "Host ordered address : " << std::hex << HostAddr << endl;
	cout << "Network ordered address : " << std::hex << NetAddr << endl;

	NetPort = ntohs(NetPort);
	NetAddr = ntohl(NetAddr);

	cout << "Host ordered port : " << std::hex << NetPort << endl;
	cout << "Host ordered address : " << std::hex << NetAddr << endl;

	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}