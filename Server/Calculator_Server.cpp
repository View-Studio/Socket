#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int BUF_SIZE = 1024;

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServSock, ClntSock;
	SOCKADDR_IN ServAddr, ClntAddr;
	int RecvLen = 0;
	int szClntAddr = 0;
	int Result_I = 0;
	string Num = "";
	string NumLen = "";
	string Operator = "";
	string Result_S = "";
	vector<int> NumVec;

	if (argc != 2)
	{
		ErrorHandling("main function parameter Error");
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
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

	if (bind(ServSock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind Error");
	}

	if (listen(ServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen Error");
	}

	szClntAddr = sizeof(ClntAddr);

	for (int i = 0; i < 5; ++i)
	{
		ClntSock = accept(ServSock, (SOCKADDR*)&ClntAddr, &szClntAddr);

		if (ClntSock == INVALID_SOCKET)
		{
			ErrorHandling("accept Error");
		}
		else
		{
			cout << "Connected to Client" << (i + 1) << endl;
		}

		while (true)
		{
			recv(ClntSock, &NumLen[0], BUF_SIZE - 1, 0);
			if (sizeof(NumLen) > 0)
			{
				break;
			}
		}

		for (int i = 0; i < stoi(NumLen); ++i)
		{
			while (true)
			{
				recv(ClntSock, &Num[0], BUF_SIZE - 1, 0);
				if (sizeof(Num) > 0)
				{
					NumVec.push_back(stoi(Num));
					break;
				}
			}
		}

		while (true)
		{
			recv(ClntSock, &Operator[0], BUF_SIZE - 1, 0);
			if (sizeof(Operator) > 0)
			{
				break;
			}
		}
		
		Result_I = NumVec[0];

		for (int i = 1; i < NumVec.size(); ++i)
		{
			if (Operator[0] == '+')
			{
				Result_I = Result_I +  NumVec[i];
			}
			else if (Operator[0] == '*')
			{
				Result_I = Result_I * NumVec[i];
			}
			else if (Operator[0] == '-')
			{
				Result_I = Result_I - NumVec[i];
			}
			else
			{
				cout << "DB에 등록되어 있지 않은 연산자입니다." << endl;
			}
		}

		Result_S = to_string(Result_I);

		if (send(ClntSock, &Result_S[0], sizeof(Result_S), 0) == SOCKET_ERROR)
		{
			ErrorHandling("send Error");
		}

		NumVec.clear();
		closesocket(ClntSock);
	}

	closesocket(ServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	cout << message << endl;
	exit(1);
}