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

/// Verwaltet ein Feld eines Sudokus
/** Das heißt sie enthält alle Möglichkeiten und
 *  eine Variable zum Speichern eines gesetzten
 *  Wertes.
 */
class Field {
public:
	/// Wert des Feldes; wenn kein Wert, dann 0
	int Value;
	/// Die Wertmöglichkeiten des Feldes
	bool Possibilities[9];

	// Konstruktor
	Field( void );
	Field( const Field& feld );
	/// Funktion um Möglichkeiten wieder zurück zu setzen
	void SetBack( void );
	/** Gibt den Wert als Char zurück. Wenn kein Wert gesetzt,
	 *  dann wird ein Leerzeichen zurückgegeben.
	 */
	char GetValue( void );
	
	Field& operator=(const Field& feld);
	Field& operator=(const int& value);
};
