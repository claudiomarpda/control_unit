//
// Created by mz on 14/11/17.
//
/*
 * Implements associative mapping for the cache memory.
 * Functions starts with acm_ (Associative Cache Memory)
 * */

#ifndef CONTROL_UNIT_CACHE_ASSOCIATIVE_H
#define CONTROL_UNIT_CACHE_ASSOCIATIVE_H

#include "memory.h"
#include "cache.h"


// Points to index of the cache where the next INSTRUCTION will be saved
int next_instruction_index;

// Points to index of the cache where the next DATA will be saved
int next_data_index;

// Cache for instructions
instruction_cache_t *acm_instructions;

// Cache for data
data_cache_t *acm_data;


/**
 * Check if an instruction exists in the cache.

 * @return 1 if true, else 0.
 */
int acm_instruction_exists(int main_memory_index);

/**
 * Checks if an address exists in the cache.
 *
 * @param main_memory_address: an address of the data memory.
 * For instance: LOAD R1 100
 * The program will load the value at the address 100 to R1.
 * So the argument to this function would be 100.
 * @see: the decoding process of the load operation for more details.
 * @return the cache index where the address is located
 */
int acm_data_address_exist(int main_memory_address);

/**
 * Reads an instruction from cache.
 */
char *acm_read_instruction(int cache_index);

/**
 * Reads a data from cache.
 */
int acm_read_data(int cache_index);

/**
 * Writes one line of the instruction memory in the data cache with Associative Mapping.
 *
 * @param main_memory_index
 * @param instruction: one line of instruction
 */
void acm_write_instruction(int main_memory_index, const char *instruction);

/**
 * Writes one line of the data memory in the data cache with Associative Mapping.
 * @param main_memory_address: where one data is located
 * @param data: data located at a specific address
 */
void acm_write_data(int main_memory_address, int data);


/**
 * Increments the index and limits it so it will be at most the size of the cache.
 *
 * @param index: the integer variable to be incremented
 */
void acm_increment_next_index(int *const index);

/**
 * Initiates the cache with appropriated values
 *
 * @param size: number of lines of the cache
 */
void acm_init(int size);

void acm_finish();

#endif //CONTROL_UNIT_CACHE_ASSOCIATIVE_H
