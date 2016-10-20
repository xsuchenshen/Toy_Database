#include "Tables.h"
#include "Dict.cpp"

Tables::Tables(std::string keyword, std::map<std::string, std::vector<std::string>> kv) {
	tableName = kv.count("TABLE") > 0 ? kv["TABLE"][0] : "";
	tablePath = tableName + "_FILE";
	parsed = kv;
	operation = keyword;
}

bool Tables::Processor() {
	if (operation == "CREATE") {
		if (CREATEPreprocessor()) {
			if (CREATEProcessor()) {
				std::cout << "Done" << endl;
			}
		}
	}
	else if (operation == "INSERT") {
		if (INSERTPreprocessor()) {
			if (INSERTProcessor()) {
				std::cout << "Done" << endl;
			}
		}
	}
	else if (operation == "SELECT") {
		if (SELECTPreprocessor()) {
			if (SELECTProcessor()) {
				std::cout << "Done" << endl;
			}
		}
	}
	else {
		std::cout << "error: Unknow operation, check spelling" << std::endl;
		return false;
	}

	return true;
}

bool Tables::CREATEProcessor() {
	Dict dict;
	dict.addTable(parsed);

	return true;
}

bool Tables::CREATEPreprocessor() {
	std::string tName;
	std::string attrType;
	int i;

	if (parsed.count("TABLE") > 0 && parsed["TABLE"][0] != "") {
		std::ifstream infile(tablePath);
		if (!infile) {
			if (parsed.count("ATTRIBUTE") > 0 && parsed["ATTRIBUTE"].size() % 2 == 0) {
				for (i = 1; i < parsed["ATTRIBUTE"].size(); i += 2) {
					if (!(parsed["ATTRIBUTE"][i] == "INT" || parsed["ATTRIBUTE"][i] == "STRING")) {
						std::cout << "error: Unknow attribute type" << std::endl;
						return false;
					}
				}
			}
			else {
				std::cout << "error: Missing attribute or type" << std::endl;
				return false;
			}
		}
		else {
			std::cout << "error: Table already exists" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "error: Missing table name" << std::endl;
		return false;
	}

	return true;
}

bool Tables::INSERTProcessor() {
	std::string fName;
	fName = tablePath;
	std::ifstream infile(fName);
	std::string line;
	std::vector<std::string> attrs;
	std::getline(infile, line);
	int i;
	std::string newRec("");
	std::string val;

	Utils::explode(line, '|', attrs);
	if (parsed.count("ATTRIBUTE") > 0) {
		std::map<std::string, std::string> attrToVal;
		for (i = 0; i < parsed["ATTRIBUTE"].size(); i++) {
			if (Utils::matchInnerPair(parsed["VALUES"][i], "'", "'", val)) {
				attrToVal.insert({ parsed["ATTRIBUTE"][i], val });
			}
			else {
				attrToVal.insert({ parsed["ATTRIBUTE"][i], parsed["VALUES"][i] });
			}
			
		}
		for (i = 0; i < attrs.size(); i++) {
			if (attrToVal.count(attrs[i]) > 0) {
				newRec += attrToVal[attrs[i]] + "|";
			}
			else {
				newRec += "|";
			}
		}
	}
	else {
		for (i = 0; i < attrs.size(); i++) {
			if (Utils::matchInnerPair(parsed["VALUES"][i], "'", "'", val)) {
				newRec += val + "|";
			}
			else {
				newRec += parsed["VALUES"][i] + "|";
			}
			
		}
	}
	std::ofstream tb(fName, ios::app);
	tb << newRec << "\n";

	return true;
}

bool Tables::INSERTPreprocessor() {
	int i;

	if (parsed.size() > 0) {
		std::ifstream infile(tablePath);
		if (infile) {
			if (parsed.count("ATTRIBUTE") > 0 && parsed.count("VALUES") > 0) {
				if (parsed["ATTRIBUTE"].size() == parsed["VALUES"].size()) {
					for (i = 0; i < parsed["ATTRIBUTE"].size(); i++) {
						if (!isValidAttr(tableName, parsed["ATTRIBUTE"][i])) {
							std::cout << "error: Unknowm attribute name" << std::endl;
							return false;
						}
						if (!attrValTypeMatch(tableName, parsed["ATTRIBUTE"][i], parsed["VALUES"][i])) {
							std::cout << "error: Value type does not match attribute" << endl;
							return false;
						}
					}
				}
				else {
					std::cout << "error: Unequal amount of attributes and values" << std::endl;
					return false;
				}
			}
			else if (parsed.count("ATTRIBUTE") == 0 && parsed.count("VALUES") > 0) {
				if (!isValidValVec(tableName, parsed["VALUES"])) {
					std::cout << "error: Values format does not conform to the database" << endl;
					return false;
				}
			}
			else {
				std::cout << "error: Missing values" << std::endl;
				return false;
			}
		}
		else {
			std::cout << "error: Table does not exist" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "error: Keyword missing/incorrect" << std::endl;
		return false;
	}

	return true;
}

bool Tables::SELECTProcessor() {
	std::vector<std::string> attrVec;
	std::vector<std::vector<std::string>> dataVec;

	if (parsed.count("WHERE") > 0) {
		if (parsed["TABLE"].size() > 1) {
			// field1 = field2
			std::string lVal(parsed["="][0]);
			std::string rVal(parsed["="][1]);
			std::string tb1(parsed["TABLE"][0]);
			std::string tb2(parsed["TABLE"][1]);
			std::ifstream infile1(tb1 + "_FILE");
			std::ifstream infile2(tb2 + "_FILE");
			std::string line1;
			std::string line2;
			int idx1, idx2;
			int i;
			std::vector<std::string> token1;
			std::vector<std::string> token2;
			attrVec.clear();
			dataVec.clear();
			std::vector<std::string> temp;

			getAttrIdx(tb1, lVal, idx1);
			getAttrIdx(tb2, rVal, idx2);
			std::getline(infile1, line1);
			std::getline(infile2, line2);
			Utils::explode(line1, '|', token1);
			Utils::explode(line2, '|', token2);
			for (i = 0; i < token1.size(); i++) {
				attrVec.push_back(token1[i]);
			}
			for (i = 0; i < token2.size(); i++) {
				attrVec.push_back(token2[i]);
			}

			while (std::getline(infile1, line1)) {
				token1.clear();
				Utils::explode(line1, '|', token1);
				while (std::getline(infile2, line2)) {
					token2.clear();
					Utils::explode(line2, '|', token2);
					if (token1[idx1] == token2[idx2]) {
						temp.clear();
						for (i = 0; i < token1.size(); i++) {
							temp.push_back(token1[i]);
						}
						for (i = 0; i < token2.size(); i++) {
							temp.push_back(token2[i]);
						}
						dataVec.push_back(temp);
					}
				}
				infile2.clear();
				infile2.seekg(0, ios::beg);
				getline(infile2, line2);
			}
		}
		else {
			// field1 = constant
			std::string lVal(parsed["="][0]);
			std::string rVal(parsed["="][1]);
			int idx;
			int i;
			std::ifstream infile(tablePath);
			std::string line;
			std::vector<std::string> token;
			attrVec.clear();
			dataVec.clear();
			std::vector<std::string> temp;

			getAttrIdx(tableName, lVal, idx);
			std::getline(infile, line);
			Utils::explode(line, '|', token);
			for (i = 0; i < token.size(); i++) {
				attrVec.push_back(token[i]);
			}
			if (!Utils::isIntNumber(rVal)) {
				Utils::matchInnerPair(rVal, "'", "'", rVal);
			}
			while (std::getline(infile, line)) {
				token.clear();
				Utils::explode(line, '|', token);
				if (token[idx] == rVal) {
					temp.clear();
					for (i = 0; i < token.size(); i++) {
						temp.push_back(token[i]);
					}
					dataVec.push_back(temp);
				}
			}
		}
	}
	else {
		std::ifstream infile(tablePath);
		std::string line;
		std::vector<std::string> token;
		attrVec.clear();
		dataVec.clear();
		std::vector<std::string> temp;
		int i;

		std::getline(infile, line);
		Utils::explode(line, '|', token);
		for (i = 0; i < token.size(); i++) {
			attrVec.push_back(token[i]);
		}
		while (std::getline(infile, line)) {
			token.clear();
			temp.clear();
			Utils::explode(line, '|', token);
			for (i = 0; i < token.size(); i++) {
				temp.push_back(token[i]);
			}
			dataVec.push_back(temp);
		}
	}

	if (dataVec.size() == 0) {
		std::cout << "empty set" << std::endl;
		return false;
	}

	// attr filter
	if (parsed["ATTRIBUTE"][0] != "*") {
		attrFilter(parsed["ATTRIBUTE"], attrVec, dataVec);
	}

	// show results
	dispRes(attrVec, dataVec);

	return true;
}

bool Tables::SELECTPreprocessor() {
	int i;

	if (parsed.count("ATTRIBUTE") > 0 && parsed.count("TABLE") > 0 && parsed.count("WHERE") == 0) {
		if (parsed["TABLE"].size() == 1) {
			std::ifstream infile(parsed["TABLE"][0] + "_FILE");
			if (infile) {
				for (i = 0; i < parsed["ATTRIBUTE"].size(); i++) {
					if (!isValidAttr(parsed["TABLE"][0], parsed["ATTRIBUTE"][i]) && parsed["ATTRIBUTE"][i] != "*") {
						std::cout << "error: Unknowm attribute name" << std::endl;
						return false;
					}
				}
			}
			else {
				std::cout << "error: Table does not exist" << std::endl;
				return false;
			}
		}
		else {
			std::cout << "error: Missing WHERE clause, multiple tables" << std::endl;
			return false;
		}
	}
	else if (parsed.count("ATTRIBUTE") > 0 && parsed.count("TABLE") > 0 && parsed.count("WHERE") > 0) {
		if (parsed.count("=") > 0) {
			if (parsed["TABLE"].size() == 2) {
				std::string lVal(parsed["="][0]);
				std::string rVal(parsed["="][1]);
				std::string tb1(parsed["TABLE"][0]);
				std::string tb2(parsed["TABLE"][1]);
				std::ifstream infile1(tb1 + "_FILE");
				std::ifstream infile2(tb2 + "_FILE");

				if (infile1 && infile2) {
					if (isValidAttr(tb1, lVal) && isValidAttr(tb2, rVal)) {
						std::string tp1("1");
						std::string tp2("2");
						getAttrType(tb1, lVal, tp1);
						getAttrType(tb2, rVal, tp2);
						if (tp1 != tp2) {
							std::cout << "error: Attribute type does not match" << std::endl;
							return false;
						}
					}
					else {
						std::cout << "error: Values format does not conform to the database" << std::endl;
						return false;
					}
				}
				else {
					std::cout << "error: Table does not exist" << std::endl;
					return false;
				}
			}
			else if (parsed["TABLE"].size() > 2) {
				std::cout << "error: Unexpected numbers of tables, should be <= 2" << std::endl;
				return false;
			}
			else {
				std::string lVal(parsed["="][0]);
				std::string rVal(parsed["="][1]);
				std::ifstream infile(tablePath);

				if (infile) {
					for (i = 0; i < parsed["ATTRIBUTE"].size(); i++) {
						if (!isValidAttr(parsed["TABLE"][0], parsed["ATTRIBUTE"][i]) && parsed["ATTRIBUTE"][i] != "*") {
							std::cout << "error: Unknowm attribute name" << std::endl;
							return false;
						}
					}
					if (isValidAttr(tableName, lVal)) {
						if (!attrValTypeMatch(tableName, lVal, rVal)) {
							std::cout << "error: Value type does not match attribute" << std::endl;
							return false;
						}
					}
					else {
						std::cout << "error: Unknowm attribute name" << std::endl;
						return false;
					}
				}
				else {
					std::cout << "error: Table does not exist" << std::endl;
					return false;
				}
			}
		}
		else {
			std::cout << "error: Symbol '=' not found" << std::endl;
			return false;
		}
	}
	else {
		std::cout << "error: Missing keyword" << std::endl;
		return false;
	}


	return true;
}

bool Tables::isValidAttr(std::string tName, std::string attr) {
	Dict dict;
	dict.loadDict();
	if (dict.keyValue.count(tName) == 0) {
		return false;
	}
	else {
		int i;
		for (i = 0; i < dict.keyValue[tName]["ATTRIBUTE"].size(); i += 2) {
			if (attr == dict.keyValue[tName]["ATTRIBUTE"][i]) {
				return true;
			}
		}
	}

	return false;
}

bool Tables::getAttrType(std::string tName, std::string attr, std::string& tp) {
	tp.clear();
	Dict dict;
	dict.loadDict();
	if (dict.keyValue.count(tName) == 0) {
		return false;
	}
	else {
		int i;
		for (i = 0; i < dict.keyValue[tName]["ATTRIBUTE"].size(); i += 2) {
			if (attr == dict.keyValue[tName]["ATTRIBUTE"][i]) {
				tp = dict.keyValue[tName]["ATTRIBUTE"][i + 1];
				return true;
			}
		}
	}

	return false;
}

bool Tables::getAttrIdx(std::string tName, std::string attr, int& idx) {
	Dict dict;
	dict.loadDict();
	if (dict.keyValue.count(tName) == 0) {
		return false;
	}
	else {
		std::string line;
		std::vector<std::string> attrs;
		std::ifstream infile(tName + "_FILE");
		std::getline(infile, line);

		Utils::explode(line, '|', attrs);
		int i;
		for (i = 0; i < attrs.size(); i++) {
			if (attr == attrs[i]) {
				idx = i;
				return true;
			}
		}
	}

	return false;
}

bool Tables::attrFilter(std::vector<std::string> attrs, std::vector<std::string>& attrVec, std::vector<std::vector<std::string>>& dataVec) {
	std::vector<std::string> attrVecTemp(attrVec);
	std::vector<std::vector<std::string>> dataVecTemp(dataVec);
	attrVec.clear();
	dataVec.clear();
	dataVec.resize(dataVecTemp.size());
	std::vector<std::string> temp;

	int i, j, k;
	for (i = 0; i < attrVecTemp.size(); i++) {
		for (j = 0; j < attrs.size(); j++) {
			if (attrs[j] == attrVecTemp[i]) {
				attrVec.push_back(attrVecTemp[i]);
				for (k = 0; k < dataVecTemp.size(); k++) {
					dataVec[k].push_back(dataVecTemp[k][i]);
				}
				break;
			}
		}
	}

	return true;
}

bool Tables::dispRes(std::vector<std::string> attrVec, std::vector<std::vector<std::string>> dataVec) {
	int i, j;
	std::string res("");
	std::string dat("");

	std::cout << "Attributes:" << endl;
	for (i = 0; i < attrVec.size(); i++) {
		res += attrVec[i] + "|";
	}
	res += "\n";

	std::cout << res << std::endl;

	std::cout << "Data:" << endl;
	for (i = 0; i < dataVec.size(); i++) {
		for (j = 0; j < dataVec[i].size(); j++) {
			dat += dataVec[i][j] + "|";
		}
		dat += "\n";
	}

	std::cout << dat << std::endl;

	return true;
}

bool Tables::attrValTypeMatch(std::string tName, std::string attr, std::string val) {
	Dict dict;
	dict.loadDict();
	std::string attrType("");
	std::string temp;
	if (dict.keyValue.count(tName) == 0) {
		return false;
	}
	else {

		int i;
		for (i = 0; i < dict.keyValue[tName]["ATTRIBUTE"].size(); i += 2) {
			if (attr == dict.keyValue[tName]["ATTRIBUTE"][i]) {
				attrType = dict.keyValue[tName]["ATTRIBUTE"][i + 1];
				break;
			}
		}
		if (attrType == "I") {
			if (!Utils::isIntNumber(val)) {
				return false;
			}
		}
		else if (attrType == "S") {
			if (!Utils::matchInnerPair(val, "'", "'", temp)) {
				return false;
			}
		}
		else {
			return false;
		}
	}

	return true;
}

bool Tables::isValidValVec(std::string tName, std::vector<std::string> valVec) {
	Dict dict;
	dict.loadDict();
	if (dict.keyValue.count(tName) == 0) {
		return false;
	}
	else {
		std::string line;
		std::vector<std::string> attrs;
		std::ifstream infile(tName + "_FILE");
		std::getline(infile, line);

		Utils::explode(line, '|', attrs);
		int i;
		if (valVec.size() == attrs.size()) {
			for (i = 0; i < attrs.size(); i++) {
				if (!attrValTypeMatch(tName, attrs[i], valVec[i])) {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	return true;
}
