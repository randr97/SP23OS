#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include "oslabs.h"

struct RCB NULL_RCB = {0,0,0,0,0};

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // If disk is free, service the new request immediately
    if (current_request.request_id == 0) {
        return new_request;
    }
    // If disk is busy, add the new request to the queue
    request_queue[*queue_cnt] = new_request;
    *queue_cnt += 1;
    return current_request;
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt) {
    // If queue is empty, return NULL_RCB
    if (*queue_cnt == 0) {
    return NULL_RCB;
    }
    // Find the RCB with earliest arrival time
    int earliest_idx = 0;
    for (int i = 1; i < *queue_cnt; i++) {
    if (request_queue[i].arrival_timestamp < request_queue[earliest_idx].arrival_timestamp) {
    earliest_idx = i;
    }
    }
    // Remove the RCB from the queue and return it
    struct RCB earliest_request = request_queue[earliest_idx];
    for (int i = earliest_idx; i < *queue_cnt - 1; i++) {
    request_queue[i] = request_queue[i + 1];
    }
    *queue_cnt -= 1;
    return earliest_request;
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // If disk is free, service the new request immediately
    if (current_request.request_id == 0) {
        return new_request;
    }
    // If disk is busy, add the new request to the queue
    request_queue[*queue_cnt] = new_request;
    *queue_cnt += 1;
    return current_request;
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder) {
    // Initialize variables to track the RCB with the shortest seek time and earliest arrival time
    int shortest_seek_time = INT_MAX;
    int earliest_arrival_time = INT_MAX;
    int next_request_index = -1;

    // Loop through the request queue and find the RCB with the shortest seek time and earliest arrival time
    for (int i = 0; i < *queue_cnt; i++) {
        int seek_time = abs(request_queue[i].cylinder - current_cylinder);
        int arrival_time = request_queue[i].arrival_timestamp;
        if (seek_time < shortest_seek_time || (seek_time == shortest_seek_time && arrival_time < earliest_arrival_time)) {
            shortest_seek_time = seek_time;
            earliest_arrival_time = arrival_time;
            next_request_index = i;
        }
    }

    // If there is a request to service next, remove it from the request queue and return its RCB
    if (next_request_index != -1) {
        struct RCB next_request = request_queue[next_request_index];
        for (int i = next_request_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i+1];
        }
        *queue_cnt -= 1;
        return next_request;
    }

    // If there is no request to service next, return NULL_RCB
    return NULL_RCB;
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // If disk is free, service the new request immediately
    if (current_request.request_id == 0) {
        return new_request;
    }
    // If disk is busy, add the new request to the queue
    request_queue[*queue_cnt] = new_request;
    *queue_cnt += 1;
    return current_request;
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction) {
    // Initialize variables to keep track of the closest RCB in each direction
    struct RCB closest_lower = NULL_RCB;
    struct RCB closest_higher = NULL_RCB;
    
    // Initialize variables to keep track of the earliest RCB with the same cylinder
    struct RCB earliest_same_cylinder = NULL_RCB;
    int earliest_arrival_time = INT_MAX;

    // // find for same
    // for (int i = 0; i < *queue_cnt; i++) {
    //     struct RCB current_rcb = request_queue[i];
    //     if (current_rcb.cylinder == current_cylinder) {
    //         if (current_rcb.arrival_timestamp < earliest_arrival_time) {
    //             earliest_same_cylinder = current_rcb;
    //             earliest_arrival_time = current_rcb.arrival_timestamp;
    //         }
    //     }
    // }
    
    // Find the closest RCB in each direction and the earliest RCB with the same cylinder
    for (int i = 0; i < *queue_cnt; i++) {
        struct RCB current_rcb = request_queue[i];
        if (current_rcb.cylinder == current_cylinder) {
            if (current_rcb.arrival_timestamp < earliest_arrival_time) {
                earliest_same_cylinder = current_rcb;
                earliest_arrival_time = current_rcb.arrival_timestamp;
            }
        } else if (current_rcb.cylinder < current_cylinder) {
            if (closest_lower.cylinder == 0 || current_cylinder - current_rcb.cylinder < current_cylinder - closest_lower.cylinder) {
                closest_lower = current_rcb;
            }
        } else {
            if (closest_higher.cylinder == 0 || current_rcb.cylinder - current_cylinder < closest_higher.cylinder - current_cylinder) {
                closest_higher = current_rcb;
            }
        }
    }

    if(earliest_same_cylinder.request_id != 0) {
        return earliest_same_cylinder;
    }

    // Determine which RCB to service next based on the current direction and the RCBs found above
    if (scan_direction == 1) {
        if (closest_higher.cylinder != 0) {
            return closest_higher;
        } else {
            return closest_lower;
        }
    } else {
        if (closest_lower.cylinder != 0) {
            return closest_lower;
        } else {
            return closest_higher;
        }
    }
}
