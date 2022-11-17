#include <iostream>
#include <string.h>
#include <sys/uio.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
	struct iovec vec[2];
	char Buf_1[MAX_BUF_SIZE] = "HAHA, ";
	char Buf_2[MAX_BUF_SIZE] = "I'm a King.";
	int SendLen = 0;

	vec[0].iov_base = Buf_1;
	vec[0].iov_len = strlen(Buf_1);
	vec[1].iov_base = Buf_2;
	vec[1].iov_len = strlen(Buf_2) + 1;

	SendLen = writev(1, vec, 2);
	cout << endl << "Write bytes: " << SendLen << endl;

	return 0;
}