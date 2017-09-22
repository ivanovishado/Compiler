#pragma once

#include <string>
#include <vector>

enum Tipos
{
	INICIAL = -1,
	IDENTIFICADOR,
	CONSTANTE,
	TIPO,
	OP_ADIC,
	OP_MULT,
	OP_RELACIONAL,
	OP_OR,
	OP_AND,
	OP_NOT,
	// OP_IGUALDAD, ?
	DELIMITADOR,
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
	CADENA,
	ENTERO,
	REAL,
	FIN,
	FINAL
};

std::string recuperaNombreTipo(int tipo);
void pausarTerminar();
void error();

template <class T>
bool estaEnVector(const std::vector<T> & v, const T & valor)
{
	return std::find(v.begin(), v.end(), valor) != v.end();
};
