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
	stack.push(0);
	bool canRead = true;
	int lexID;
	while (true)
	{
		if (canRead)
			lexID = lex.nextSymbol();
		int action = lrTable[stack.top()][lexID];
		if (action == 0)
			throw SyntaxException("The table threw an error.");
		if (action > 0)
		{
			desplazamiento(lexID, action);
			canRead = true;
		}
		else if (action == -1)
		{
			std::cout << "Syntactically valid!\n";
			break; // Accepted
		}	
		else
		{
			reduction(action);
			canRead = false;
		}
	}
}

void SyntaxAnalyzer::desplazamiento(int read, int action)
{
	stack.push(read);
	stack.push(action);
}

void SyntaxAnalyzer::reduction(int action)
{
	//int rule = -(action + 2);
	int rule = abs(action + 2);
	for (int i = 0; i < rulesInfo[rule].second; i++)
		stack.pop();
	int ruleID = rulesInfo[rule].first, state = stack.top();
	stack.push(ruleID);
	stack.push(lrTable[state][ruleID]);
}
