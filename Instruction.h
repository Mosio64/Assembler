//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction() {
        m_NumOpCode = 0;
        m_OpCode = "";
        m_IsNumericOperand = true;
        m_Label = "";
        m_OperandValue = 0;
        m_instruction = "";
        m_Operand = "";
    };
    ~Instruction() { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End                  // end instruction.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    //Computes the NumOpCode
    int computeNumOpCode();

    // To access the label
    inline string& GetLabel() {

        return m_Label;
    };

    //To access the OpCode as an int
    inline int& GetNumOpCode() {
        m_NumOpCode = computeNumOpCode();
        return m_NumOpCode;
    };

    //To access the OpCode
    inline string& GetOpCode() {
        return m_OpCode;
    };

    //To access the operand
    inline string& GetOperand() {
        return m_Operand;
    };

    //To access the original instruction
    inline string& GetOrgInst() {
        return m_instruction;
    };
    // To determine if a label is blank.
    inline bool isLabel() {

        return !m_Label.empty();
    };

private:


    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand;      // The operand. 

    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand;// == true if the operand is numeric.
    int m_OperandValue;    // The value of the operand if it is numeric.

};


