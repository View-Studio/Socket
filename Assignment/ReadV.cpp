#include <iostream>
#include <sys/uio.h>

using namespace std;

const int MAX_BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
	struct iovec vec[2];
	char Buf_1[MAX_BUF_SIZE];
	char Buf_2[MAX_BUF_SIZE];
	int RecvLen = 0;

	vec[0].iov_base = Buf_1;
	vec[0].iov_len = 7;
	vec[1].iov_base = Buf_2;
	vec[1].iov_len = MAX_BUF_SIZE;

	RecvLen = readv(0, vec, 2);
	cout << "Receive Length: " << RecvLen << endl;
	cout << "BUF_1 Length: " << Buf_1 << endl;
	cout << "BUF_2 Length: " << Buf_2 << endl;

	return 0;
}