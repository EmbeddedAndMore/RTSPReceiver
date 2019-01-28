#include "ClientSocket.h"
#include <Ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


//https://docs.microsoft.com/en-us/windows/desktop/winsock/getting-started-with-winsock

ClientSocket::ClientSocket()
{
	
}
ClientSocket::ClientSocket(int bufferSize, std::string port, std::string serverAddress ): 
	_incomeBuffer(bufferSize,0),
	_port(port),
	_serverIpAddress(serverAddress)
{

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

ClientSocket::~ClientSocket()
{
}



ClientSocket::SocketErrorTypes ClientSocket::Connect()
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
	{
		return SocketErrorTypes::WSAStartupfailedError;
	}

	if ((getaddrinfo(_serverIpAddress.c_str(), _port.c_str(), &hints, &result)) != 0)
	{
		return SocketErrorTypes::InvalidAddressError;
	} 
	ptr = result;
	_connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (_connectSocket == INVALID_SOCKET) {
		return SocketErrorTypes::InvalidSocketError;
	}

	if( connect(_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
	{
		return SocketErrorTypes::ConnectionFailed;
	}

	freeaddrinfo(result);
}
