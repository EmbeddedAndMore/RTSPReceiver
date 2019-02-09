#include "ClientSocket.h"

void main(int argv, char** argc)
{
	ClientSocket cSocket(1000,"6742","127.0.0.1");
	ClientSocket::SocketErrorTypes response = cSocket.Connect();
	
	std::getchar();
	
}


