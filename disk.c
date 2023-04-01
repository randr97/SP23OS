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
    struct RCB next_request = NULL_RCB;
    int shortest_distance = INT_MAX;
    int earliest_arrival_time = INT_MAX;

    // Check if queue is empty
    if (*queue_cnt == 0) {
        return NULL_RCB;
    }

    // Find the request with earliest arrival time and closest cylinder
    for (int i = 0; i < *queue_cnt; i++) {
        struct RCB request = request_queue[i];
        int distance = abs(request.cylinder - current_cylinder);

        // Check if request has same cylinder as current cylinder
        if (request.cylinder == current_cylinder) {
            if (request.arrival_timestamp < earliest_arrival_time) {
                next_request = request;
                earliest_arrival_time = request.arrival_timestamp;
            }
        }
        // Check if scan direction is 1 and request has a larger cylinder than current cylinder
        else if (scan_direction == 1 && request.cylinder > current_cylinder) {
            if (distance < shortest_distance) {
                next_request = request;
                shortest_distance = distance;
            }
        }
        // Check if scan direction is 1 and there are no requests with larger cylinders than current cylinder
        else if (scan_direction == 1 && i == (*queue_cnt - 1)) {
            next_request = request;
        }
        // Check if scan direction is 0 and request has a smaller cylinder than current cylinder
        else if (scan_direction == 0 && request.cylinder < current_cylinder) {
            if (distance < shortest_distance) {
                next_request = request;
                shortest_distance = distance;
            }
        }
        // Check if scan direction is 0 and there are no requests with smaller cylinders than current cylinder
        else if (scan_direction == 0 && i == (*queue_cnt - 1)) {
            next_request = request;
        }
    }

    // Remove the next_request from the request queue
    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].request_id == next_request.request_id) {
            for (int j = i; j < (*queue_cnt - 1); j++) {
                request_queue[j] = request_queue[j + 1];
            }
            (*queue_cnt)--;
            break;
        }
    }

    return next_request;
}
