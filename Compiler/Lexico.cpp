#include <sstream>
#include <fstream>
#include "Lexico.h"

Lexico::Lexico(std::ifstream& ifs)
{
	std::stringstream ss;
	ss << ifs.rdbuf();
	entrada = ss.str();
	ifs.close();

	indiceCaracterActual = 0;
}

void Lexico::sigSimbolo()
{
	estado = INICIAL;
	simbolo = "";
	continua = true;

	while(continua)
	{
		do
		{
			c = entrada[indiceCaracterActual++];
		}
		while(c == ' ' || c == '\t' || c == '\n');

		switch(estado)
		{
		case INICIAL:
			if (esOpAdic(c))
				aceptacion(OP_ADIC);
			else if (esOpMult(c))
				aceptacion(OP_MULT);
			else if (esDelimitador(c))
				aceptacion(DELIMITADOR);
			else if (esParentesisApertura(c))
				aceptacion(PARENTESIS_APERTURA);
			else if (esParentesisCierre(c))
				aceptacion(PARENTESIS_CIERRE);
			else if (esOpNot(c))
				sigEstado(OP_NOT);
			else if (esOpAsignacion(c))
				sigEstado(OP_ASIGNACION);
			else if (esOpRelacional(c))
				sigEstado(OP_RELACIONAL);
			else if (c == '\"')
				sigEstado(CADENA);
			else if (esCaracterPuntuacion(c) || isalpha(c))
				sigEstado(IDENTIFICADOR);
			else if (isdigit(c))
				sigEstado(ENTERO);
			else if (c == '&')
				sigEstado(OP_AND);
			else if (c == '|')
				sigEstado(OP_OR);
			else if (c == '\0')
				aceptacion(FINAL);
			else
				error();
			break;

		case IDENTIFICADOR:
			if (isalpha(c) || isdigit(c) || esCaracterPuntuacion(c))
				sigEstado(IDENTIFICADOR);
			else if (esFijo(c))
				aceptacionFija(IDENTIFICADOR);
			else
				error();
			break;

		case ENTERO:
			if (isdigit(c))
				sigEstado(ENTERO);
			else if (c == '.')
				sigEstado(REAL);
			else if (esFijo(c))
				aceptacionFija(ENTERO);
			else
				error();
			break;

		case REAL:
			if (isdigit(c))
				sigEstado(REAL);
			else if (esFijo(c))
				aceptacionFija(REAL);
			else
				error();
			break;

		case CADENA:
			if (c == '\"')
				aceptacion(CADENA);
			else
				sigEstado(CADENA);
			break;

		case OP_NOT:
			if (esOpAsignacion(c))
				aceptacion(OP_RELACIONAL);
			else
				aceptacionFija(OP_NOT);
			break;

		case OP_ASIGNACION:
			if (esOpAsignacion(c))
				aceptacion(OP_RELACIONAL);
			else
				aceptacionFija(OP_ASIGNACION);
			break;

		case OP_RELACIONAL:
			if (esOpAsignacion(c))
				aceptacion(OP_RELACIONAL);
			else
				aceptacionFija(OP_RELACIONAL);
			break;

		case OP_AND:
			if (c == '&')
				aceptacion(OP_AND);
			else
				error();
			break;

		case OP_OR:
			if (c == '|')
				aceptacion(OP_OR);
			else
				error();
			break;
		}
	}
}
