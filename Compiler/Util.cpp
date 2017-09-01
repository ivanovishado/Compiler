#include <iostream>
#include "Util.h"
#include "Defs.h"
#include "Lexico.h"

void error()
{
	std::cerr << "ERROR: EXISTE UN CARACTER NO VALIDO\n";
	pausarTerminar();
	exit(EXIT_FAILURE);
}

void pausarTerminar()
{
	std::cout << "Presione entrar para terminar . . .";
	std::cin.get();
}

std::string recuperaNombreTipo(int tipo)
{
	switch (tipo)
	{
	case INICIAL: 			return "INICIAL";
	case PALABRA_RESERVADA:	return "Palabra reservada";
	case OP_ADIC: 			return "Operador de Adicion";
	case OP_MULT: 			return "Operador de Multiplicacion";
	case OP_RELACIONAL: 	return "Operador relacional";
	case DELIMITADOR: 		return "Delimitador";
	case PARENTESIS: 		return "Parentesis";
	case OP_LOGICO: 		return "Operador logico";
	case OP_ASIGNACION: 	return "Operador de asignacion";
	case AND: 				return "AND";
	case OR: 				return "OR";
	case CADENA: 			return "Cadena";
	case IDENTIFICADOR: 	return "Identificador";
	case ENTERO: 			return "Entero";
	case REAL: 				return "Real";
	case MIENTRAS:			return "Mientras";
	case SI:				return "Si";
	case OTRO:				return "Otro";
	case FIN: 				return "Fin";
	case FINAL:				return "Final";
	default: 				return "Tipo no definido";
	}
}
