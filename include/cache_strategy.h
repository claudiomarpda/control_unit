//
// Created by mz on 18/11/17.
//
/*
 * This is an abstraction of Cache Memory access
 * which runs the algorithm according to the current type of mapping
 */
#ifndef CONTROL_UNIT_CACHE_STRATEGY_H
#define CONTROL_UNIT_CACHE_STRATEGY_H

#include <wchar.h>

enum mappings {
    ASSOCIATIVE, DIRECT
};

// The type of mapping for memory cache
static enum mappings cache_mapping;


/**
 * Check if an instruction exists in the cache.

 * @return 1 if true, else 0.
 */
int cache_instruction_exists(int main_memory_index);

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
int cache_data_address_exist(int main_memory_address);

/**
 * Reads an instruction from cache.
 */
char *cache_read_instruction(int cache_index);

/**
 * Reads a data from cache.
 */
int cache_read_data(int cache_index);

/**
 * Writes one line of the instruction memory in the data cache according to mapping type.
 *
 * @param main_memory_index
 * @param instruction: one line of instruction
 */
void cache_write_instruction(int main_memory_index, const char *instruction);

/**
 * Writes one line of the data memory in the data cache with according to mapping type.
 *
 * @param main_memory_address: where one data is located
 * @param data: data located at a specific address
 */
void cache_write_data(int main_memory_address, int data);

/**
 * Initiates the cache with appropriated values
 *
 * @param size: number of lines of the cache
 */
void cache_init(int mapping, int cache_size);

void cache_finish();

#endif //CONTROL_UNIT_CACHE_STRATEGY_H
