#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"

struct PTE NULL_PTE = {0,0,0,0,0};

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    int i, frame_number = -1;

    // check if page is already in memory
    for (i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].frame_number != -1 && page_table[i].frame_number < *frame_cnt && page_table[i].frame_number >= 0 && i == page_number) {
            frame_number = page_table[i].frame_number;
            page_table[i].last_access_timestamp = current_timestamp;
            page_table[i].reference_count++;
            break;
        }
    }

    // if page not found in memory
    if (frame_number == -1) {
        // check if there are free frames
        if (*frame_cnt > 0) {
            frame_number = frame_pool[0];
            // remove frame from pool
            for (i = 0; i < (*frame_cnt) - 1; i++) {
                frame_pool[i] = frame_pool[i + 1];
            }
            (*frame_cnt)--;

            // update page-table entry
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame_number;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        // if no free frames, select a page to replace using FIFO
        else {
            int min_timestamp = INT_MAX;
            int min_timestamp_index = -1;

            for (i = 0; i < *table_cnt; i++) {
                if (page_table[i].is_valid && page_table[i].arrival_timestamp < min_timestamp) {
                    min_timestamp = page_table[i].arrival_timestamp;
                    min_timestamp_index = i;
                }
            }

            // update page-table entry of replaced page
            page_table[min_timestamp_index] = NULL_PTE;

            // update page-table entry of newly-referenced page
            frame_number = page_table[page_number].frame_number = page_table[min_timestamp_index].frame_number;
            page_table[page_number].is_valid = 1;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
    }

    return frame_number;
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
    int page_faults = 0;
    int current_timestamp = 1;
    int i;

    // process each page access in the reference string
    for (i = 0; i < reference_cnt; i++) {
        int page_number = refrence_string[i];
        int frame_number = process_page_access_fifo(page_table, &table_cnt, page_number, frame_pool, &frame_cnt, current_timestamp);

        // if the returned frame number is -1, it means there was a page fault
        if (frame_number == -1) {
            page_faults++;
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

