#include "globals.h"

// -------------------------------------------------
bool Same(const std::string& a, const std::string& b)
{
	return _stricmp(a.c_str(), b.c_str()) == 0;
}

bool Same(const char* a, const std::string& b)
{
	return _stricmp(a, b.c_str()) == 0;
}

bool Same(const std::string& a, const char* b)
{
	return _stricmp(a.c_str(), b) == 0;
}

int Roll(int min, int max)
{
	return (max > 0) ? min + (rand() % (max - min)) : 0;
}

void Tokenize(const std::string& line, std::vector<std::string>& arguments)
{
	const char* str = line.c_str();

	do
	{
		const char *begin = str;

		while (*str != ' ' && *str)
			str++;

		arguments.push_back(std::string(begin, str));
	} while (0 != *str++);
}

void split(const std::string &line, std::vector<std::string>& parameters,const std::string delimeter) {

	std::string aux = line;

	int pos = 0;
	std::string token;

	while ((pos = aux.find(delimeter)) != std::string::npos) {
		token = aux.substr(0, pos);
		aux.erase(0, pos + delimeter.length());
		parameters.push_back(token);
	}

	parameters.push_back(aux);

}