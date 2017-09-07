//
// Created by mz on 06/09/17.
//

#include "../include/control_unit.h"
#include "../include/memory.h"
#include "../include/arithmetic_logic_unit.h"

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
        printf("Fetching.... ");
    }
    // check empty instruction
    if (strcmp(instructionMemory[pc], "") == 0) {
        return NULL;
    } else {
        return instructionMemory[pc++];
    }
}

/**
 * Decodes the instruction and prepare the operands and registers to the execution state.
 *
 * @param instruction: a string according to a language syntax
 */
void decode(const char *instruction) {

    if (LOG) {
        printf("%s", instruction);
        printf("Decoding.... ");
    }

    // Get the instruction size
    size_t iSize = strlen(instruction);
    // A copy of the instruction
    char instructionCopy[iSize];
    strcpy(instructionCopy, instruction);

    // Will split the instruction to identify one by one, like operations, registers, addresses and constants.
    // Examples: LOAD, 100, MOVE, R1...
    char *token = NULL;

    // Get the operation
    token = strtok(instructionCopy, " ");
    if (strcmp(token, "LOAD") == 0) {
        decodeLoadOperation(token);

    } else if (strcmp(token, "MOVE") == 0) {
        decodeMoveOperation(token);

    } else if (strcmp(token, "STORE") == 0) {
        operation = STORE;
        decodeStoreOperation(token);

    } else if (strcmp(token, "ADD") == 0) {
        decodeArithmeticOperation(token);
        operation = ADD;

    } else if (strcmp(token, "SUBTRACT") == 0) {
        decodeArithmeticOperation(token);
        operation = SUBTRACT;

    } else if (strcmp(token, "MULTIPLY") == 0) {
        decodeArithmeticOperation(token);
        operation = MULTIPLY;

    } else if (strcmp(token, "DIVIDE") == 0) {
        decodeArithmeticOperation(token);
        operation = DIVIDE;

    } else if (strcmp(token, "JUMP") == 0) {
        // TODO: decode this instruction
        operation = JUMP;
    }
}

/**
 * Extracts the index of the register from an instruction.
 * Example: R1, R2, R3...
 *
 * @param token: must have a register as the first string
 * @return the index found or -1 otherwise
 */
int getRegisterIndex(const char *token) {
    token = strtok(NULL, " ");
    if (token[0] == 'R') {
        int operand = token[1] - '0'; // the number of the register
        return operand - 1; // decrements 1 to fit in the index of the registers
    }
    return -1;
}

/**
 * Load in the register the value from the address.
 * Pattern: LOAD Rn ADDRESS
 * Example: LOAD R1 100
 *
 * @param token: string with the operands
 */
void decodeLoadOperation(const char *token) {
    operation = LOAD;

    // Get the first operand, which must be a register
    operand1 = getRegisterIndex(token);

    // Get the second operand, which must be an address
    token = strtok(NULL, " ");
    operand2 = atoi(token);
}

/**
 * Move to the first register the value of the right-side operand, which can be a register or a constant.
 * Pattern 1: MOVE Rn CONSTANT
 * Example 1: MOVE R1 2017
 * Pattern 2: MOVE Rn Rn
 * Example 2: MOVE R1 R2
 *
 * @param token: string with the operands
 */
void decodeMoveOperation(const char *token) {

    // Get the first operand, which must be a register
    operand1 = getRegisterIndex(token);

    // Get the second operand, which must be a value or a register
    token = strtok(NULL, " ");
    // Check if it is a register
    if (token[0] == 'R') {
        operation = MOVE_REGISTER;
        operand2 = token[1] - '0'; // the number of the register
        operand2 -= 1; // decrements 1 to fit in the index of the registers
    } else {
        // It must be a value
        operation = MOVE_CONST;
        operand2 = atoi(token);
    }
}

/**
 * Store the value of the register in the address.
 * Pattern: STORE Rn ADDRESS
 * Example: STORE R1 100
 *
 * @param token: string with the operands
 */
void decodeStoreOperation(const char *token) {

    // Get the first operand, which must be a register
    operand1 = getRegisterIndex(token);

    // Get the second operand, which must be an address
    token = strtok(NULL, " ");
    operand2 = atoi(token);
}

/**
 * Do arithmetic with the two right-side operands and assign the result to the first register.
 * Pattern: OPERATION Rn Rn Rn
 * Example 1: ADD R1 R2 R3
 * Example 2: DIVIDE R1 R2 R3 is equivalent to R1 = (R2 / R3)
 *
 * @param token: string with the operands
 */
void decodeArithmeticOperation(const char *token) {

    // Get the three registers
    operand1 = getRegisterIndex(token);
    operand2 = getRegisterIndex(token);
    operand3 = getRegisterIndex(token);

    if (LOG) {
        printf("Arithmetic with %d and %d; \t", reg[operand2], reg[operand3]);
    }
}

/**
 * Executes one instruction using the values of operands and registers.
 *
 * @param operation: the first word of an instruction
 */
void execute(const int operation) {
    if (LOG) {
        printf("\nExecuting... ");
    }
    switch (operation) {
        case LOAD:
            mar = operand2;
            mbr = indirectMemoryAccess(mar);
            reg[operand1] = mbr;

            if (LOG) {
                printf("R%d = %d from address %d\n\n", operand1 + 1, reg[operand1], operand2);
            }
            break;
        case MOVE_REGISTER:
            reg[operand1] = reg[operand2];
            if (LOG) {
                printf("R%d = %d from R%d\n\n", operand1 + 1, reg[operand1], operand2 + 1);
            }
            break;
        case MOVE_CONST:
            reg[operand1] = operand2;
            if (LOG) {
                printf("R%d = %d constant\n\n", operand1 + 1, reg[operand1]);
            }
            break;
        case STORE:
            mar = operand2;
            mbr = reg[operand1];
            updateMemory(mar, mbr);
            if (LOG) {
                printf("STORE %d from R%d at %d\n\n", reg[operand1], operand1 + 1, operand2);
            }
            break;
        case ADD:
            reg[operand1] = add(reg[operand2], reg[operand3]);
            if (LOG) {
                printf("R%d = %d\n\n", operand1 + 1, reg[operand1]);
            }
            break;
        case SUBTRACT:
            reg[operand1] = subtract(reg[operand2], reg[operand3]);
            if (LOG) {
                printf("R%d = %d\n\n", operand1 + 1, reg[operand1]);
            }
            break;
        case MULTIPLY:
            reg[operand1] = multiply(reg[operand2], reg[operand3]);
            if (LOG) {
                printf("R%d = %d\n\n", operand1 + 1, reg[operand1]);
            }
            break;
        case DIVIDE:
            reg[operand1] = divide(reg[operand2], reg[operand3]);
            if (LOG) {
                printf("R%d = %d\n\n", operand1 + 1, reg[operand1]);
            }
            break;
        case JUMP:
            // TODO: Execute this operation
            break;
    }
}