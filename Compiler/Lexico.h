#pragma once

#include "Util.h"

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
		sigEstado(edo);
		tipo = edo;
		continua = false;
	}
	void aceptacionFija(int edo)
	{
		aceptacion(edo);
		indiceCaracterActual--;
	}
	bool esCaracterPuntuacion(char c) 	{ return (c == '_'); }
	bool esOpRelacional(char c) 		{ return (c == '>' || c == '<'); }
	bool esOpAsignacion(char c) 		{ return (c == '='); }
	bool esDelimitador(char c) 			{ return (c == ';' || c == ',' || c == '\n'); }
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
};
