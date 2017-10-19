#pragma once
#include <exception>
#include <string>

class LexicalException : public std::exception
{
public:
	explicit LexicalException() : message("A lexical error ocurred.")
	{}

	explicit LexicalException(int row, int column) :
		message("LEXICAL ERROR at [" + std::to_string(row) + ":" + std::to_string(column) + "]")
	{}

	const char* what() const throw() override
	{
		return message.c_str();
	}
private:
	std::string message;
};

class SyntaxException : public std::exception
{
public:
	explicit SyntaxException(const std::string& msg) :
		message("SYNTAX ERROR: " + msg)
	{}

	const char* what() const throw() override
	{
		return message.c_str();
	}
private:
	std::string message;
};

class SemanticException : public std::exception {
public:
	explicit SemanticException(const std::string& msg) :
		message("SEMANTIC ERROR: " + msg)
	{}

	const char* what() const throw() override
	{
		return message.c_str();
	}
private:
	std::string message;
};
