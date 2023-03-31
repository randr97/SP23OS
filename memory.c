#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"


struct MEMORY_BLOCK NULL_BLOCK = {0,0,0,0};

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    return NULL_BLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    return NULL_BLOCK;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address) {
    return NULL_BLOCK;
}

bool isNullBlock(struct MEMORY_BLOCK* o) {
    return o->start_address == 0 && o->end_address == 0 && o->segment_size == 0 && o->process_id == 0;
}

void insert(struct MEMORY_BLOCK arr[], int pos, struct MEMORY_BLOCK new_element, int size) {
    int i;
    // shifting the elements to the right
    for(i=size - 1; i>pos; i--) {
        arr[i] = arr[i-1];
    }
    arr[pos-1] = new_element;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt) {
    int i, j;
    // finding the index of the freed block in memory map
    for(i=0; i<*map_cnt; i++) {
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


struct MEMORY_BLOCK best_fit_allocate(
    int request_size,
    struct MEMORY_BLOCK memory_map[MAPMAX],
    int *map_cnt,
    int process_id
)
{
    int i;
    int diff_size = MAX_INPUT, index;
    for(i=0; i<map_cnt; i++) {
        if(memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {
            if(diff_size > memory_map[i].segment_size - request_size) {
                diff_size = memory_map[i].segment_size - request_size;
                index = i;
            }
        }
    }
    if(diff_size == MAX_INPUT) return NULL_BLOCK;
    struct MEMORY_BLOCK new_block = {
        memory_map[index].start_address,
        memory_map[index].start_address + request_size - 1,
        request_size,
        process_id
    };
    insert(memory_map, index, new_block, map_cnt);
    memory_map[index + 1].start_address = memory_map[index].end_address + 1;
    memory_map[index + 1].segment_size = memory_map[index + 1].end_address - memory_map[index + 1].start_address + 1;
    map_cnt++;
    return new_block;
}
