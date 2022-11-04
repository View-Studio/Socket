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

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

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
	ServAddr.sin_port = htons(atoi(argv[1]));
	//ServAddr.sin_port = htons(9999);

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
		InputFile.open("C:/Users/msi/Desktop/Socket/PracticeMaterial/Japan.jpg", ios::in | ios::binary);
		break;
	case FileKind::Ainmation:
		InputFile.open("C:/Users/msi/Desktop/Socket/PracticeMaterial/Animation.jpg", ios::in | ios::binary);
		break;
	case FileKind::Wise_Saying:
		InputFile.open("C:/Users/msi/Desktop/Socket/PracticeMaterial/Wise_Saying.jpg", ios::in | ios::binary);
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
			InputFile.close();
			// 출력 스트림만 종료하는 Half-close.
			shutdown(ClntSock, SD_SEND);
			// 상대 소켓이 closesocket 또는 shutdown을 호출했을 경우 EOF가 내 소켓의 입력버퍼에 전송됨.
			// 단, 소켓에서의 EOF는 상수 0 이다.
			while (recv(ClntSock, &RecvBuffer[0], BUF_SiZE, 0) != 0)
			break;
		}
	}
	
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