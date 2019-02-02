#pragma once
#include "RTSPMessage.h"
#include <string>
#include <vector>
class RTSPListener
{

	enum ReadingState {
		NewCommand,
		Headers,
		Data,
		End,
		InterleavedData,
		MoreInterleavedData,
	};
public:
	RTSPListener();
	~RTSPListener();

	RTSPMessage ReadOneMessage(std::vector<unsigned char> data);
	
};

