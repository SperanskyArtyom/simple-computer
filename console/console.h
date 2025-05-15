#pragma once

#include <myTerm.h>

void printCell (int address, enum colors fg, enum colors bg);
void printFlags (void);
void printDecodedCommand (int value);
void printAccumulator (void);
void printCounters (void);
void printTerm (int address, int input);
void printCommand (void);
void printBorders (void);

void appendToHist (int address, int input);
void printBigCell (int *bigchars, int value);
