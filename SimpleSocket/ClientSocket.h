#include <winsock2.h>
#include <vector>
#include <string>
#pragma once

class ClientSocket
{
	struct sockaddr_in _address;
	struct sockaddr_in _serverAddress;
	
	SOCKET _connectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	int _sock = 0;
	std::string _port;
	std::string _serverIpAddress;
	std::vector<char> _incomeBuffer;
	WSADATA _wsaData;
public:
	ClientSocket();
	ClientSocket(int bufferSize, std::string port, std::string serverAddress);
	~ClientSocket();

	enum SocketErrorTypes {
		SocketCreationError,
		InvalidAddressError,
		ConnectionFailed,
		InvalidSocketError,
		WSAStartupfailedError,
		OK
	};


	SocketErrorTypes Connect();

};

