#include <stdio.h>

#include "include/memory.h"
#include "include/control_unit.h"

int main() {
    // Load memory of instructions and data
    if (!loadMemory("instructions-memory.txt", "data-memory.txt")) {
        puts("ERROR: Could not load memory");
        return 0;
    }

    state = FETCH;

    // Handle the instruction cycles
    while (state != FINISH) {
        switch (state) {
            case FETCH:
                ir = fetch(instructionMemory);
                if(ir == NULL) {
                    if(LOG) {
                        puts("End of instructions");
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
//            case INDIRECT_ACCESS:
                mbr = indirectMemoryAccess(mar);
        }
    }

    return 0;
}