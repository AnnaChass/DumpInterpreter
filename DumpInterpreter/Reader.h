#pragma once
#include <string>
#include <cstdio>
#include <io.h>
#include "Errors.h"

#define FILENAME "vtk_device_data_flow_dump"
#define BUFFER_SIZE 1024

class Reader
{
private:
	FILE* file;
	uint8_t data[BUFFER_SIZE] = { 0 };
	int dataPointer = 0;
	int dataSize = 0;
	bool isEof = false;

	int ReadNewData();

public:
	int Init();
	int GetNBytes(int n, uint8_t* buffer);
	int FindNewPacket(uint8_t* buffer, int bufferPos);
	int FindNext(uint16_t value); 
	int GetTrashLength();
	int ChangePos(int n);
	int Deinit();
};

