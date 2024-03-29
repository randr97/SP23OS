#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"


struct MEMORY_BLOCK NULL_BLOCK = {0,0,0,0};

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    // finding the first free memory block whose size is at least as large as the requested size
    int i;
    for(i=0; i<*map_cnt; i++) {
        if(memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            // found a free block that is at least as large as the requested size
            struct MEMORY_BLOCK allocated_block = {memory_map[i].start_address, memory_map[i].start_address+request_size-1, request_size, process_id};
            // splitting the block if it is larger than the requested size
            if(memory_map[i].segment_size > request_size) {
                // creating a new free block from the remaining memory
                struct MEMORY_BLOCK free_block = {allocated_block.end_address+1, memory_map[i].end_address, memory_map[i].end_address-allocated_block.end_address, 0};
                // shifting the rest of the blocks to the right
                int j;
                for(j=*map_cnt-1; j>i; j--) {
                    memory_map[j+1] = memory_map[j];
                }
                // updating the memory map with the new blocks
                memory_map[i] = allocated_block;
                memory_map[i+1] = free_block;
                (*map_cnt)++;
            } else {
                // updating the memory map with the allocated block
                memory_map[i] = allocated_block;
            }
            return allocated_block;
        }
    }
    // no free block of memory is at least as large as the requested size
    return NULL_BLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    int i, j;
    int max_size = -1;
    int max_index = -1;
    // finding the largest free memory block
    for(i=0; i<*map_cnt; i++) {
        if(memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            if(memory_map[i].segment_size > max_size) {
                max_size = memory_map[i].segment_size;
                max_index = i;
            }
        }
    }
    // no free block of memory is at least as large as the requested size
    if(max_index == -1) {
        return NULL_BLOCK;
    }
    // found a free block that is at least as large as the requested size
    struct MEMORY_BLOCK allocated_block = {memory_map[max_index].start_address, memory_map[max_index].start_address+request_size-1, request_size, process_id};
    // splitting the block if it is larger than the requested size
    if(memory_map[max_index].segment_size > request_size) {
        // creating a new free block from the remaining memory
        struct MEMORY_BLOCK free_block = {allocated_block.end_address+1, memory_map[max_index].end_address, memory_map[max_index].end_address-allocated_block.end_address, 0};
        // shifting the rest of the blocks to the right
        for(j=*map_cnt-1; j>max_index; j--) {
            memory_map[j+1] = memory_map[j];
        }
        // updating the memory map with the new blocks
        memory_map[max_index] = allocated_block;
        memory_map[max_index+1] = free_block;
        (*map_cnt)++;
    } else {
        // updating the memory map with the allocated block
        memory_map[max_index] = allocated_block;
    }
    return allocated_block;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address) {
    int i;
    // searching for a free block starting from the last allocation address
    for(i=last_address; i<*map_cnt; i++) {
        if(memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            // found a free block that is at least as large as the requested size
            struct MEMORY_BLOCK allocated_block = {memory_map[i].start_address, memory_map[i].start_address+request_size-1, request_size, process_id};
            // splitting the block if it is larger than the requested size
            if(memory_map[i].segment_size > request_size) {
                // creating a new free block from the remaining memory
                struct MEMORY_BLOCK free_block = {allocated_block.end_address+1, memory_map[i].end_address, memory_map[i].end_address-allocated_block.end_address, 0};
                // shifting the rest of the blocks to the right
                int j;
                for(j=*map_cnt-1; j>i; j--) {
                    memory_map[j+1] = memory_map[j];
                }
                // updating the memory map with the new blocks
                memory_map[i] = allocated_block;
                memory_map[i+1] = free_block;
                (*map_cnt)++;
            } else {
                // updating the memory map with the allocated block
                memory_map[i] = allocated_block;
            }
            return allocated_block;
        }
    }
    // search from the beginning if no free block is found from the last address
    for(i=0; i<last_address; i++) {
        if(memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            // found a free block that is at least as large as the requested size
            struct MEMORY_BLOCK allocated_block = {memory_map[i].start_address, memory_map[i].start_address+request_size-1, request_size, process_id};
            // splitting the block if it is larger than the requested size
            if(memory_map[i].segment_size > request_size) {
                // creating a new free block from the remaining memory
                struct MEMORY_BLOCK free_block = {allocated_block.end_address+1, memory_map[i].end_address, memory_map[i].end_address-allocated_block.end_address, 0};
                // shifting the rest of the blocks to the right
                int j;
                for(j=*map_cnt-1; j>i; j--) {
                    memory_map[j+1] = memory_map[j];
                }
                // updating the memory map with the new blocks
                memory_map[i] = allocated_block;
                memory_map[i+1] = free_block;
                (*map_cnt)++;
            } else {
                // updating the memory map with the allocated block
                memory_map[i] = allocated_block;
            }
            return allocated_block;
        }
    }
    return NULL_BLOCK;
}

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    // finding the free memory block whose size is closest to the requested size
    int i, best_index = -1, smallest_diff = INT_MAX;
    for(i=0; i<*map_cnt; i++) {
        if(memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            // found a free block that is at least as large as the requested size
            int diff = memory_map[i].segment_size - request_size;
            if(diff < smallest_diff) {
                best_index = i;
                smallest_diff = diff;
            }
        }
    }
    // allocating the best fit block
    if(best_index >= 0) {
        struct MEMORY_BLOCK allocated_block = {memory_map[best_index].start_address, memory_map[best_index].start_address+request_size-1, request_size, process_id};
        // splitting the block if it is larger than the requested size
        if(memory_map[best_index].segment_size > request_size) {
            // creating a new free block from the remaining memory
            struct MEMORY_BLOCK free_block = {allocated_block.end_address+1, memory_map[best_index].end_address, memory_map[best_index].end_address-allocated_block.end_address, 0};
            // shifting the rest of the blocks to the right
            int j;
            for(j=*map_cnt-1; j>best_index; j--) {
                memory_map[j+1] = memory_map[j];
            }
            // updating the memory map with the new blocks
            memory_map[best_index] = allocated_block;
            memory_map[best_index+1] = free_block;
            (*map_cnt)++;
        } else {
            // updating the memory map with the allocated block
            memory_map[best_index] = allocated_block;
        }
        return allocated_block;
    }
    // no free block of memory is at least as large as the requested size
    return NULL_BLOCK;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt) {
    int i, j;
    // finding the index of the freed block in memory map
    for(i=0; i<(*map_cnt); i++) {
        if(memory_map[i].process_id == freed_block.process_id) {
            // marking the freed block as free
            memory_map[i].process_id = 0;
            // merging with adjacent free blocks if any
            if(i > 0 && memory_map[i-1].process_id == 0) {
                // merging with the previous block
                memory_map[i-1].end_address = memory_map[i].end_address;
                memory_map[i-1].segment_size = memory_map[i-1].end_address - memory_map[i-1].start_address + 1;
                // shifting the rest of the blocks to the left
                for(j=i; j<*map_cnt-1; j++) {
                    memory_map[j] = memory_map[j+1];
                }
                (*map_cnt)--;
                i--;
            }
            if(i < *map_cnt-1 && memory_map[i+1].process_id == 0) {
                // merging with the next block
                memory_map[i].end_address = memory_map[i+1].end_address;
                memory_map[i].segment_size = memory_map[i].end_address - memory_map[i].start_address + 1;
                // shifting the rest of the blocks to the left
                for(j=i+1; j<*map_cnt-1; j++) {
                    memory_map[j] = memory_map[j+1];
                }
                (*map_cnt)--;
            }
            break;
        }
    }
}
