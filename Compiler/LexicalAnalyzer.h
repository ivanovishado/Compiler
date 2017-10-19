#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "Util.h"

class LexicalAnalyzer
{
	const std::vector<std::string> TYPES;

protected:
	size_t actualCharIndex;
	std::string input;
	std::string symbol;
	bool continues;
	int state;
	int type;
	char c;

	void sigEstado(int edo)
	{
		state = edo;
		symbol += c;
	}
	void aceptacion(int edo)
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
		else if (esTipo(symbol))
			type = TYPE;
		else
			type = edo;
		sigEstado(edo);
		continues = false;
		std::cout << "Tipo= " << recoverTypeName(type) << '\n';
	}
	void aceptacionFija(int edo)
	{
		aceptacion(edo);
		actualCharIndex--;
		symbol.pop_back();
	}
	bool esCaracterPuntuacion(char c) 	{ return c == '_'; }
	bool esOpRelacional(char c) 		{ return (c == '>' || c == '<'); }
	bool esOpAsignacion(char c) 		{ return c == '='; }
	bool esEspacio(char c)				{ return c == ' '; }
	bool esPuntoYComa(char c) 			{ return c == ';'; }
	bool esComa(char c)					{ return c == ','; }
	bool esDosPuntos(char c)			{ return c == ':'; }
	bool esParentesisApertura(char c) 	{ return c == '('; }
	bool esParentesisCierre(char c)		{ return c == ')'; }
	bool esLlaveApertura(char c)		{ return c == '{'; }
	bool esLlaveCierre(char c)			{ return c == '}'; }
	bool esOpNot(char c) 				{ return c == '!'; }
	bool esOpAnd(char c)				{ return c == '&'; }
	bool esOpOr(char c)					{ return c == '|'; }
	bool esOpAdic(char c) 				{ return c == '+' || c == '-'; }
	bool esOpMult(char c) 				{ return c == '*' || c == '/'; }
	bool esFijo(char c)
	{
		return esOpAdic(c) || esOpMult(c) || esOpAsignacion(c) || esOpRelacional(c) || esComa(c)
			   || esOpNot(c) || esPuntoYComa(c) || esDosPuntos(c) || esParentesisApertura(c) 
				|| esParentesisCierre(c) || esLlaveApertura(c) || esLlaveCierre(c) || esEspacio(c);
	}

public:
	explicit LexicalAnalyzer(std::string& nombreArchivo);

	int nextSymbol();

	std::string getSymbol() const { return symbol; }

	int getType() const { return type; }

	bool fin() const { return actualCharIndex >= input.size(); }

private:
	bool esTipo(const std::string& simbolo) const
	{
		return isInVector(TYPES, simbolo);
	}
};
