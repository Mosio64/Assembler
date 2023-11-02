//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

// Don't forget to comment the function headers.
FileAccess::FileAccess(int argc, char* argv[])
{
    // Check that there is exactly one run time parameter.
    if (argc != 2) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit(1);
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open(argv[1], ios::in);

    // If the open failed, report the error and terminate.
    if (!m_sfile) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit(1);
    }
}
FileAccess::~FileAccess()
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close();
}
// Get the next line from the file.
/*
NAME

GetNextLine - retrieves the next line from the file

SYNOPSIS

bool GetNextLine(string& a_line);
    a_line      -> Variable to store the contents of the entire line

DESCRIPTION

This function retrieves the next line inside of the file

RETURNS

False if the end of the file has been reached. True if there was a line to retrive and stores that line in memory
*/
bool FileAccess::GetNextLine(string& a_line)
{
    // If there is no more data, return false.
    if (m_sfile.eof()) {

        return false;
    }
    getline(m_sfile, a_line);

    // Return indicating success.
    return true;
}
/*
NAME

rewind - returns to the beginning of the file

SYNOPSIS

void rewind();

DESCRIPTION

Returns to the beginning of the file that was supplied so that PassII can be completed

*/
void FileAccess::rewind()
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg(0, ios::beg);
}

