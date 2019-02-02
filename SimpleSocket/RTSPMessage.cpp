#include "RTSPMessage.h"



RTSPMessage::RTSPMessage():
	Data(1024,0),
	Creation(std::chrono::system_clock::now())
{
	RTSPMessage::_rtspVersionTest = "^RTSP / \d\.\d";
}


RTSPMessage::~RTSPMessage()
{
}



RTSPMessage RTSPMessage::GetRTSPMessage(std::string aRequestLine)
{
	if (aRequestLine.empty())
		return RTSPMessage();

	std::vector<std::string> requestParts = Utils::split(aRequestLine, ' ');
	RTSPMessage returnValue;

	if (requestParts.size() == 3)
	{
		// A request is : Method SP Request-URI SP RTSP-Version
		// A response is : RTSP-Version SP Status-Code SP Reason-Phrase
		// RTSP-Version = "RTSP" "/" 1*DIGIT "." 1*DIGIT

		if (std::regex_match(requestParts[2], _rtspVersionTest)) {
			//returnValue = RtspRequest.GetRtspRequest(requestParts);
		}
		else if (std::regex_match(requestParts[0], _rtspVersionTest)) {
			//returnValue = new RtspResponse();
		}

		return returnValue;

	}
}