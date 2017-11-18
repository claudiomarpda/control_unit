//
// Created by mz on 18/11/17.
//
#include "../include/cache_strategy.h"
#include "../include/cache_direct.h"
#include "../include/cache_associative.h"

void cache_init(int mapping, int cache_size) {
    cache_mapping = (enum mappings) mapping;
    switch (mapping) {
        case ASSOCIATIVE:
            acm_init(cache_size);
            break;
        case DIRECT:
            dcm_init(cache_size);
            break;
    }
}

void cache_finish() {
    // Release resources
    switch (cache_mapping) {
        case ASSOCIATIVE:
            acm_finish();
            break;
        case DIRECT:
            dcm_finish();
            break;
    }
}

int cache_instruction_exists(int main_memory_index) {
    int index = -1;
    // Check if the instruction is in the cache
    switch (cache_mapping) {
        case ASSOCIATIVE:
            index = acm_instruction_exists(main_memory_index);
            break;
        case DIRECT:
            index = dcm_instruction_exists(main_memory_index);
            break;
    }
    return index;
}

int cache_data_address_exist(int main_memory_address) {
    int index = -1;
    switch (cache_mapping) {
        case ASSOCIATIVE:
            index = acm_data_address_exist(main_memory_address);
            break;
        case DIRECT:
            index = dcm_data_address_exist(main_memory_address);
            break;
    }
    return index;
}

char *cache_read_instruction(int cache_index) {
    char *instruction = NULL;
    switch (cache_mapping) {
        case ASSOCIATIVE:
            instruction = acm_read_instruction(cache_index);
            break;
        case DIRECT:
            instruction = dcm_read_instruction(cache_index);
            break;
    }
    return instruction;
}

int cache_read_data(int cache_index) {
    int data = -9999;
    switch (cache_mapping) {
        case ASSOCIATIVE:
            data = acm_read_data(cache_index);
            break;
        case DIRECT:
            data = dcm_read_data(cache_index);
            break;
    }
    return data;
}

void cache_write_instruction(int main_memory_index, const char *instruction) {
    // Update cache

    switch (cache_mapping) {
        case ASSOCIATIVE:
            acm_write_instruction(main_memory_index, instruction);
            break;
        case DIRECT:
            dcm_write_instruction(main_memory_index, instruction);
            break;
    }
}

void cache_write_data(int main_memory_address, int data) {
    // Update cache

    switch (cache_mapping) {
        case ASSOCIATIVE:
            acm_write_data(main_memory_address, data);
            break;
        case DIRECT:
            dcm_write_data(main_memory_address, data);
            break;
    }
}
