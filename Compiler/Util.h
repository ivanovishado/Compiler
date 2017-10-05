#pragma once

#include <string>
#include <vector>
#include <fstream>

enum Tipos
{
	INICIAL = -1,
	IDENTIFICADOR,
	CONSTANTE,
	TIPO,
	OP_SUMA,
	OP_MULT,
	OP_RELACIONAL,
	OP_OR,
	OP_AND,
	OP_NOT,
	OP_IGUALDAD,
	PUNTO_Y_COMA,
	COMA,
	PARENTESIS_APERTURA,
	PARENTESIS_CIERRE,
	LLAVE_APERTURA,
	LLAVE_CIERRE,
	OP_ASIGNACION,
	IF,
	WHILE,
	DO,
	FOR,
	DOS_PUNTOS,
	RETURN,
	ELSE,
	FINAL,
	CADENA,
	ENTERO,
	REAL,
	FIN
};

std::string recuperaNombreTipo(int tipo);
void pausarTerminar();
void error();
std::ifstream abrirArchivo(const std::string& nombreArchivo);

template <class T>
bool estaEnVector(const std::vector<T> & v, const T & valor)
{
	return std::find(v.begin(), v.end(), valor) != v.end();
};
