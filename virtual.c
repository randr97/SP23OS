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
    int page_faults = 0;
    int current_timestamp = 1;
    int next_frame_index = 0;

    for (int i = 0; i < reference_cnt; i++) {
        int page_number = reference_string[i];
        bool page_hit = false;

        // Check if the page is in memory
        for (int j = 0; j < table_cnt; j++) {
            if (page_table[j].is_valid && page_table[j].frame_number == page_number) {
                page_hit = true;
                page_table[j].last_access_timestamp = current_timestamp;
                page_table[j].reference_count++;
                break;
            }
        }

        // Page not in memory, need to load it
        if (!page_hit) {
            // Check if there are any free frames
            if (frame_cnt > 0) {
                int frame_number = frame_pool[next_frame_index++];
                page_table[table_cnt++] = (struct PTE){1, frame_number, current_timestamp, current_timestamp, 1};
                page_faults++;
                frame_cnt--;
            } else {
                // Page replacement needed
                int oldest_page_index = 0;
                int oldest_page_timestamp = INT_MAX;
                for (int j = 0; j < table_cnt; j++) {
                    if (page_table[j].is_valid && page_table[j].arrival_timestamp < oldest_page_timestamp) {
                        oldest_page_index = j;
                        oldest_page_timestamp = page_table[j].arrival_timestamp;
                    }
                }
                page_table[oldest_page_index].is_valid = 0;
                page_table[oldest_page_index].frame_number = -1;
                page_table[oldest_page_index].arrival_timestamp = -1;
                page_table[oldest_page_index].last_access_timestamp = -1;
                page_table[oldest_page_index].reference_count = -1;

                int frame_number = page_table[oldest_page_index].frame_number;
                page_table[oldest_page_index] = (struct PTE){1, frame_number, current_timestamp, current_timestamp, 1};
                page_faults++;
            }
        }

        current_timestamp++;
    }

    return page_faults;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    return 0;
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    return 0;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    return 0;
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    return 0;
}
