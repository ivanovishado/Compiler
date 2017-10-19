#include <sstream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "Excepciones.h"

LexicalAnalyzer::LexicalAnalyzer(std::string& nombreArchivo) : TYPES{ "int", "float", "double", "char", "void"}
{
	std::ifstream archivo = openFile(nombreArchivo);
	std::stringstream ss;
	ss << archivo.rdbuf();
	input = ss.str();

	actualCharIndex = 0;
}

int LexicalAnalyzer::nextSymbol()
{
	state = INITIAL;
	symbol = "";
	continues = true;

	while(continues)
	{
		do
		{
			c = input[actualCharIndex++];
		}
		while (c == '\t' || c == '\n');

		//std::cout << "Caracter leido= " << c << '\n';

		switch(state)
		{
		case INITIAL:
			if (esOpAdic(c))
				aceptacion(ADD_OP);
			else if (esOpMult(c))
				aceptacion(MUL_OP);
			else if (esPuntoYComa(c))
				aceptacion(SEMICOLON);
			else if (esComa(c))
				aceptacion(COMMA);
			else if (esParentesisApertura(c))
				aceptacion(START_PARENTHESES);
			else if (esParentesisCierre(c))
				aceptacion(END_PARENTHESES);
			else if (esLlaveApertura(c))
				aceptacion(START_BRACES);
			else if (esLlaveCierre(c))
				aceptacion(END_BRACES);
			else if (esDosPuntos(c))
				aceptacion(COLON);
			else if (esOpNot(c))
				sigEstado(NOT_OP);
			else if (esOpAsignacion(c))
				sigEstado(ASIGNMENT_OP);
			else if (esOpRelacional(c))
				sigEstado(RELATIONAL_OP);
			else if (c == '\"')
				sigEstado(STRING);
			else if (esCaracterPuntuacion(c) || isalpha(c))
				sigEstado(IDENTIFIER);
			else if (isdigit(c))
				sigEstado(CONSTANT);
			else if (esOpAnd(c))
				sigEstado(AND_OP);
			else if (esOpOr(c))
				sigEstado(OR_OP);
			else if (c == '\0')
				aceptacion(FINAL);
			else if (esEspacio(c))
				;
			else
				throw ExcepcionLexica();
			break;

		case IDENTIFIER:
			if (isalpha(c) || isdigit(c) || esCaracterPuntuacion(c))
				sigEstado(IDENTIFIER);
			else if (esFijo(c))
				aceptacionFija(IDENTIFIER);
			else
				throw ExcepcionLexica();
			break;

		case CONSTANT:
			if (isdigit(c))
				sigEstado(CONSTANT);
			else if (c == '.')
				sigEstado(FLOAT);
			else if (esFijo(c))
				aceptacionFija(CONSTANT);
			else
				throw ExcepcionLexica();
			break;

		case FLOAT:
			if (isdigit(c))
				sigEstado(FLOAT);
			else if (esFijo(c))
				aceptacionFija(FLOAT);
			else
				throw ExcepcionLexica();
			break;

		case STRING:
			if (c == '\"')
				aceptacion(STRING);
			else
				sigEstado(STRING);
			break;

		case NOT_OP:
			if (esOpAsignacion(c))
				aceptacion(RELATIONAL_OP);
			else
				aceptacionFija(NOT_OP);
			break;

		case ASIGNMENT_OP:
			if (esOpAsignacion(c))
				aceptacion(EQUALITY_OP);
			else
				aceptacionFija(ASIGNMENT_OP);
			break;

		case RELATIONAL_OP:
			if (esOpAsignacion(c))
				aceptacion(RELATIONAL_OP);
			else
				aceptacionFija(RELATIONAL_OP);
			break;

		case AND_OP:
			if (esOpAnd(c))
				aceptacion(AND_OP);
			else
				throw ExcepcionLexica();
			break;

		case OR_OP:
			if (esOpOr(c))
				aceptacion(OR_OP);
			else
				throw ExcepcionLexica();
			break;
		}
	}
	std::cout << "Simbolo aceptado= \"" << symbol << "\" que es un: " << recoverTypeName(type) << '\n';
	return type;
}
