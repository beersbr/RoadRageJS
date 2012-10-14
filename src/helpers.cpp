#include "helpers.hpp"

std::string chomp(std::string src)
{
	std::string bald_string = src;

	// remove whitespace from the front of the string
	int i = 0;
	char ch = bald_string[i];
	while(ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
	{
		bald_string.erase(i, 1);
		ch = bald_string[i];
	}

	// remove whitespace from the end of the string
	i = bald_string.length()-1;
	ch = bald_string[i];
	while(ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
	{
		bald_string.erase(i, 1);
		ch = bald_string[--i];
	}

	return bald_string;
}

std::vector<std::string > split(std::string src, char token)
{
	std::vector<std::string> strings;

	int substr_size = 1;
	for(int i = 0; i < (int)src.length(); i++)
	{
		if(src[i] == token)
		{
			std::string temp = src.substr((i-substr_size+1), substr_size);
			strings.push_back(temp);
			substr_size = 1;
		}
		substr_size += 1;
	}

	return strings;
}

int print_as_binary(const char* string, const int sz)
{
	for(int i = 0; i < sz; i+=2)
	{
		for(int j = 0; j < 16; j++)
		{
			std::cout << (int)((string[i] >> j) & 1);
		}
		std::cout << std::endl;
	}
}