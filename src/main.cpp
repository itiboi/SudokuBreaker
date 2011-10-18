//
// Copyright (c) Tim Bolender 2008-2011 <itiboi@hotmail.de>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <global.h>
#include <Sudoku.h>

#include <iostream>
#include <time.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <stdlib.h>

using namespace std;

void PrintSyntax(void);

int main(int argc, char* args[])
{
	// Zufallsfunktion initialisieren
	srand((unsigned)time(NULL));
	// Sudoku erstellen und Eingeschaften festlegen
	Sudoku Sudoku( false );
	// Info ausgeben
	printf(ABOUT, TITLE, VERSION, YEAR, AUTHOR);
	cout << '\n' << '\n';
	
	// Argumente auswerten
	if( argc == 1 || !strcmp(args[1], "-h") ) {
		PrintSyntax();
		return 0;
	}
	
	string sudoku = "";
	char* output = NULL;
	
	for( int i = 1; i < argc; i++ ) {
		if( !strcmp( args[i], "-o" ) ) {
			if( ++i < argc )
				output = args[i];
			else {
				cerr << "ERR : Fehlender Datei-Parameter\n";
				PrintSyntax();
				return -1;
			}
		}
		else {
			if( sudoku != "" ) {
				cerr << "ERR : Ungueltiger Parameter\n";
				PrintSyntax();
				return -1;
			}
			sudoku = args[i];
		}
	}
	
	// Sudoku laden
	if( !Sudoku.Laden( sudoku ) ) {
		cerr << "ERR : Der Sudokuparameter ist ungueltig\n";
		return 1;
	}
	
	cout << "INFO: Das unbearbeitete Sudoku:\n";
	cout << Sudoku.GetSudoku().c_str();
	
	// Sudoku lösen
	cout << "INFO: Loese Sudoku...\n";
	if( Sudoku.Break() )
		cout << "INFO: Loesen erfolgreich\n";
	else {
		cout << "INFO: Loesen fehlgeschlagen\n";
		return 2;
	}
	
	// ggf Sudoku schreiben
	if( output )
		if( !Sudoku.Schreiben( output ) )
			cerr << "ERR : Schreiben fehlgeschlagen\n";
	
	//Sudoku ausgeben
	cout << "\nINFO: Das bearbeitete Sudoku:\n";
	cout << Sudoku.GetSudoku().c_str();
	
	return 0;
}

void PrintSyntax(void)
{
	char Vorschub[] = "    ";
	cout << "Parameter:" << '\n';
	cout << Vorschub << "SudokuBreaker.exe <Sudoku> [-o <Ausgabe-Datei>]" << '\n' << '\n';
	cout << Vorschub << "-o   Stellt die Ausgabe-Datei dar, in der das Ergebnis gespeichert wird" << '\n';
	cout << Vorschub << "-h   Gibt diesen Dialog aus" << '\n';
}
