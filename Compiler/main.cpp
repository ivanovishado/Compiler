/*
 * @author Ivan Fernando Galaviz Mendoza
 * Seminario de Solución de Problemas de Traductores de Lenguajes II
 * Tarea 1 - Analizador Léxico
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

	std::ifstream ifs("..//Files//" + nombreArchivo, std::ios::in);

	if(!ifs.is_open())
	{
		std::cerr << "Ocurrio un problema al abrir el archivo . . .\n";
		pausarTerminar();
		exit(EXIT_FAILURE);
	}

	Sintactico sintactico(ifs);

	sintactico.programa();

	pausarTerminar();

	return EXIT_SUCCESS;
}
