#pragma once
#include <string>
typedef struct Buf
{
	std::string data;
	size_t pos;
} Buf;

std::string ReadFileToBuffer(const std::string& path);