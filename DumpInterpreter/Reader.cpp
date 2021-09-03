#include "Reader.h"

int Reader::ReadNewData()
{
    int new_pointer;
    for (new_pointer = 0; dataPointer < dataSize; new_pointer++, dataPointer++)
    {
        data[new_pointer] = data[dataPointer];
    }
    //dataPointer = new_pointer;
    dataPointer = 0;

    /*dataSize = fread(data + new_pointer, 1, BUFFER_SIZE - new_pointer, file) + new_pointer;
    if (dataSize < BUFFER_SIZE)
        isEof = true;*/
    int read_bytes = fread(data + new_pointer, 1, BUFFER_SIZE - new_pointer, file);
    if (read_bytes < BUFFER_SIZE - new_pointer)
    {
        if (feof(file))
            isEof = true;
    }
    dataSize = read_bytes + new_pointer;

    return 0;
}

Reader::Reader()
{
    Init();
}

Reader::~Reader()
{
    Deinit();
}

int Reader::Init()
{
    fopen_s(&file, FILENAME, "rb");
    //dataSize = fread(data, 1, BUFFER_SIZE, file);

    int read_bytes = fread(data, 1, BUFFER_SIZE, file);
    if (read_bytes < BUFFER_SIZE)
    {
        if (feof(file))
            isEof = true;
    }
    dataSize = read_bytes;

    //if (size < BUFFER_SIZE)
    //    isEof = true;
    return 0;
}

int Reader::GetNBytes(int n, uint8_t* buffer)
{
    if (dataPointer + n > dataSize)
        ReadNewData();
    memcpy(buffer, data + dataPointer, n);
    dataPointer += n;
    return 0;
}

// find next occurrence of value in dump file
// return distance to next occurrence
int Reader::FindNext(uint16_t value)
{
    uint8_t val[2];
    val[1] = value;
    val[0] = value >> 8;
    while (!(data[dataPointer] == val[0] && data[dataPointer + 1] == val[1]) && dataPointer + 1 < dataSize)
        dataPointer++;
    return 0;
}

int Reader::GetTrashLength()
{
    return 0;
}

int Reader::ChangePos(int n)
{
    dataPointer += n;
    return 0;
}

// find next occurence of 0x1F
// fill in the buffer with skipped data
int Reader::FindNewPacket()
{
    while (data[dataPointer] != 0x1F && dataPointer < dataSize)
        dataPointer++;
    //if (data[dataPointer] == 0x1F)
    //    dataPointer++; // skip 0x1F
    if (dataPointer == dataSize)
    {
        if (isEof)
            return ERR_EOF;
    }
    return 0;
}

int Reader::Deinit()
{
    fclose(file);
    return 0;
}
