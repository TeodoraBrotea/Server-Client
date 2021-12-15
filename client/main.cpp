//	must also add ws2_32.lib to linker settings in code::blocks
//	specify command line arguments or default values will be used

#include <ws2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

#define BUFF_SIZE 1023

using namespace std;

int main(int argc, char * argv[])
{
    cout << "This is the client program\n" << endl;

    int client_fd;
    int len, err;
    struct sockaddr_in address;
    struct hostent * host;

    int result;
    char buf[BUFF_SIZE+1];//char buf[1024];
    int num_chars;
    char ch = 'A';

	WORD wVersionRequested;
	WSADATA wsaData;

	// this piece of code is necessary, otherwise socket creation fails
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup( wVersionRequested, &wsaData );

    // create socket for client.
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd <0)
	{
		err = WSAGetLastError();
		cout << " socket() failed, error = " << err << endl;

		if (err == WSAENETDOWN)	cout << "NETDOWN" << endl;
		else if (err == WSANOTINITIALISED) cout << "NOINIT" << endl;
		else if (err == WSAEAFNOSUPPORT) cout << "NOSUPPORT" << endl;
		else if (err == WSAEINPROGRESS) cout << "IN PROGRESS" << endl;
		else if (err == WSAEMFILE) cout << "EM FILE" << endl;
		else if (err == WSAENOBUFS) cout << "NOBUFS" << endl;
		else if (err == WSAEPROTONOSUPPORT) cout << "PROTO" << endl;
		else if (err == WSAEPROTOTYPE) cout << "PROTOTYPE" << endl;
		else if (err == WSAESOCKTNOSUPPORT) cout << "SOCK NO SUPP" << endl;
		else cout << "strange error" <<endl;

		cin >> ch;
		exit(1);
	}

	if ((host=gethostbyname(argv[1]))==(struct hostent*)NULL) {
		cout << "gethostbyname error. will use the default = localhost" << endl;
		host = gethostbyname("localhost");
	}

    //	name the socket as agreed with server.
    address.sin_family = AF_INET;
	memcpy((char*)&address.sin_addr, (char*)host->h_addr, host->h_length);
	if (isnan(atoi(argv[2]))) argv[2] = "8080";
    address.sin_port = htons((u_short)atoi(argv[2]));
    len = sizeof(address);

    result = connect(client_fd, (struct sockaddr *)&address, len);

    if(result < 0)
    {
        cout << "connection error has occurred. will exit.\n";
        exit(0);
    }
    else
		cout << "successfull connection\n" << endl;

	//	read and write via client_fd
	send(client_fd, argv[3], strlen(argv[3]), 0);

	if ((num_chars = recv(client_fd, buf, sizeof(buf), 0)) < 0)
		cout << "read error" << endl;

	buf[num_chars]=0;
	cout << buf << endl;

	getchar();
    closesocket(client_fd);

    exit(0);
}
