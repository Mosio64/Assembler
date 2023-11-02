#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"


/*
NAME

ParseInstruction - pareses a line and gets the instruction type

SYNOPSIS

Instruction::InstructionType ParseInstruction(string a_line)
	a_line		-> Contents of the entire line from the source file

DESCRIPTION

This function determines the type of instruction contained in the line and populated the values of the line into variables for the label opCode and Operand

RETURNS

The type of instruction
*/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
	//Machine language instrcutions
	string inst_ops[]{ "ADD", "add", "SUB", "sub", "MULT", "mult", "DIV", "div", "LOAD", "load", "STORE", "store", "READ", "read", "WRITE", "write", "B", "b", "BM", "bm", "BZ", "bz", "BP", "bp", "HALT", "halt" };
	//Assembler instructions
	string assem_ops[]{ "DC", "dc", "DS", "ds", "ORG", "org" };

	m_instruction = a_line; //original line for printing 
	if (m_instruction.at(0) == ';' || m_instruction.empty()) {
		return ST_Comment;
	}
	if (!m_Label.empty() && m_OpCode.empty()) {
		string msg = "Label and OpCode are empty";
		Errors::RecordError(msg);
	}
	int j = 0;
	while (m_instruction[j] == ' ' && j != m_instruction.length()) { // Remove all trailing spaces
		j++;
	}
	if (m_instruction.substr(j, 3) == "end") {
		return InstructionType::ST_End;
	}
	if (m_instruction.substr(j, 4) == "halt") {
		m_OpCode = "halt";
		return InstructionType::ST_MachineLanguage;
	}
	int is = a_line.find(";");//Removes all characters past and including ;
	if (is != string::npos) {
		a_line.erase(is);
	}
	
	istringstream input(a_line);
	string tmp1, tmp2, tmp3, tmp4;
	input >> tmp1 >> tmp2 >> tmp3 >> tmp4;
	if (a_line[0] == ' ') { // If the beginning has a space then there is no label
		m_Label = "";
		m_OpCode = tmp1;
		m_Operand = tmp2;

		if (!tmp3.empty()) {
			string msg = "Error: tmp3 is not empty";
			Errors::RecordError(msg);
		}
	}
	else {
		m_Label = tmp1;
		m_OpCode = tmp2;
		m_Operand = tmp3;

		if (!tmp4.empty()) {
			string msg = "Error: tmp4 is not empty";
			Errors::RecordError(msg);
		}
	}
	if (find(begin(assem_ops), end(assem_ops), m_OpCode) != end(assem_ops)) { // Assembler instruction
		m_type = ST_AssemblerInstr;
	}
	else if (find(begin(inst_ops), end(inst_ops), m_OpCode) != end(inst_ops)) { // Machine Language Instruction
		m_type = ST_MachineLanguage;
	}
	else {
		string msg = "Error: Invalid/Illegal Instructions";
		Errors::RecordError(msg);
	}
	return m_type;
}

	
/*
NAME

LocationNextInstruction - updates the location

SYNOPSIS

int LocationNextInstruction(int a_loc);
	a_loc		-> current location being used

DESCRIPTION

This function updates the location to be used in the program based on the opCode and previous location

RETURNS

The next loction to be used in memory
*/
int Instruction::LocationNextInstruction(int a_loc)
{
	if (m_OpCode == "DS" || m_OpCode == "ds") { // Each time, return the base case, loc + 1 if the operand is not numeric
		for (int i = 0; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc + 1;
		}
		return a_loc + stoi(m_Operand);
	}
	if (m_OpCode == "ORG" || m_OpCode == "org") {
		for (int i = 1; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc + 1;
		}
		return a_loc + stoi(m_Operand);
	}
	if (m_OpCode == "DC" || m_OpCode == "dc") {
		for (int i = 0; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc;
		}
	}
	// Otherwise add 1 to the location
	return a_loc + 1;
}

/*
NAME

computeNumOpCode - establishes the value of the OpCode

SYNOPSIS

int ComputeNumOpCode();

DESCRIPTION

Computes the corresponding digit for the OpCode that is currently being processed

RETURNS

The value of the OpCode
*/
int Instruction::computeNumOpCode() {
	if (m_OpCode == "ADD" || m_OpCode == "add") {
		m_NumOpCode = 1; // ADD
	}
	else if (m_OpCode == "SUB" || m_OpCode == "sub") {
		m_NumOpCode = 2; // SUBTRACT
	}
	else if (m_OpCode == "MULT" || m_OpCode == "mult") {
		m_NumOpCode = 3; // MULT
	}
	else if (m_OpCode == "DIV" || m_OpCode == "div") {
		m_NumOpCode = 4; // DIV
	}
	else if (m_OpCode == "LOAD" || m_OpCode == "load") {
		m_NumOpCode = 5; // LOAD
	}
	else if (m_OpCode == "STORE" || m_OpCode == "store") {
		m_NumOpCode = 6; // STORE
	}
	else if (m_OpCode == "READ" || m_OpCode == "read") {
		m_NumOpCode = 7; // READ
	}
	else if (m_OpCode == "WRITE" || m_OpCode == "write") {
		m_NumOpCode = 8; // WRITE
	}
	else if (m_OpCode == "B" || m_OpCode == "b") {
		m_NumOpCode = 9; // B
	}
	else if (m_OpCode == "BM" || m_OpCode == "bm") {
		m_NumOpCode = 10; // BM
	}
	else if (m_OpCode == "BZ" || m_OpCode == "bz") {
		m_NumOpCode = 11; // BZ
	}
	else if (m_OpCode == "BP" || m_OpCode == "bp") {
		m_NumOpCode = 12; // BP
	}
	else if (m_OpCode == "HALT" || m_OpCode == "halt") {
		m_NumOpCode = 13; // HALT
	}
	else {
		m_NumOpCode = 0; // This means it has to be an assembler instruction
	}
	return m_NumOpCode;
}
