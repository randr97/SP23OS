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
    return 0;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    // Check if the page being referenced is already in memory
    for (int i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].arrival_timestamp != -1 && page_table[i].frame_number != -1 && page_table[i].frame_number == page_number) {
            // Modify the last_access_timestamp and reference_count fields
            page_table[i].last_access_timestamp = current_timestamp;
            page_table[i].reference_count++;

            // Return the frame number
            return page_number;
        }
    }

    // If the page being referenced is not in memory, check if there are any free frames
    if (*frame_cnt > 0) {
        // Remove a frame from the process frame pool and insert it into the page-table entry
        page_table[*table_cnt].is_valid = 1;
        page_table[*table_cnt].frame_number = frame_pool[--(*frame_cnt)];
        page_table[*table_cnt].arrival_timestamp = current_timestamp;
        page_table[*table_cnt].last_access_timestamp = current_timestamp;
        page_table[*table_cnt].reference_count = 1;

        // Increment the table count and return the frame number
        (*table_cnt)++;
        return page_table[*table_cnt - 1].frame_number;
    }

    // If there are no free frames, select the page with smallest last_access_timestamp
    int min_last_access_timestamp = INT_MAX;
    int page_to_replace = -1;

    for (int i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].arrival_timestamp != -1 && page_table[i].frame_number != -1) {
            if (page_table[i].last_access_timestamp < min_last_access_timestamp) {
                min_last_access_timestamp = page_table[i].last_access_timestamp;
                page_to_replace = i;
            }
        }
    }

    // Mark the page-table entry as invalid and free the frame
    int frame_to_free = page_table[page_to_replace].frame_number;
    page_table[page_to_replace].is_valid = 0;
    page_table[page_to_replace].frame_number = -1;
    page_table[page_to_replace].arrival_timestamp = -1;
    page_table[page_to_replace].last_access_timestamp = -1;
    page_table[page_to_replace].reference_count = -1;
    frame_pool[(*frame_cnt)++] = frame_to_free;

    // Insert the new page into the page-table entry and set the appropriate fields
    page_table[page_to_replace].is_valid = 1;
    page_table[page_to_replace].frame_number = frame_to_free;
    page_table[page_to_replace].arrival_timestamp = current_timestamp;
    page_table[page_to_replace].last_access_timestamp = current_timestamp;
    page_table[page_to_replace].reference_count = 1;

    // Return the frame number
    return frame_to_free;
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
