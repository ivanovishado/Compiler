#include <iostream>
#include "Util.h"
#include "Lexico.h"

void error()
{
	std::cerr << "ERROR LEXICO: EXISTE UN CARACTER NO VALIDO\n";
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
	case OP_ADIC: 			return "Operador de Adicion";
	case OP_MULT: 			return "Operador de Multiplicacion";
	case OP_RELACIONAL: 	return "Operador relacional";
	case DELIMITADOR: 		return "Delimitador";
	case PARENTESIS_APERTURA: 		return "Parentesis de apertura";
	case PARENTESIS_CIERRE: return "Parentesis de cierre";
	case OP_NOT: 		return "Operador logico";
	case OP_ASIGNACION: 	return "Operador de asignacion";
	case OP_AND: 				return "AND";
	case OP_OR: 			return "OR";
	case CADENA: 			return "Cadena";
	case IDENTIFICADOR: 	return "Identificador";
	case ENTERO: 			return "Entero";
	case REAL: 				return "Real";
	case WHILE:			return "Mientras";
	case IF:				return "Si";
	case ELSE:				return "Otro";
	case FIN: 				return "Fin";
	case FINAL:				return "Final";
	default: 				return "Tipo no definido";
	}
}
