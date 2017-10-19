/*
 * @author Ivan Fernando Galaviz Mendoza
 * Compilers
 */

#include <iostream>
#include <string>
#include "Util.h"
#include "SyntaxAnalyzer.h"

int main()
{
	std::string filename;

	std::cout << "Input filename (with extension): ";
	getline(std::cin, filename);

	SyntaxAnalyzer syntaxAnalyzer(filename);
	
	try
	{
		syntaxAnalyzer.analyze();
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << '\n';
		pauseTerminate();
		exit(EXIT_FAILURE);
	}

	std::cout << "The input is valid!\n";

	pauseTerminate();

	return EXIT_SUCCESS;
}
