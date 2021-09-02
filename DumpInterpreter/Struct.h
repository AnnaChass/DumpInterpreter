#pragma once
#include <iostream>

#define PARAMETER_NUMBER 26

struct s_message
{
	uint8_t tag;
	unsigned int length;
	bool arbitraryLength = false;
	uint8_t* value;
};
struct s_data
{
	bool isCorrupted = false;
	uint16_t length;
	uint16_t descriptor;
	struct s_message* message;
};

enum e_format
{
	ASCII,
	dec_ASCII,
	binary,
	utf8,
	empty,
	no_limit
};
struct s_parameter
{
	std::string descriptor;
	enum e_format format;
	int maxLength;
};
const struct s_parameter parameter[PARAMETER_NUMBER] =
{
	{"", empty, 0},
	{"Message name", ASCII, 3},
	{"", empty, 0},
	{"Operation number", dec_ASCII, 8},
	{"Amount in minor currency unit", dec_ASCII, 12},
	{"Keepalive interval in seconds", dec_ASCII, 3},
	{"Operation timeout in seconds", dec_ASCII, 3},
	{"Event name", ASCII, 5},
	{"Event number", dec_ASCII, 8},
	{"Product id", dec_ASCII, 6},
	{"QR-Code data", ASCII, no_limit},
	{"TCP/IP destination", binary, no_limit},
	{"Outgoing byte counter", dec_ASCII, 12},
	{"Simple data block", binary, no_limit},
	{"Confirmable data block", binary, no_limit},
	{"Product name", ASCII, no_limit},
	{"POS management data", ASCII, no_limit},
	{"Local time", ASCII, no_limit},
	{"System information", ASCII, no_limit},
	{"Banking receipt", utf8, no_limit},
	{"Display time in milliseconds", dec_ASCII, 8},
	{"Image name", ASCII, 64},
	{"Image flags", binary, no_limit},
	{"Display text", ASCII, no_limit},
	{"Coordinates and color", ASCII, 64},
	{"Host Id", dec_ASCII, 3}
};
