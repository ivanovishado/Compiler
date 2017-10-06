#pragma once

#include <stack>
#include "Lexico.h"

class Sintactico
{
	const int FILAS_TABLA_LR = 108;
	const int COLUMNAS_TABLA_LR = 46;
	const std::string NOMBRE_ARCHIVO_TABLA_LR = "tablaLR.txt";
	const std::string NOMBRE_ARCHIVO_INFO_REGLAS = "infoReglas.txt";
	std::vector<std::vector<int>> tablaLR;
	std::vector<std::pair<int, int>> infoReglas;
	std::stack<int> pila;

	void desplazamiento(int leido, int accion);
	void reduccion(int accion);
protected:
	//PODRÍAN SER CLASES AMIGAS
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

public:
	explicit Sintactico(std::string& nombreArchivo);

	void programa();

	void analiza();

	std::string dameSimbolo() const
	{
		return lexico.dameSimbolo();
	}

	int dameTipo() const
	{
		return lexico.dameTipo();
	}

};

