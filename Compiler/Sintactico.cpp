#include "Sintactico.h"

Sintactico::Sintactico(std::ifstream& ifs)
{
	lexico.asignaEntrada(ifs);
	lexico.sigSimbolo();
}

void Sintactico::programa()
{
	if (lexico.dameTipo() == FINAL)
		return;

	definiciones();

	programa();
}

void Sintactico::definiciones()
{
	switch (lexico.dameTipo())
	{
	case IDENTIFICADOR:
		lexico.sigSimbolo();
		comprueba(OP_ASIGNACION);
		expresion();
		if (lexico.dameSimbolo() == "(")
			lexico.sigSimbolo();
		else
			comprueba("\n");
		break;
	case WHILE:
		lexico.sigSimbolo();
		expresionRelacional();
		comprueba("\n");
		while (lexico.dameTipo() != FIN)
			definiciones();
		comprueba(FIN);
		break;
	case IF:
		lexico.sigSimbolo();
		expresionRelacional();
		comprueba("\n");
		while (lexico.dameTipo() != ELSE && lexico.dameTipo() != FIN)
			definiciones();
		otro();
		comprueba(FIN);
		break;
	default: error();
	}
}

void Sintactico::expresion()
{
	multiplicacion();
	std::string sim = lexico.dameSimbolo();
	while (sim == "+" || sim == "-")
	{
		lexico.sigSimbolo();
		multiplicacion();
		sim = lexico.dameSimbolo();
	}
}

void Sintactico::multiplicacion()
{
	factor();
	std::string sim = lexico.dameSimbolo();
	while (sim == "*" || sim == "/")
	{
		lexico.sigSimbolo();
		factor();
		sim = lexico.dameSimbolo();
	}
}

void Sintactico::factor()
{
	std::string sim = lexico.dameSimbolo();
	if (sim == "+" || sim == "-")
	{
		lexico.sigSimbolo();
		factor();
	}
	else if (sim == "(")
	{
		lexico.sigSimbolo();
		expresion();
		comprueba(")");
	}
	else
	{
		switch (lexico.dameTipo())
		{
		case IDENTIFICADOR:
		case ENTERO:
		case REAL:
			lexico.sigSimbolo();
			break;
		default: error();
		}
	}
}

void Sintactico::expresionRelacional()
{
	expresion();
	comprueba(OP_RELACIONAL);
	expresion();
	otraExpresion();
}

void Sintactico::otraExpresion()
{
	switch (lexico.dameTipo())
	{
	case OP_AND:
	case OP_OR:
		lexico.sigSimbolo();
		expresionRelacional();
		break;
	}
}

void Sintactico::otro()
{
	if (lexico.dameTipo() == ELSE)
	{
		lexico.sigSimbolo();
		comprueba("\n");
		while (lexico.dameTipo() != FIN)
			definiciones();
	}
}

void Sintactico::puntoYComa(std::string sim)
{
	if (sim == ";")
		lexico.sigSimbolo();
}
