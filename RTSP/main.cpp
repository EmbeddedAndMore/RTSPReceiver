#include  <iostream>

#include "main.h"
#include "evpp/tcp_conn.h"
#include "evpp/buffer.h"
#include "evpp/tcp_server.h"

#ifdef _WIN32
#include "winmain-inl.h"
#endif

void OnMessage(const evpp::TCPConnPtr& conn,
	evpp::Buffer* msg) {
	std::string s = msg->NextAllString();
	std::cout << "Received a message "<< conn->remote_addr() <<"[" << s << "]"  << std::endl;
	conn->Send(s);

	if (s == "quit" || s == "exit") {
		conn->Close();
	}
}

void OnConnection(const evpp::TCPConnPtr& conn) {
	if (conn->IsConnected()) {
		std::cout << "Accept a new connection from " << conn->remote_addr();
	}
	else {
		std::cout << "Disconnected from " << conn->remote_addr();
	}
}

void main(int argv, char** argc)
{
	google::InitGoogleLogging(argc[0]);
	google::SetStderrLogging(google::NUM_SEVERITIES);

	std::string port = "3625";
	std::string addr = "127.0.0.1:" + port;
	evpp::EventLoop loop;
	evpp::TCPServer server(&loop, addr, "RTSPClinet", 0);
	server.SetMessageCallback(&OnMessage);
	server.SetConnectionCallback(&OnConnection);
	server.Init();
	server.Start();
	loop.Run();
}


