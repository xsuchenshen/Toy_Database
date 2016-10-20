#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include "Utils.h"

using namespace std;

class SQLParser {
public:
	std::string sql;

	std::string keyword;

	std::map<std::string, std::vector<std::string>> keyValue;

	bool ProcessSQL();

protected:
	bool SplitSTR();

	bool GetKeyword();

	bool GetEntry();

	bool CREATEProcessor();

	bool INSERTProcessor();

	bool SELECTProcessor();
};

#endif