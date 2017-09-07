#include <stdio.h>

#include "include/memory.h"
#include "include/control_unit.h"

int main() {
    // Load memory of instructions and data

    char *instructionsMemoryFileName = "instructions-memory.txt";
    char *dataMemoryFileName = "data-memory.txt";

    if (!loadMemory(instructionsMemoryFileName, dataMemoryFileName)) {
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
                execute(operation, dataMemoryFileName);
                state = FETCH;
                break;
//            case INDIRECT_ACCESS:
                mbr = indirectMemoryAccess(mar);
        }
    }

    FILE *f = fopen("opa.txt", "w");
    fprintf(f, "aaaaaaaaaaaaaaaaaa");
    fclose(f);

    return 0;
}