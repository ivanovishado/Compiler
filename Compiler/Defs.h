#pragma once
#include <stack>

class StackElement
{
public:
	virtual ~StackElement() = 0;
};

inline StackElement::~StackElement() { }

class Node
{
protected:
	std::string symbol;
	std::string classType;
	Node* next;

public:
	Node()
	{
		symbol = "";
	}

	void setNext(Node* next)
	{
		this->next = next;
	}
};

class Terminal : public StackElement
{
	std::string symbol;
public:
	Terminal(std::string symbol)
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
		Node* node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//next = ((NotTerminal)stack.pop()).getNode();
		next = node;
	}
};

class Id : public Node
{
public:
	Id(std::string symbol)
	{
		this->symbol = symbol;
		classType = "id";
	}
};

class Type : public Node
{
public:
	Type(std::string _symbol)
	{
		symbol = _symbol;
	}
};

class DefVar : public Node
{
	Node* type;
	Node* id;
	Node* lvar;

public:
	DefVar(std::stack<StackElement*>& stack)//<DefVar> ::= tipo id <ListaVar> ; 
	{
		stack.pop();//quita estado
		stack.pop(); //quita  ;
		stack.pop(); //quita estado estado
		lvar = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//lvar = ((NotTerminal)stack.pop()).getNode(); //quita ListaVar
		stack.pop(); //quita estado
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//id = new Id(((Terminal)stack.pop()).getSymbol()); //quita Id
		stack.pop(); //quita estado
		type = new Type(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//tipo = new Type(((Terminal)stack.pop()).getSymbol()); //quita tipo
	}
};

class DefFunc : public Node
{
	Node* type;
	Node* id;
	Node* parameters;
	Node* blockFunc;

public:
	static std::string varlocal;
	DefFunc(std::stack<StackElement*>& stack)//<DefFunc> ::= tipo id ( <Parametros> ) <BloqFunc> 
	{
		stack.pop();//quita estado
		blockFunc = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//blockFunc = ((NotTerminal)stack.pop()).getNode();//quita <bloqfunc>
		stack.pop();//quita estado
		stack.pop();//quita )
		stack.pop();//quita estado
		parameters = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//parameters = ((NotTerminal)stack.pop()).getNode();//quita <parametros>
		stack.pop();//quita estado
		stack.pop();//quita (
		stack.pop();//quita estado
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//id = new Id(((Terminal)stack.pop()).getSymbol());//quita id
		stack.pop();//quita estado
		type = new Type(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//type = new Type(((Terminal)stack.pop()).getSymbol());//quita el tipo
	}
};

class Parameters : public Node	//<Parametros> ::= tipo id <ListaParam> 
{
	Node* type;
	Node* id;
	Node* parametersL;

public:
	Parameters(std::stack<StackElement*>& stack)
	{
		stack.pop();//quita estado
		parametersL = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//parametersL = ((NotTerminal)stack.pop()).getNode();//quita la lista de aprametros
		stack.pop();//quita estado
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//id = new Id(((Terminal)stack.pop()).getSymbol());//quita el id
		stack.pop(); //quita estado
		type = new Type(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//type = new Type(((Terminal)stack.pop()).getSymbol());//quita el tipo
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
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression = ((NotTerminal)stack.pop()).getNode();//quita expresion
		stack.pop();
		stack.pop();//quita =
		stack.pop();
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//id = new Id(((Terminal)stack.pop()).getSymbol());//quita id
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
		//other = ((NotTerminal)stack.pop()).getNode();
		stack.pop();
		sentenceBlock = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//sentenceBlock = ((NotTerminal)stack.pop()).getNode();//quita sentencia bloque
		stack.pop();
		stack.pop(); //quita )
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression = ((NotTerminal)stack.pop()).getNode(); //quita exprecion
		stack.pop();
		stack.pop(); //quita (
		stack.pop();
		stack.pop(); //quita if
	}
};

class While : public Node //<Sentencia> ::= while ( <Expresion> ) <Bloque> 
{
	Node* expression;
	Node* block;

public:
	While(std::stack<StackElement*>& stack)
	{
		stack.pop();
		block = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//block = ((NotTerminal)stack.pop()).getNode();//quita bloque
		stack.pop();
		stack.pop(); //quita )
		stack.pop();
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression = ((NotTerminal)stack.pop()).getNode();//quita expresion
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
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression = ((NotTerminal)stack.pop()).getNode();//quita exprecion
		stack.pop();
		stack.pop();//quita (
		stack.pop();
		stack.pop();//quita el while
		stack.pop();
		block = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//block = ((NotTerminal)stack.pop()).getNode();//quita bloque
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
		sentenceBlock = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//sentenceBlock = ((NotTerminal)stack.pop()).getNode();//quita senteciabloque
		stack.pop();
		expression3 = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression3 = ((NotTerminal)stack.pop()).getNode();//quita expresion
		stack.pop();
		stack.pop();//quita ;
		stack.pop();
		expression2 = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression2 = ((NotTerminal)stack.pop()).getNode();//quita expresion
		stack.pop();
		expression1 = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression1 = ((NotTerminal)stack.pop()).getNode();//quita expresion
		stack.pop();
		stack.pop();//quita =
		stack.pop();
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//id = new Id(((Terminal)stack.pop()).getSymbol());//quita id
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
		expression = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//expression = ((NotTerminal)stack.pop()).getNode();//quita exprecion
		stack.pop();
		stack.pop();//quita return
	}
};

class Constant : public Node
{
public:
	Constant(std::string symbol)
	{
		this->symbol = symbol;
		classType = "cons";
	}
};

class FuncCall : public Node//<LlamadaFunc> ::= id ( <Argumentos> )
{
	Node* id;
	Node* arguments;

public:
	FuncCall(std::stack<StackElement*>& stack)
	{
		classType = "fun";
		stack.pop();
		stack.pop();//quita )
		stack.pop();
		arguments = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//arguments = ((NotTerminal)stack.pop()).getNode();//quita exprecion
		stack.pop();
		stack.pop();//quita (
		stack.pop();
		id = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//id = new Id(((Terminal)stack.pop()).getSymbol());//quita id
	}
};

class Operation1 : public Node//<Expresion> ::= opSuma <Expresion> 
{
	Node* right;

public:
	Operation1(std::stack<StackElement*>& stack)
	{
		stack.pop();
		right = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//right = ((NotTerminal)stack.pop()).getNode();//quita exprsion
		stack.pop();
		symbol = dynamic_cast<Terminal*>(stack.top())->getSymbol();
		stack.pop();
		//symbol = ((Terminal)stack.pop()).getSymbol();//quita el operador
	}
};

class Operation2 : public Node
{
	Node* right;
	Node* left;

public:
	Operation2(std::stack<StackElement*>& stack)
	{
		stack.pop();
		right = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//right = ((NotTerminal)stack.pop()).getNode();//quita exprsion
		stack.pop();
		symbol = dynamic_cast<Terminal*>(stack.top())->getSymbol();
		stack.pop();
		//symbol = ((Terminal)stack.pop()).getSymbol();//quita el operador
		stack.pop();
		left = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//left = ((NotTerminal)stack.pop()).getNode();//quita expresion
	}
};
