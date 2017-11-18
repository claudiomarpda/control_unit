#include <stdio.h>

#include "include/memory.h"
#include "include/control_unit.h"
#include "include/cache_associative.h"
#include "include/cache_direct.h"

char *instructions_memory_file_name = NULL;
char *data_memory_file_name = NULL;

void run(int program_number) {

    switch (program_number) {
        case 1:
            instructions_memory_file_name = "instructions-memory_arithmetic.txt";
            data_memory_file_name = "data-memory_arithmetic.txt";
            break;
        case 2:
            instructions_memory_file_name = "instructions-memory_factorial.txt";
            data_memory_file_name = "data-memory_factorial.txt";
            break;
        case 3:
            instructions_memory_file_name = "instructions-memory_fibonacci.txt";
            data_memory_file_name = "data-memory_fibonacci.txt";
            break;
        case 4:
            instructions_memory_file_name = "instructions-memory_bubble-sort.txt";
            data_memory_file_name = "data-memory_bubble-sort.txt";
            break;
        case 5:
            instructions_memory_file_name = "instructions-memory_matrix.txt";
            data_memory_file_name = "data-memory_matrix.txt";
            break;
        default:
            break;
    }

    if (!load_memory(instructions_memory_file_name, data_memory_file_name)) {
        puts("ERROR: Could not load memory");
        return;
    }

    state = FETCH;

    // Handle the instruction cycle
    while (state != FINISH) {
        switch (state) {
            case FETCH:
                ir = fetch(instruction_memory);
                if (ir == NULL) {
                    if (LOG) {
                        puts("End of instructions #");
                    }
                    // There are no more instructions
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

}

/**
 * Reads instructions and data from memory and handles the instruction cycle until the end of the instructions.
 */
int main() {

    // Choose program
    int program_number = 5;
    // Choose cache size
    int cache_size = 10;
    // Choose mapping type
    cache_mapping = ASSOCIATIVE;

    init_control_unit();

    switch(cache_mapping) {
        case ASSOCIATIVE:
            acm_init(cache_size);
            break;
        case DIRECT:
            dcm_init(cache_size);
            break;
    }

    // Run the same program several times
    for (int i = 0; i < 5; i++) {
        run(program_number);
        pc = 0;
    }

    write_data_memory(data_memory_file_name, mar, mbr);
    printf("Process finished with %d cycles.\n", cycles);

    // Release resources
    switch(cache_mapping) {
        case ASSOCIATIVE:
            acm_finish();
            break;
        case DIRECT:
            dcm_finish();
            break;
    }

    return 0;
}