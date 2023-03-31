#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    int frame_number = -1;
    int i;

    // Check if page is already in memory
    for (i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].frame_number != -1 && page_table[i].frame_number < *frame_cnt && page_table[i].frame_number >= 0 && page_table[i].is_valid == 1 && page_table[i].last_access_timestamp != -1 && page_table[i].reference_count != -1) {
            if (page_table[i].frame_number == page_number) {
                page_table[i].last_access_timestamp = current_timestamp;
                page_table[i].reference_count++;
                return page_number;
            }
        }
    }

    // Check if there are free frames
    if (*frame_cnt < POOLMAX) {
        frame_number = *frame_cnt;
        frame_pool[frame_number] = 1;
        (*frame_cnt)++;
        page_table[*table_cnt].frame_number = frame_number;
        page_table[*table_cnt].is_valid = 1;
        page_table[*table_cnt].arrival_timestamp = current_timestamp;
        page_table[*table_cnt].last_access_timestamp = current_timestamp;
        page_table[*table_cnt].reference_count = 1;
        (*table_cnt)++;
        return frame_number;
    }

    // Replace page using FIFO
    int min_arrival_time = INT_MAX;
    int min_arrival_index = -1;

    for (i = 0; i < *table_cnt; i++) {
        if (page_table[i].is_valid && page_table[i].frame_number != -1 && page_table[i].frame_number < *frame_cnt && page_table[i].frame_number >= 0 && page_table[i].is_valid == 1 && page_table[i].last_access_timestamp != -1 && page_table[i].reference_count != -1) {
            if (page_table[i].arrival_timestamp < min_arrival_time) {
                min_arrival_time = page_table[i].arrival_timestamp;
                min_arrival_index = i;
            }
        }
    }

    page_table[min_arrival_index].is_valid = 0;
    frame_number = page_table[min_arrival_index].frame_number;
    page_table[min_arrival_index].frame_number = -1;
    page_table[min_arrival_index].arrival_timestamp = -1;
    page_table[min_arrival_index].last_access_timestamp = -1;
    page_table[min_arrival_index].reference_count = -1;

    page_table[*table_cnt].frame_number = frame_number;
    page_table[*table_cnt].is_valid = 1;
    page_table[*table_cnt].arrival_timestamp = current_timestamp;
    page_table[*table_cnt].last_access_timestamp = current_timestamp;
    page_table[*table_cnt].reference_count = 1;
    (*table_cnt)++;
    return frame_number;
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int reference_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
    int i, page_faults = 0;
    int current_timestamp = 1;

    for (i = 0; i < reference_cnt; i++) {
        int page_number = reference_string[i];
        int frame_number = process_page_access_fifo(page_table, &table_cnt, page_number, frame_pool, &frame_cnt, current_timestamp);
        if (frame_number != page_number) {
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
