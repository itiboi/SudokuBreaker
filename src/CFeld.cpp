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

#include "CFeld.h"

// Konstruktor
CFeld::CFeld( void ) : Value(0) {
	SetBack();
}

CFeld::CFeld( const CFeld& feld ) {
	(*this) = feld;
}

// Funktion um Möglichkeiten wieder zurück zu setzen
void CFeld::SetBack(void) {
	// Alle Möglichkeiten auf true setzen
	for(int i = 0; i < 9; i++)
		Possibilities[i] = true;
}

// Gibt den Wert als Char zurück
char CFeld::GetValue(void) {
	if(Value >= 1 && Value <= 9)
		return static_cast<char>(Value + 48);
	return ' ';
}

CFeld& CFeld::operator=(const CFeld& feld) {
	Value = feld.Value;
	for( int i = 0; i < 9; i++ )
		Possibilities[i] = feld.Possibilities[i];
	return *this;
}
CFeld& CFeld::operator=(const int& value) {
	Value = value;
	Possibilities[value] = true;
	for( int i = 0; i < 9; i++ ) {
		if( i != (value+1) )
			Possibilities[i] = false;
		else
			Possibilities[i] = true;
	}
	return *this;
}
