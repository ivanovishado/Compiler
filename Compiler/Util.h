#pragma once

#include <vector>
#include <fstream>
#include "Defs.h"

enum Types
{
	INITIAL = -1,
	IDENTIFIER,
	CONSTANT,
	TYPE,
	ADD_OP,
	MUL_OP,
	RELATIONAL_OP,
	OR_OP,
	AND_OP,
	NOT_OP,
	EQUALITY_OP,
	SEMICOLON,
	COMMA,
	START_PARENTHESES,
	END_PARENTHESES,
	START_BRACES,
	END_BRACES,
	ASIGNMENT_OP,
	IF,
	WHILE,
	DO,
	FOR,
	COLON,
	RETURN,
	ELSE,
	FINAL,
	STRING,
	INTEGER,
	FLOAT,
	FIN
};

std::string recoverTypeName(int type);
void pauseTerminate();
void error();
std::ifstream openFile(const std::string& filename);

template <class T>
bool isInVector(const std::vector<T> & v, const T & value)
{
	return std::find(v.begin(), v.end(), value) != v.end();
};
