#include <sstream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "Exceptions.h"

LexicalAnalyzer::LexicalAnalyzer(std::string& filename) :
	TYPES{ "int", "float", "double", "char", "void"}
{
	std::ifstream file = openFile(filename);
	std::stringstream ss;
	ss << file.rdbuf();
	input = ss.str();

	actualCharIndex = 0;
}

int LexicalAnalyzer::nextSymbol()
{
	state = INITIAL;
	symbol = "";
	continues = true;

	while(continues)
	{
		do
		{
			c = input[actualCharIndex++];
		}
		while (c == '\t' || c == '\n');

		//std::cout << "Caracter leido= " << c << '\n';

		switch(state)
		{
		case INITIAL:
			if (isAddOp(c))
				accept(ADD_OP);
			else if (isMulOp(c))
				accept(MUL_OP);
			else if (isSemicolon(c))
				accept(SEMICOLON);
			else if (isComma(c))
				accept(COMMA);
			else if (isStartParentheses(c))
				accept(START_PARENTHESES);
			else if (isEndParentheses(c))
				accept(END_PARENTHESES);
			else if (isStartBraces(c))
				accept(START_BRACES);
			else if (isEndBraces(c))
				accept(END_BRACES);
			else if (isColon(c))
				accept(COLON);
			else if (isNotOp(c))
				nextState(NOT_OP);
			else if (isAsignmentOp(c))
				nextState(ASIGNMENT_OP);
			else if (isRelationalOp(c))
				nextState(RELATIONAL_OP);
			else if (c == '\"')
				nextState(STRING);
			else if (isPunctuationChar(c) || isalpha(c))
				nextState(IDENTIFIER);
			else if (isdigit(c))
				nextState(CONSTANT);
			else if (isAndOp(c))
				nextState(AND_OP);
			else if (isOrOp(c))
				nextState(OR_OP);
			else if (c == '\0')
				accept(FINAL);
			else if (isSpace(c))
				;
			else
				throw LexicalException();
			break;

		case IDENTIFIER:
			if (isalpha(c) || isdigit(c) || isPunctuationChar(c))
				nextState(IDENTIFIER);
			else if (isFixed(c))
				fixedAccept(IDENTIFIER);
			else
				throw LexicalException();
			break;

		case CONSTANT:
			if (isdigit(c))
				nextState(CONSTANT);
			else if (c == '.')
				nextState(FLOAT);
			else if (isFixed(c))
				fixedAccept(CONSTANT);
			else
				throw LexicalException();
			break;

		case FLOAT:
			if (isdigit(c))
				nextState(FLOAT);
			else if (isFixed(c))
				fixedAccept(FLOAT);
			else
				throw LexicalException();
			break;

		case STRING:
			if (c == '\"')
				accept(STRING);
			else
				nextState(STRING);
			break;

		case NOT_OP:
			if (isAsignmentOp(c))
				accept(RELATIONAL_OP);
			else
				fixedAccept(NOT_OP);
			break;

		case ASIGNMENT_OP:
			if (isAsignmentOp(c))
				accept(EQUALITY_OP);
			else
				fixedAccept(ASIGNMENT_OP);
			break;

		case RELATIONAL_OP:
			if (isAsignmentOp(c))
				accept(RELATIONAL_OP);
			else
				fixedAccept(RELATIONAL_OP);
			break;

		case AND_OP:
			if (isAndOp(c))
				accept(AND_OP);
			else
				throw LexicalException();
			break;

		case OR_OP:
			if (isOrOp(c))
				accept(OR_OP);
			else
				throw LexicalException();
			break;
		}
	}
	std::cout << "Accepted symbol= \"" << symbol << "\" which is: " << recoverTypeName(type)<<'\n';
	return type;
}
