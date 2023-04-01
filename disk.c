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
    
    // If request queue is empty, return NULLRCB
    if (*queue_cnt == 0) {
        return NULL_RCB;
    }
    
    // Find the next request to service
    int next_request_idx = -1; // index of the next request to service in the queue
    int min_distance = INT_MAX; // min distance between current cylinder and request cylinder
    
    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (request_queue[i].arrival_timestamp < request_queue[next_request_idx].arrival_timestamp || next_request_idx == -1) {
                next_request_idx = i;
            }
        } else if (scan_direction == 1 && request_queue[i].cylinder > current_cylinder) {
            int distance = request_queue[i].cylinder - current_cylinder;
            if (distance < min_distance) {
                next_request_idx = i;
                min_distance = distance;
            }
        } else if (scan_direction == 1 && i == *queue_cnt - 1) {
            next_request_idx = i;
        } else if (scan_direction == 0 && request_queue[i].cylinder < current_cylinder) {
            int distance = current_cylinder - request_queue[i].cylinder;
            if (distance < min_distance) {
                next_request_idx = i;
                min_distance = distance;
            }
        } else if (scan_direction == 0 && i == *queue_cnt - 1) {
            next_request_idx = i;
        }
    }
    
    // Remove the next request to service from the queue
    struct RCB next_request = request_queue[next_request_idx];
    for (int i = next_request_idx; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    *queue_cnt -= 1;
    
    return next_request;
}
