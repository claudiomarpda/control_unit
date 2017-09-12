#include <stdio.h>

#include "include/memory.h"
#include "include/control_unit.h"

/**
 * Reads instructions and data from memory and handles the instruction cycle until the end of the instructions.
 */
int main() {

    ir = NULL;
    char *instructionsMemoryFileName = "instructions-memory2.txt";
    char *dataMemoryFileName = "data-memory2.txt";

    if (!loadMemory(instructionsMemoryFileName, dataMemoryFileName)) {
        puts("ERROR: Could not load memory");
        return 0;
    }

    state = FETCH;

    // Handle the instruction cycle
    while (state != FINISH) {
        switch (state) {
            case FETCH:
                ir = fetch(instructionMemory);
                if (ir == NULL) {
                    if (LOG) {
                        puts("End of instructions #");
                    }
                    // There is no more instructions
                    state = FINISH;
                    break;
                }
                state = DECODE;
                break;
            case DECODE:
                decode(ir);
                state = EXECUTE;
                break;
            case EXECUTE:
                execute(operation);
                state = FETCH;
                break;
        }
    }

    writeMemory(dataMemoryFileName, mar, mbr);

    return 0;
}