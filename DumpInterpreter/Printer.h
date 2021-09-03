#pragma once
#include <iomanip>
#include "Struct.h"


class Printer
{
private:
	bool IsCorrectTag(uint8_t tag);

public:
	Printer() {};
	int Print(const s_data &data, const unsigned int n);
};

