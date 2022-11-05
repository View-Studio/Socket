#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void SIGINT_Handler(int sig);

int main(int argc, char* argv[])
{
	struct sigaction SigAct;
	SigAct.sa_handler = SIGINT_Handler;
	sigemptyset(&SigAct.sa_mask);
	SigAct.sa_flags = 0;
	sigaction(SIGINT, &SigAct, 0);

	while (true)
	{
		cout << "Playing..." << endl;
		sleep(1000);
	}

	return 0;
}

void SIGINT_Handler(int sig)
{
	char Key = ' ';
	cout << "���α׷��� �����Ͻðڽ��ϱ�?" << endl;
	cout << "(Y / N) ";
	cin >> Key;

	if (Key == 'y' || Key == 'Y')
	{
		cout << "���α׷��� �����մϴ�." << endl;
		exit(1);
	}
	else if (Key == 'n' || Key == 'N')
	{
		return;
	}
	else
	{
		cout << "�ùٸ� ���� �Է����ּ���." << endl;
		return;
	}
}