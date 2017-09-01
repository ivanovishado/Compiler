#pragma once

#include <vector>
#include "Util.h"

const std::vector<std::string> palabrasReservadas =
{
	"auto",	"else",	"long",	"switch", "break", "enum", "register", "typedef", "case", "extern",
	"return", "union", "char", "float",	"short", "unsigned", "const", "for", "signed", "void",
	"continue",	"goto",	"sizeof", "volatile", "default", "if", "static", "while", "do",	"int",
	"struct", "double"
};

class Lexico
{
protected:
	size_t indiceCaracterActual;
	std::string entrada;
	std::string simbolo;
	bool continua;
	int estado;
	int tipo;
	char c;

	void sigEstado(int edo)
	{
		estado = edo;
		simbolo += c;
	}
	void aceptacion(int edo)
	{
		if (esPalabraReservada(simbolo))
			tipo = PALABRA_RESERVADA;
		else
			tipo = edo;
		sigEstado(edo);
		continua = false;
	}
	void aceptacionFija(int edo)
	{
		aceptacion(edo);
		indiceCaracterActual--;
		simbolo.pop_back();
	}
	bool esCaracterPuntuacion(char c) 	{ return (c == '_'); }
	bool esOpRelacional(char c) 		{ return (c == '>' || c == '<'); }
	bool esOpAsignacion(char c) 		{ return (c == '='); }
	bool esDelimitador(char c) 			{ return (c == ';' || c == ','); }
	bool esParentesis(char c) 			{ return (c == '(' || c == ')'); }
	bool esOpLogico(char c) 			{ return (c == '!'); }
	bool esOpAdic(char c) 				{ return (c == '+' || c == '-'); }
	bool esOpMult(char c) 				{ return (c == '*' || c == '/'); }
	bool esFijo(char c)
	{
		return (esOpAdic(c) || esOpMult(c) || esOpAsignacion(c) || esOpRelacional(c)
			   || esOpLogico(c) || esDelimitador(c) || esParentesis(c) || esCaracterPuntuacion(c));
	}

public:
	explicit Lexico(std::ifstream& ifs);

	void sigSimbolo();

	const char* dameSimbolo() const { return simbolo.c_str(); }

	int dameTipo() const { return tipo; }

	bool fin() const { return (indiceCaracterActual >= entrada.size()); }

private:
	bool esPalabraReservada(const std::string& simbolo)
	{
		return estaEnVector(palabrasReservadas, simbolo);
	}
};
