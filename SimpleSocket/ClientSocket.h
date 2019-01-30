#include <winsock2.h>
#include <vector>
#include <string>
#include <thread>
#pragma once

class ClientSocket
{
private:
	struct sockaddr_in _address;
	struct sockaddr_in _serverAddress;
	
	SOCKET _connectSocket = INVALID_SOCKET;
	struct addrinfo *_result, *_ptr , hints;

	int _sock = 0;
	std::string _port;
	std::string _serverIpAddress;
	std::vector<char> _incomeBuffer;
	WSADATA _wsaData;

	std::thread _receiveThread;

	void receiveThread_function();
	
public:
	ClientSocket();
	ClientSocket(int bufferSize, std::string port, std::string serverAddress);
	~ClientSocket();

	enum class SocketErrorTypes {
		SocketCreationError,
		InvalidAddressError,
		ConnectionFailed,
		InvalidSocketError,
		WSAStartupfailedError,
		OK
	};


	SocketErrorTypes Connect();

};

