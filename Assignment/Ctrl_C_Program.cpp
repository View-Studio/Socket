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
	cout << "프로그램의 종료하시겠습니까?" << endl;
	cout << "(Y / N) ";
	cin >> Key;

	if (Key == 'y' || Key == 'Y')
	{
		cout << "프로그램을 종료합니다." << endl;
		exit(1);
	}
	else if (Key == 'n' || Key == 'N')
	{
		return;
	}
	else
	{
		cout << "올바른 값을 입력해주세요." << endl;
		return;
	}
}