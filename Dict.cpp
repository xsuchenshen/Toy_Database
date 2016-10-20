#include "Dict.h"

Dict::Dict() {
	dictPath = "DB_DICT";
}

bool Dict::loadDict() {
	std::ifstream infile(dictPath);
	std::string line;
	std::string tName;
	std::string tAttr;
	std::string attrType;
	std::string fName;
	std::map<std::string, std::vector<std::string>> kv;
	std::vector<std::string> temp1;
	std::vector<std::string> temp2;
	keyValue.clear();

	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		iss >> tName >> tAttr >> attrType >> fName;
		temp1.clear();
		temp2.clear();
		kv.clear();

		temp1.push_back(tAttr);
		temp1.push_back(attrType);
		temp2.push_back(fName);

		kv.insert({ "ATTRIBUTE", temp1 });
		kv.insert({ "FILE", temp2 });

		if (keyValue.count(tName) == 0) {
			keyValue.insert({ tName, kv });
		}
		else {
			keyValue[tName]["ATTRIBUTE"].push_back(tAttr);
			keyValue[tName]["ATTRIBUTE"].push_back(attrType);
		}
	}
	
	return true;
}

bool Dict::addTable(std::map<std::string, std::vector<std::string>> parsed) {
	std::string tName;
	std::string attr;
	std::string attrType;
	std::string fName;
	int i;

	tName = parsed["TABLE"][0];
	fName = tName + "_FILE";
	std::ofstream dict(dictPath, ios::app);
	std::ofstream tb(fName, ios::app);
	for (i = 0; i < parsed["ATTRIBUTE"].size(); i += 2) {
		attr = parsed["ATTRIBUTE"][i];
		if (parsed["ATTRIBUTE"][i + 1] == "INT") {
			attrType = "I";
		}
		else if (parsed["ATTRIBUTE"][i + 1] == "STRING") {
			attrType = "S";
		}
		dict << tName << " " << attr << " " << attrType << " " << fName << "\n";
		tb << attr << "|";
	}
	tb << "\n";

	return true;
}