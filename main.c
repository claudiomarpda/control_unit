#include <stdio.h>

#include "include/memory.h"
#include "include/control_unit.h"
#include "include/cache_strategy.h"

char *instructions_memory_file_name = NULL;
char *data_memory_file_name = NULL;

void run(int program_number, int memory_size) {

    switch (program_number) {
        case 1:
            instructions_memory_file_name = "instructions-memory_arithmetic.txt";
            data_memory_file_name = "data-memory_arithmetic.txt";
            break;
        case 2:
            instructions_memory_file_name = "instructions-memory_fibonacci.txt";
            data_memory_file_name = "data-memory_fibonacci.txt";
            break;
        case 3:
            instructions_memory_file_name = "instructions-memory_bubble-sort.txt";
            data_memory_file_name = "data-memory_bubble-sort.txt";
            break;
        case 4:
            instructions_memory_file_name = "instructions-memory_matrix.txt";
            data_memory_file_name = "data-memory_matrix.txt";
            break;
        default:
            break;
    }

    if (!load_memory(memory_size, instructions_memory_file_name, data_memory_file_name)) {
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
    int program_number = 4;
    // Choose memory size
    int memory_size = 100;

    int total_hits[9];
    int total_misses[9];

    // Initial cache size
    int k = 2;

    for (int i = 0; i < 9; i++) {
        init_control_unit();
        // Set cache size
        int cache_size = k;
        cache_init(DIRECT, cache_size);

        // Run the same program several times with different cache size
        for (int j = 0; j < 5; j++) {
            run(program_number, memory_size);
            // Go to the begin of the instructions
            pc = 0;
        }

        write_data_memory(data_memory_file_name, mar, mbr);

        printf("Process finished with %d cycles.\n", cycles);
        printf("Cache hits: %d\n", cache_hit);
        printf("Cache misses: %d\n", cache_miss);

        cache_finish();
        memo_finish();

        total_hits[i] = cache_hit;
        total_misses[i] = cache_miss;

        // Increments cache size
        // 2 4 8 16 32 64 128
        k = k * 2;
    }

    putchar('\n');
    for(int i = 0; i < 9; i ++){
        printf("Cache size: 2^%d\n", i + 1);
        printf("Total hits: %d\n", total_hits[i]);
        printf("Total misses: %d\n\n", total_misses[i]);
    }


    return 0;
}