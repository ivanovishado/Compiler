#include "SyntaxAnalyzer.h"
#include "Exceptions.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::string& filename) : lrTable(LR_TABLE_ROWS,
														std::vector<int>(LR_TABLE_COLUMNS)),
														lex(filename)
{
	auto lrTableFile = openFile(LR_TABLE_FILENAME);

	for (int i = 0; i < LR_TABLE_ROWS; i++)
	{
		for (int j = 0; j < LR_TABLE_COLUMNS; j++)
			lrTableFile >> lrTable[i][j];
	}

	auto rulesFile = openFile(RULES_INFO_FILENAME);

	int id, productionsAmount;
	while (rulesFile >> id >> productionsAmount)
		rulesInfo.push_back({ id, productionsAmount * 2 });
}

void SyntaxAnalyzer::analyze()
{
	stack.push(new State(0));
	bool canRead = true;
	int lexID;
	while (true)
	{
		if (canRead)
			lexID = lex.nextSymbol();
		//int action = lrTable[stack.top()][lexID];
		int action = lrTable[dynamic_cast<State*>(stack.top())->getStateNum()][lexID];
		if (action == 0)
			throw SyntaxException("The LR table threw an error.");
		if (action > 0)
		{
			shift(lexID, action);
			canRead = true;
		}
		else if (action == -1)
		{
			std::cout << "Syntactically valid!\n";
			break; // Accepted
		}	
		else
		{
			reduce(action);
			canRead = false;
		}
	}
}

void SyntaxAnalyzer::shift(int lexID, int action)
{
	// pila.push(NotTerminal (nodo))
	//stack.push(action);
	//stack.push(new NotTerminal(read));
	stack.push(new Terminal(lex.getSymbol())); // ya no es necesario el lexID?
	stack.push(new State(action));
}

void SyntaxAnalyzer::reduce(int action)
{
	//int rule = -(action + 2);
	/*int rule = abs(action + 2); siempre será negativo el resultado?
	for (int i = 0; i < rulesInfo[rule].second; i++)
		stack.pop();
	int ruleID = rulesInfo[rule].first, state = stack.top();
	stack.push(ruleID);
	stack.push(lrTable[state][ruleID]);*/

	int rule = -(action + 2);
	/*Node* node = new Node();
	node = nullptr;*/
	Node* node = nullptr;
	Node* aux;

	std::cout << "Rule= " << rule << '\n';

	switch (rule + 1)
	{

	//case 1:	//<programa> ::= <Definiciones> 
		//node=new  programa(stack);
		//			break;

	case 3:		//<Definiciones> ::= <Definicion> <Definiciones>
	case 16:	//<DefLocales> ::= <DefLocal> <DefLocales>
	case 20:	//<Sentencias> ::= <Sentencia> <Sentencias>
	case 32:	//<Argumentos> ::= <Expresion> <ListaArgumentos>
		stack.pop();//quita estado
		aux = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//Node* aux = ((NotTerminal)stack.pop()).node;//quita <definiciones>
		stack.pop();//quita estado
		node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//node = ((NotTerminal)stack.pop()).node;//quita <definicion>
		node->setNext(aux);
		break;
	case 1:
	case 4://<Definicion> ::= <DefVar>
	case 5://<Definicion> ::= <DefFunc> 
	case 17://<DefLocal> ::= <DefVar> 
	case 18://<DefLocal> ::= <Sentencia> 
	case 35://<Atomo> ::= <LlamadaFunc> 
	case 39://<SentenciaBloque> ::= <Sentencia> 
	case 40://<SentenciaBloque> ::= <Bloque> 
	case 50:	//<Expresion> ::= <Atomo>
		stack.pop();//quita estado
		//node = ((NotTerminal)stack.pop()).node;//quita defvar
		node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		break;
	case 6:// <DefVar> ::= tipo id <ListaVar> ;
		node = new DefVar(stack);
		break;

	case 8: //<ListaVar> ::= , id <ListaVar>
	{
		stack.pop();//quita estado
		Node* lvar = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//Node lvar = ((NotTerminal)stack.pop()).node;
		stack.pop();//quita estado
		node = new Id(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		//node = new Id(((Terminal)stack.pop()).simbolo);//quita id
		//node.next = lvar;
		node->setNext(lvar);
		stack.pop();//quita estado
		stack.pop();//quita la coma
		break;
	}

	case 9://<DefFunc> ::= tipo id ( <Parameters> ) <BloqFunc>
		node = new DefFunc(stack);
		break;

	case 11://<Parameters> ::= tipo id <ListaParam>
		node = new Parameters(stack);
		break;

	case 13://<ListaParam> ::= , tipo id <ListaParam>
		node = new Parameters(stack);
		stack.pop();//quita estado;
		stack.pop();//quita la coma
		break;

	case 14://<BloqFunc> ::= { <DefLocales> }
	case 30://<Bloque> ::= { <Sentencias> } 
	case 41://<Expresion> ::= ( <Expresion> ) 
		stack.pop();//quita estado
		stack.pop();//quita }
		stack.pop();//quita estado
		node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//node = ((NotTerminal)stack.pop()).node;//quita <deflocales> o <sentencias>
		stack.pop();
		stack.pop();//quita la {
		break;

	case 21: //<Sentencia> ::= id = <Expresion> ;
		node = new Asignment(stack);
		break;

	case 22://<Sentencia> ::= if ( <Expresion> ) <SentenciaBloque> <Otro>
		node = new If(stack);
		break;

	case 23://<Sentencia> ::= while ( <Expresion> ) <Bloque> 
		node = new While(stack);
		break;

	case 24://<Sentencia> ::= do <Bloque> while ( <Expresion> ) ;
		node = new Dowhile(stack);
		break;

	case 25://<Sentencia> ::= for id = <Expresion> : <Expresion> : <Expresion> <SentenciaBloque>
		node = new For(stack);
		break;

	case 26://<Sentencia> ::= return <Expresion> ;
		node = new Return(stack);
		break;

	case 27://<Sentencia> ::= <LlamadaFunc> ;
		stack.pop();
		stack.pop();//quita ;
		stack.pop();
		node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//node = ((NotTerminal)stack.pop()).node;//quita llamadafunc
		break;

	case 29://<Otro> ::= else <SentenciaBloque> 
		stack.pop();
		node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//node = ((NotTerminal)stack.pop()).node;//quita sentencia bloque
		stack.pop();
		stack.pop();//quita el else
		break;

	case 34:// <ListaArgumentos> ::= , <Expresion> <ListaArgumentos> 

		stack.pop();
		aux = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//aux = ((NotTerminal)stack.pop()).node;//quita la lsta de argumentos
		stack.pop();
		node = dynamic_cast<NotTerminal*>(stack.top())->getNode();
		stack.pop();
		//node = ((NotTerminal)stack.pop()).node;//quita expresion
		stack.pop();
		stack.pop();//quita la ,
		//node.next = aux;
		node->setNext(aux);
		break;

	/*case 37: // <Atomo> :: = constante
		stack.pop();
		node = new Constant(dynamic_cast<Terminal*>(stack.top())->getSymbol());
		stack.pop();
		break;*/

		//aqui cae R2,R7,R10,R12,R15,R19,R28,R31,R33
	default:
		for (int i = 0; i < rulesInfo[rule].second; i++)
			stack.pop();
	}

	//textBox1.Text=Convert.ToString(((State)stack.top()).numestado);
	int row = dynamic_cast<State*>(stack.top())->getStateNum();
	//row=Convert.ToInt32(stack.top());

	int column = rulesInfo[rule].first;
	int transition = lrTable[row][column];
	NotTerminal* nt = new NotTerminal(rulesInfo[rule].first);
	nt->setNode(node);
	stack.push(nt);
	stack.push(new State(transition));
	//textBox1.Text=Convert.ToString(stack.top());
	//action = lrTable[transition][(int)lex.tiposimlr];
}
