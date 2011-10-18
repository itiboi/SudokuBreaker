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

#ifndef SUDOKU_H
#define SUDOKU_H

#include <Field.h>
#include <string>
#include <vector>

using namespace std;

typedef char byte;

/** Speichert ein Sudoku-Feld und enthält eine Löse-Funktion. Das Sudoku
 *  kann geladen und gespeichert werden. Die Klasse enthält
 *  die Normallösungsmethode sowie eine Backtrackingmethode. Die
 *  Normalmethode kontrolliert nicht nur jede Reihe, Spalte und Quadrat
 *  sondern löscht auch Kandidaten mit Hilfe der "TwoStep"-Methode
 *  (Beschreibung: http://sudokugarden.de/de/loesen/mehrstufig).
 */
class Sudoku {
private:
	/** Felder des Sudokus. Die Felder werden reihenweise durchnummeriert.
	 */
	Field Felder[81];
	/** Backup des Sudokus, das bei der Backtrackingmethode verwendet wird.
	 *  Die Backupmethode reserviert Speicher für die Kopien, die
	 *  Restoremethode gibt ihn wieder frei.
	 */
	vector<Field*> BackupFelder;

	/** Löscht die Möglichkeiten des übergebenen Feldes. Alle Möglichkeiten
	 *  der Zahl werden in Reihe, Spalte und Quadrat gelöscht.
	 */
	void Kill( byte Punkt, byte zahl );
	/** Legt die Möglichkeiten der Sudokufelder fest. Löscht in
	 *  entsprechender Reihe, Spalte und Quadrat die entsprechende
	 *  Möglichkeit.
	 */
	void Update( void );

	/** Kontrolliert alle Felder ob Sudoku noch lösbar. Testet bei allen
	 *  Feldern ob noch mindestens eine Möglichkeit übrig ist.
	 */
	bool ControlEmpty( void );
	// Kontrolliert ein Feld auf eine überbliebende Möglichkeit
	byte ControlField( byte punkt );
	// Kontrolliert die Möglichkeiten der Zahl in der übergebenen Reihe
	byte ControlRow( byte row, byte zahl );
	// Kontrolliert die Möglichkeiten der Zahl in der übergebenen Spalte
	byte ControlColumn( byte column, byte zahl );
	// Kontrolliert die Möglichkeiten der Zahl im übergebenen Quadrat
	byte ControlSquare( byte quad, byte zahl );
	// Löscht Möglichkeiten nach der "TwoStep"-Methode
	bool TwoStepMethod( byte quad, byte zahl, bool horizontal );
	bool DoTwoStep( void );
	
	// Überprüft ob Sudoku fertig gelöst wurde
	bool IsFinished( int* zahl = NULL );

	// Gibt den aktuellen Status aus
	void PrintStatus( void );

	// Löst das Sudoku nach Standard-Methode
	bool NormalSolve( void );
	// Löst das Sudoku nach Backtracking-Methode
	bool BacktrackingSolve( void );

	// Backup-Funktion
	void Backup( void );
	// Restore-Funktion
	void Restore( void );
	
	void PrintInfo( string text );
public:
	// Konstruktor
	Sudoku( bool quiet = false );

	// Sollen Infos ausgegeben werden
	bool Silent;

	// Läd ein Sudoku
	bool Laden( string sudoku );
	// Löst ein geladenes Sudoku
	bool Break( void );
	// Schreibt das bearbeitete Sudoku in eine Datei
	bool Schreiben( char* path );
	// Gibt Sudoku zurück
	string GetSudoku( void );
};

#endif // SUDOKU_H
