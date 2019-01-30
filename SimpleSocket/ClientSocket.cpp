#include "ClientSocket.h"
#include <Ws2tcpip.h>
#include <chrono>

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
	_serverIpAddress(serverAddress),
	_result(NULL),
	_ptr(NULL)
{

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	
}

ClientSocket::~ClientSocket()
{

}

void ClientSocket::receiveThread_function()
{
	while (true)
	{
		try
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			int iResult = recv(_connectSocket, &_incomeBuffer[0], 1000, 0);
			if (iResult > 0)
				printf("Bytes received: %s\n", std::string(_incomeBuffer.begin(), _incomeBuffer.begin() + iResult).c_str());
		}
		catch (const std::exception&)
		{

		}
	}
}


ClientSocket::SocketErrorTypes ClientSocket::Connect()
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
	{
		return SocketErrorTypes::WSAStartupfailedError;
	}

	if ((getaddrinfo(_serverIpAddress.c_str(), _port.c_str(), &hints, &_result)) != 0)
	{
		return SocketErrorTypes::InvalidAddressError;
	} 
	_ptr = _result;
	_connectSocket = socket(_ptr->ai_family, _ptr->ai_socktype,
		_ptr->ai_protocol);

	if (_connectSocket == INVALID_SOCKET) {
		return SocketErrorTypes::InvalidSocketError;
	}

	if( connect(_connectSocket, _ptr->ai_addr, (int)_ptr->ai_addrlen) == SOCKET_ERROR)
	{
		return SocketErrorTypes::ConnectionFailed;
	}

	freeaddrinfo(_result);


	_receiveThread = std::thread(&ClientSocket::receiveThread_function, this);
	_receiveThread.join();
}
