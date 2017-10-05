#include "Sintactico.h"
#include "Excepciones.h"

Sintactico::Sintactico(std::string& nombreArchivo) : tablaLR(FILAS_TABLA_LR,
                                                     std::vector<int>(COLUMNAS_TABLA_LR)),
													 lexico(nombreArchivo)
{
	std::ifstream archivo = abrirArchivo(NOMBRE_ARCHIVO_TABLA_LR);

	for (int i = 0; i < FILAS_TABLA_LR; i++)
	{
		for (int j = 0; j < COLUMNAS_TABLA_LR; j++)
			archivo >> tablaLR[i][j];
	}

	archivo = abrirArchivo(NOMBRE_ARCHIVO_IDS_TABLA_LR);

	int temp;

	while (archivo >> temp)
		idsReglas.push_back(temp);

	archivo = abrirArchivo(NOMBRE_ARCHIVO_CANT_GENERACIONES_TABLA_LR);

	while (archivo >> temp)
		cantGeneracionesReglas.push_back(temp);
}

void Sintactico::analiza()
{
	pila.push(0);
	bool lee = true;
	int idLex;
	while (true)
	{
		if (lee)
			idLex = lexico.sigSimbolo();
		int accion = tablaLR[pila.top()][idLex];
		std::cout << "Accion= " << accion << '\n';
		if (accion == 0)
			throw ExcepcionSintactica("La tabla arrojo un error");
		if (accion > 0)
		{
			//desplazamiento(/*leido*/, accion);
			desplazamiento(idLex, accion);
			lee = true;
		}
		else if (accion == -1)
		{
			std::cout << "Aceptado sintacticamente!\n";
			break; // Aceptación
		}	
		else
		{
			reduccion(accion);
			lee = false;
		}
	}
}

void Sintactico::desplazamiento(int leido, int accion)
{
	pila.push(leido);
	pila.push(accion);
}

void Sintactico::reduccion(int accion)
{
	int regla = -(accion + 1);
	for (int i = 0; i < cantGeneracionesReglas[regla] * 2; i++)
		pila.pop();
	int columna = idsReglas[regla];
	pila.push(columna);
	pila.push(tablaLR[pila.top()][columna]);
}

/*
void Sintactico::programa()
{
	if (lexico.dameTipo() == FINAL)
		return;

	definiciones();

	programa();
}

void Sintactico::definiciones()
{
	defVar();

	/*switch (lexico.dameTipo())
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

void Sintactico::listaVar()
{
	while (lexico.dameTipo() == COMA)
	{
		lexico.sigSimbolo();
		comprueba(IDENTIFICADOR);
	}
}

void Sintactico::parametros()
{
	if (lexico.dameTipo() == TIPO)
	{
		lexico.sigSimbolo();
		comprueba(IDENTIFICADOR);
		listaParam();
	}
}

void Sintactico::listaParam()
{
	while (lexico.dameTipo() == COMA)
	{
		lexico.sigSimbolo();
		comprueba(TIPO);
		comprueba(IDENTIFICADOR);
	}
}


void Sintactico::bloqFunc()
{
	comprueba(LLAVE_APERTURA);
	defLocales();
	comprueba(LLAVE_CIERRE);
}

void Sintactico::defLocales()
{
	defLocal();
}

void Sintactico::defLocal()
{
	defVar();
	sentencia();
}

void Sintactico::sentencia()
{
	switch (lexico.dameTipo())
	{
	case IDENTIFICADOR:
		lexico.sigSimbolo();
		if (lexico.dameTipo() == PARENTESIS_APERTURA)
		{
			lexico.sigSimbolo();
			llamadaFunc();
		}
		else
		{
			comprueba(OP_ASIGNACION);
			expresion();
		}
		comprueba(PUNTO_Y_COMA);
		break;
	case IF:
		lexico.sigSimbolo();
		comprueba(PARENTESIS_APERTURA);
		expresion();
		comprueba(PARENTESIS_CIERRE);
		sentenciaBloque();
		otro();
		break;
	case WHILE:
		lexico.sigSimbolo();
		comprueba(PARENTESIS_APERTURA);
		expresion();
		comprueba(PARENTESIS_CIERRE);
		bloque();
		break;
	case DO:
		lexico.sigSimbolo();
		bloque();
		comprueba(WHILE);
		comprueba(PARENTESIS_APERTURA);
		expresion();
		comprueba(PARENTESIS_CIERRE);
		comprueba(PUNTO_Y_COMA);
		break;
	case FOR:
		lexico.sigSimbolo();
		comprueba(IDENTIFICADOR);
		comprueba(OP_ASIGNACION);
		expresion();
		comprueba(DOS_PUNTOS);
		expresion();
		comprueba(DOS_PUNTOS);
		expresion();
		sentenciaBloque();
		break;
	case RETURN:
		lexico.sigSimbolo();
		expresion();
		comprueba(PUNTO_Y_COMA);
		break;
	default: error();
	}
}

void Sintactico::otro()
{
	if (lexico.dameTipo() == ELSE)
	{
		lexico.sigSimbolo();
		sentenciaBloque();
	}
}

void Sintactico::atomo()
{
	switch(lexico.dameTipo())
	{
	case IDENTIFICADOR: 
		lexico.sigSimbolo();
		if (lexico.dameTipo() == PARENTESIS_APERTURA)
		{
			lexico.sigSimbolo();
			llamadaFunc();
		}
		break;
	case ENTERO:
	case REAL:
		lexico.sigSimbolo();
	default: error();
	}
}

void Sintactico::llamadaFunc()
{
	argumentos();
	comprueba(PARENTESIS_CIERRE);
}

void Sintactico::bloque()
{
	comprueba(LLAVE_APERTURA);
	sentencias();
	comprueba(LLAVE_CIERRE);
}

void Sintactico::argumentos()
{
	expresion();
	listaArgumentos();
}

void Sintactico::listaArgumentos()
{
	while(lexico.dameTipo() == COMA)
	{
		lexico.sigSimbolo();
		expresion();
	}
}

void Sintactico::defVar()
{
	if (lexico.dameTipo() == TIPO)
	{
		lexico.sigSimbolo();
		comprueba(IDENTIFICADOR);
		if (lexico.dameTipo() == PARENTESIS_APERTURA)
		{
			lexico.sigSimbolo();
			defFunc();
		}
		else
		{
			listaVar();
			comprueba(PUNTO_Y_COMA);
		}
	}
}

void Sintactico::defFunc()
{
	parametros();
	comprueba(PARENTESIS_CIERRE);
	bloqFunc();
}

void Sintactico::expresion()
{
	if (lexico.dameTipo() == PARENTESIS_APERTURA)
	{
		lexico.sigSimbolo();
		expresion();
		comprueba(PARENTESIS_CIERRE);
	}
	else if (lexico.dameTipo() == OP_SUMA || lexico.dameTipo() == OP_NOT)
	{
		lexico.sigSimbolo();
		expresion();
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
*/