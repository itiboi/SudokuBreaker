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

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "global.h"
#include "CSudoku.h"

using namespace std;

// Konstruktor
CSudoku::CSudoku( bool quiet ) : Silent(quiet) {
}

void CSudoku::PrintInfo( string text ) {
	if( !Silent )
		cout << text.c_str();
};

// Löscht die Möglichkeiten des übergebenen Feldes
void CSudoku::Kill( byte punkt, byte zahl ) {
	byte quad = (punkt/27)*3 +   /* "Reihe" des Quadrats */ \
				(punkt%27%9)/3 /* Abstand zum Rand */;
	byte point = (quad/3)*27 + /* Erstes Feld der Reihe des Quadrats */ \
				 (quad%3)*3;   /* Abstand zum Rand */
	for( byte i = 0; i < 9; i++, point++ ) {
		// errechnung neuen punktes
		Felder[punkt].Possibilities[i] = false;
		Felder[(punkt%9) + i*9].Possibilities[zahl-1] = false; // vertikal
		Felder[(punkt/9)*9 + i].Possibilities[zahl-1] = false; // horizontal
		Felder[point].Possibilities[zahl-1] = false;           // quadrat
		// ggf in nächste zeile springen
		if( i && !((i+1)%3) ) point += 6;
	}
}

// Legt die Möglichkeiten der Sudokufelder fest
void CSudoku::Update(void) {
	PrintInfo( "INFO: Aktualisiere die Kandidaten\n" );
	for( byte i = 0; i < 81; i++ )
		if( Felder[i].Value != 0 )
			Kill( i, Felder[i].Value );
}

// Kontrolliert alle Felder ob Sudoku noch lösbar
bool CSudoku::ControlEmpty(void) {
	for( byte i = 0; i < 81; i++ ) {
		if( Felder[i].Value )
			continue;
		bool found = false;
		for( byte zahl = 0; zahl < 9; zahl++ )
			if( Felder[i].Possibilities[zahl] ) {
				found = true;
				break;
			}
		// Keiner Vorhanden, Fehler melden
		if( found )
			continue;
		return false;
	}
	return true;
}
// Kontrolliert ein Feld auf eine überbliebende Möglichkeit
byte CSudoku::ControlField( byte punkt ) {
	int zahl = 0;
	for( byte possibility = 0; possibility < 9; possibility++ )
		if( Felder[punkt].Possibilities[possibility] ) {
			if( zahl )
				return 0;
			zahl = possibility + 1;
		}
	return zahl;
}
// Kontrolliert die Möglichkeiten der Zahl in der übergebenen Reihe
byte CSudoku::ControlRow( byte row, byte zahl ) {
	byte found = -1;
	for( byte column = 0; column < 9; column++ )
		if( Felder[row*9 + column].Possibilities[zahl - 1] ) {
			// Wenn schon mal gefunden wurde, dann abbrechen
			if( found != -1 )
				return -1;
			else
				found = row*9 + column;
		}
	return found;
}
// Kontrolliert die Möglichkeiten der Zahl in der übergebenen Spalte
byte CSudoku::ControlColumn( byte column, byte zahl ) {
	byte found = -1;
	for( byte row = 0; row < 9; row++ )
		if( Felder[column*9 + row].Possibilities[zahl - 1] ) {
			// Wenn schon mal gefunden wurde, dann abbrechen
			if( found != -1 )
				return -1;
			else
				found = column*9 + row;
		}
	return found;
}
// Kontrolliert die Möglichkeiten der Zahl im übergebenen Quadrat
byte CSudoku::ControlSquare( byte quad, byte zahl ) {
	byte found = -1;
	byte point = (quad/3)*27 + (quad%3)*3;
	for( byte i = 0; i < 9; i++, point++ ) {
		if( Felder[point].Possibilities[zahl - 1] ) {
			// Wenn schon mal gefunden wurde, dann abbrechen
			if( found != -1 )
				return -1;
			else
				found = point;
		}
		// ggf in nächste zeile springen
		if( i && !((i+1)%3) ) point += 6;
	}
	// Wert zurückgeben
	return found;
}
bool CSudoku::TwoStepMethod( byte quad, byte zahl, bool horizontal ) {
	byte point = (quad/3)*27 + (quad%3)*3;
	byte found = -1;
	byte addvalueFor = horizontal ? 1 : 9;
	byte addvaluePoint = horizontal ? 6 : (-26);
	for( byte count = 0; count < 3; count++ ) {
		for( byte i = 0; i < 3; i++, point += addvalueFor ) {
			if( Felder[point].Possibilities[zahl-1] ) {
				if( found != -1 )
					return false;
				else {
					found = point;
					point += (3-i)*addvalueFor; // durch break werden einige übersprungen
					break;
				}
			}
		}
		point += addvaluePoint;
	}
	bool change = false;
	if( found != -1 ) {
		byte value1 = horizontal ? (quad%3)*3 : (quad/3)*3;
		byte value2 = horizontal ? ((quad%3)+1)*3 : ((quad/3)+1)*3;
		for( byte i = 0; i < 9; i++ )
			if( value1 > i || value2 <= i ) { // zum aktuellen quadrat dazugehörige
				byte index = horizontal ? ((found/9)*9 + i) : ((found%9) + i*9);
				if( Felder[index].Possibilities[zahl-1] )
					change = true;
				Felder[index].Possibilities[zahl-1] = false;  // ausklammern
			}
	}
	return change;
}
bool CSudoku::DoTwoStep( void ) {
	bool change = false;
	for( byte i = 0; i < 9; i++ )
		for( byte zahl = 1; zahl <= 9; zahl++ ) {
			if( TwoStepMethod( i, zahl, true ) )
				change = true;
			if( TwoStepMethod( i, zahl, false ) )
				change = true;
		}
	return change;
}

// überprüft ob Sudoku fertig gelöst wurde
bool CSudoku::IsFinished( int* zahl ) {
	// Alle festgelegte Felder zählen
	int counter = 0;
	for(int i = 0; i < 81; i++)
		if(Felder[i].Value != 0)
			counter++;
	if( zahl )
		*zahl = counter;
	return (counter == 81);
}

// Gibt den aktuellen Status aus
void CSudoku::PrintStatus(void) {
	if( Silent )
		return;
	cout << "\nINFO: Der Status des Sudokus:\n";
	char header[] = "++---+---+---++---+---+---++---+---+---++\n";
	// Sudoku ausgeben
	for( int row = 0; row < 9; row++ ) {
		// Trennung ausgeben
		cout << header;
		if( !(row%3) )
			cout << header;
		for( int line = 0; line < 3; line++ ) { // jede reihe beinhaltet drei zeilen
			for( int column = 0; column < 9; column++ ) {
				int i = row*9 + column;
				cout << '|';
				if( !(column%3) )
					cout << '|';
				if( Felder[i].Value ) {
					if( line == 1 )
						cout << ' ' << Felder[i].Value << ' ';
					else
						cout << "   ";
				}
				else {
					for( int pos = line*3; pos < (line+1)*3; pos++ )
						if( Felder[i].Possibilities[pos] )
							cout << '*';
						else
							cout << ' ';
				}	
			}
			cout << "||\n";
		}
	}
	cout << header << header;
}

// Löst das Sudoku nach Standard-Methode
bool CSudoku::NormalSolve(void) {
	bool found;
	byte punkt = -1;
	do {
		// found zurücksetzen
		found = false;
		// Jede mögliche Reihe und Spalte überprüfen
		for( byte i = 0; i < 9; i++ )	{
			for( byte zahl = 1; zahl <= 9; zahl++ ) {
				punkt = ControlRow( i, zahl );
				if( punkt != -1 ) {
					if( !Silent )
						printf( MSG_INSERT_ROW, zahl, punkt%9+1, punkt/9+1 );
					// Wert setzen
					Felder[punkt].Value = zahl;
					Kill( punkt, zahl );
					// Festlegen, dass etwas geändert wurde
					found = true;
				}
				punkt = ControlColumn( i, zahl );
				if( punkt != -1 ) {
					if( !Silent )
						printf( MSG_INSERT_COLUMN, zahl, punkt%9+1, punkt/9+1 );
					// Wert setzen
					Felder[punkt].Value = zahl;
					Kill( punkt, zahl );
					// Festlegen, dass etwas geändert wurde
					found = true;
				}
				punkt = ControlSquare( i, zahl );
				if( punkt != -1 )	{
					if( !Silent )
						printf( MSG_INSERT_SQUARE, zahl, punkt%9+1, punkt/9+1, i+1 );
					// Wert setzen
					Felder[punkt].Value = zahl;
					Kill( punkt, zahl );
					// Festlegen, dass etwas geändert wurde
					found = true;
				}
			}
		}
		// Jedes Feld prüfen
		for( byte i = 0; i < 81; i++ ) {
			byte zahl = ControlField( i );
			if( zahl ) {
				if( !Silent )
					printf( MSG_INSERT_FIELD, zahl, i%9+1, i/9+1 );
				// Wert setzen
				Felder[i].Value = zahl;
				Kill( i, zahl );
				// Festlegen, dass etwas geändert wurde
				found = true;
			}
		}
	} while( found || (found ? false : DoTwoStep()) /*ControlEmpty-Check?*/ );
	// Wert zurück geben
	return IsFinished();
}
// Löst das Sudoku nach Backtracking-Methode
bool CSudoku::BacktrackingSolve(void) {
	byte punkt;
	// Jedes Feld überprüfen
	for( punkt = 0; punkt < 81; punkt++ )
		// Jede Zahl überprüfen
		for( byte zahl = 1; zahl <= 9; zahl++ )
			// Wenn möglich, dann backtracken
			if( Felder[punkt].Possibilities[zahl - 1] ) {
				// Info ausgeben
				if( !Silent ) {
					printf( "INFO: Starte Backtracking mit Wert %d in Spalte %d, Reihe %d um Loesung zu erhalten\n", zahl, punkt%9+1, punkt/9+1 );
					cout << "INFO: Mache eine Sicherheitskopie des Feldes\n";
				}
				// Stand speichern
				Backup();
				if( !Silent )
					printf( "INFO: Setze Kandidat %d in Spalte %d, Reihe %d\n", zahl, punkt%9+1, punkt/9+1 );
				// Wert setzen
				Felder[punkt] = zahl;
				// Möglichkeit löschen
				Kill( punkt, zahl );
				// überprüfen ob Setzung richtig war
				if( NormalSolve() ) {
					// Info ausgeben
					if( !Silent )
						printf( "INFO: Backtracking mit Wert %d in Spalte %d, Reihe %d war erfolgreich\n", zahl, punkt%9+1, punkt/9+1 );
					return true;
				}
				else {
					// Fallse möglich, einen tieferen Backtracking-Schritt machen
					if( ControlEmpty() ) {
						// Info ausgeben
						if( !Silent )
							cout << "INFO: Mache einen tieferen Backtracking-Schritt\n";
						// Einen tieferen Backtracking-Schritt machen
						if( BacktrackingSolve() )
							return true;
					}
					// Info ausgeben
					if( !Silent ) {
						printf( "INFO: Backtracking mit Wert %d in Spalte %d, Reihe %d fehlgeschlagen\n", zahl, punkt%9+1, punkt/9+1 );
						cout << "INFO: Stelle Sicherheitskopie wieder her\n";
					}
					// Stand wiederherstellen
					Restore();
				}
			}
	return false;
}

// Backup-Funktion
void CSudoku::Backup( void ) {
	BackupFelder.resize( BackupFelder.size()+1 );
	BackupFelder.back() = new CFeld[81];
	for( int i = 0; i < 81; i++ )
		BackupFelder.back()[i] = Felder[i];
}
// Restore-Funktion
void CSudoku::Restore( void ) {
	for( int i = 0; i < 81; i++ )
		Felder[i] = BackupFelder.back()[i];
	delete[] BackupFelder.back();
	BackupFelder.pop_back();
}

// Läd ein Sudoku
bool CSudoku::Laden( string sudoku ) {
	// Überprüfen, ob String gültig
	if( sudoku.length() != 81 )
		return false;
	// Jedes Zeichen einlesen
	for( byte i = 0; i < 81; i++ ) {
		char value = sudoku[i];
		// Zahl überprüfen
		if(value <= '9' && value >= '1')
			Felder[i].Value = value - '0';
		else
			Felder[i].Value = 0;
	}
	// Wert zurückgeben
	return true;
}

// Löst ein geladenes Sudoku
bool CSudoku::Break(void) {
	// Möglichkeiten aktualisieren
	Update();
	// Prüfen ob Sudoku normal gelöst wurde
	if( !NormalSolve() ) {
		PrintStatus();
		if( !Silent ) 
			cout << "\nINFO: Die normale Loesungsmethode ist fehlgeschlagen. Starte Backtracking um Loesung zu erhalten\n";
		// Ansonsten mit Backtracking weiter machen
		BacktrackingSolve();
		// Backups löschen; werden nicht mehr benötigt
		for( int i = 0; i < BackupFelder.size(); i++ )
			delete[] BackupFelder[i];
		BackupFelder.clear();
	}
	// Wert zurückgeben
	return IsFinished();
}
// Schreibt das bearbeitete Sudoku in eine Datei
bool CSudoku::Schreiben(char* path)
{
	// Datei öffnen
	fstream file(path, ios::out);
	// Überprüfen, ob Datei geöffnet wurde
	if( !file )
		return false;
	// Startinfo schreiben
	file << "Das Sudoku:\n";
	// Komplettes Sudoku schreiben
	file << GetSudoku().c_str();
	// Info schreiben
	file << "\nErstellt von:\n";
	char buffer[100];
	sprintf(buffer, ABOUT, TITLE, VERSION, YEAR, AUTHOR);
	file << buffer;
	// Datei schließen
	file.close();
	return true;
}

// Gibt Sudoku zurück
string CSudoku::GetSudoku(void) {
	string sudoku = "";

	char header[] = "+---+---+---+\n";
	// Sudoku ausgeben
	for( int i = 0; i < 81; i++ ) {
		// Trennung ausgeben
		if( i /* gegen einen zeilenumbruch direkt am anfang */ && !(i%9) )
			sudoku += "|\n";
		if( !(i%27) )
			sudoku += header;
		if( !(i%3) )
			sudoku += '|';
		sudoku += Felder[i].GetValue();
	}
	sudoku += "|\n";
	sudoku += header;
	return sudoku;
}
