//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
        a_symbol	-> The name of the symbol to be added to the symbol table.
        a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void SymbolTable::AddSymbol(const string& a_symbol, int a_loc)
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st != m_symbolTable.end()) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Records the location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
NAME

DisplaySymbolTable - prints symbol table

SYNOPSIS

void DisplaySymbolTable();

DESCRIPTION

This function is used to display the contents of the symbol table 

*/
void SymbolTable::DisplaySymbolTable()
{
    map<string, int>::iterator mp;
    cout << "Symbol #  " << "Symbol  " << "Location" << endl;
    int symCount = 0;
    for (mp = m_symbolTable.begin(); mp != m_symbolTable.end(); mp++) {
        cout << symCount << "\t  " << mp->first << setw(10) << mp->second << endl;
        symCount++;
    }
}

/*
NAME

LookupSymbol- ensures a symbol exists at a specified location

SYNOPSIS

bool LookupSymbol(const string &a_symbol, int &a_loc);
    a_symbol	-> The name of the symbol added to the symbol table.
    a_loc		-> the location to be associated with the symbol.

DESCRIPTION

This function checks to make sure the symbol that was added to the symbol table is at the correct locaiton 

RETURNS

True if the symbol is in the correct location and false if it is not
*/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    if (m_symbolTable[a_symbol] == a_loc) {
        return true;
    }
    return false;
}

/*
NAME

GetValueForKey - finds the location of a symbol

SYNOPSIS

int GetValueForKey(const string& a_symbol)
    a_symbol        -> The name of the symbol in the symbol table

DESCRIPTION

This function acquires the location of the symbol in the symbol table 

RETURNS

The location associated to the symbol in the symbol table
*/
int SymbolTable::GetValueForKey(const string& a_symbol) {
    return m_symbolTable[a_symbol]; // Return the value corresponding to the symbol given
}
