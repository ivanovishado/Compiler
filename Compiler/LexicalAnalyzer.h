#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "Util.h"

class LexicalAnalyzer
{
	const std::vector<std::string> TYPES;

	bool isType(const std::string& symbol) const
	{
		return isInVector(TYPES, symbol);
	}

	bool isPunctuationChar(char c) { return c == '_'; }
	bool isRelationalOp(char c) { return (c == '>' || c == '<'); }
	bool isAsignmentOp(char c) { return c == '='; }
	bool isSpace(char c) { return c == ' '; }
	bool isSemicolon(char c) { return c == ';'; }
	bool isComma(char c) { return c == ','; }
	bool isColon(char c) { return c == ':'; }
	bool isStartParentheses(char c) { return c == '('; }
	bool isEndParentheses(char c) { return c == ')'; }
	bool isStartBraces(char c) { return c == '{'; }
	bool isEndBraces(char c) { return c == '}'; }
	bool isNotOp(char c) { return c == '!'; }
	bool isAndOp(char c) { return c == '&'; }
	bool isOrOp(char c) { return c == '|'; }
	bool isAddOp(char c) { return c == '+' || c == '-'; }
	bool isMulOp(char c) { return c == '*' || c == '/'; }
	bool isFixed(char c)
	{
		return isAddOp(c) || isMulOp(c) || isAsignmentOp(c) || isRelationalOp(c) || isComma(c)
			|| isNotOp(c) || isSemicolon(c) || isColon(c) || isStartParentheses(c)
			|| isEndParentheses(c) || isStartBraces(c) || isEndBraces(c) || isSpace(c);
	}

protected:
	size_t actualCharIndex;
	std::string input;
	std::string symbol;
	bool continues;
	int state;
	int type;
	char c;

	void nextState(int state)
	{
		this->state = state;
		symbol += c;
	}
	void accept(int state)
	{
		if (symbol == "if")
			type = IF;
		else if (symbol == "while")
			type = WHILE;
		else if (symbol == "do")
			type = DO;
		else if (symbol == "for")
			type = FOR;
		else if (symbol == "return")
			type = RETURN;
		else if (symbol == "else")
			type = ELSE;
		else if (isType(symbol))
			type = TYPE;
		else
			type = state;
		nextState(state);
		continues = false;
		//std::cout << "Tipo= " << recoverTypeName(type) << '\n';
	}
	void fixedAccept(int state)
	{
		accept(state);
		actualCharIndex--;
		symbol.pop_back();
	}

public:
	explicit LexicalAnalyzer(std::string& filename);

	int nextSymbol();

	std::string getSymbol() const { return symbol; }

	int getType() const { return type; }

	bool fin() const { return actualCharIndex >= input.size(); }
};
