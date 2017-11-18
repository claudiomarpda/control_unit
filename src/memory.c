//
// Created by mz on 05/09/17.
//

#include "../include/memory.h"

#include <stdio.h>
#include <string.h>

/**
	Loads the instructions into the memory. File must end with an empty line.

	@return 1 if success, 0 otherwise
*/
int load_instructions(const char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        printf("File %s could not be opened.\n", file_name);
        return 0;
    }

    // Goes through the file
    for (int i = 0; !feof(file); i++) {
        // Read a line and store it as an instruction
        fgets(instruction_memory[i], INSTRUCTION_LENGTH, file);
    }

    fclose(file);
    return 1;
}

/**
	Loads the data into the memory. Each line of data contains an index and a value.
    The index is the memory address of the value.
    File must end with an empty line.

	@return 1 if success, 0 otherwise
*/
int load_data(const char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        printf("File %s could not be opened.\n", file_name);
        return 0;
    }

    /*
     * Read the data for the memory
     * [0] is the index of the value
     * [1] is the value
     */
    for (int i = 0; !feof(file); i++) {
        fscanf(file, "%d", &data_memory[i][0]);
        fscanf(file, "%d", &data_memory[i][1]);
    }

    fclose(file);
    return 1;
}

/**
 * Initiates default values.
 */
void init_memory() {
    for (int i = 0; i < MAX_LINES; i++) {
        strcpy(instruction_memory[i], "");
        data_memory[i][0] = -1;
        data_memory[i][1] = -1;
    }
}

/**
	Loads the instructions and data into the memory.

	@param instructionsFile: where the instructions are stored.
	@param dataFile: where the data are stored.
	@return 1 if success, 0 otherwise.
*/
int load_memory(const char *instructions_file_name, const char *data_file_name) {
    init_memory();

    if (!load_instructions(instructions_file_name)) {
        return 0;
    }
    if (!load_data(data_file_name)) {
        return 0;
    }
    return 1;
}

/**
 * Gets a value from an address of memory data.
 *
 * @param address: where the value is stored
 * @return the value stored, 0 if not found
 */
int indirect_memory_access(int address) {
    // run through memory of data
    for (int i = 0; i < MAX_LINES; i++) {
        // look for the address
        if (data_memory[i][0] == address) {
            return data_memory[i][1];
        }
    }
    return -1;
}

/**
 * Updates memory of data looking for the right address and storing the new value.
 *
 * @param mar: the address of the new value
 * @param mbr: the new value
 */
void update_memory(int mar, int mbr) {
    // Look for existent address
    for (int i = 0; i < MAX_LINES; i++) {
        if (data_memory[i][0] == mar) {
            data_memory[i][1] = mbr;
            return;
        }
    }
    // If address is not in use, store it in the first free space of the array
    for (int i = 0; i < MAX_LINES; i++) {
        // Free spaces are identified by the default value -1
        if (data_memory[i][0] == -1) {
            data_memory[i][0] = mar;
            data_memory[i][1] = mbr;
            return;
        }
    }
}

/**
 * Writes the data from memory in an new output file.
 *
 * @param dataMemoryFile: the input file name
 * @param mar: the address of the value
 * @param mbr: the value
 */
void write_data_memory(const char *data_memory_file_name, int mar, int mbr) {
    update_memory(mar, mbr);

    size_t nameSize = strlen(data_memory_file_name) + strlen("output-");
    char outName[nameSize];
    strcpy(outName, "output_");
    strcat(outName, data_memory_file_name);
    FILE *file = fopen(outName, "w");

    if (file == NULL) {
        printf("File %s could not be opened.\n", outName);
        return;
    }

    // Goes through the array of data until a default index value or the end of array
    for (int i = 0; i < MAX_LINES; i++) {
        if (data_memory[i][0] != -1) {
            fprintf(file, "%d %d\n", data_memory[i][0], data_memory[i][1]);
        }
    }

    fclose(file);
}