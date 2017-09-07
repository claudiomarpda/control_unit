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
    LOAD, MOVE_REGISTER, MOVE_CONST, STORE, ADD, MULTIPLY, SUBTRACT, DIVIDE, JUMP
};

// The current state of the instruction cycle
enum states state;
// The current operation
enum operations operation;

// Program counter points to the index of the next instruction of the memory
int pc;
// Instruction register keeps an instruction in string format
char *ir;
// Registers load data from memory and are used to do arithmetic operations
int reg[3];
// Register for loop
int loopReg;
// Memory Bus Register
int mbr;
// Memory Access Register
int mar;

/*
 * Operands identifiers. Handles three operand at most at a time.
 * For registers, it will receive the index of the register.
 * For memory access, it will receive an address.
 * For constants, it will receive a constant value.
 */
int operand1, operand2, operand3;


// Prototypes

// Some prototypes are commented to avoid interface usage

char *fetch(char instructionMemory[][INSTRUCTION_LENGTH]);

void decode(const char *instruction);

void execute(int operation);

//void decodeLoadOperation(const char *token);

//void decodeMoveOperation(const char *token);

//void decodeStoreOperation(const char *token);

//void decodeArithmeticOperation(const char *token);

//void decodeJumpOperation(char *token);

//int getRegisterIndex(const char *token);


#endif //CONTROLUNIT_CONTROL_UNIT_H
