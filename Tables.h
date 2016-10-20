#ifndef TABLES_H
#define TABLES_H

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

class Tables {
public:
	Tables(std::string keyword, std::map<std::string, std::vector<std::string>> parsed);

	std::string tableName;

	std::string tablePath;

	std::string operation;

	bool Processor();

protected:
	std::map<std::string, std::vector<std::string>> parsed;

	bool CREATEProcessor();

	bool INSERTProcessor();

	bool SELECTProcessor();

	bool CREATEPreprocessor();

	bool INSERTPreprocessor();

	bool SELECTPreprocessor();

	bool isValidAttr(std::string tName, std::string attr);

	bool getAttrType(std::string tName, std::string attr, std::string& tp);

	bool getAttrIdx(std::string tName, std::string attr, int& idx);

	bool attrFilter(std::vector<std::string> attrs, std::vector<std::string>& attrVec, std::vector<std::vector<std::string>>& dataVec);

	bool dispRes(std::vector<std::string> attrVec, std::vector<std::vector<std::string>> dataVec);

	bool attrValTypeMatch(std::string tName, std::string attr, std::string val);

	bool isValidValVec(std::string tName, std::vector<std::string> valVec);
};

#endif