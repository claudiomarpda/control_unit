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
 * @param token: must contain a register as the first string
 * @return the index found or -1 otherwise
 */
int getRegisterIndex(const char *token) {
    if (token[0] == 'R') {
        int operand = token[1] - '0'; // the number of the register
        return operand - 1; // decrements 1 to fit in the index of the registers
    }
    return -1;
}

/**
 * Load in the register the value from the address.
 * The address can come from a constant value or a value of a register.
 * Pattern: LOAD Rn CONSTANT
 * Example: LOAD R1 100
 * Pattern: LOAD Rn Rn
 * Example: LOAD R1 R2
 *
 * @param token: instruction with the operands
 */
void decodeLoadOperation(const char *token) {
    // Get the first operand, which must be a register
    token = strtok(NULL, " ");
    operand1 = getRegisterIndex(token);
    token = strtok(NULL, " ");

    // Get the second operand
    // The value is in a register
    if (token[0] == 'R') {
        operation = LOAD_REGISTER;
        operand2 = getRegisterIndex(token);
    }
        // The value is an address
    else {
        operation = LOAD_ADDRESS;
        operand2 = atoi(token);
    }
}

/**
 * Move to the first register the value of the right-side operand, which can be a register or a constant.
 * Pattern 1: MOVE Rn CONSTANT
 * Example 1: MOVE R1 2017
 * Pattern 2: MOVE Rn Rn
 * Example 2: MOVE R1 R2
 *
 * @param token: instruction with the operands
 */
void decodeMoveOperation(const char *token) {

    // Get the first operand, which must be a register
    token = strtok(NULL, " ");
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
 * @param token: instruction with the operands
 */
void decodeStoreOperation(const char *token) {

    // Get the first operand, which must be a register
    token = strtok(NULL, " ");
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
 * @param token: instruction with the operands
 */
void decodeArithmeticOperation(const char *token) {

    // Get the three registers
    token = strtok(NULL, " ");
    operand1 = getRegisterIndex(token);
    token = strtok(NULL, " ");
    operand2 = getRegisterIndex(token);
    token = strtok(NULL, " ");
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
 * @param token: instruction with the register, the value to be compared, and the index of the instruction
 */
void decodeConditionalJumpOperation(const char *token) {
    operation = CONDITIONAL_JUMP;
    // The register
    operand1 = getRegisterIndex(token);
    token = strtok(NULL, " ");
    // Value to be compared with the value of the register
    operand2 = atoi(token);
    token = strtok(NULL, " ");
    // The index of the next instruction, in case the comparison results not equal
    operand3 = atoi(token);
}


/**
 * Decodes unconditional jump operation.
 *
 * @param token: instruction with the index of the next instruction
 */
void decodeUnconditionalJumpOperation(const char *token) {
    operation = UNCONDITIONAL_JUMP;
    // The index of the next instruction
    operand1 = atoi(token);
}

/**
 * Identifies the jump operations type: conditional or unconditional.
 *
 * @param token: the instruction with the operands
 */
void decodeJumpOperation(const char *token) {
    token = strtok(NULL, " ");
    // The first operand is a register
    if (token[0] == 'R') {
        decodeConditionalJumpOperation(token);
    }
        // The first and only operand is an instruction index
    else {
        decodeUnconditionalJumpOperation(token);
    }
}

/**
 * Decodes the instruction to increment 1 to the value of a register.
 *
 * @param token: the instruction with the register
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
 * Decodes the comparison of two operands and stores it in a register.
 * The comparison works according to arithmetic logic unit.
 * Pattern: COMPARE Rn Rn Rn
 * Example: COMPARE R3 R1 R2
 *
 * @param token: instruction with two operands.
 */
void decodeCompareOperation(const char *token) {
    token = strtok(NULL, " ");
    // The first register where the result will be stored
    operand1 = getRegisterIndex(token);
    token = strtok(NULL, " ");
    // The second register
    operand2 = getRegisterIndex(token);
    token = strtok(NULL, " ");
    // The third register
    operand3 = getRegisterIndex(token);
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

    } else if (strcmp(token, "MOVE") == 0) {
        decodeMoveOperation(token);

    } else if (strcmp(token, "STORE") == 0) {
        decodeStoreOperation(token);

    } else if (strcmp(token, "ADD") == 0) {
        operation = ADD;
        decodeArithmeticOperation(token);

    } else if (strcmp(token, "SUBTRACT") == 0) {
        operation = SUBTRACT;
        decodeArithmeticOperation(token);

    } else if (strcmp(token, "MULTIPLY") == 0) {
        operation = MULTIPLY;
        decodeArithmeticOperation(token);

    } else if (strcmp(token, "DIVIDE") == 0) {
        operation = DIVIDE;
        decodeArithmeticOperation(token);

    } else if (strcmp(token, "JUMP") == 0) {
        decodeJumpOperation(token);

    } else if (strcmp(token, "INCREMENT") == 0) {
        operation = INCREMENT;
        decodeIncrementOperation(token);

    } else if (strcmp(token, "DECREMENT") == 0) {
        operation = DECREMENT;
        decodeIncrementOperation(token);

    } else if (strcmp(token, "COMPARE") == 0) {
        operation = COMPARE;
        decodeCompareOperation(token);
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
        case LOAD_ADDRESS:
            mar = operand2;
            mbr = indirectMemoryAccess(mar);
            reg[operand1] = mbr;
            // Let's assume that every fetch in memory costs 4 cycles additionally
            cycles += 4;
            if (LOG) {
                printf("R%d = %d from address %d\n\n", operand1 + 1, reg[operand1], operand2);
            }
            break;
        case LOAD_REGISTER:
            mar = reg[operand2];
            mbr = indirectMemoryAccess(mar);
            reg[operand1] = mbr;
            // Let's assume that every fetch in memory costs 4 cycles additionally
            cycles += 4;
            if (LOG) {
                printf("R%d = %d from address %d\n\n", operand1 + 1, reg[operand1], reg[operand2]);
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
        case CONDITIONAL_JUMP:
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
        case UNCONDITIONAL_JUMP:
            pc = operand1;
            if (LOG) {
                printf("Jump to instruction of index %d\n", pc);
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
        case COMPARE:
            if (LOG) {
                printf("Compare %d with %d\n", reg[operand2], reg[operand3]);
            }
            // Compares two values from registers and stores the result in one register
            reg[operand1] = compare(reg[operand2], reg[operand3]);
            break;
    }
}