#include <sstream>
#include <fstream>
#include "Lexico.h"
#include "Excepciones.h"

Lexico::Lexico(std::string& nombreArchivo) : TIPOS{ "int", "float", "double", "char", "void"}
{
	std::ifstream archivo = abrirArchivo(nombreArchivo);
	std::stringstream ss;
	ss << archivo.rdbuf();
	entrada = ss.str();

	indiceCaracterActual = 0;
}

int Lexico::sigSimbolo()
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
		while (c == '\t' || c == '\n');

		//std::cout << "Caracter leido= " << c << '\n';

		switch(estado)
		{
		case INICIAL:
			if (esOpAdic(c))
				aceptacion(OP_SUMA);
			else if (esOpMult(c))
				aceptacion(OP_MULT);
			else if (esPuntoYComa(c))
				aceptacion(PUNTO_Y_COMA);
			else if (esComa(c))
				aceptacion(COMA);
			else if (esParentesisApertura(c))
				aceptacion(PARENTESIS_APERTURA);
			else if (esParentesisCierre(c))
				aceptacion(PARENTESIS_CIERRE);
			else if (esLlaveApertura(c))
				aceptacion(LLAVE_APERTURA);
			else if (esLlaveCierre(c))
				aceptacion(LLAVE_CIERRE);
			else if (esDosPuntos(c))
				aceptacion(DOS_PUNTOS);
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
				sigEstado(CONSTANTE);
			else if (c == '&')
				sigEstado(OP_AND);
			else if (c == '|')
				sigEstado(OP_OR);
			else if (c == '\0')
				aceptacion(FINAL);
			else if (esEspacio(c))
				;
			else
				throw ExcepcionLexica();
			break;

		case IDENTIFICADOR:
			if (isalpha(c) || isdigit(c) || esCaracterPuntuacion(c))
				sigEstado(IDENTIFICADOR);
			else if (esFijo(c))
				aceptacionFija(IDENTIFICADOR);
			else
				throw ExcepcionLexica();
			break;

		case CONSTANTE:
			if (isdigit(c))
				sigEstado(CONSTANTE);
			else if (c == '.')
				sigEstado(REAL);
			else if (esFijo(c))
				aceptacionFija(CONSTANTE);
			else
				throw ExcepcionLexica();
			break;

		case REAL:
			if (isdigit(c))
				sigEstado(REAL);
			else if (esFijo(c))
				aceptacionFija(REAL);
			else
				throw ExcepcionLexica();
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
				aceptacion(OP_IGUALDAD);
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
				throw ExcepcionLexica();
			break;

		case OP_OR:
			if (c == '|')
				aceptacion(OP_OR);
			else
				throw ExcepcionLexica();
			break;
		}
	}
	std::cout << "Simbolo aceptado= \"" << simbolo << "\" que es un: " << recuperaNombreTipo(tipo) << '\n';
	return tipo;
}
