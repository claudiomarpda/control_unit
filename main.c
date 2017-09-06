#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/memory.h"

// Every possible state of the instruction cycle
enum states {
    FETCH, DECODE, EXECUTE, INDIRECT_ACCESS, FINISH
};

// Enables logging for printing details
const int LOG = 1;
// The current state of the instruction cycle
enum states state;

// Program counter points to the index of the next instruction of the memory
int pc;
// Instruction register keeps an instruction in string format
char *ir = NULL;
// Registers load data from memory and are used to do arithmetic operations
int reg[3];
// Register for loop
int loopReg;
// Memory Bus Register
int mbr;
// Memory Access Register
int mar;

/**
 * Fetches the next instruction according to program counter.
 *
 * @return an instruction as string
 */
char *fetch() {

    if(LOG) {
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
 * Decodes one instruction in pieces and handles every string.
 *
 * @param instruction
 */
void decode(const char *instruction) {

    if (LOG) {
        printf("%s", instruction);
        printf("Decoding...");
    }

    // TODO: identify operations, registers and constants
}

/**
 * Gets a value from memory of data.
 *
 * @param address: where the value is stored
 * @return the value stored, 0 if not found
 */
int indirectMemoryAccess(int address) {
    // run through memory of data
    for (int i = 0; i < MAX_LINES; i++) {
        // look for the address
        if (dataMemory[i][0] == address) {
            return dataMemory[i][1];
        }
    }
    return 0;
}

int main() {
    // load memory of instructions and data
    if (!loadMemory("instructions-memory.txt", "data-memory.txt")) {
        puts("ERROR: Could not load memory");
        return 0;
    }

    state = FETCH;

    // handle the instruction cycles
    while (1) {
        switch (state) {
            case FETCH:
                ir = fetch();
                state = DECODE;
            case DECODE:
                decode(ir);
                state = EXECUTE;
                break;
            case EXECUTE:
//                execute();
//                state = FETCH;
                state = FINISH;
                break;
//            case INDIRECT_ACCESS:
                mbr = indirectMemoryAccess(mar);
//            default:
//                state = FINISH;
        }
        if (state == FINISH) {
            break;
        }
    }

    return 0;
}