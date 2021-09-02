#include "Interpreter.h"

int Interpreter::Start()
{
	Init();

	while (true)
	{
		if (Check0x1F() == ERR_EOF)
			break;
		CheckLength();
		CheckDescriptor();
		if (CheckMessage())
			data.isCorrupted = true;
		CheckCRC();
		Print();
		CleanPacket();
	}

	Deinit();

	return 0;
}

int Interpreter::Init()
{
	trash = new uint8_t[MAX_BUFFER_SIZE];
	curPacket = new uint8_t[MAX_BUFFER_SIZE];
	data.message = new struct s_message[MAX_MESSAGE_NUMBER];

	reader.Init();
	return 0;
}

int Interpreter::Check0x1F()
{
	if (int err = reader.FindNewPacket(trash))
	{
		return err;
	}
	return 0;
}

int Interpreter::CheckLength()
{
	reader.GetNBytes(2, curPacket + curPos);
	data.length = curPacket[curPos] * 0x100 + curPacket[1];
	//data.length = *((uint16_t *)curPacket);
	curPos += 2;
	return 0;
}

int Interpreter::CheckDescriptor()
{
	reader.GetNBytes(2, curPacket + curPos);
	//data.descriptor = *((uint16_t*)curPacket);
	data.descriptor = curPacket[curPos] * 0x100 + curPacket[curPos + 1];
	curPos += 2;
	realLength += 2;
	return 0;
}

int Interpreter::CheckMessage()
{
	while (realLength < data.length)
	{
		if (int err = CheckTag()) 
			return err;
		CheckMessageLength();
		CheckValue();
		messagePos++;
		//break;
	}
	return 0;
}

int Interpreter::CheckTag()
{
	reader.GetNBytes(1, curPacket + curPos);
	if (curPacket[curPos] >= 0x19)
	{
		// ???????????????????????????
		curPos--;
		reader.ChangePos(-1); 
		return ERR_HUGE_TAG;
	}
	data.message[messagePos].tag = curPacket[curPos];
	curPos++;
	realLength++;
	return 0;
}

int Interpreter::CheckMessageLength()
{
	reader.GetNBytes(1, curPacket + curPos);
	if (curPacket[curPos] <= 0x7F)
	{
		data.message[messagePos].length = curPacket[curPos];
		curPos++;
		realLength++;
	}
	else if (curPacket[curPos] == 0x80)
	{
		data.message[messagePos].arbitraryLength = true;
		curPos++;
		realLength++;
	}
	else if (curPacket[curPos] == 0x81)
	{
		curPos++;
		reader.GetNBytes(1, curPacket + curPos);
		data.message[messagePos].length = curPacket[curPos];
		curPos++;
		realLength += 2;
	}
	else if (curPacket[curPos] == 0x82)
	{
		curPos++;
		reader.GetNBytes(2, curPacket + curPos);
		//data.message[messagePos].length = *((uint16_t *)curPacket);
		data.message[messagePos].length = curPacket[curPos] * 0x100 + curPacket[curPos + 1];
		curPos += 2;
		realLength += 3;
	}
	else if (curPacket[curPos] == 0x83)
	{
		curPos++;
		reader.GetNBytes(2, curPacket + curPos);
		//data.message[messagePos].length = *((uint32_t*)curPacket);
		//data.message[messagePos].length >> 8;
		data.message[messagePos].length = curPacket[curPos] * 0x10000 + curPacket[curPos + 1] * 0x100 + curPacket[curPos + 2];
		curPos += 3;
		realLength += 4;
	}

	return 0;
}

int Interpreter::CheckValue()
{
	int length;
	if (data.message[messagePos].arbitraryLength)
		length = reader.FindNext((uint16_t)0x0000) + 2; // read with 0x0000
	else
		length = data.message[messagePos].length;
	data.message[messagePos].value = new uint8_t[length];

	reader.GetNBytes(length, curPacket + curPos);
	memcpy(data.message[messagePos].value,
		curPacket + curPos, 
		(data.message[messagePos].arbitraryLength ? length - 2 : length)); // if value length is arbitrary, copy value to data.message[messagePos].value without 0x0000 at the end
		
	curPos += length;
	realLength += length;

	return 0;
}

int Interpreter::CheckCRC()
{
	// read crc from dump
	uint8_t crc_buf[2];
	reader.GetNBytes(2, crc_buf);
	////uint16_t crc = *(uint16_t*)crc_buf;
	//uint16_t crc = crc_buf[0] * 0x100 + crc_buf[1];

	//// count real crc of the packet
	//uint16_t i, tmp, real_crc = 0xffff;
	//for (i = 0; i < data.length; i++)
	//{
	//	tmp = (real_crc >> 8) ^ (0x00ff & curPacket[i]);
	//	real_crc = (real_crc << 8) ^ crc16_ccitt_table[tmp];
	//}
	//
	//// compare crc and real crc
	//if (crc == real_crc)
	//	return 0;
	//else return real_crc;
	return 0;
}

int Interpreter::Print()
{
	printer.Print(data, messagePos);
	return 0;
}

int Interpreter::CleanPacket()
{
	CleanData();
	curPos = 0;
	trashPos = 0;
	messagePos = 0;
	realLength = 0;

	return 0;
}

int Interpreter::CleanData()
{
	data.descriptor = 0;
	data.length = 0;
	data.isCorrupted = false;
	for (int i = 0; i < messagePos; i++)
	{
		data.message[i].arbitraryLength = false;
		data.message[i].length = 0;
		data.message[i].tag = 0;
		delete[] data.message[i].value;
	}
	messagePos = 0;

	return 0;
}

int Interpreter::Deinit()
{
	delete[] trash;
	delete[] curPacket;
	delete[] data.message;

	reader.Deinit();
	return 0;
}
