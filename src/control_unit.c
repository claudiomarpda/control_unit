//
// Created by mz on 06/09/17.
//

#include "../include/control_unit.h"
#include "../include/memory.h"
#include "../include/arithmetic_logic_unit.h"
#include "../include/cache_strategy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Fetches the next instruction which the program counter is pointing to.
 *
 * @return an instruction as string
 */
char *fetch(char **instruction_memory) {

    if (LOG) {
        printf("Fetching.... ");
    }
    // Check empty instruction
    if (strcmp(instruction_memory[pc], "") == 0) {
        return NULL;
    }

    int index = -1;
    char *instruction = NULL;

    // Check if the instruction is in the cache
    index = cache_instruction_exists(pc);

    if (index >= 0) {
        // Is in the cache
        if (LOG) {
            printf("Instruction Cache HIT: ");
        }

        // Loading instruction from main memory costs 3 cycles
        cycles += 3;
        cache_hit++;

        instruction = cache_read_instruction(index);

    } else {
        // Is not in the cache
        if (LOG) {
            printf("Instruction Cache MISS: ");
        }

        // Loading instruction from main memory costs 239 cycles
        cycles += 239;
        cache_miss++;

        // Get instruction from main memory
        instruction = instruction_memory[pc];

        // Save it in the cache
        cache_write_instruction(pc, instruction);
    }

    // Points to the next instruction
    pc++;
    return instruction;
}

/**
 * Extracts the index of the register from an instruction.
 * Example: R1, R2, R3...
 *
 * @param token: must contain a register as the first string
 * @return the index found or -1 otherwise
 */
static int get_register_index(const char *token) {
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
static void decode_load_operation(const char *token) {
    // Get the first operand, which must be a register
    token = strtok(NULL, " ");
    operand1 = get_register_index(token);
    token = strtok(NULL, " ");

    // Get the second operand
    // The value is in a register
    if (token[0] == 'R') {
        operation = LOAD_REGISTER;
        operand2 = get_register_index(token);
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
static void decode_move_operation(const char *token) {

    // Get the first operand, which must be a register
    token = strtok(NULL, " ");
    operand1 = get_register_index(token);

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
static void decode_store_operation(const char *token) {

    // Get the first operand, which must be a register
    token = strtok(NULL, " ");
    operand1 = get_register_index(token);

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
static void decode_arithmetic_operation(const char *token) {

    // Get the three registers
    token = strtok(NULL, " ");
    operand1 = get_register_index(token);
    token = strtok(NULL, " ");
    operand2 = get_register_index(token);
    token = strtok(NULL, " ");
    operand3 = get_register_index(token);

    if (LOG) {
        printf("Arithmetic with %d and %d; \t", reg[operand2], reg[operand3]);
    }
}

/**
 * Decodes the instruction to jump to an index of the instruction memory.
 * Compares the first constant value with the value in the comparison register and,
 * if they are equal, jump condition is true.
 * Note that to update the register, Compare operation must be done before calling the Jump operation.
 * Pattern: JUMPc CONSTANT INDEX
 * Example: JUMPc 0 5.
 * It means: Jump, if the value in the comparison register is 0, to the instruction of index 5
 *
 * @param token: instruction with the value to be compared, and the index of the instruction
 */
static void decode_conditional_jump_operation(const char *token) {
    operation = CONDITIONAL_JUMP;
    token = strtok(NULL, " ");
    // Value to be compared with the value of the register
    operand1 = atoi(token);
    token = strtok(NULL, " ");
    // The index of the next instruction, in case the comparison results not equal
    operand2 = atoi(token);
}

/**
 * Decodes unconditional jump operation.
 * Pattern: JUMPu CONSTANT
 * Example: JUMPu 10
 * It means: Jump to instruction of index 10
 *
 * @param token: instruction with the index of the next instruction
 */
static void decode_unconditional_jump_operation(const char *token) {
    operation = UNCONDITIONAL_JUMP;
    token = strtok(NULL, " ");
    // The index of the next instruction
    operand1 = atoi(token);
}

/**
 * Decodes the instruction to increment 1 to the value of a register.
 *
 * @param token: the instruction with the register
 */
static void decode_increment_operation(char *token) {
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
 * Decodes the comparison of two operands and stores it in the comparison register.
 * The comparison works according to arithmetic logic unit.
 * The first operand must be a register and the second must be a register or a constant.
 * Pattern: COMPARE Rn Rn
 * Example: COMPARE R1 R2
 * Pattern: COMPARE Rn CONSTANT
 * Example: COMPARE R1 0
 *
 * @param token: instruction with two operands.
 */
static void decode_compare_operation(const char *token) {
    token = strtok(NULL, " ");
    // The first register where the result will be stored
    operand1 = get_register_index(token);

    token = strtok(NULL, " ");
    // Get the second operand, which must be a value or a register
    // Check if it is a register
    if (token[0] == 'R') {
        operation = COMPARE_REGISTER;
        operand2 = token[1] - '0'; // the number of the register
        operand2 -= 1; // decrements 1 to fit in the index of the registers
    } else {
        // It must be a value
        operation = COMPARE_CONST;
        operand2 = atoi(token);
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
        decode_load_operation(token);

    } else if (strcmp(token, "MOVE") == 0) {
        decode_move_operation(token);

    } else if (strcmp(token, "STORE") == 0) {
        decode_store_operation(token);

    } else if (strcmp(token, "ADD") == 0) {
        operation = ADD;
        decode_arithmetic_operation(token);

    } else if (strcmp(token, "SUBTRACT") == 0) {
        operation = SUBTRACT;
        decode_arithmetic_operation(token);

    } else if (strcmp(token, "MULTIPLY") == 0) {
        operation = MULTIPLY;
        decode_arithmetic_operation(token);

    } else if (strcmp(token, "DIVIDE") == 0) {
        operation = DIVIDE;
        decode_arithmetic_operation(token);

    } else if (strcmp(token, "JUMPc") == 0) {
//        decodeJumpOperation(token);
        decode_conditional_jump_operation(token);

    } else if (strcmp(token, "JUMPu") == 0) {
//        decodeJumpOperation(token);
        decode_unconditional_jump_operation(token);

    } else if (strcmp(token, "INCREMENT") == 0) {
        operation = INCREMENT;
        decode_increment_operation(token);

    } else if (strcmp(token, "DECREMENT") == 0) {
        operation = DECREMENT;
        decode_increment_operation(token);

    } else if (strcmp(token, "COMPARE") == 0) {
        decode_compare_operation(token);
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

    // Execution costs 1 cycle
    cycles++;

    switch (operation) {
        case LOAD_ADDRESS:
            mar = operand2;
            mbr = find_data_in_address(mar);
            reg[operand1] = mbr;
            if (LOG) {
                printf("R%d = %d from address %d\n\n", operand1 + 1, reg[operand1], operand2);
            }
            break;
        case LOAD_REGISTER:
            mar = reg[operand2];
            mbr = find_data_in_address(mar);
            reg[operand1] = mbr;
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
            update_memory(mar, mbr);
            if (LOG) {
                printf("Store %d from R%d at %d\n\n", reg[operand1], operand1 + 1, reg[operand2]);
            }
            break;
        case STORE_CONST:
            mar = operand2;
            mbr = reg[operand1];
            update_memory(mar, mbr);
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
                printf("Jump try: %d compared to %d; Target instruction: %d;", rc, operand1, operand2);
            }
            // Check the loop condition
            if (rc == operand1) {
                // Updates PC with the new instruction index
                pc = operand2;
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
                printf("Jump to instruction of index %d\n\n", pc);
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
        case COMPARE_REGISTER:
            if (LOG) {
                printf("Compare %d with %d\n\n", reg[operand1], reg[operand2]);
            }
            // Compares two values from registers and stores the result in one register
            rc = compare(reg[operand1], reg[operand2]);
            break;
        case COMPARE_CONST:
            if (LOG) {
                printf("Compare %d with %d\n\n", reg[operand1], operand2);
            }
            // Compares two values from registers and stores the result in one register
            rc = compare(reg[operand1], operand2);
            break;
    }
}

void init_control_unit() {
    ir = NULL;
    cycles = 0;
    cache_hit = 0;
    cache_miss = 0;
}

static int find_data_in_address(int mar) {

    int data = 0;
    int index = cache_data_address_exist(mar);

    if (index >= 0) {
        // The address exists in cache
        if (LOG) {
            printf("Data Cache HIT: ");
        }
        // Loading data from cache costs 3 cycles
        cycles += 3;
        cache_hit++;

        // Read data from cache
        data = cache_read_data(index);

    } else {
        // The address is not in the cache
        // Searches at the main memory

        if (LOG) {
            printf("Data Cache MISS: ");
        }

        // Loading data from main memory costs 239 cycles
        cycles += 239;
        cache_miss++;

        // Read data from main memory
        data = indirect_memory_access(mar);

        // Update cache
        cache_write_data(mar, data);

    }
    return data;
}