//
// Created by mz on 06/09/17.
//

#include "../include/control_unit.h"
#include "../include/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Fetches the next instruction which the program counter is pointing to.
 *
 * @return an instruction as string
 */
char *fetch(char instructionMemory[][INSTRUCTION_LENGTH]) {

    if (LOG) {
        printf("Fetching...\n");
    }
    // check empty instruction
    if (strcmp(instructionMemory[pc], "") == 0) {
        return NULL;
    } else {
        return instructionMemory[pc++];
    }
}

/**
 * Decodes an instruction in pieces and handles every string.
 *
 * @param instruction
 */
void decode(const char *instruction) {

    if (LOG) {
        printf("%s", instruction);
        puts("Decoding...");
    }

    // Get the instruction size
    size_t iSize = strlen(instruction);
    // A copy of the instruction
    char instructionCopy[iSize];
    strcpy(instructionCopy, instruction);

    // Will keep every piece of an instruction, one by one.
    // Examples: LOAD, 0, MOVE, R1...
    char *token = NULL;

    // Get the operation
    token = strtok(instructionCopy, " ");
    if (strcmp(token, "LOAD") == 0) {
        operation = LOAD;
        // Pattern: LOAD R ADDRESS
        // Example: LOAD R1 100
        // Load the value from the address into the register

        // Get the first operand, which must be a register
        token = strtok(NULL, " ");
        if (token[0] == 'R') {
            operand1 = token[1] - '0'; // the number of the register
            operand1 -= 1; // decrements 1 to fit in the index of the registers
        }

        // Get the second operand, which must be an address
        token = strtok(NULL, " ");
        operand2 = atoi(token);

    } else if (strcmp(token, "MOVE") == 0) {
        // Move to the first register R the CONSTANT or the value of the register R

        // Pattern 1: MOVE R CONSTANT
        // Example 1: MOVE R1 2017

        // Pattern 2: MOVE R R
        // Example 2: MOVE R1 R2

        // Get the first operand, which must be a register
        token = strtok(NULL, " ");
        if (token[0] == 'R') {
            operand1 = token[1] - '0'; // the index of the register
            operand1 -= 1; // decrements 1 to fit in the index of the registers
        }

        // Get the second operand, which must be a value or a register
        token = strtok(NULL, " ");
        // Check if it is a register
        if (token[0] == 'R') {
            operation = MOVE_REGISTER;
            operand2 = token[1] - '0'; // the number of the register
            operand1 -= 1; // decrements 1 to fit in the index of the registers
        } else {
            // It must be a value
            operation = MOVE_CONST;
            operand2 = atoi(token);
        }

    } else if (strcmp(token, "STORE") == 0) {
        operation = STORE;
        // Store in ADDRESS the value of register R
        // Pattern: STORE R ADDRESS
        // Example: STORE R1 100

        // Get the first operand, which must be a register
        token = strtok(NULL, " ");
        if (token[0] == 'R') {
            operand1 = token[1] - '0'; // the number of the register
            operand1 = reg[operand1-1]; // decrements 1 to fit in the index of the registers
        }

        // Get the second operand, which must be an address
        token = strtok(NULL, " ");
        operand2 = atoi(token);

    } else if (strcmp(token, "ADD") == 0) {
        // TODO: Decode this operation
        operation = ADD;
    } else if (strcmp(token, "SUBTRACT") == 0) {
        // TODO: Decode this operation
        operation = SUBTRACT;
    } else if (strcmp(token, "MULTIPLY") == 0) {
        // TODO: Decode this operation
        operation = MULTIPLY;
    } else if (strcmp(token, "DIVIDE") == 0) {
        // TODO: Decode this operation
        operation = DIVIDE;
    } else if (strcmp(token, "JUMP") == 0) {
        // TODO: Decode this operation
        operation = JUMP;
    }
}

/**
 *
 * @param operation
 */
void execute(const int operation, char *dataMemoryFile) {
    switch (operation) {
        case LOAD:
            puts("LOAD operation");
            mar = operand2;
            mbr = indirectMemoryAccess(mar);
            reg[operand1] = mbr;

            if (LOG) {
                printf("R%d receives %d from address %d\n\n", operand1 + 1, reg[operand1], mar);
            }
            break;
        case MOVE_REGISTER:
            puts("MOVE operation");
            reg[operand1] = reg[operand2];
            if (LOG) {
                printf("R%d receives %d from R%d\n\n", operand1 + 1, reg[operand1], operand2 + 1);
            }
            break;
        case MOVE_CONST:
            puts("MOVE operation");
            reg[operand1] = operand2;
            if (LOG) {
                printf("R%d receives %d constant\n\n", operand1 + 1, reg[operand1]);
            }
            break;
        case STORE:
            puts("STORE operation");
            mar = operand2;
            mbr = operand1;
            writeMemory(dataMemoryFile, mar, mbr);
            if(LOG) {
                puts("Memory was written\n");
            }
            break;
        case ADD:
            puts("ADD operation\n\n");
            // TODO: Execute this operation
            break;
        case SUBTRACT:
            puts("SUBTRACT operation\n\n");
            // TODO: Execute this operation
            break;
        case MULTIPLY:
            puts("MULTIPLY operation\n\n");
            // TODO: Execute this operation
            break;
        case DIVIDE:
            puts("DIVIDE operation\n\n");
            // TODO: Execute this operation
            break;
        case JUMP:
            puts("JUMP operation\n\n");
            // TODO: Execute this operation
            break;
    }
}