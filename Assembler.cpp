//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

using namespace std;

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv)
{
    // Nothing else to do here at this point.
}
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler()
{
}
/*
NAME

PassI - establishes label positions

SYNOPSIS

void PassI();

DESCRIPTION

This fucntion iterates through the source file and adds symbols to a map and establishes label positions to the same map for printing later

*/
void Assembler::PassI()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (; ; ) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::ST_End) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if (st == Instruction::ST_Comment)
        {
            continue;
        }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {

            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}

/*
NAME

PassII - outputs a tanslation and symbol table 

SYNOPSIS

void PassII();

DESCRIPTION

This function iterates through the source file and gathers all of the lines. It then generates a translation containing the location and contents as well the original statements and any errors found.

*/
void Assembler::PassII()
{
    string msg;
    m_facc.rewind();
    Errors::InitErrorReporting();
    int loc = 0;
    cout << "Location   " << "Contents   " << "Original Statement" << endl;
    string newBuff = "";
    string m_Oprd = m_inst.GetOperand(); // Operand
    string m_Op = m_inst.GetOpCode(); // Opcode
    int m_NumOp = m_inst.GetNumOpCode(); // NumOpCode

    for (; ;) {
        //Then this should read the next line from the source file.
        string buff; // the intstruction for the code.
        if (!m_facc.GetNextLine(buff)) {
            msg = "No End Statement Exists";
            //If there are no more existing lines, error will be reported that the end statement is missing.
            Errors::RecordError(msg);
            //Errors::DisplayErrors();
            return;
        }
        //Parse the line and get the intrusction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(buff);
        // If this is the end statement, break out of the loop and check if there are more lines after the statement.
        if (st == Instruction::InstructionType::ST_End) {
            cout << setw(25) << buff << endl;
            newBuff = buff;
            break;
        }

        //If there is a comment, print it out and continue
        else if (st == Instruction::InstructionType::ST_Comment) {
            cout << "                      " << buff << endl;
            continue;
        }

        //If it appears to be machine language instructions then report an error if the operand is numeric.
        else if (st == Instruction::InstructionType::ST_MachineLanguage) {
            AssembInstruction(loc);

        }
        //The DC operand assumes everything an integer, cant work with strings.
        
        else if (m_Op == "DC" || m_Op == "dc") {
            string currentOp = m_inst.GetOpCode();
            int currentOprd = stoi(m_inst.GetOperand());
            //If enter a string, program will terminate. m_NumCode ==0
            for (int i = 0; i < m_Oprd.length(); i++) {
                if (!(isdigit(m_Oprd.at(i)))) {
                    m_Oprd = "Undefined";
                    break;
                }
            }
            if (m_Oprd == "Undefined" || m_Oprd.empty()) {
                msg = "Error: DC operand must be numeric. ";
                cout << "?" << setw(16) << right << m_inst.GetOrgInst() << msg << endl;
                Errors::RecordError(msg);
                continue;
            }
            
            if (currentOp == "org" || currentOp == "ds") {
                cout << loc << "                     " << setw(5) << right << m_inst.GetOrgInst();
            }
            else if (currentOp == "dc") {
                cout << loc << "        ";
                ConvertNumToASC();
                cout << currentOprd << "       " << setw(5) << right << m_inst.GetOrgInst();
            }
            if (!m_emul.insertMemory(loc, stoi(m_Oprd))) {
                msg = " Error: Location hit the max for VC407 Memory";
                cout << msg;
                Errors::RecordError(msg);
            }
        }

        cout << endl;
        //Next is to compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
    if (m_facc.GetNextLine(newBuff)) {
        msg = "Error: There should be no lines after the end instructions";
        //If there is more after the instruction end, report error
        Errors::RecordError(msg);
    }
    Errors::DisplayErrors();
}

/*
NAME

RunInEmulator - runs the program by loading up the accumulator

SYNOPSIS

void RunInEmulator();

DESCRIPTION

This function runs the source code within the file and take input from the user to perform the actions requested

*/
void Assembler::RunProgramInEmulator()
{
    m_emul.runProgram();
}

/*
NAME

ConvertNumToASC - adds the correct number of zeros to the end of the contents during PassI

SYNOPSIS

void ConvertNumToASC(int num, string &value);

DESCRIPTION

Retrieves the operand and prints the corresponding number of zeros to ensure the contents value is six digits 

*/
void Assembler::ConvertNumToASC() {
    int loc = 0;
    string m_Operd = m_inst.GetOperand(); //Operand
    if (stoi(m_Operd) < 10) cout << "00000";
    else if (stoi(m_Operd) < 100) cout << "0000";
    else if (stoi(m_Operd) < 1000) cout << "000";
    else if (stoi(m_Operd) < 10000) cout << "00";
    else if (stoi(m_Operd) < 100000) cout << "0";
    else if ((stoi(m_Operd) < 999999)) {//Report error if greater
        cout << "???????";
        string msg = "Error: Invalid constant";
        Errors::RecordError(msg);
        loc = m_inst.LocationNextInstruction(loc);
    }
    return;
}

/*
NAME

AssembInstruction - reports an error is the operand is not numeric

SYNOPSIS

void AssembInstruction(int &a_loc);
    a_loc   -> the location in memory currently

DESCRIPTION

Retrieves the variables of the line for the operand and opcode and reports an error if the operand is not numeric

*/
void Assembler::AssembInstruction(int& a_loc) {
    int loc = a_loc;
    string msg;
    cout << loc << "        ";
    string m_Operd = m_inst.GetOperand(); //Operand
    string m_Op = m_inst.GetOpCode(); //OpCode
    int m_NumOp = m_inst.GetNumOpCode(); //NumOpCode

    if (m_NumOp < 10)cout << "0";
    cout << m_NumOp;

    for (int i = 0; i < m_Operd.length(); i++) {
        if (isdigit(m_Operd.at(i))) {
            cout << "????";
            msg = "Invalid Operand Choice";
            Errors::RecordError(msg);
        }
    }
    int value = m_symtab.GetValueForKey(m_Operd);
    int symbolVal = m_symtab.GetValueForKey(m_inst.GetLabel());
    if (symbolVal == -999) { // If the symbol already exists report error for duplicate symbol.
        msg = "Duplicate Defined Symbol";
        cout << "???" << msg;
        Errors::RecordError(msg);

    }
    else if (value < 1000)cout << "0";
    if (value == 0 && m_Op != "HALT" && m_Op != "halt") {
        cout << "?? , Undefined Symbol: ";
        msg = "Error: Undefined Symbol";
        Errors::RecordError(msg);
    }

    else if (m_Op == "HALT" || m_Op == "halt")  cout << "000";
    else cout << value;
    cout << "     " << setw(5) << right << m_inst.GetOrgInst();
    for (int i = 0; i < m_Operd.length(); i++) {
        if (isdigit(m_Operd.at(i))) {
            msg = "Error: Operand must be symbolic";
            cout << msg;
            Errors::RecordError(msg);
        }
    }
    //If it is not a  halt/either the operand or opcode are empty then report error 
    if (m_Op != "HALT" && (m_Operd.empty() || m_inst.GetOpCode().empty())) {
        msg = "Error: Invalid Instructions";
        cout << msg;
        Errors::RecordError(msg);

    }
    //Find the location of instructions in the memory.
    int content;
    if (m_Op == "HALT" || m_Op == "halt") content = stoi(to_string(m_NumOp) + "0000");

    else if (m_symtab.GetValueForKey(m_Operd) < 1000) {
        content = stoi(to_string(m_NumOp) + "0" + to_string(m_symtab.GetValueForKey(m_Operd)));
    }
    else content = stoi(to_string(m_NumOp) + to_string(m_symtab.GetValueForKey(m_Operd)));
    if (!m_emul.insertMemory(loc, content)) {
        // If location value exceeds 10000 then report error location does not fit memory
        msg = "Error: Location hit a max for VC407 Memory";
        cout << msg;
        Errors::RecordError(msg);
    }
}