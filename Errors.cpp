#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/*
NAME

InitErrorReporting - clears the vector of errors

SYNOPSIS

void InitErrorReproting();

DESCRIPTION

Iterates through the vector of erros and clears all of them

*/
void Errors::InitErrorReporting() {
	for (int i = 0; i < m_ErrorMsgs.size(); i++) {
		m_ErrorMsgs.at(i) = "";
	}
}

/*
NAME

RecordError - pushes an error into a stack

SYNOPSIS

void RecordError(string a_emsg);
	a_emsg		-> The message to be recorded as an error

DESCRIPTION

Takes an error that is passed to it and pushes it into a stack to be printed later

*/
void Errors::RecordError(string a_emsg)
{
	m_ErrorMsgs.push_back(a_emsg);
}

/*
NAME

DisplayErrors - prints errors

SYNOPSIS

void DisplayErrors();

DESCRIPTION

Goes through the stack of errors that have been gathered and prints all of them.

*/
void Errors::DisplayErrors()
{
	cout << "Errors Collected: ";
	if (m_ErrorMsgs.size() == 0) {
		cout << "None" << endl;
		return;
	}
	cout << endl;

	for (int i = 0; i < m_ErrorMsgs.size(); i++) {
		cout << "Error #" << i + 1 << ": " << m_ErrorMsgs.at(i) << endl;
	}
}
