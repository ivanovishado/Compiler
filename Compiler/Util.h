#pragma once

#include <string>
#include <vector>

enum Tipos
{
	INICIAL,
	PALABRA_RESERVADA,
	OP_ADIC,
	OP_MULT,
	OP_DIV,
	OP_RELACIONAL,
	DELIMITADOR,
	PARENTESIS,
	OP_LOGICO,
	OP_ASIGNACION,
	AND,
	OR,
	CADENA,
	IDENTIFICADOR,
	ENTERO,
	REAL,
	MIENTRAS,
	SI,
	OTRO,
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
