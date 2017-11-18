//
// Created by mz on 17/11/17.
//

#include <string.h>
#include <malloc.h>
#include "../include/cache_direct.h"


int dcm_direct_mapping(int main_memory_index) {
    return main_memory_index % cache_lines;
}

int dcm_instruction_exists(int main_memory_index) {
    // Get the mapped index
    int i = dcm_direct_mapping(main_memory_index);
    // Check if index is saved
    if (dcm_instructions[i].tag == main_memory_index) {
        return i;
    }
    return -1;
}

int dcm_data_address_exist(int main_memory_address) {
    // Get the mapped index
    int i = dcm_direct_mapping(main_memory_address);
    // Check if address is saved
    if (dcm_data[i].address == main_memory_address) {
        return i;
    }
    return -1;
}

char *dcm_read_instruction(int cache_index) {
    return dcm_instructions[cache_index].instruction;
}

int dcm_read_data(int cache_index) {
    return dcm_data[cache_index].data;
}

void dcm_write_instruction(int main_memory_index, const char *instruction) {
    // Get the mapped index
    int i = dcm_direct_mapping(main_memory_index);
    // Save tag
    dcm_instructions[i].tag = main_memory_index;
    // Copy the instruction
    strcpy(dcm_instructions[i].instruction, instruction);
}

void dcm_write_data(int main_memory_address, int data) {
    // Get the mapped index
    int i = dcm_direct_mapping(main_memory_address);
    // Save address
    dcm_instructions[i].tag = main_memory_address;
    // Copy the data
    dcm_data[i].data = data;
}

void dcm_init(int cache_size) {
    cache_lines = cache_size;

    dcm_instructions = malloc(sizeof(instruction_cache_t) * cache_lines);
    dcm_data = malloc(sizeof(data_cache_t) * cache_lines);

    for (int i = 0; i < cache_lines; i++) {
        dcm_instructions[i].tag = -1;
        dcm_data[i].address = -1;
    }
}

void dcm_finish() {
    free(dcm_instructions);
    free(dcm_data);
}

