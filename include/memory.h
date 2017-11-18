//
// Created by mz on 05/09/17.
//

#ifndef CONTROLUNIT_MEMORY_H
#define CONTROLUNIT_MEMORY_H

int memo_lines;
#define MEMORY_LINES 100
#define INSTRUCTION_LENGTH 100

// Global variables
char **instruction_memory;
int **data_memory;


/**
	Loads the instructions into the memory. File must end with an empty line.

	@return 1 if success, 0 otherwise
*/
static int load_instructions(const char *file_name);

/**
	Loads the data into the memory. Each line of data contains an index and a value.
    The index is the memory address of the value.
    File must end with an empty line.

	@return 1 if success, 0 otherwise
*/
static int load_data(const char *file_name);

/**
	Loads the instructions and data into the memory.

	@param instructionsFile: where the instructions are stored.
	@param dataFile: where the data are stored.
	@return 1 if success, 0 otherwise.
*/
int load_memory(int memory_size, const char *instructions_file_name, const char *data_file_name);

/**
 * Gets a value from an address of memory data.
 *
 * @param address: where the value is stored
 * @return the value stored, 0 if not found
 */
int indirect_memory_access(int address);

/**
 * Updates memory of data looking for the right address and storing the new value.
 *
 * @param mar: the address of the new value
 * @param mbr: the new value
 */
void update_memory(int mar, int mbr);

/**
 * Writes the data from memory in an new output file.
 *
 * @param dataMemoryFile: the input file name
 * @param mar: the address of the value
 * @param mbr: the value
 */
void write_data_memory(const char *data_memory_file_name, int mar, int mbr);

/**
 * Allocate memory dynamically ir order to let user choose the size of the memory
 *
 * @param memory_size
 */
static void memo_allocate(int memory_size);

/**
 * Deallocate memory
 */
void memo_finish();


#endif //CONTROLUNIT_MEMORY_H
