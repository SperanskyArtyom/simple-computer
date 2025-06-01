#pragma once

#define FLAG_OVERFLOW 0
#define FLAG_DIV_ZERO 1
#define FLAG_MEM_OOB 2
#define FLAG_INVALID_CMD 3
#define FLAG_IGNORE_CLOCK 4

int sc_memoryInit (void);
int sc_memorySet (int address, int value);
int sc_memoryGet (int address, int *value);
int sc_memorySave (char *filename);
int sc_memoryLoad (char *filename);

int sc_regInit (void);
int sc_regSet (int flag, int value);
int sc_regGet (int flag, int *value);

int sc_accumulatorInit (void);
int sc_accumulatorSet (int value);
int sc_accumulatorGet (int *value);

int sc_icounterInit (void);
int sc_icounterSet (int value);
int sc_icounterGet (int *value);

int sc_commandEncode (int sign, int command, int operand, int *value);
int sc_commandDecode (int value, int *sign, int *command, int *operand);
int sc_commandValidate (int command);

void CU (void);
int ALU (int command, int operand);
void IRC (int signum);
