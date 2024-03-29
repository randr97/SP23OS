#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {

    // check if page is already in memory
    if (page_table[page_number].is_valid == 1) {
        // update last access time and reference count
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        // return the frame number
        return page_table[page_number].frame_number;
    }
    
    // check if there are free frames
    if (*frame_cnt > 0) {
        // get a frame from the frame pool
        int frame_number = frame_pool[(*frame_cnt)-1];
        (*frame_cnt)--;
        // update page table entry
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        // return the frame number
        return frame_number;
    }
    
    // no free frames, replace a page using FIFO policy
    int oldest_timestamp = INT_MAX;
    int frame_to_replace = -1;
    for (int i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid == 1 && page_table[i].arrival_timestamp < oldest_timestamp) {
            oldest_timestamp = page_table[i].arrival_timestamp;
            frame_to_replace = page_table[i].frame_number;
        }
    }
    // update the page table entry of the page to be replaced
    for (int i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid == 1 && page_table[i].frame_number == frame_to_replace) {
            page_table[i].is_valid = 0;
            page_table[i].frame_number = -1;
            page_table[i].arrival_timestamp = -1;
            page_table[i].last_access_timestamp = -1;
            page_table[i].reference_count = -1;
            break;
        }
    }
    // update the page table entry of the newly referenced page
    page_table[page_number].is_valid = 1;
    page_table[page_number].frame_number = frame_to_replace;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    // return the frame number
    return frame_to_replace;
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
    int faults = 0;
    int timestamp = 1;
    int oldest_timestamp = 1;
    int oldest_page_number = -1;

    if(table_cnt == 0) {
        int i;
        for(i=0; i<TABLEMAX; i++) {
            page_table[i].is_valid = 0;
        }
    }

    for (int i = 0; i < reference_cnt; i++) {
        int page_number = reference_string[i];
        timestamp++;
        if (page_table[page_number].is_valid) {
            page_table[page_number].last_access_timestamp = timestamp;
            page_table[page_number].reference_count++;
        } else {
            if (frame_cnt > 0) {
                int frame_number = frame_pool[--frame_cnt];
                page_table[page_number].frame_number = frame_number;
                page_table[page_number].is_valid = true;
                page_table[page_number].arrival_timestamp = timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1;
                faults++;
            } else {
                oldest_timestamp = INT_MAX;
                oldest_page_number = -1;

                for (int j = 0; j < table_cnt; j++) {
                    if (page_table[j].is_valid && page_table[j].arrival_timestamp < oldest_timestamp) {
                        oldest_timestamp = page_table[j].arrival_timestamp;
                        oldest_page_number = j;
                    }
                }

                page_table[oldest_page_number].is_valid = false;
                page_table[oldest_page_number].frame_number = -1;
                page_table[oldest_page_number].arrival_timestamp = 0;
                page_table[oldest_page_number].last_access_timestamp = 0;
                page_table[oldest_page_number].reference_count = 0;

                int frame_number = page_table[page_number].frame_number;
                page_table[page_number].frame_number = frame_number;
                page_table[page_number].is_valid = true;
                page_table[page_number].arrival_timestamp = timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1;
                faults++;
            }
        }
    }

    return faults;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {

    // check if page is already in memory
    if (page_table[page_number].is_valid == 1) {
        // update last access time and reference count
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        // return the frame number
        return page_table[page_number].frame_number;
    }

    // check if there are free frames
    if (*frame_cnt > 0) {
        // get a frame from the frame pool
        int frame_number = frame_pool[(*frame_cnt)-1];
        (*frame_cnt)--;
        // update page table entry
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        // return the frame number
        return frame_number;
    }

    // no free frames, replace a page using LRU policy
    int oldest_timestamp = INT_MAX;
    int frame_to_replace = -1;
    for (int i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid == 1 && page_table[i].last_access_timestamp < oldest_timestamp) {
            oldest_timestamp = page_table[i].last_access_timestamp;
            frame_to_replace = page_table[i].frame_number;
        }
    }
    // update the page table entry of the page to be replaced
    for (int i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid == 1 && page_table[i].frame_number == frame_to_replace) {
            page_table[i].is_valid = 0;
            page_table[i].frame_number = -1;
            page_table[i].arrival_timestamp = -1;
            page_table[i].last_access_timestamp = -1;
            page_table[i].reference_count = -1;
            break;
        }
    }
    // update the page table entry of the newly referenced page
    page_table[page_number].is_valid = 1;
    page_table[page_number].frame_number = frame_to_replace;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    // return the frame number
    return frame_to_replace;
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt,int reference_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    int faults = 0;
    int timestamp = 1;

    if(table_cnt == 0) {
        int i;
        for(i=0; i<TABLEMAX; i++) {
            page_table[i].is_valid = 0;
        }
    }

    for (int i = 0; i < reference_cnt; i++) {
        int page_number = reference_string[i];
        timestamp++;
        if (page_table[page_number].is_valid) {
            page_table[page_number].last_access_timestamp = timestamp;
            page_table[page_number].reference_count++;
        } else {
            if (frame_cnt > 0) {
                int frame_number = frame_pool[--frame_cnt];
                page_table[page_number].frame_number = frame_number;
                page_table[page_number].is_valid = true;
                page_table[page_number].arrival_timestamp = timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1;
                faults++;
            } else {
                int min_timestamp = INT_MAX;
                int min_page_number = -1;

                for (int j = 0; j < table_cnt; j++) {
                    if (page_table[j].is_valid && page_table[j].last_access_timestamp < min_timestamp) {
                        min_timestamp = page_table[j].last_access_timestamp;
                        min_page_number = j;
                    }
                }

                page_table[min_page_number].is_valid = false;
                page_table[min_page_number].frame_number = -1;
                page_table[min_page_number].arrival_timestamp = 0;
                page_table[min_page_number].last_access_timestamp = 0;
                page_table[min_page_number].reference_count = 0;

                int frame_number = page_table[page_number].frame_number;
                page_table[page_number].frame_number = frame_number;
                page_table[page_number].is_valid = true;
                page_table[page_number].arrival_timestamp = timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1;
                faults++;
            }
        }
    }

    return faults;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    int i, min_ref_count = INT_MAX, min_arrival_time = INT_MAX, min_ref_index = -1, free_frame_index = -1;

    // Check if the page is already in memory
    for (i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].frame_number != -1 && i == page_number) {
            page_table[i].last_access_timestamp = current_timestamp;
            page_table[i].reference_count++;
            return page_table[i].frame_number;
        }
    }

    // Page is not in memory, check if there are free frames
    if (*frame_cnt > 0) {
        free_frame_index = frame_pool[*frame_cnt - 1];
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].frame_number = free_frame_index;
        page_table[page_number].is_valid = 1;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return free_frame_index;
    }

    // Page is not in memory and no free frames, select page to replace using LFU policy
    for (i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].frame_number != -1) {
            if (page_table[i].reference_count < min_ref_count) {
                min_ref_count = page_table[i].reference_count;
                min_arrival_time = page_table[i].arrival_timestamp;
                min_ref_index = i;
            } else if (page_table[i].reference_count == min_ref_count && page_table[i].arrival_timestamp < min_arrival_time) {
                min_arrival_time = page_table[i].arrival_timestamp;
                min_ref_index = i;
            }
        }
    }

    // Replace selected page with newly-referenced page
    free_frame_index = page_table[min_ref_index].frame_number;
    page_table[min_ref_index].is_valid = 0;
    page_table[min_ref_index].frame_number = -1;
    page_table[min_ref_index].arrival_timestamp = -1;
    page_table[min_ref_index].last_access_timestamp = -1;
    page_table[min_ref_index].reference_count = -1;
    page_table[page_number].frame_number = free_frame_index;
    page_table[page_number].is_valid = 1;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;
    return free_frame_index;
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
    int faults = 0;
    int timestamp = 1;

    for (int i = 0; i < reference_cnt; i++) {
        int page_number = refrence_string[i];

        if (page_table[page_number].is_valid) {
            page_table[page_number].last_access_timestamp = timestamp;
            page_table[page_number].reference_count++;
        } else {
            if (frame_cnt > 0) {
                int frame_number = frame_pool[--frame_cnt];
                page_table[page_number].frame_number = frame_number;
                page_table[page_number].is_valid = true;
                page_table[page_number].arrival_timestamp = timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1;
                faults++;
            } else {
                int min_count = INT_MAX;
                int min_timestamp = INT_MAX;
                int min_page_number = -1;

                for (int j = 0; j < table_cnt; j++) {
                    if (page_table[j].is_valid && page_table[j].reference_count < min_count) {
                        min_count = page_table[j].reference_count;
                        min_timestamp = page_table[j].arrival_timestamp;
                        min_page_number = j;
                    } else if (page_table[j].is_valid && page_table[j].reference_count == min_count && page_table[j].arrival_timestamp < min_timestamp) {
                        min_timestamp = page_table[j].arrival_timestamp;
                        min_page_number = j;
                    }
                }

                page_table[min_page_number].is_valid = false;
                page_table[min_page_number].frame_number = -1;
                page_table[min_page_number].arrival_timestamp = 0;
                page_table[min_page_number].last_access_timestamp = 0;
                page_table[min_page_number].reference_count = 0;

                int frame_number = page_table[page_number].frame_number;
                page_table[page_number].frame_number = frame_number;
                page_table[page_number].is_valid = true;
                page_table[page_number].arrival_timestamp = timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1;
                faults++;
            }
        }

        timestamp++;
    }

    return faults;
}
