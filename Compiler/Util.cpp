#include <iostream>
#include "Util.h"
#include "Lexico.h"

void pausarTerminar()
{
	std::cout << "Presione entrar para terminar . . .";
	std::cin.get();
}

std::ifstream abrirArchivo(const std::string& nombreArchivo)
{
	std::ifstream archivo("../Files/"+nombreArchivo, std::ios::in);

	if (!archivo.is_open())
	{
		std::cerr << "Ocurrio un error al abrir el archivo . . .\n";
		pausarTerminar();
		exit(EXIT_FAILURE);
	}

	return archivo;
}

std::string recuperaNombreTipo(int tipo)
{
	switch (tipo)
	{
	case INICIAL: 			return "INICIAL";
	case OP_SUMA: 			return "Operador de Adicion";
	case OP_MULT: 			return "Operador de Multiplicacion";
	case OP_RELACIONAL: 	return "Operador relacional";
	case DOS_PUNTOS:		return "Dos puntos";
	case PUNTO_Y_COMA: 		return "Punto y coma";
	case COMA:				return "Coma";
	case PARENTESIS_APERTURA: 		return "Parentesis de apertura";
	case PARENTESIS_CIERRE: return "Parentesis de cierre";
	case LLAVE_APERTURA:	return "Llave de apertura";
	case LLAVE_CIERRE:		return "Llave de cierre";
	case OP_NOT: 			return "Operador logico";
	case OP_ASIGNACION: 	return "Operador de asignacion";
	case OP_AND: 			return "AND";
	case OP_OR: 			return "OR";
	case CADENA: 			return "Cadena";
	case IDENTIFICADOR: 	return "Identificador";
	case ENTERO: 			return "Entero";
	case REAL: 				return "Real";
	case WHILE:				return "while";
	case FOR:				return "for";
	case IF:				return "if";
	case ELSE:				return "else";
	case RETURN:			return "return";
	case TIPO:				return "Tipo";
	case FIN: 				return "Fin";
	case FINAL:				return "Final";
	default: 				return "Tipo no definido";
	}
}
