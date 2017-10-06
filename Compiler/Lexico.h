#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "Util.h"

class Lexico
{
	const std::vector<std::string> TIPOS;

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
		std::cout << "Tipo= " << recuperaNombreTipo(tipo) << '\n';
	}
	void aceptacionFija(int edo)
	{
		aceptacion(edo);
		indiceCaracterActual--;
		simbolo.pop_back();
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
	bool esOpAdic(char c) 				{ return c == '+' || c == '-'; }
	bool esOpMult(char c) 				{ return c == '*' || c == '/'; }
	bool esFijo(char c)
	{
		return esOpAdic(c) || esOpMult(c) || esOpAsignacion(c) || esOpRelacional(c) || esComa(c)
			   || esOpNot(c) || esPuntoYComa(c) || esDosPuntos(c) || esParentesisApertura(c) 
				|| esParentesisCierre(c) || esLlaveApertura(c) || esLlaveCierre(c) || esEspacio(c);
	}

public:
	explicit Lexico(std::string& nombreArchivo);

	int sigSimbolo();

	std::string dameSimbolo() const { return simbolo; }

	int dameTipo() const { return tipo; }

	bool fin() const { return indiceCaracterActual >= entrada.size(); }

private:
	bool esTipo(const std::string& simbolo) const
	{
		return estaEnVector(TIPOS, simbolo);
	}
};
