#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
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
	SOCKET MySock;
	SOCKADDR_IN ServAddr;
	char SendBuffer[BUF_SiZE], RecvBuffer[BUF_SiZE];
	int SendBufferLength = 0, RecvBufferLength = 0;
	int TotalRecvLen = 0, RecvLenTmp = 0;
	ofstream OutputFile;

	if (argc != 3)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
	{
		ErrorHandling("WSAStartup Error");
	}

	MySock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySock == INVALID_SOCKET)
	{
		ErrorHandling("socket Error");
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
	ServAddr.sin_port = htons(atoi(argv[2]));

	if (connect(MySock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect Error");
	}

	cout << "다운받을 이미지 파일의 번호를 골라주세요." << endl;
	cout << "(1) Japan_Street, (2) Animation, (3) Wise_Saying" << endl;
	cout << "> ";
	cin >> &SendBuffer[1];

	switch (atoi(&SendBuffer[1]))
	{
	case FileKind::Japan:
		OutputFile.open("C:/Users/msi/Desktop/Socket/PracticeMaterial/Japan_Copy.jpg", ios::out | ios::binary);
		break;
	case FileKind::Ainmation:
		OutputFile.open("C:/Users/msi/Desktop/Socket/PracticeMaterial/Ainmation_Copy.jpg", ios::out | ios::binary);
		break;
	case FileKind::Wise_Saying:
		OutputFile.open("C:/Users/msi/Desktop/Socket/PracticeMaterial/Wise_Saying_Copy.jpg", ios::out | ios::binary);
		break;
	default:
		break;
	}

	SendBufferLength = (int)(strlen(&SendBuffer[1]));
	SendBuffer[0] = (char)SendBufferLength;

	if (send(MySock, SendBuffer, sizeof(SendBuffer), 0) == SOCKET_ERROR)
	{
		ErrorHandling("send Error");
	}
	
	while (true)
	{
		RecvLenTmp = recv(MySock, RecvBuffer, BUF_SiZE - 1, 0);
		OutputFile.write(RecvBuffer, RecvLenTmp);

		cout << RecvLenTmp << endl;
		if (RecvLenTmp == SOCKET_ERROR)
		{
			ErrorHandling("recv Error 2");
		}
		else if (RecvLenTmp == EOF)
		{
			break;
		}
	}

	OutputFile.close();
	closesocket(MySock);
	WSACleanup();


	return 0;

}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}