#pragma once
#include <stack>
#include <vector>
#include <cctype>

class StackElement
{
public:
	virtual ~StackElement() =0;
};

inline StackElement::~StackElement() { }

class SymbolsTableElement
{
	std::string id;
	char type;
	std::string scope;
	std::string parametersStr;
public:
	SymbolsTableElement(const std::string& id, char type, const std::string& scope,
		const std::string& parametersStr)
		: id(id)
		, type(type)
		, scope(scope)
		, parametersStr(parametersStr) {}
	SymbolsTableElement(const std::string& id, char type, const std::string& scope)
		: SymbolsTableElement(id, type, scope, "") {}
	std::string getID() const
	{
		return id;
	}
	std::string getParametersStr() const
	{
		return parametersStr;
	}
	char getType() const
	{
		return type;
	}
	std::string getScope() const
	{
		return scope;
	}
};

inline bool exists(std::vector<SymbolsTableElement*>& symbolsTable, const std::string& id,
	char dataType, const std::string& scope)
{
	for (const auto & tableElement : symbolsTable)
	{
		if (tableElement->getID() == id
			&& tableElement->getType() == dataType
			&& tableElement->getScope() == scope)
			return true;
	}
	return false;
}

inline char searchType(std::vector<SymbolsTableElement*>& symbolsTable, const std::string& id)
{
	for (const auto & tableElement : symbolsTable)
	{
		if (tableElement->getID() == id)
			return tableElement->getType();
	}
	return 'e';
}

inline char searchType(std::vector<SymbolsTableElement*>& symbolsTable)
{
	for (const auto & t : symbolsTable)
	{
		if (t->getID() == t->getScope())
			return t->getType();
	}
	return 'v';
}

inline std::string searchType3(std::vector<SymbolsTableElement*>& symbolsTable)
{
	for (const auto & t : symbolsTable)
	{
		if (t->getID() == t->getScope())
			return t->getScope();
	}
	return "";
}

inline char searchType(std::vector<SymbolsTableElement*>& symbolsTable, const std::string& id,
	const std::string& scope)
{
	for (const auto & tableElement : symbolsTable)
	{
		if (tableElement->getID() == id
			&& tableElement->getScope() == scope)
			return tableElement->getType();
	}

	for (size_t i = 0; i < id.size(); i++)
	{
		if (isdigit(id[i]))
		{
			if (id[i] == '.')
				return 'f';
		}
		else
			return 'e';
	}
	return 'i';
}

class Node
{
protected:
	std::string symbol;
	std::string classType;
	char dataType;
	Node* next;
	static int labelNum;

public:
	static std::string varglobal;
	std::string parametersStr;
	std::string scope;

	Node()
	{
		symbol = "";
	}

	void setNext(Node* next)
	{
		this->next = next;
	}

	Node* getNext() const
	{
		return next;
	}

	char getDataType() const
	{
		return dataType;
	}

	std::string getClassType() const
	{
		return classType;
	}

	std::string nextLabel();

	static char getType(Node* node)
	{
		if (node->getSymbol() == "int")
			return 'i';
		if (node->getSymbol() == "float")
			return 'f';
		if (node->getSymbol() == "char")
			return 'c';
		return 'v';
	}

	std::string getSymbol() const
	{
		return symbol;
	}

	virtual void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors)
	{
		if (next)
		{
			if (scope.size() > 0)
				next->scope = scope;
			next->validateTypes(symbolsTable, errors);
		}
	}

	virtual void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		const std::string& s, std::vector<std::string>& errors)
	{
		if (next)
		{
			if (scope.size() > 0)
				next->scope = scope;
			next->validateTypes(symbolsTable, s, errors);
		}
	}

	virtual std::string generateCode()
	{
		if (next)
			return next->generateCode();
		return "";
	}

	virtual std::string generateCodeLocalVar()
	{
		if (next)
			return next->generateCodeLocalVar();
		return "";
	}
};

class Terminal : public StackElement
{
	std::string symbol;
public:
	Terminal(const std::string& symbol)
	{
		this->symbol = symbol;
	}

	~Terminal() {}

	std::string getSymbol() const
	{
		return symbol;
	}
};

class NotTerminal : public StackElement
{
	int nTerminal;
	Node* node;
public:
	NotTerminal(int nTerminal)
	{
		this->nTerminal = nTerminal;
	}

	~NotTerminal() {}

	Node* getNode() const
	{
		return node;
	}

	void setNode(Node* node)
	{
		this->node = node;
	}
};

class State : public StackElement
{
	int stateNum;
public:
	State(int stateNum)
	{
		this->stateNum = stateNum;
	}

	~State() {}

	int getStateNum() const
	{
		return stateNum;
	}
};

class Program : public Node
{
public:
	Program(std::stack<StackElement*>& stack)
	{
		stack.pop();//quita estado
		next = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
	}
};

class Id : public Node
{
public:
	Id(const std::string& symbol)
	{
		this->symbol = symbol;
		classType = "id";
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		dataType = searchType(symbolsTable, symbol);
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable, const std::string& s,
		std::vector<std::string>& errors) override
	{
		dataType = searchType(symbolsTable, symbol, s);
	}
	std::string generateCode() override
	{
		return "push " + symbol + "\n";
	}
};

class Type : public Node
{
public:
	Type(const std::string& _symbol)
	{
		symbol = _symbol;
	}
	std::string generateCode() override
	{
		if (symbol == "int")
			return " DWORD 0 ";
		if (symbol == "float")
			return " real4 0.0 ";
		return "";
	}
	std::string generateCodeLocalVar() override
	{
		if (symbol == "int")
			return ": DWORD ";
		if (symbol == "float")
			return ": real4 ";
		return "";
	}
};

class DefFunc : public Node
{
	Node* type;
	Node* id;
	Node* parameters;
	Node* blockFunc;

public:
	static std::string localVar;
	DefFunc(std::stack<StackElement*>& stack)//<DefFunc> ::= tipo id ( <Parametros> ) <BloqFunc> 
	{
		stack.pop();//quita estado
		blockFunc = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita <bloqfunc>
		stack.pop();
		stack.pop();//quita estado
		stack.pop();//quita )
		stack.pop();//quita estado
		parameters = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita <parametros>
		stack.pop();
		stack.pop();//quita estado
		stack.pop();//quita (
		stack.pop();//quita estado
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita id
		stack.pop();
		stack.pop();//quita estado
		type = new Type(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita el tipo
		stack.pop();
	}

	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		dataType = getType(type);
		scope = id->getSymbol();
		std::string par;
		if (parameters)
		{
			parameters->scope = id->getSymbol();
			parameters->validateTypes(symbolsTable, errors);
			par = parameters->parametersStr;
		}
		else
			par = parametersStr;
		if (!exists(symbolsTable, id->getSymbol(), dataType, scope))
		{
			symbolsTable.push_back(new SymbolsTableElement(id->getSymbol(), dataType, scope,
				par));
		}
		else
			errors.push_back("La funci�n " + id->getSymbol() + "ya existe");
		parametersStr = "";
		if (blockFunc)
		{
			blockFunc->scope = id->getSymbol();
			blockFunc->validateTypes(symbolsTable, errors);
		}
		scope = "";
		if (next)
		{
			next->scope = id->getSymbol();
			next->validateTypes(symbolsTable, errors);
		}
	}
	std::string generateCode() override
	{
		scope = id->getSymbol();
		std::string code;
		std::string blockCode = "";
		code = "\n" + id->getSymbol() + " proc";

		if (parameters)
		{
			std::string parametersCode = parameters->generateCode();
			parametersCode.pop_back();
			code += parametersCode;
		}

		if (blockFunc)
			blockCode += blockFunc->generateCode();

		code += "\n" + localVar + blockCode + "\nret\n" + id->getSymbol() + " endp\n";

		localVar = "";
		if (next)
			return code += next->generateCode();
		return code;
	}
};

class DefVar : public Node
{
	Node* type;
	Node* id;
	Node* varList;

public:
	DefVar(std::stack<StackElement*>& stack)//<DefVar> ::= tipo id <ListaVar> ; 
	{
		stack.pop();//quita estado
		stack.pop(); //quita  ;
		stack.pop(); //quita estado estado
		varList = dynamic_cast<NotTerminal*>(stack.top())->getNode(); //quita ListaVar
		stack.pop();
		stack.pop(); //quita estado
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita Id
		stack.pop();
		stack.pop(); //quita estado
		type = new Type(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita tipo
		stack.pop();
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		dataType = getType(type);
		if (!exists(symbolsTable, id->getSymbol(), dataType, scope))
			symbolsTable.push_back(new SymbolsTableElement(id->getSymbol(), dataType, scope));
		else
			errors.push_back("La variable " + id->getSymbol() + " ya existe.");
		auto aux = varList;
		while (aux)
		{
			if (!exists(symbolsTable, aux->getSymbol(), dataType, scope))
				symbolsTable.push_back(new SymbolsTableElement(aux->getSymbol(), dataType, scope));
			else
				errors.push_back("La variable " + aux->getSymbol() + " ya existe.");
			aux = aux->getNext();
			if (next)
			{
				next->scope = scope;
				next->validateTypes(symbolsTable, errors);
			}
		}
	}
	std::string generateCode() override
	{
		std::string code;
		Node* aux = varList;
		std::string dataType;
		std::string vars;

		if (scope == "")
		{
			dataType = type->generateCode();
			vars = id->getSymbol() + dataType + "\n";
			varglobal += vars;
		}
		else
		{
			dataType = type->generateCodeLocalVar();
			vars = id->getSymbol() + dataType;
			DefFunc::localVar += "local " + vars + "\n";
		}

		if (scope == "")
		{
			while (aux)
			{
				dataType = type->generateCode();
				vars = aux->getSymbol() + dataType + "\n";
				varglobal += vars;
				aux = aux->getNext();
			}
		}
		else
		{
			while (aux)
			{
				dataType = type->generateCodeLocalVar();
				vars = aux->getSymbol() + dataType;
				DefFunc::localVar += "local " + vars + "\n";
				aux = aux->getNext();
			}
		}
		if (varList)
			code += varList->generateCode();
		if (next)
			code += next->generateCode();
		return code;
	}
};

class Parameters : public Node	//<Parametros> ::= tipo id <ListaParam> 
{
	Node* type;
	Node* id;
	Node* parametersList;

public:
	Parameters(std::stack<StackElement*>& stack)
	{
		stack.pop();//quita estado
		parametersList = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita la lista de parametros
		stack.pop();
		stack.pop();//quita estado
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita el id
		stack.pop();
		stack.pop(); //quita estado
		type = new Type(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita el tipo
		stack.pop();
	}

	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		dataType = getType(type);
		if (!exists(symbolsTable, id->getSymbol(), dataType, scope))
			symbolsTable.push_back(new SymbolsTableElement(id->getSymbol(), dataType, scope));
		else
			errors.push_back("La variable " + id->getSymbol() + " ya fue declarada");
		parametersStr += type->getSymbol()[0];
		if (parametersList)
			parametersList->validateTypes(symbolsTable, errors);
		if (next)
		{
			next->scope = scope;
			next->validateTypes(symbolsTable, errors);
		}
	}

	std::string generateCode() override
	{
		std::string code = "";
		if (type->getSymbol() == "int")
			code += " " + id->getSymbol() + " : DWORD,";
		else
			code += " " + id->getSymbol() + " : real 4,";

		if (parametersList)
			code += parametersList->generateCode();
		if (next)
			code += next->generateCode();

		return code;
	}
};

class Asignment : public Node//<Sentencia> ::= id = <Expresion> ; 
{
	Node* id;
	Node* expression;

public:
	Asignment(std::stack<StackElement*>& stack)//<Sentencia> ::= id = <Expresion> ;
	{
		stack.pop();
		stack.pop();//quita la ;
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		stack.pop();//quita =
		stack.pop();
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita id
		stack.pop();
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		id->validateTypes(symbolsTable, errors);
		expression->validateTypes(symbolsTable, errors);
		if (id->getDataType() == 'c' && expression->getDataType() == 'c')
			dataType = 'c';
		else if (id->getDataType() == 'i' && expression->getDataType() == 'i')
			dataType = 'i';
		else if (id->getDataType() == 'f' && expression->getDataType() == 'f')
			dataType = 'f';
		else
		{
			dataType = 'e';
			errors.push_back("El tipo de dato de " + id->getSymbol() +
				" es diferente al de la expresi�n.");
		}
		if (expression->getNext())
		{
			expression->scope = scope;
			expression->getNext()->validateTypes(symbolsTable, errors);
		}
			
		if (next)
		{
			next->scope = scope;
			next->validateTypes(symbolsTable, errors);
		}
	}

	std::string generateCode() override
	{
		std::string code = "";
		if (expression->getClassType() == "cons" || expression->getClassType() == "id")
			code += "\n\tmov eax," + expression->getSymbol() + "\n";
		else if (expression->getSymbol() != "+" && expression->getSymbol() != "-"
			&& expression->getSymbol() != "*" && expression->getSymbol() != "/")
			code += expression->generateCode();
		else
			code += expression->generateCode() + "\n\tpop eax";
		code += "\n\tmov " + id->getSymbol() + ",eax";

		if (next)
			return code += next->generateCode();
		return code;
	}
};

class If : public Node//<Sentencia> ::= if ( <Expresion> ) <SentenciaBloque> <Otro> 
{
	Node* expression;
	Node* sentenceBlock;
	Node* other;

public:
	If(std::stack<StackElement*>& stack)
	{
		stack.pop();
		other = dynamic_cast<NotTerminal*>(stack.top())->getNode(); //quita otro
		stack.pop();
		stack.pop();
		sentenceBlock = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita sentencia bloque
		stack.pop();
		stack.pop();
		stack.pop(); //quita )
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita exprecion
		stack.pop();
		stack.pop();
		stack.pop(); //quita (
		stack.pop();
		stack.pop(); //quita if
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		sentenceBlock->scope = scope;
		sentenceBlock->validateTypes(symbolsTable, errors);
		expression->scope = scope;
		expression->validateTypes(symbolsTable, errors);
		if (other)
		{
			other->scope = scope;
			other->validateTypes(symbolsTable, errors);
		}
		if (next)
		{
			next->scope = scope;
			next->validateTypes(symbolsTable, errors);
		}
	}

	std::string generateCode();
};

class While : public Node //<Sentencia> ::= while ( <Expresion> ) <Bloque> 
{
	Node* expression;
	Node* block;

public:
	While(std::stack<StackElement*>& stack)
	{
		stack.pop();
		block = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita bloque
		stack.pop();
		stack.pop();
		stack.pop(); //quita )
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		stack.pop(); //quita (
		stack.pop();
		stack.pop(); //quita while
	}
};

// <Sentencia> ::= do <Bloque> while ( <Expresion> ) ;
class Dowhile : public Node {
	Node* block;
	Node* expression;

public:
	Dowhile(std::stack<StackElement*>& stack)
	{
		stack.pop();
		stack.pop();//quita ;
		stack.pop();
		stack.pop();//quita )
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita exprecion
		stack.pop();
		stack.pop();
		stack.pop();//quita (
		stack.pop();
		stack.pop();//quita el while
		stack.pop();
		block = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita bloque
		stack.pop();
		stack.pop();
		stack.pop();//quita do
	}
};

class For : public Node //<Sentencia> ::= for id = <Expresion> : <Expresion> : <Expresion> <SentenciaBloque>
{
	Node* sentenceBlock;
	Node* expression1;
	Node* expression2;
	Node* expression3;
	Node* id;

public:
	For(std::stack<StackElement*>& stack)
	{
		stack.pop();
		sentenceBlock = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita senteciabloque
		stack.pop();
		stack.pop();
		expression3 = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		stack.pop();//quita ;
		stack.pop();
		expression2 = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		expression1 = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		stack.pop();//quita =
		stack.pop();
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita id
		stack.pop();
		stack.pop();
		stack.pop();//quita for
	}
};

class Return : public Node//<Sentencia> ::= return <Expresion> ;
{
	Node* expression;

public:
	Return(std::stack<StackElement*>& stack)
	{
		stack.pop();
		stack.pop();//quita ;
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		stack.pop();//quita return
	}
	// c�mo obtener el �mbito aqu�?
	// validaci�n de tipos de una expresi�n?
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		expression->validateTypes(symbolsTable, errors);
		char type = searchType(symbolsTable);
		if (expression->getDataType() == type)
			dataType = type;
		else
		{
			dataType = 'e';
			errors.push_back("El tipo de dato que regresa " + expression->getSymbol()
				+ " no es igual al de la funci�n " + scope);
		}
	}

	std::string generateCode() override
	{
		if (expression->getClassType() == "cons" || expression->getClassType() == "id")
			return "\n\tmov eax," + expression->getSymbol() + "\n\tret";
		else
			return expression->generateCode();
	}
};

class Constant : public Node
{
public:
	Constant(const std::string& symbol)
	{
		this->symbol = symbol;
		classType = "cons";
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		for (size_t i = 0; i < symbol.size(); i++)
		{
			if (isdigit(symbol[i]))
			{
				if (symbol[i] == '.')
				{
					dataType = 'f';
					return;
				}
			}
			else
			{
				dataType = 'e';
				return;
			}
		}
		dataType = 'i';
	}
	std::string generateCode() override
	{
		return "push " + symbol + "\n";
	}
};

class FuncCall : public Node//<LlamadaFunc> ::= id ( <Argumentos> )
{
	Node* id;
	Node* arguments;

	bool funcExists(std::vector<SymbolsTableElement*>& symbolsTable, const std::string& id,
		const std::string& parametersStr, std::vector<std::string>& errors) const
	{
		bool exists = false;
		for (const auto & tableElement : symbolsTable)
		{
			if (tableElement->getID() == id)
			{
				exists = true;
				if (tableElement->getParametersStr() == parametersStr)
					return true;
				errors.push_back("Los par�metros de la funci�n " + id + " son incorrectos.");
			}
		}
		if (!exists)
			errors.push_back("La funci�n " + id + " no existe.");
		return false;
	}

public:
	FuncCall(std::stack<StackElement*>& stack)
	{
		classType = "fun";
		stack.pop();
		stack.pop();//quita )
		stack.pop();
		arguments = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		stack.pop();//quita (
		stack.pop();
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol()); // quita id
		stack.pop();
	}

	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		//Node* aux = new Node(scope); // �Es necesario esto?
		Node* aux = arguments;
		dataType = searchType(symbolsTable, id->getSymbol());
		//aux = arguments;
		std::string s;
		scope = searchType3(symbolsTable);
		while (aux)
		{
			char type2 = searchType(symbolsTable, aux->getSymbol(), scope);
			s += type2;
			aux = aux->getNext();

		}

		if (arguments)
		{
			arguments->scope = scope;
			arguments->validateTypes(symbolsTable, errors);
		}
		if (id->getSymbol() == "print")
			id->validateTypes(symbolsTable, errors);
		else
		{
			if (funcExists(symbolsTable, id->getSymbol(), s, errors))
				id->validateTypes(symbolsTable, parametersStr, errors);
		}
		if (next)
		{
			next->scope = scope;
			next->validateTypes(symbolsTable, errors);
		}
	}

	std::string generateCode() override
	{
		std::string code = "";
		Node* aux = arguments;
		if (id->getSymbol() == "print")
			code += "\n\tprint str$(" + arguments->getSymbol() + ")\n";
		else
		{
			while(aux)
			{
				code += "\tpush " + aux->getSymbol() + "\n";
				aux = aux->getNext();
			}
			code += "\tcall " + id->getSymbol() + "\npush eax\n";
		}
		if (next)
			return code += next->generateCode();
		return code;
	}
};

class Operation1 : public Node//<Expresion> ::= opSuma <Expresion> 
{
	Node* right;

public:
	Operation1(std::stack<StackElement*>& stack)
	{
		stack.pop();
		right = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita exprsion
		stack.pop();
		stack.pop();
		symbol = dynamic_cast<Terminal*>(stack.top())->getSymbol(); // quita el operador
		stack.pop();
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		right->validateTypes(symbolsTable, errors);
		if (next)
			next->validateTypes(symbolsTable, errors);
	}
};

class Operation2 : public Node
{
	Node* right;
	Node* left;

	std::string expressionCode(const std::string& symbol)
	{
		std::string code = "";

		code += "\n\tpop eax";
		code += "\n\tpop ebx";

		if (symbol == "*")
			code += "\n\timul ebx";
		else if (symbol == "+")
			code += "\n\tadd eax,ebx";
		else if (symbol == "-")
			code += "\n\tsub eax,ebx";
		else
			code += "\n\tidiv ebx";
		code += "\n\tpush eax\n";

		return code;
	}

public:
	Operation2(std::stack<StackElement*>& stack)
	{
		stack.pop();
		right = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
		stack.pop();
		symbol = dynamic_cast<Terminal*>(stack.top())->getSymbol(); // quita el operador
		stack.pop();
		stack.pop();
		left = dynamic_cast<NotTerminal*>(stack.top())->getNode(); // quita expresion
		stack.pop();
	}
	void validateTypes(std::vector<SymbolsTableElement*>& symbolsTable,
		std::vector<std::string>& errors) override
	{
		left->scope = scope;
		left->validateTypes(symbolsTable, errors);
		right->scope = scope;
		right->validateTypes(symbolsTable, errors);
		if (left->getDataType() == 'c' && right->getDataType() == 'c')
			dataType = 'c';
		else if (left->getDataType() == 'i' && right->getDataType() == 'i')
			dataType = 'i';
		else if (left->getDataType() == 'f' && right->getDataType() == 'f')
			dataType = 'f';
		else
		{
			dataType = 'e';
			errors.push_back("Error en la expresi�n: " + left->getSymbol() + " " + symbol
				+ " " + right->getSymbol());
		}
	}

	std::string generateCode() override
	{
		std::string code = "";
		if (symbol == "<" || symbol == "<=" || symbol == ">" || symbol == ">=" || symbol == "=="
			|| symbol == "!=")
		{
			if (right->getSymbol() == "*" || right->getSymbol() == "+" || right->getSymbol() == "-" || right->getSymbol() == "/")
			{
				code += right->generateCode();
				code += "\n\tmov ecx,eax";
			}
			else
				code += "\n\tmov ecx," + right->getSymbol();
			if (left->getSymbol() == "*" || left->getSymbol() == "+" || left->getSymbol() == "-" || left->getSymbol() == "/")
			{
				code += left->generateCode();
				code += "\n\tmov edx,eax";
			}
			else
				code += "\n\tmov edx," + left->getSymbol();
			code += "\n\tcmp edx,ecx";
		}
		
		if (symbol == "<")
			return code + "\n\tjnl ";
		if (symbol == "<=")
			return code + "\n\tjnle ";
		if (symbol == ">")
			return code + "\n\tjng ";
		if (symbol == ">=")
			return code + "\n\tjnge ";
		if (symbol == "==")
			return code + "\n\tjne ";
		if (symbol == "!=")
			return code + "\n\tje ";


		if (right->getSymbol() == "*" || right->getSymbol() == "+" || right->getSymbol() == "-" || right->getSymbol() == "/" || left->getSymbol() == "*" || left->getSymbol() == "+" || left->getSymbol() == "-" || right->getSymbol() == "/")
		{
			code += right->generateCode();
			code += left->generateCode();
			code += expressionCode(symbol);
		}
		else if ((right->getSymbol() != "*" && right->getSymbol() != "+" && right->getSymbol() != "-"&&right->getSymbol() != "/") && (left->getSymbol() != "*" && left->getSymbol() != "+" && left->getSymbol() != "-"&&left->getSymbol() == "/"))
		{
			if (right->getSymbol() == "")
			{
				code += right->generateCode();
				code += "\n\tpush eax";
			}
			else
				code += "\n\tpush " + right->getSymbol() + "\n";
			if (left->getSymbol() == "")
			{
				code += left->generateCode();
				code += "\n\tpush eax";
			}
			else
				code += "\n\tpush " + left->getSymbol() + "\n";
			code += expressionCode(symbol);
		}
		else if (right->getSymbol() != "*" && right->getSymbol() != "+" && right->getSymbol() != "-"&&right->getSymbol() != "/")
		{
			if (right->getSymbol() == "")
			{
				code += right->generateCode();
				code += "\n\tpush eax";
			}
			else
				code += "\n\tpush " + right->getSymbol() + "\n";
			code += left->generateCode();
			code += expressionCode(symbol);
		}
		else if (left->getSymbol() != "*" && left->getSymbol() != "+" && left->getSymbol() != "-"&&left->getSymbol() == "/")
		{
			code += right->generateCode();
			if (left->getSymbol() == "")
			{
				code += left->generateCode();
				code += "\n\tpush eax";
			}
			else
				code += "\n\tpush " + left->getSymbol() + "\n";
			code += expressionCode(symbol);
		}

		return code;
	}
};
