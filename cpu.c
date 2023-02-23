#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "oslabs.h"

struct PCB NULL_PCB;
NULL_PCB = {0,0,0,0,0,0,0};

bool isNullPCB(struct PCB* process) {
    if(
        process->process_id == 0 &&
        process->arrival_timestamp == 0 &&
        process->total_bursttime == 0 &&
        process->execution_starttime == 0 &&
        process->execution_endtime == 0 &&
        process->remaining_bursttime == 0 &&
        process->process_priority == 0) return true;
    else return false;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX],
    int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
    if(isNullPCB(&current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = new_process.total_bursttime + timestamp;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    if(current_process.process_priority <= new_process.process_priority) {
        new_process.execution_endtime = 0;
        new_process.execution_starttime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[new_process.process_id] = new_process;
        return current_process;
    } else {
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime = current_process.remaining_bursttime - (timestamp - current_process.execution_starttime);
        ready_queue[current_process.process_id] = current_process;
        // new process
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = new_process.total_bursttime + timestamp;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
};

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    int i, min = INT_MAX;
    struct PCB highest_priority;
    for(i=0;i<QUEUEMAX;i++) {
        if(!isNullPCB(&ready_queue[i])) {
            if(ready_queue[i].process_priority < min) {
                min = ready_queue[i].process_priority;
                highest_priority = ready_queue[i];
            }
        }
    }
    if(min == INT_MAX) {
        return NULL_PCB;
    } else {
        ready_queue[i] = NULL_PCB;
        highest_priority.execution_starttime = timestamp;
        highest_priority.execution_endtime = timestamp + highest_priority.remaining_bursttime;
        return highest_priority;
    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX],
    int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp)
{
    if(isNullPCB(&current_process)) {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    if(current_process.remaining_bursttime < new_process.total_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_starttime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[new_process.process_id] = new_process;
        return current_process;
    } else {
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime = current_process.remaining_bursttime - (time_stamp - current_process.execution_starttime);
        ready_queue[current_process.process_id] = current_process;
        // new process
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
}
