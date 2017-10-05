#pragma once
#include <exception>

class ExcepcionLexica : public std::exception
{
public:
	explicit ExcepcionLexica() : mensaje("Ocurrio un error lexico.")
	{}

	explicit ExcepcionLexica(int fila, int columna) :
		mensaje("ERROR LEXICO en [" + std::to_string(fila) + ":" + std::to_string(columna) + "]")
	{}

	const char* what() const throw() override
	{
		return mensaje.c_str();
	}
private:
	std::string mensaje;
};

class ExcepcionSintactica : public std::exception
{
public:
	explicit ExcepcionSintactica(const std::string& msj) :
		mensaje("ERROR SINTACTICO: " + msj)
	{}

	const char* what() const throw() override
	{
		return mensaje.c_str();
	}
private:
	std::string mensaje;
};

class ExcepcionSemantica : public std::exception {
public:
	explicit ExcepcionSemantica(const std::string& msg) :
		mensaje("ERROR SEMANTICO: " + msg)
	{}

	const char* what() const throw() override
	{
		return mensaje.c_str();
	}
private:
	std::string mensaje;
};
