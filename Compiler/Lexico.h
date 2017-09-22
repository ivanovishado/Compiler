#pragma once

#include <vector>
#include "Util.h"

const std::vector<std::string> palabrasReservadas =
{
	"if", "while", "do", "for", "return", "else"
};

const std::vector<std::string> tipos = { "int", "float", "double", "char", "void" };

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
		if (simbolo == "if")
			tipo = IF;
		else if (simbolo == "while")
			tipo = WHILE;
		else if (simbolo == "do")
			tipo = DO;
		else if (simbolo == "for")
			tipo = FOR;
		else if (simbolo == "return")
			tipo = RETURN;
		else if (simbolo == "else")
			tipo = ELSE;
		else if (esTipo(simbolo))
			tipo = TIPO;
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
	bool esParentesisApertura(char c) 	{ return c == '('; }
	bool esParentesisCierre(char c)		{ return c == ')'; }
	bool esLlaveApertura(char c) { return c == '{'; }
	bool esLlaveCierre(char c) { return c == '}'; }
	bool esOpNot(char c) 				{ return (c == '!'); }
	bool esOpAdic(char c) 				{ return (c == '+' || c == '-'); }
	bool esOpMult(char c) 				{ return (c == '*' || c == '/'); }
	bool esFijo(char c)
	{
		return (esOpAdic(c) || esOpMult(c) || esOpAsignacion(c) || esOpRelacional(c)
			   || esOpNot(c) || esDelimitador(c) || esParentesisApertura(c) || esParentesisCierre(c)
				|| esCaracterPuntuacion(c) || esLlaveApertura(c) || esLlaveCierre(c));
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
	bool esTipo(const std::string& simbolo)
	{
		return estaEnVector(tipos, simbolo);
	}
};
