//
// Created by mz on 17/11/17.
//
/*
 * Implements Direct Mapping for the cache memory.
 * Functions starts with dcm_ (Direct Cache Memory)
 * */

#ifndef CONTROL_UNIT_CACHE_DIRECT_H
#define CONTROL_UNIT_CACHE_DIRECT_H

#include "cache.h"

// Cache for instructions
instruction_cache_t *dcm_instructions;
//instruction_cache_t acm_instructions[cache_lines];

// Cache for data
data_cache_t *dcm_data;
//data_cache_t acm_data[cache_lines];


/**
 * Implements Direct Mapping for memory cache.
 *
 * @return an address to which the cache maps
 */
int dcm_direct_mapping(int main_memory_index);

/**
 * Check if an instruction exists in the cache.

 * @return 1 if true, else 0.
 */
int dcm_instruction_exists(int main_memory_index);

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
int dcm_data_address_exist(int main_memory_address);

/**
 * Reads an instruction from cache.
 */
char *dcm_read_instruction(int cache_index);

/**
 * Reads a data from cache.
 */
int dcm_read_data(int cache_index);

/**
 * Writes one line of the instruction memory in the data cache with Direct Mapping.
 *
 * @param main_memory_index
 * @param instruction: one line of instruction
 */
void dcm_write_instruction(int main_memory_index, const char *instruction);

/**
 * Writes one line of the data memory in the data cache with Direct Mapping.
 *
 * @param main_memory_address: where one data is located
 * @param data: data located at a specific address
 */
void dcm_write_data(int main_memory_address, int data);

/**
 * Initiates the cache with appropriated values
 *
 * @param size: number of lines of the cache
 */
void dcm_init(int cache_size);

void dcm_finish();

#endif //CONTROL_UNIT_CACHE_DIRECT_H
