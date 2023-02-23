#include <dirent.h> 
#include <stdio.h> 
#include <oslab.h>
#define QUEUEMAX 10


struct PCB {
    int process_id;
    int arrival_timestamp;
    int total_bursttime;
    int execution_starttime;
    int execution_endtime;
    int remaining_bursttime;
    int process_priority;
};

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    DIR *d;
    struct dirent *dir;
    struct PCB pcb1;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return pcb1;
}
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    struct PCB pcb1;
    return pcb1;
}
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    struct PCB pcb1;
    return pcb1;
}
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    struct PCB pcb1;
    return pcb1;
}
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    struct PCB pcb1;
    return pcb1;
}
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    struct PCB pcb1;
    return pcb1;
}

// #include <stdio.h>
// #include <stdbool.h>
// #include "oslab.h"

// bool isNullPCB(struct PCB process) {
//     if(
//         process.process_id == 0 &&
//         process.arrival_timestamp == 0 &&
//         process.total_bursttime == 0 &&
//         process.execution_starttime == 0 &&
//         process.execution_endtime == 0 &&
//         process.remaining_bursttime == 0 &&
//         process.process_priority == 0) return true;
//     else return false;
// }

// // code to implement handle_process_arrival_pp
// struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX],
//     int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
// {
//     if(isNullPCB(current_process)) {
//         new_process.execution_starttime = timestamp;
//         new_process.execution_endtime = new_process.total_bursttime + timestamp;
//         new_process.remaining_bursttime = new_process.total_bursttime;
//         return new_process;
//     }
//     if(current_process.process_priority <= new_process.process_priority) {
//         new_process.execution_endtime = 0;
//         new_process.execution_starttime = 0;
//         new_process.remaining_bursttime = new_process.total_bursttime;
//         ready_queue[new_process.process_id] = new_process;
//         return current_process;
//     } else {
//         current_process.execution_endtime = 0;
//         current_process.remaining_bursttime = current_process.remaining_bursttime - (timestamp - current_process.execution_starttime);
//         ready_queue[current_process.process_id] = current_process;
//         new_process.execution_starttime = timestamp;
//         new_process.execution_endtime = new_process.total_bursttime + timestamp;
//         new_process.remaining_bursttime = new_process.total_bursttime;
//         return new_process;
//     }
// };

// int main(int argc, char *argv[]) {
//     printf("Successfully compiled!");
//     return 0;
// }
