/*
 * @author Ivan Fernando Galaviz Mendoza
 * Seminario de Solución de Problemas de Traductores de Lenguajes II
 * Tarea 1 - Analizador Léxico
 */

#include <iostream>
#include <fstream>
#include "Lexico.h"
#include "Util.h"

int main()
{
	std::ifstream ifs;
	std::string nombreArchivo;

	std::cout << "Dame nombre de archivo (con extension): ";
	getline(std::cin, nombreArchivo);

	ifs.open("..//Files//" + nombreArchivo, std::ios::in);

	if(!ifs.is_open())
	{
		std::cerr << "Ocurrio un problema al abrir el archivo . . .\n";
		pausarTerminar();
		exit(EXIT_FAILURE);
	}

	Lexico lex(ifs);

	while(!lex.fin())
	{
		lex.sigSimbolo();
		std::cout << "Simbolo= " << lex.dameSimbolo() << "\tTipo= " << recuperaNombreTipo(
			lex.dameTipo()) << '\n';
	}

	pausarTerminar();

	return EXIT_SUCCESS;
}
