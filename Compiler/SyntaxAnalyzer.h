#pragma once

#include <stack>
#include "LexicalAnalyzer.h"

class SyntaxAnalyzer
{
	const int LR_TABLE_ROWS = 108;
	const int LR_TABLE_COLUMNS = 46;
	const std::string LR_TABLE_FILENAME = "lrTable.txt";
	const std::string RULES_INFO_FILENAME = "rulesInfo.txt";
	std::vector<std::vector<int>> lrTable;
	std::vector<std::pair<int, int>> rulesInfo;
	std::stack<int> stack;

	void desplazamiento(int read, int action);
	void reduction(int action);
protected:
	//PODRÍAN SER CLASES AMIGAS
	LexicalAnalyzer lex;

	void check(std::string symbol)
	{
		(lex.getSymbol() == symbol) ? lex.nextSymbol() : error();
	}

	void check(int type)
	{
		(lex.getType() == type) ? lex.nextSymbol() : error();
	}

public:
	explicit SyntaxAnalyzer(std::string& filename);

	void analyze();

	std::string getSymbol() const
	{
		return lex.getSymbol();
	}

	int getType() const
	{
		return lex.getType();
	}

};

