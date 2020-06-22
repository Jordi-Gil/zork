#ifndef __Globals__
#define __Globals__

#include <string>
#include <vector>

bool Same(const std::string& a, const std::string& b);
bool Same(const char* a, const std::string& b);
bool Same(const std::string& a, const char* b);
int Roll(int min, int max);
void Tokenize(const std::string& line, std::vector<std::string>& arguments);

#endif //__Globals__