#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

enum FileKind
{
	Japan = 1,
	Ainmation,
	Wise_Saying
};

const int BUF_SiZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock, ClntSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	char SendBuffer[BUF_SiZE], RecvBuffer[BUF_SiZE];
	int SizeofClntAddr = 0;
	int SendMessageSize = 0;
	ifstream InputFile;

	/*if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	ServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(9999);
	//ServAddr.sin_port = htons(atoi(argv[1]));

	if (bind(ServSock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	SizeofClntAddr = sizeof(ClntAddr);
	ClntSock = accept(ServSock, (SOCKADDR*)&ClntAddr, &SizeofClntAddr);
	if (ClntSock == INVALID_SOCKET)
	{
		ErrorHandling("accept Error");
	}

	if (recv(ClntSock, RecvBuffer, BUF_SiZE - 1, 0) == SOCKET_ERROR)
	{
		ErrorHandling("recv Error");
	}

	switch (atoi(&RecvBuffer[1]))
	{
	case FileKind::Japan:
		InputFile.open("../PracticeMaterial/Japan.jpg", ios::in | ios::binary);
		break;
	case FileKind::Ainmation:
		InputFile.open("../PracticeMaterial/Animation.jpg", ios::in | ios::binary);
		break;
	case FileKind::Wise_Saying:
		InputFile.open("../PracticeMaterial/Wise_Saying.jpg", ios::in | ios::binary);
		break;
	default:
		break;
	}

	while (true)
	{
		InputFile.read(SendBuffer, sizeof(SendBuffer));

		send(ClntSock, SendBuffer, sizeof(SendBuffer), 0);

		if (InputFile.eof())
		{
			Sleep(2000);
			break;
		}
	}
	
	InputFile.close();
	closesocket(ClntSock);
	closesocket(ServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}