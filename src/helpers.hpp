#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <iostream>
#include <vector>
#include <string>

std::string 			 chomp(std::string src);
std::vector<std::string> split(std::string src, char token);

int print_as_binary(const char* string, const int sz);

#endif