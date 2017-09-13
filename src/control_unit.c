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
 * Store the value of the register in the address. The address can be a constant or a register value.
 * Pattern: STORE Rn CONSTANT
 * Example: STORE R1 100
 * Pattern: STORE Rn Rn
 * Example: STORE R1 R2
 *
 * @param token: string with the operands
 */
void decodeStoreOperation(const char *token) {

    // Get the first operand, which must be a register
    operand1 = getRegisterIndex(token);

    // Get the second operand, which must be a value or a register
    token = strtok(NULL, " ");
    // Check if it is a register
    if (token[0] == 'R') {
        operation = STORE_REGISTER;
        operand2 = token[1] - '0'; // the number of the register
        operand2 -= 1; // decrements 1 to fit in the index of the registers
    } else {
        // It must be a value
        operation = STORE_CONST;
        operand2 = atoi(token);
    }
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
 * Decodes the instruction to jump to an index of the instruction memory.
 * Compares the value of a register with a constant value and, in case they are not equal, jumps to an instruction address.
 * Pattern: JUMP R CONSTANT INDEX
 * Example: JUMP R1 0 5. It means: Jump, if R1 is not equal to 0, to the instruction of index 5.
 *
 * @param token: string with the register, the value to be compared, and the index of the instruction
 */
void decodeJumpOperation(const char *token) {
    // The register
    operand1 = getRegisterIndex(token);
    token = strtok(NULL, " ");
    // Value to be compared with the value of the register
    operand2 = atoi(token);
    token = strtok(NULL, " ");
    // The index of the instruction, in case the comparison results not equal
    operand3 = atoi(token);
}

/**
 * Decodes the instruction to increment 1 to the value of a register.
 *
 * @param token: the string with the register
 */
void decodeIncrementOperation(char *token) {
    token = strtok(NULL, " ");
    // Counts the number of characters until it hits a '\n'
    size_t i = strcspn(token, "\n");
    token[i] = '\0';
    if (token[0] == 'R') {
        operand1 = token[1] - '0'; // the number of the register
        operand1--; // decrements 1 to fit in the index of the registers
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

    // Will receive pieces of the instruction to identify one by one, like operations, registers, addresses and constants.
    // Examples: LOAD, 100, MOVE, R1...
    char *token = NULL;

    // Get the operation
    token = strtok(instructionCopy, " ");
    if (strcmp(token, "LOAD") == 0) {
        decodeLoadOperation(token);
        operation = LOAD;

    } else if (strcmp(token, "MOVE") == 0) {
        decodeMoveOperation(token);

    } else if (strcmp(token, "STORE") == 0) {
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
        decodeJumpOperation(token);
        operation = JUMP;

    } else if (strcmp(token, "INCREMENT") == 0) {
        decodeIncrementOperation(token);
        operation = INCREMENT;

    } else if (strcmp(token, "DECREMENT") == 0) {
        decodeIncrementOperation(token);
        operation = DECREMENT;
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

    // Let's assume that every execution costs 1 cycle
    cycles++;

    switch (operation) {
        case LOAD:
            mar = operand2;
            mbr = indirectMemoryAccess(mar);
            reg[operand1] = mbr;
            // Let's assume that every fetch in memory costs 4 cycles additionally
            cycles += 4;
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
        case STORE_REGISTER:
            mar = reg[operand2];
            mbr = reg[operand1];
            updateMemory(mar, mbr);
            if (LOG) {
                printf("Store %d from R%d at %d\n\n", reg[operand1], operand1 + 1, reg[operand2]);
            }
            break;
        case STORE_CONST:
            mar = operand2;
            mbr = reg[operand1];
            updateMemory(mar, mbr);
            if (LOG) {
                printf("Store %d from R%d at %d\n\n", reg[operand1], operand1 + 1, operand2);
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
            if (LOG) {
                printf("Jump try: R%d = %d compared to %d; Target instruction: %d;", operand1 + 1, reg[operand1],
                       operand2, operand3);
            }
            // Check the loop condition
            if (reg[operand1] != operand2) {
                // Updates PC with the new instruction index
                pc = operand3;
                if (LOG) {
                    printf(" Loop\n\n");
                }
            } else {
                if (LOG) {
                    printf(" Loop condition fails\n\n");
                }
            }
            break;
        case INCREMENT:
            if (LOG) {
                printf("Increment R%d = %d to %d\n\n", operand1 + 1, reg[operand1], reg[operand1] + 1);
            }
            reg[operand1]++;
            break;
        case DECREMENT:
            if (LOG) {
                printf("Decrement R%d = %d to %d\n\n", operand1 + 1, reg[operand1], reg[operand1] - 1);
            }
            reg[operand1]--;
            break;
    }
}