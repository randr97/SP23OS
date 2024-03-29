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
    if (*queue_cnt == 0) {
        return NULL_RCB;
    }

    for (int i = 0; i < *queue_cnt - 1; i++) {
        for (int j = i + 1; j < *queue_cnt; j++) {
            if (request_queue[i].cylinder > request_queue[j].cylinder) {
                struct RCB temp = request_queue[i];
                request_queue[i] = request_queue[j];
                request_queue[j] = temp;
            }
        }
    }

    int i, closest_distance = INT_MAX;
    struct RCB *closest_request = NULL;
    int closest_timestamp = INT_MAX;
    int closest_index = -1;

    for (i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (request_queue[i].arrival_timestamp < closest_timestamp) {
                closest_request = &request_queue[i];
                closest_timestamp = request_queue[i].arrival_timestamp;
                closest_index = i;
            }
        }
    }

    if (closest_request != NULL) {

        struct RCB selected_request = request_queue[closest_index];
        for (i = closest_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        
        return selected_request;
    }

    if (scan_direction == 1) {
        for (i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder > current_cylinder) {
                int distance = request_queue[i].cylinder - current_cylinder;
                if (distance < closest_distance) {
                    closest_request = &request_queue[i];
                    closest_distance = distance;
                    closest_index = i;
                }
            }
        }
        
    }

    if (scan_direction == 0) {
        for (i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder < current_cylinder) {
                int distance = current_cylinder - request_queue[i].cylinder;
                if (distance < closest_distance) {
                    closest_request = &request_queue[i];
                    closest_distance = distance;
                    closest_index = i;
                }
            }
        }
    }

    if (closest_request != NULL) {
        // Remove selected request from request queue
        struct RCB selected_request = request_queue[closest_index];
        for (int i = closest_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        
        return selected_request;
    }

    struct RCB selected_request = NULL_RCB;
    int shortest_distance = INT_MAX;
    int earliest_timestamp = INT_MAX;
    int selected_index = -1;

    for (int i = 0; i < *queue_cnt; i++) {
        int distance = abs(request_queue[i].cylinder - current_cylinder);
        if (distance < shortest_distance) {
            shortest_distance = distance;
            earliest_timestamp = request_queue[i].arrival_timestamp;
            selected_request = request_queue[i];
            selected_index = i;
        } else if (distance == shortest_distance && request_queue[i].arrival_timestamp < earliest_timestamp) {
            earliest_timestamp = request_queue[i].arrival_timestamp;
            selected_request = request_queue[i];
            selected_index = i;
        }
    }

    if (selected_index != -1) {
        for (int i = selected_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
    }
    
    return selected_request;
}
