#-------------------------------------
# CMakeFile to compile 
# SudokuBreaker (c) 2011 by Tim Bolender
# 
# This file includes all needed parts of wxWidgets and
# compiles the program and links it.
#-------------------------------------

# Add Srcs Files
SET( SRCs ${SRCs} ${SRC_DIR}/Field.cpp )
SET( SRCs ${SRCs} ${SRC_DIR}/Sudoku.cpp )
SET( SRCs ${SRCs} ${SRC_DIR}/main.cpp )
