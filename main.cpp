#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <iostream>

#include "SQLParser.cpp"
#include "Tables.cpp"
#include "Utils.cpp"

using namespace std;

bool proc(std::string input) {
	SQLParser parser;
	parser.sql = input;
	parser.ProcessSQL();

	Tables tb(parser.keyword, parser.keyValue);
	tb.Processor();

	return true;
}

int main() {
	/*SQLParser parser;
	string s("CREATE TABLE tab(aa INT,bb STRING, cc STRING)");
	string s5("CREATE TABLE tab(aa INT)");
	string s2("INSERT INTO tab(aa,cc,bb)VALUES('dfqw','grwg','hty')");
	string s3("INSERT INTO tab VALUES('asa','bdb','c3c')");
	string s4("SELECT * FROM tab,tab2 WHERE cc=ee");
	string s6("SELECT aa,bb FROM tab WHERE bb='hty'");
	string s7("fewfew");

	parser.sql = s7;
	parser.ProcessSQL();

	Tables tb(parser.keyword, parser.keyValue);
	tb.Processor();*/



	std::string input;
	std::vector<SQLParser> ps;
	do {
		std::cout << "SQL > ";
		std::getline(std::cin, input, '\n');
		Utils::trim(input);
		proc(input);

	} while (input != "exit");


	

	return 1;
}