#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"

/*
NAME

insetMemory - inserts the contents of a line into memory

SYNOPSIS

bool insertMemory(int a_location, long long a_contents);
	a_location		-> location currently being used
	a_contents		-> the contents of the line operand 

DESCRIPTION

Aquires the location and contents of a line and inserts it into memory for later use

RETURNS

true if the memory can be input. False if the memory exceeds the max memory
*/
bool emulator::insertMemory(int a_location, long long a_contents)
{
	if (a_location >= MEMSZ) { // If we have exceeded to maximum memory size then we cannot add the location to memory.
		return false;
	}
	// Otherwise add the opcode and the address at the specified location in memory
	m_memory[a_location] = a_contents;
	return true;
}

/*
NAME

runProgram - runs the program inside the emulator

SYNOPSIS

bool runProgram();

DESCRIPTION

Using the information gathered in previously from the contents of the source file runs an emulator to allow the user to execute the lines input 

RETURNS 

True unless the location of the contents is past the mem size allocated 
*/
bool emulator::runProgram()
{
	int loc = 100; // First location in memory
	bool done = false; // While this is false run the program
	m_acc = 0;
	while (!done) {
		if (loc >= 10000) { // If we have exceeded past memory size report error and terminate!
			string msg = "Location exceeded past memory size, emulator terminated.";
			Errors::RecordError(msg);
			Errors::DisplayErrors();
			return false;
		}
		int inst = m_memory[loc]; // location of instruction
		int opcode = inst / 10000; // opcode to process
		int address = inst % 10000; // address of instruction
		switch (opcode) {
		case 1: // ADD: Add the value to the accumulator
			m_acc += m_memory[address];
			loc++;
			break;
		case 2: // SUBTRACT: Subtract from the accumulator
			m_acc -= m_memory[address];
			loc++;
			break;
		case 3: // MULT: Multiply the values together
			m_acc *= m_memory[address];
			loc++;
			break;
		case 4: // DIV: Divide the values together. If dividing by zero report error and continue.
			m_acc /= m_memory[address];
			if (m_memory[address] == 0) {
				string msg = "Cannot divide by zero";
				Errors::RecordError(msg);
			}
			loc++;
			break;
		case 5: // LOAD: Set the accumulator equal to the value in memory
			m_acc = m_memory[address];
			loc++;
			break;
		case 6: // STORE: Store accumulator contents in address
			address = m_acc;
			loc++;
			break;
		case 7: // READ: Ask the user to enter a number which is placed in memory
			cout << "?";
			cin >> m_memory[address];
			loc++;
			break;
		case 8: // WRITE: Display the number to the screen
			cout << m_acc << endl;
			loc++;
			break;
		case 9: // B: Loop back to given label
			loc = address;
			break;
		case 10: // BM: Loop back if accumulator has a negative value
			if (m_acc < 0) loc = address;
			else loc++;
			break;
		case 11: // BZ: Loop back if the accumulator has the value of zero
			if (m_acc == 0) loc = address;
			else loc++;
			break;
		case 12: // BP: Loop back if the accumulator has a positive value
			if (m_acc > 0) loc = address;
			else loc++;
			break;
		case 13: // HALT: Terminate execution
			done = true;
			break;
		default: // Must handle the default case. In this case it will be to display an error message from emulation.
			string msg = "Error reported during emulation";
			Errors::RecordError(msg);
			break;
		}
	}
	Errors::DisplayErrors();
	return true;
}
