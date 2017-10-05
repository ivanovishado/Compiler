/*
 * @author Ivan Fernando Galaviz Mendoza
 * Seminario de Soluci�n de Problemas de Traductores de Lenguajes II
 * Tarea 1 - Analizador L�xico
 */

#include <iostream>
#include <fstream>
#include "Lexico.h"
#include "Util.h"
#include "Sintactico.h"

int main()
{
	std::string nombreArchivo;

	std::cout << "Dame nombre de archivo (con extension): ";
	getline(std::cin, nombreArchivo);

	Sintactico sintactico(nombreArchivo);

	sintactico.analiza();
	
	try
	{
		sintactico.analiza();
	}
	catch(std::exception& e)
	{
		e.what();
		pausarTerminar();
		exit(EXIT_FAILURE);
	}

	std::cout << "La entrada es valida.\n";

	pausarTerminar();

	return EXIT_SUCCESS;
}
