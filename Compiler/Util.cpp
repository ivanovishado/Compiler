#include <iostream>
#include "Util.h"
#include "LexicalAnalyzer.h"

void pauseTerminate()
{
	std::cout << "Press enter to terminate . . .";
	std::cin.get();
}

std::ifstream openFile(const std::string& filename)
{
	std::ifstream file("../Files/" + filename, std::ios::in);

	if (!file.is_open())
	{
		std::cerr << "There was a problem opening the file . . .\n";
		pauseTerminate();
		exit(EXIT_FAILURE);
	}

	return file;
}

std::string recoverTypeName(int type)
{
	switch (type)
	{
	case INITIAL: 			return "INITIAL";
	case ADD_OP: 			return "Add operator";
	case MUL_OP: 			return "Multiplication operator";
	case RELATIONAL_OP: 	return "Relational operator";
	case COLON:				return "Colon";
	case SEMICOLON: 		return "Semicolon";
	case COMMA:				return "Comma";
	case START_PARENTHESES: 		return "Start parentheses";
	case END_PARENTHESES: return "End parentheses";
	case START_BRACES:	return "Start braces";
	case END_BRACES:		return "End braces";
	case NOT_OP: 			return "Logic operator";
	case ASIGNMENT_OP: 	return "Asignment operator";
	case AND_OP: 			return "AND";
	case OR_OP: 			return "OR";
	case STRING: 			return "String";
	case IDENTIFIER: 	return "Identifier";
	case INTEGER: 			return "Integer";
	case CONSTANT:			return "Constant";
	case FLOAT: 				return "Float";
	case WHILE:				return "while";
	case FOR:				return "for";
	case IF:				return "if";
	case ELSE:				return "else";
	case RETURN:			return "return";
	case TYPE:				return "Type";
	case FIN: 				return "Fin";
	case FINAL:				return "Final";
	default: 				return "Undefined type";
	}
}
