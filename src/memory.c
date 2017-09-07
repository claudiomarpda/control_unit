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
int loadInstructions(const char *fileName) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("File %s could not be opened.\n", fileName);
        return 0;
    }

    // Goes through the file
    for (int i = 0; !feof(file); i++) {
        // Read a line and store it as an instruction
        fgets(instructionMemory[i], INSTRUCTION_LENGTH, file);
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
int loadData(const char *fileName) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("File %s could not be opened.\n", fileName);
        return 0;
    }

    /*
     * Read the data for the memory
     * [0] is the index of the value
     * [1] is the value
     */
    for (int i = 0; !feof(file); i++) {
        fscanf(file, "%d", &dataMemory[i][0]);
        fscanf(file, "%d", &dataMemory[i][1]);
    }

    fclose(file);
    return 1;
}

/**
 * Initiates default values.
 */
void initMemory() {
    for (int i = 0; i < MAX_LINES; i++) {
        strcpy(instructionMemory[i], "");
        dataMemory[i][0] = -1;
        dataMemory[i][1] = -1;
    }
}

/**
	Loads the instructions and data into the memory.

	@param instructionsFile: where the instructions are stored.
	@param dataFile: where the data are stored.
	@return 1 if success, 0 otherwise.
*/
int loadMemory(const char *instructionsFileName, const char *dataFileName) {
    initMemory();

    if (!loadInstructions(instructionsFileName)) {
        return 0;
    }
    if (!loadData(dataFileName)) {
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
int indirectMemoryAccess(int address) {
    // run through memory of data
    for (int i = 0; i < MAX_LINES; i++) {
        // look for the address
        if (dataMemory[i][0] == address) {
            return dataMemory[i][1];
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
void updateMemory(int mar, int mbr) {
    // Look for existent address
    for (int i = 0; i < MAX_LINES; i++) {
        if (dataMemory[i][0] == mar) {
            dataMemory[i][1] = mbr;
            return;
        }
    }
    // If address is not in use, store it in the first free space of the array
    for (int i = 0; i < MAX_LINES; i++) {
        // Free spaces are identified by the default value -1
        if(dataMemory[i][0] == -1) {
            dataMemory[i][0] = mar;
            dataMemory[i][1] = mbr;
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
void writeMemory(const char *dataMemoryFileName, const int mar, const int mbr) {
    updateMemory(mar, mbr);

    size_t nameSize = strlen(dataMemoryFileName) + strlen("output-");
    char outName[nameSize];
    strcpy(outName, "output-");
    strcat(outName, dataMemoryFileName);
    FILE *file = fopen(outName, "w");

    if (file == NULL) {
        printf("File %s could not be opened.\n", outName);
        return;
    }

    // Goes through the array of data until a default index value or the end of array
    for (int i = 0; i < MAX_LINES; i++) {
        if(dataMemory[i][0] != -1) {
            fprintf(file, "%d %d\n", dataMemory[i][0], dataMemory[i][1]);
        }
    }

    fclose(file);
}