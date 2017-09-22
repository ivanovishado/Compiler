#pragma once

#include <iostream>
#include <cstring>
#include "Lexico.h"

class Sintactico
{
protected:
	Lexico lexico;

	void comprueba(std::string simbolo)
	{
		(lexico.dameSimbolo() == simbolo) ? lexico.sigSimbolo() : error();
	}

	void comprueba(int tipo)
	{
		(lexico.dameTipo() == tipo) ? lexico.sigSimbolo() : error();
	}

	void asignacion();
	void expresion();
	void multiplicacion();
	void factor();
	void definiciones();
	void expresionRelacional();
	void otraExpresion();
	void definicion();
	void defVar();
	void defFunc();
	void listaVar();
	void parametros();
	void bloqFunc();
	void listaParam();
	void defLocales();
	void defLocal();
	void sentencia();
	void sentencias();
	void sentenciaBloque();
	void otro();
	void bloque();
	void llamadaFunc();
	void listaArgumentos();
	void argumentos();
	void atomo();
	void puntoYComa(std::string sim);

public:
	Sintactico(std::ifstream& ifs);

	void programa();

	std::string dameSimbolo()
	{
		return lexico.dameSimbolo();
	}

	int dameTipo()
	{
		return lexico.dameTipo();
	}

};

