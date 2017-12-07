/*
 * @author Ivan Fernando Galaviz Mendoza
 * Compilers
 */

#include <iostream>
#include <string>
#include "Util.h"
#include "SyntaxAnalyzer.h"

inline void printTable(std::vector<SymbolsTableElement*>& symbolsTable)
{
	for (const auto& t : symbolsTable)
	{
		std::cout << "ID= " << t->getID() << '|'
			<< "Type= " << t->getType() << '|'
			<< "Scope= " << t->getScope() << '|'
			<< "ParameterSTR= " << t->getParametersStr() << '\n';
	}
}

int main()
{
	std::string filename;
	Node* root;
	std::vector<SymbolsTableElement*> symbolsTable;
	std::vector<std::string> errors;

	std::cout << "Input filename (with extension): ";
	getline(std::cin, filename);

	SyntaxAnalyzer syntaxAnalyzer(filename);
	
	try
	{
		syntaxAnalyzer.analyze(root);
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << '\n';
		pauseTerminate();
		exit(EXIT_FAILURE);
	}

	root->validateTypes(symbolsTable, errors);

	if (errors.size() > 0)
	{
		std::cout << "Semantic Errors:\n";
		for (const auto& error : errors)
			std::cerr << error << '\n';
		pauseTerminate();
		exit(EXIT_FAILURE);
	}

	writeASMCodeInFile(filename, root->generateCode());

	pauseTerminate();

	return EXIT_SUCCESS;
}
