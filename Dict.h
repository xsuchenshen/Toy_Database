#ifndef DICT_H
#define DICT_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Utils.h"

using namespace std;

class Dict {
public:
	Dict();

	std::string dictPath;

	std::map<std::string, std::map<std::string, std::vector<std::string>>> keyValue;

	bool loadDict();

	bool addTable(std::map<std::string, std::vector<std::string>> parsed);
};

#endif