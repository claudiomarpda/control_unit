//
// Created by mz on 05/09/17.
//

#ifndef CONTROLUNIT_MEMORY_H
#define CONTROLUNIT_MEMORY_H

#define MAX_LINES 100
#define INSTRUCTION_LENGTH 100

// Global variables
char instructionMemory[MAX_LINES][INSTRUCTION_LENGTH];
int dataMemory[MAX_LINES][2];

// Prototypes
int loadInstructions(const char *fileName);
int loadData(const char *fileName);
int loadMemory(const char *instructionsFileName, const char *dataFileName);
int indirectMemoryAccess(int address);
void writeMemory(const char *dataMemoryFileName, int mar, int mbr);

#endif //CONTROLUNIT_MEMORY_H
