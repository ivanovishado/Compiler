#include "Defs.h"
#include <string>

int Node::labelNum = 0;
std::string Node::varglobal = "";
std::string DefFunc::localVar = "";

std::string Node::nextLabel()
{
	return "E" + std::to_string(labelNum++);
}

std::string If::generateCode()
{
	std::string label = nextLabel();
	std::string endLabel = nextLabel();
	std::string code = expression->generateCode() + label;
	if (sentenceBlock)
		code += sentenceBlock->generateCode();
	code += "\n\tjmp " + endLabel + "\n" + label + ":";
	if (other)
		code += other->generateCode() + "\n" + endLabel + ":";
	if (next)
		return code += next->generateCode();
	return code;
}