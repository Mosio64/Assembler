//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "FileAccess.h"
#include "SymTab.h"
#include "Instruction.h"
#include "Emulator.h"

class Assembler {

public:
    Assembler(int argc, char* argv[]);
    ~Assembler();

    // Pass I - establish the locations of the symbols
    void PassI();

    // Pass II - generate a translation
    void PassII();

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }

    // Run emulator on the translation.
    void RunProgramInEmulator();

    //Prints additional zeros for even number of digits in contents when a halt is called
    void ConvertNumToASC();

    //Reports an error if operand is not numeric
    void AssembInstruction(int& a_loc);

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
};

