#include "RTSPListener.h"



RTSPListener::RTSPListener()
{
}


RTSPListener::~RTSPListener()
{
}

RTSPMessage RTSPListener::ReadOneMessage(std::vector<unsigned char> data)
{
	ReadingState currentReadingState = ReadingState::NewCommand;
	// current decode message , create a fake new to permit compile.
	RTSPMessage currentMessage;

	int size = 0;
	int byteReaden = 0;
	std::vector<char> buffer(256);
	std::string oneLine;
	while (currentReadingState != ReadingState::End)
	{
		// if the system is not reading binary data.
		if (currentReadingState != ReadingState::Data && currentReadingState != ReadingState::MoreInterleavedData)
		{
			int _cnt = 0;
			bool needMoreChar = true;
			// I do not know to make readline blocking
			while (needMoreChar)
			{
				unsigned char currentByte = data[_cnt++];
				switch (currentByte)
				{
					case -1:
						// the read is blocking, so if we got -1 it is because the client close;
						currentReadingState = ReadingState::End;
						needMoreChar = false;
						break;
					case '\n':
						oneLine = std::string(buffer.begin(), buffer.end());
						buffer.clear();
						needMoreChar = false;
						break;
					case '\r':
						// simply ignore this
						break;
					case '$': // if first caracter of packet is $ it is an interleaved data packet
						if (currentReadingState == ReadingState::NewCommand && buffer.size() == 0)
						{
							currentReadingState = ReadingState::InterleavedData;
							needMoreChar = false;
							break;
						}
					default:
						buffer.push_back((unsigned char)currentByte);
						break;
				}
			}
		}

		switch (currentReadingState)
		{
		case ReadingState::NewCommand:
			currentMessage = RTSPMessage::GetRTSPMessage(oneLine);
			currentReadingState = ReadingState::Headers;
			break;
		case ReadingState::Headers:
			std::string line = oneLine;
			if (line.empty())
			{
				currentReadingState = ReadingState::Data;
				currentMessage.InitialiseDataFromContentLength();
			}
			else
			{
				currentMessage.AddHeader(line);
			}
			break;
		case ReadingState::Data:
			if (currentMessage.Data.Length > 0)
			{
				// Read the remaning data
				int byteCount = commandStream.Read(currentMessage.Data, byteReaden,
					currentMessage.Data.size() - byteReaden);
				if (byteCount <= 0) {
					currentReadingState = ReadingState::End;
					break;
				}
				byteReaden += byteCount;
				//_logger.Debug(CultureInfo.InvariantCulture, "Readen {0} byte of data", byteReaden);
			}
			// if we haven't read all go there again else go to end. 
			if (byteReaden >= currentMessage.Data.size())
				currentReadingState = ReadingState::End;
			break;
		case ReadingState::InterleavedData:
			currentMessage = new RtspData();
			int channelByte = commandStream.ReadByte();
			if (channelByte == -1) {
				currentReadingState = ReadingState::End;
				break;
			}
			currentMessage.Channel = channelByte;

			int sizeByte1 = commandStream.ReadByte();
			if (sizeByte1 == -1) {
				currentReadingState = ReadingState::End;
				break;
			}
			int sizeByte2 = commandStream.ReadByte();
			if (sizeByte2 == -1) {
				currentReadingState = ReadingState::End;
				break;
			}
			size = (sizeByte1 << 8) + sizeByte2;
			currentMessage.Data = new unsigned char[size];
			currentReadingState = ReadingState::MoreInterleavedData;
			break;
		case ReadingState::MoreInterleavedData:
			// apparently non blocking
		{
			int byteCount = commandStream.Read(currentMessage.Data, byteReaden, size - byteReaden);
			if (byteCount <= 0) {
				currentReadingState = ReadingState::End;
				break;
			}
			byteReaden += byteCount;
			if (byteReaden < size)
				currentReadingState = ReadingState::MoreInterleavedData;
			else
				currentReadingState = ReadingState::End;
			break;
		}
		default:
			break;
		}

	}
}
