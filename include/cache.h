//
// Created by mz on 14/11/17.
//
#ifndef CONTROL_UNIT_CACHE_H
#define CONTROL_UNIT_CACHE_H

// Size of the cache to be defined by the user
int cache_lines;

typedef struct {
    int tag; // Index of the main memory of instructions
    char instruction[INSTRUCTION_LENGTH]; // The instruction is a string
} instruction_cache_t;

typedef struct {
    int address; // Address of the data
    int data; // Data located at the address
} data_cache_t;


#endif //CONTROL_UNIT_CACHE_H
