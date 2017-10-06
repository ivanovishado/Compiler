/*
 * @author Ivan Fernando Galaviz Mendoza
 * Seminario de Solución de Problemas de Traductores de Lenguajes II
 * Tarea 1 - Analizador Léxico
 */

#include <iostream>
#include <string>
#include "Util.h"
#include "Sintactico.h"

int main()
{
	std::string nombreArchivo;

	std::cout << "Dame nombre de archivo (con extension): ";
	getline(std::cin, nombreArchivo);

	Sintactico sintactico(nombreArchivo);
	
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
