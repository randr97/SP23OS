#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    int frame_number;

    // Check if the page being referenced is already in memory
    if (page_table[page_number].is_valid) {
        // Update last_access_timestamp and reference_count fields
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        frame_number = page_table[page_number].frame_number;
    } else {
        // Check if there are any free frames
        if (*frame_cnt > 0) {
            // Remove a frame from the process frame pool
            frame_number = frame_pool[(*frame_cnt) - 1];
            (*frame_cnt)--;

            // Insert frame number into the page-table entry
            page_table[page_number].frame_number = frame_number;
            page_table[page_number].is_valid = true;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        } else {
            // Find the page with the smallest arrival_timestamp
            int oldest_page = 0;
            for (int i = 1; i < *table_cnt; i++) {
                if (page_table[i].is_valid && page_table[i].arrival_timestamp < page_table[oldest_page].arrival_timestamp) {
                    oldest_page = i;
                }
            }

            // Free the frame associated with the oldest page
            frame_number = page_table[oldest_page].frame_number;
            page_table[oldest_page].is_valid = false;
            page_table[oldest_page].frame_number = -1;
            page_table[oldest_page].arrival_timestamp = -1;
            page_table[oldest_page].last_access_timestamp = -1;
            page_table[oldest_page].reference_count = -1;

            // Insert frame number into the page-table entry of the newly-referenced page
            page_table[page_number].frame_number = frame_number;
            page_table[page_number].is_valid = true;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
    }

    return frame_number;
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
    return 0;
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
