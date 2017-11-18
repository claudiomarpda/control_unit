//
// Created by mz on 14/11/17.
//
#include <string.h>
#include <malloc.h>
#include "../include/cache_associative.h"

int acm_instruction_exists(int main_memory_index) {
    for (int i = 0; i < cache_lines; i++) {
        if (acm_instructions[i].tag == main_memory_index) {
            return i;
        }
    }
    return -1;
}

int acm_data_address_exist(int main_memory_address) {
    for (int i = 0; i < cache_lines; i++) {
        if (acm_data[i].address == main_memory_address) {
            return i;
        }
    }
    return -1;
}

char *acm_read_instruction(int cache_index) {
    return acm_instructions[cache_index].instruction;
}

int acm_read_data(int cache_index) {
    return acm_data[cache_index].data;
}

void acm_increment_next_index(int *const index) {
    *index = (*index + 1) % cache_lines;
}

void acm_write_instruction(int main_memory_index, const char *instruction) {
    // Save tag
    acm_instructions[next_instruction_index].tag = main_memory_index;
    // Save instruction
    strcpy(acm_instructions[next_instruction_index].instruction, instruction);
    // Increment index
    acm_increment_next_index(&next_instruction_index);
}

void acm_write_data(int main_memory_address, int data) {
    // Save address
    acm_data[next_data_index].address = main_memory_address;
    // Save data
    acm_data[next_data_index].data = data;
    // Increment index
    acm_increment_next_index(&next_instruction_index);
}

void acm_init(int cache_size) {

    cache_lines = cache_size;

    acm_instructions = malloc(sizeof(instruction_cache_t) * cache_lines);
    acm_data = malloc(sizeof(data_cache_t) * cache_lines);

    for (int i = 0; i < cache_lines; i++) {
        acm_instructions[i].tag = -1;
        acm_data[i].address = -1;
    }
}

void acm_finish() {
    free(acm_instructions);
    free(acm_data);
}
