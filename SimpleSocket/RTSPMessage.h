#pragma once

#include <vector>
#include <string>
#include <regex>
#include <chrono>
#include "Utils.h"

class RTSPMessage
{

private:

	

	static std::regex _rtspVersionTest;

	std::chrono::system_clock::time_point Creation;

public:

	std::vector<unsigned char> Data;

	RTSPMessage();
	~RTSPMessage();

	static RTSPMessage GetRTSPMessage(std::string aRequestLine);
};

