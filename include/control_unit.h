//
// Created by mz on 06/09/17.
//

#ifndef CONTROLUNIT_CONTROL_UNIT_H
#define CONTROLUNIT_CONTROL_UNIT_H


// Enables logging for printing details
#define LOG 1
#define INSTRUCTION_LENGTH 100

// All possible states of the instruction cycle
enum states {
    FETCH, DECODE, EXECUTE, FINISH
};

// All possible operation types
enum operations {
    LOAD_ADDRESS, LOAD_REGISTER, MOVE_REGISTER, MOVE_CONST,
    STORE_REGISTER, STORE_CONST, ADD, MULTIPLY, SUBTRACT, DIVIDE,
    CONDITIONAL_JUMP, UNCONDITIONAL_JUMP, INCREMENT, DECREMENT,
    COMPARE_REGISTER, COMPARE_CONST
};

enum mappings {ASSOCIATIVE, DIRECT};

// The current state of the instruction cycle
enum states state;
// The current operation
enum operations operation;
// The type of mapping for memory cache
enum mappings cache_mapping;

// Program counter points to the index of the next instruction of the memory
int pc;
// Instruction register keeps an instruction in string format
char *ir;
// Registers load data from memory and are used to do arithmetic operations
static int reg[9];
// Memory Bus Register
int mbr;
// Memory Access Register
int mar;
// Comparison Register stores the result of the last compare operation
static int rc;

/*
 * Operands identifiers. Handles three operand at most at a time.
 * For registers, it will receive the index of the register.
 * For memory access, it will receive an address.
 * For constants, it will receive a constant value.
 */
static int operand1, operand2, operand3;

// Number of cycles for CPU clock when accessing main memory and cache memory
int cycles;


/**
 * Fetches the next instruction which the program counter is pointing to.
 *
 * @return an instruction as string
 */
char *fetch(char instruction_memory[][INSTRUCTION_LENGTH]);

/**
 * Extracts the index of the register from an instruction.
 * Example: R1, R2, R3...
 *
 * @param token: must contain a register as the first string
 * @return the index found or -1 otherwise
 */
static int get_register_index(const char *token);

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
static void decode_load_operation(const char *token);

/**
 * Move to the first register the value of the right-side operand, which can be a register or a constant.
 * Pattern 1: MOVE Rn CONSTANT
 * Example 1: MOVE R1 2017
 * Pattern 2: MOVE Rn Rn
 * Example 2: MOVE R1 R2
 *
 * @param token: instruction with the operands
 */
static void decode_move_operation(const char *token);

/**
 * Store the value of the register in the address. The address can be a constant or a register value.
 * Pattern: STORE Rn CONSTANT
 * Example: STORE R1 100
 * Pattern: STORE Rn Rn
 * Example: STORE R1 R2
 *
 * @param token: instruction with the operands
 */
static void decode_store_operation(const char *token);

/**
 * Do arithmetic with the two right-side operands and assign the result to the first register.
 * Pattern: OPERATION Rn Rn Rn
 * Example 1: ADD R1 R2 R3
 * Example 2: DIVIDE R1 R2 R3 is equivalent to R1 = (R2 / R3)
 *
 * @param token: instruction with the operands
 */
static void decode_arithmetic_operation(const char *token);

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
static void decode_conditional_jump_operation(const char *token);

/**
 * Decodes unconditional jump operation.
 * Pattern: JUMPu CONSTANT
 * Example: JUMPu 10
 * It means: Jump to instruction of index 10
 *
 * @param token: instruction with the index of the next instruction
 */
static void decode_unconditional_jump_operation(const char *token);

/**
 * Decodes the instruction to increment 1 to the value of a register.
 *
 * @param token: the instruction with the register
 */
static void decode_increment_operation(char *token);

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
static void decode_compare_operation(const char *token);

/**
 * Decodes the instruction and prepare the operands and registers to the execution state.
 *
 * @param instruction: a string according to a language syntax
 */
void decode(const char *instruction);

/**
 * Executes one instruction using the values of operands and registers.
 *
 * @param operation: the first word of an instruction
 */
void execute(int operation);

void init_control_unit();

/**
 * Finds the data in the cache or in the memory
 */
static int find_data_in_address(int mar);

#endif //CONTROLUNIT_CONTROL_UNIT_H
