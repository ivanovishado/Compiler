#include "Sintactico.h"
#include "Excepciones.h"

Sintactico::Sintactico(std::string& nombreArchivo) : tablaLR(FILAS_TABLA_LR,
                                                     std::vector<int>(COLUMNAS_TABLA_LR)),
													lexico(nombreArchivo)
{
	auto archivoTablaLR = abrirArchivo(NOMBRE_ARCHIVO_TABLA_LR);

	for (int i = 0; i < FILAS_TABLA_LR; i++)
	{
		for (int j = 0; j < COLUMNAS_TABLA_LR; j++)
			archivoTablaLR >> tablaLR[i][j];
	}

	auto archivoReglas = abrirArchivo(NOMBRE_ARCHIVO_INFO_REGLAS);

	int id, cantProducciones;
	while (archivoReglas >> id >> cantProducciones)
		infoReglas.push_back({ id, cantProducciones * 2 });
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
		if (accion == 0)
			throw ExcepcionSintactica("La tabla arrojo un error");
		if (accion > 0)
		{
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

void Sintactico::desplazamiento(int idLex, int accion)
{
	pila.push(idLex);
	pila.push(accion);
}

void Sintactico::reduccion(int accion)
{
	int regla = -(accion + 2);
	for (int i = 0; i < infoReglas[regla].second; i++)
		pila.pop();
	int idRegla = infoReglas[regla].first, estado = pila.top();
	pila.push(idRegla);
	pila.push(tablaLR[estado][idRegla]);
}
