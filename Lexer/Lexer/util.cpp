#include <fstream>
#include <string>

std::string ReadFileToBuffer(const std::string& path)
{
	std::ifstream ifs(path);
	std::string buffer((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return buffer;
}