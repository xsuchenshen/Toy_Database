#include "SQLParser.h"

bool SQLParser::ProcessSQL() {
	GetKeyword();
	GetEntry();

	return true;
}

bool SQLParser::SplitSTR() {

	return true;
}

bool SQLParser::GetKeyword() {
	std::istringstream s(sql);
	s >> keyword;
	Utils::trim(keyword);
	sql = sql.substr(keyword.size());
	Utils::trim(sql);
	
	return true;
}

bool SQLParser::GetEntry() {
	if (keyword == "CREATE") {
		CREATEProcessor();
	}
	else if (keyword == "INSERT") {
		INSERTProcessor();
	}
	else if (keyword == "SELECT") {
		SELECTProcessor();
	}
	else {
		keyword = "NULL";
	}

	return true;
}

bool SQLParser::CREATEProcessor() {
	//std::istringstream ss(sql);
	//std::string s;
	//ss >> s;
	//sql = sql.substr(s.size());
	Utils::trim(sql);
	std::string tName;
	
	std::vector<std::string> temp;
	std::vector<std::string> token;
	std::vector<std::string> subToken;
	int i;
	//if (s == "TABLE") {
		tName = sql.substr(0, sql.find("("));
		Utils::trim(tName);
		temp.push_back(tName);
		keyValue.insert({ "TABLE", temp });

		if (Utils::matchInnerPair(sql, "(", ")", sql)) {
			Utils::explode(sql, ',', token);
			temp.clear();
			for (i = 0; i < token.size(); i++) {
				subToken.clear();
				Utils::trim(token[i]);
				Utils::explode(token[i], ' ', subToken);
				Utils::trim(subToken[0]);
				Utils::trim(subToken[subToken.size() - 1]);
				temp.push_back(subToken[0]);
				temp.push_back(subToken[subToken.size() - 1]);
			}
			keyValue.insert({ "ATTRIBUTE", temp });
		}
	//}

	return true;
}

//bool SQLParser::INSERTProcessor() {
//	std::istringstream ss(sql);
//	std::string s;
//	ss >> s;
//	sql = sql.substr(s.size());
//	Utils::trim(sql);
//
//	std::vector<std::string> temp;
//	std::vector<std::string> token;
//	std::vector<std::string> subToken;
//	std::string lValue;
//	std::string rValue;
//	std::string temp2;
//	std::string temp3;
//	std::string tName;
//	int i;
//	if (s == "INTO") {
//		std::size_t found = sql.find("VALUES");
//		if (found != std::string::npos) {
//			// left of VALUES
//			Utils::getLeftString(sql, "VALUES", lValue);
//			if (Utils::matchInnerPair(lValue, "(", ")", temp2)) {
//				tName = lValue.substr(0, lValue.find("("));
//				Utils::trim(tName);
//				temp.push_back(tName);
//				keyValue.insert({ "TABLE", temp });
//
//				temp.clear();
//				Utils::explode(temp2, ',', token);
//				for (i = 0; i < token.size(); i++) {
//					Utils::trim(token[i]);
//					temp.push_back(token[i]);
//				}
//				keyValue.insert({ "ATTRIBUTE", temp });
//			}
//			else {
//				Utils::trim(lValue);
//				temp.push_back(lValue);
//				keyValue.insert({ "TABLE", temp });
//			}
//
//			// right of VALUES
//			Utils::getRightString(sql, "VALUES", rValue);
//			temp2.clear();
//			if (Utils::matchInnerPair(rValue, "(", ")", temp2)) {
//				temp.clear();
//				token.clear();
//				Utils::explode(temp2, ',', token);
//				for (i = 0; i < token.size(); i++) {
//					Utils::trim(token[i]);
//					temp.push_back(token[i]);
//				}
//				keyValue.insert({ "VALUES", temp });
//			}
//		}
//	}
//
//	return true;
//}

bool SQLParser::INSERTProcessor() {
	std::istringstream ss(sql);
	std::string s;
	ss >> s;
	sql = sql.substr(s.size());
	Utils::trim(sql);

	std::vector<std::string> temp;
	std::vector<std::string> token;
	std::vector<std::string> subToken;
	std::string lValue;
	std::string rValue;
	std::string temp2;
	std::string temp3;
	std::string tName;
	int i;
	if (s == "INTO") {
		//std::size_t found = sql.find("VALUES");
		//if (found != std::string::npos) {
			// left of VALUES
			//Utils::getLeftString(sql, "VALUES", lValue);
			if (Utils::matchInnerPair(sql, "(", ")", temp2)) {
				tName = sql.substr(0, sql.find("("));
				Utils::trim(tName);
				temp.push_back(tName);
				keyValue.insert({ "TABLE", temp });

				temp.clear();
				token.clear();
				Utils::explode(temp2, ',', token);
				for (i = 0; i < token.size(); i++) {
					Utils::trim(token[i]);
					temp.push_back(token[i]);
				}
				keyValue.insert({ "VALUES", temp });
			}
		}
		//}
	

	return true;
}

bool SQLParser::SELECTProcessor() {
	std::vector<std::string> temp;
	std::vector<std::string> token;
	std::vector<std::string> subToken;
	std::string lValue;
	std::string rValue;
	std::string temp2;
	std::string temp3;
	int i;
	
	Utils::getRightString(sql, "FROM", rValue);
	std::size_t found = sql.find("FROM");
	if (found != std::string::npos) {
		// left of FROM
		Utils::getLeftString(sql, "FROM", lValue);
		Utils::explode(lValue, ',', token);
		for (i = 0; i < token.size(); i++) {
			Utils::trim(token[i]);
			temp.push_back(token[i]);
		}
		keyValue.insert({ "ATTRIBUTE", temp });

		// right of FROM
		Utils::getRightString(sql, "FROM", rValue);

		// left of WHERE
		found = rValue.find("WHERE");
		if (found != std::string::npos) {
			Utils::getLeftString(rValue, "WHERE", lValue);
			token.clear();
			temp.clear();
			Utils::explode(lValue, ',', token);
			for (i = 0; i < token.size(); i++) {
				Utils::trim(token[i]);
				temp.push_back(token[i]);
			}
			keyValue.insert({ "TABLE", temp });
			keyValue.insert({ "WHERE", temp });

			// right of WHERE
			Utils::getRightString(rValue, "WHERE", rValue);
			found = rValue.find("=");
			if (found != std::string::npos) {
				token.clear();
				temp.clear();
				Utils::explode(rValue, '=', token);
				for (i = 0; i < token.size(); i++) {
					Utils::trim(token[i]);
					temp.push_back(token[i]);
				}
				keyValue.insert({ "=", temp });
			}
		}
		else {
			Utils::trim(rValue);
			temp.clear();
			temp.push_back(rValue);
			keyValue.insert({ "TABLE", temp });
		}
	}

	return true;
}