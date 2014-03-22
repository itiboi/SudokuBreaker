SudokuBreaker
=============

This program solves sudoku puzzles with some strategies. If this fails,
it will to find a solution via bruteforce.

The first version was written back in 2008 when I started learning programming
C++. It does not need anything than the STL library.

Just pass the sudoku written from top to bottom from left to right with
'.' or '0' for empty fields to the program.

Compile
-------
Just go into build directory with
> cd build
Then use CMake to create Makefile
> cmake ..
and start build process
> make

License
-------
(c) 2008-2011 by Tim Bolender

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
SudokuBreaker; see the file COPYING. If not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.

