//
// Created by mz on 05/09/17.
//

#ifndef CONTROLUNIT_MEMORY_H
#define CONTROLUNIT_MEMORY_H

#define MAX_LINES 100
#define LINE_LENGTH 100

// Global variables
char instructionMemory[MAX_LINES][LINE_LENGTH];
int dataMemory[MAX_LINES][2];

// Prototypes
int loadInstructions(const char *fileName);
int loadData(const char *fileName);
int loadMemory(const char *instructionsFileName, const char *dataFileName);

#endif //CONTROLUNIT_MEMORY_H
