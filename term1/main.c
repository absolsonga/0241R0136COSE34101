#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 5

typedef struct {
    int process_id;
    int cpu_burst_time;
    int io_burst_time;
    int arrival_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int remaining_time;
} Process;

void Evaluation();

Process processes[MAX_PROCESSES];
int num_processes = 0; // 프로세스가 생성되었는지 확인하기 위해 초기값을 0으로 설정
int time_quantum;

// 프로세스를 랜덤하게 5개 생성해주는 함수
void Create_Process() {
    num_processes = MAX_PROCESSES;
    printf("Number of processes created: %d\n", num_processes);

    int used_pids[MAX_PROCESSES] = {0};

    for (int i = 0; i < num_processes; i++) {
        int pid;
        do {
            pid = rand() % 2000 + 1;
        } while (used_pids[pid % MAX_PROCESSES] != 0);

        used_pids[pid % MAX_PROCESSES] = 1;
        processes[i].process_id = pid;
        processes[i].cpu_burst_time = rand() % 5 + 1;
        processes[i].io_burst_time = rand() % 5 + 1;
        processes[i].arrival_time = rand() % 10;
        processes[i].priority = rand() % 5 + 1;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].remaining_time = processes[i].cpu_burst_time;
    }
}

// 프로세스 상태를 초기화하는 함수
void Reset_Process_State() {
    for (int i = 0; i < num_processes; i++) {
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].remaining_time = processes[i].cpu_burst_time;
    }
}

// 현재 생성된 프로세스를 프린트 해주는 함수.
void Print_Process() {
    printf("PID\t\tCPU Burst\tI/O Burst\tArrival\t\tPriority\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%-10d\t%-10d\t%-10d\t%-10d\t%-10d\n", processes[i].process_id, processes[i].cpu_burst_time,
               processes[i].io_burst_time, processes[i].arrival_time, processes[i].priority);
    }
}

// 갠트 차트를 프린트 해주는 함수
void PrintGanttChart(int* execution_order, int* execution_times, int num_executions, int initial_time) {
    printf("\nGantt Chart:\n");
    
    // 프로세스 라인
    for (int i = 0; i < num_executions; i++) {
        printf("|  P%-4d ", processes[execution_order[i]].process_id);
    }
    printf("|\n");

    // 구분선
    for (int i = 0; i < num_executions; i++) {
        printf("---------");
    }
    printf("\n");

    // 시간 라인
    int current_time = initial_time;
    printf("%-3d", current_time);
    for (int i = 0; i < num_executions; i++) {
        current_time += execution_times[i];
        printf("      %-3d", current_time);
    }
    printf("\n");
}

// FCFS 스케줄링 알고리즘을 구현한 함수
void FCFS() {
    if (num_processes == 0) {
        printf("No processes created yet. Please create processes first.\n");
        return;
    }

    Reset_Process_State();
    int current_time = 0;
    int completed_processes = 0;

    int execution_order[MAX_PROCESSES];
    int execution_times[MAX_PROCESSES];
    int num_executions = 0;

    while (completed_processes < num_processes) {
        int min_arrival_time = 99999;
        int min_arrival_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                if (processes[i].arrival_time < min_arrival_time) {
                    min_arrival_time = processes[i].arrival_time;
                    min_arrival_process = i;
                }
            }
        }

        if (min_arrival_process == -1) {
            current_time++;
            continue;
        }

        // CPU 버스트 실행
        execution_order[num_executions] = min_arrival_process;
        execution_times[num_executions] = processes[min_arrival_process].cpu_burst_time;
        num_executions++;

        current_time += processes[min_arrival_process].cpu_burst_time;
        processes[min_arrival_process].completion_time = current_time;
        processes[min_arrival_process].turnaround_time = current_time - processes[min_arrival_process].arrival_time;
        processes[min_arrival_process].waiting_time = processes[min_arrival_process].turnaround_time - processes[min_arrival_process].cpu_burst_time;

        completed_processes++;
    }

    PrintGanttChart(execution_order, execution_times, num_executions, processes[execution_order[0]].arrival_time);
    Evaluation();
}

// 비선점형 SJF 스케줄링 알고리즘을 구현한 함수
void NonPreemptiveSJF() {
    if (num_processes == 0) {
        printf("No processes created yet. Please create processes first.\n");
        return;
    }

    Reset_Process_State();
    int current_time = 0;
    int completed_processes = 0;

    int execution_order[MAX_PROCESSES];
    int execution_times[MAX_PROCESSES];
    int num_executions = 0;

    while (completed_processes < num_processes) {
        int min_burst_time = 99999;
        int min_burst_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                if (processes[i].cpu_burst_time < min_burst_time) {
                    min_burst_time = processes[i].cpu_burst_time;
                    min_burst_process = i;
                }
            }
        }

        if (min_burst_process == -1) {
            current_time++;
            continue;
        }

        // CPU 버스트 실행
        execution_order[num_executions] = min_burst_process;
        execution_times[num_executions] = processes[min_burst_process].cpu_burst_time;
        num_executions++;

        current_time += processes[min_burst_process].cpu_burst_time;
        processes[min_burst_process].completion_time = current_time;
        processes[min_burst_process].turnaround_time = current_time - processes[min_burst_process].arrival_time;
        processes[min_burst_process].waiting_time = processes[min_burst_process].turnaround_time - processes[min_burst_process].cpu_burst_time;

        completed_processes++;
    }

    PrintGanttChart(execution_order, execution_times, num_executions, processes[execution_order[0]].arrival_time);
    Evaluation();
}

// 선점형 SJF 스케줄링 알고리즘을 구현한 함수
void PreemptiveSJF() {
    if (num_processes == 0) {
        printf("No processes created yet. Please create processes first.\n");
        return;
    }

    Reset_Process_State();
    int current_time = 0;
    int completed_processes = 0;

    int execution_order[MAX_PROCESSES * 20];
    int execution_times[MAX_PROCESSES * 20];
    int num_executions = 0;

    int last_process = -1;
    int last_execution_time = 0;

    while (completed_processes < num_processes) {
        int min_burst_time = 99999;
        int min_burst_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_burst_time) {
                    min_burst_time = processes[i].remaining_time;
                    min_burst_process = i;
                }
            }
        }

        if (min_burst_process == -1) {
            current_time++;
            continue;
        }

        // 동일한 프로세스가 연속적으로 실행될 경우
        if (last_process == min_burst_process) {
            last_execution_time++;
        } else {
            if (last_process != -1) {
                execution_order[num_executions] = last_process;
                execution_times[num_executions] = last_execution_time;
                num_executions++;
            }
            last_process = min_burst_process;
            last_execution_time = 1;
        }

        processes[min_burst_process].remaining_time -= 1;
        current_time++;

        if (processes[min_burst_process].remaining_time == 0) {
            completed_processes++;
            processes[min_burst_process].completion_time = current_time;
            processes[min_burst_process].turnaround_time = current_time - processes[min_burst_process].arrival_time;
            processes[min_burst_process].waiting_time = processes[min_burst_process].turnaround_time - processes[min_burst_process].cpu_burst_time;
        }
    }

    // 마지막 프로세스 처리
    if (last_process != -1) {
        execution_order[num_executions] = last_process;
        execution_times[num_executions] = last_execution_time;
        num_executions++;
    }

    PrintGanttChart(execution_order, execution_times, num_executions, processes[execution_order[0]].arrival_time);
    Evaluation();
}

// 비선점형 우선순위 스케줄링 알고리즘을 구현한 함수
void NonPreemptivePriority() {
    if (num_processes == 0) {
        printf("No processes created yet. Please create processes first.\n");
        return;
    }

    Reset_Process_State();
    int current_time = 0;
    int completed_processes = 0;

    int execution_order[MAX_PROCESSES];
    int execution_times[MAX_PROCESSES];
    int num_executions = 0;

    while (completed_processes < num_processes) {
        int highest_priority = -1;
        int highest_priority_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0) {
                if (processes[i].priority > highest_priority) {
                    highest_priority = processes[i].priority;
                    highest_priority_process = i;
                }
            }
        }

        if (highest_priority_process == -1) {
            current_time++;
            continue;
        }

        // CPU 버스트 실행
        execution_order[num_executions] = highest_priority_process;
        execution_times[num_executions] = processes[highest_priority_process].cpu_burst_time;
        num_executions++;

        current_time += processes[highest_priority_process].cpu_burst_time;
        processes[highest_priority_process].completion_time = current_time;
        processes[highest_priority_process].turnaround_time = current_time - processes[highest_priority_process].arrival_time;
        processes[highest_priority_process].waiting_time = processes[highest_priority_process].turnaround_time - processes[highest_priority_process].cpu_burst_time;

        completed_processes++;
    }

    PrintGanttChart(execution_order, execution_times, num_executions, processes[execution_order[0]].arrival_time);
    Evaluation();
}

// 선점형 우선순위 스케줄링 알고리즘을 구현한 함수
void PreemptivePriority() {
    if (num_processes == 0) {
        printf("No processes created yet. Please create processes first.\n");
        return;
    }

    Reset_Process_State();
    int current_time = 0;
    int completed_processes = 0;

    int execution_order[MAX_PROCESSES * 20];
    int execution_times[MAX_PROCESSES * 20];
    int num_executions = 0;

    int last_process = -1;
    int last_execution_time = 0;

    while (completed_processes < num_processes) {
        int highest_priority = -1;
        int highest_priority_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].priority > highest_priority) {
                    highest_priority = processes[i].priority;
                    highest_priority_process = i;
                }
            }
        }

        if (highest_priority_process == -1) {
            current_time++;
            continue;
        }

        // 동일한 프로세스가 연속적으로 실행될 경우
        if (last_process == highest_priority_process) {
            last_execution_time++;
        } else {
            if (last_process != -1) {
                execution_order[num_executions] = last_process;
                execution_times[num_executions] = last_execution_time;
                num_executions++;
            }
            last_process = highest_priority_process;
            last_execution_time = 1;
        }

        processes[highest_priority_process].remaining_time -= 1;
        current_time++;

        if (processes[highest_priority_process].remaining_time == 0) {
            completed_processes++;
            processes[highest_priority_process].completion_time = current_time;
            processes[highest_priority_process].turnaround_time = current_time - processes[highest_priority_process].arrival_time;
            processes[highest_priority_process].waiting_time = processes[highest_priority_process].turnaround_time - processes[highest_priority_process].cpu_burst_time;
        }
    }

    // 마지막 프로세스 처리
    if (last_process != -1) {
        execution_order[num_executions] = last_process;
        execution_times[num_executions] = last_execution_time;
        num_executions++;
    }

    PrintGanttChart(execution_order, execution_times, num_executions, processes[execution_order[0]].arrival_time);
    Evaluation();
}

// 라운드 로빈 스케줄링 알고리즘을 구현한 함수
void RR() {
    if (num_processes == 0) {
        printf("No processes created yet. Please create processes first.\n");
        return;
    }

    Reset_Process_State();
    int current_time = 0;
    int completed_processes = 0;
    int num_executions = 0;
    int execution_order[MAX_PROCESSES * 20];
    int execution_times[MAX_PROCESSES * 20];
    int ready_queue[MAX_PROCESSES * 20];
    int front = 0, rear = 0;

    // 첫 번째로 도착하는 프로세스를 찾기 위해 초기 시간을 설정
    int earliest_arrival_time = processes[0].arrival_time;
    for (int i = 1; i < num_processes; i++) {
        if (processes[i].arrival_time < earliest_arrival_time) {
            earliest_arrival_time = processes[i].arrival_time;
        }
    }
    current_time = earliest_arrival_time;

    // 초기 ready_queue를 설정
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time == earliest_arrival_time) {
            ready_queue[rear++] = i;
        }
    }

    while (completed_processes < num_processes) {
        if (front == rear) {
            current_time++;
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].arrival_time == current_time) {
                    ready_queue[rear++] = i;
                }
            }
            continue;
        }

        int i = ready_queue[front++];
        if (processes[i].remaining_time > time_quantum) {
            execution_order[num_executions] = i;
            execution_times[num_executions] = time_quantum;
            num_executions++;

            current_time += time_quantum;
            processes[i].remaining_time -= time_quantum;

            // 현재 시간에 도착한 프로세스를 ready_queue에 추가
            for (int j = 0; j < num_processes; j++) {
                if (processes[j].arrival_time > current_time - time_quantum && processes[j].arrival_time <= current_time && processes[j].remaining_time > 0) {
                    ready_queue[rear++] = j;
                }
            }

            ready_queue[rear++] = i;
        } else {
            execution_order[num_executions] = i;
            execution_times[num_executions] = processes[i].remaining_time;
            num_executions++;

            current_time += processes[i].remaining_time;
            processes[i].remaining_time = 0;
            processes[i].completion_time = current_time;

            processes[i].turnaround_time = current_time - processes[i].arrival_time;
            processes[i].waiting_time = processes[i].turnaround_time - processes[i].cpu_burst_time;

            completed_processes++;

            // 현재 시간에 도착한 프로세스를 ready_queue에 추가
            for (int j = 0; j < num_processes; j++) {
                if (processes[j].arrival_time > current_time - execution_times[num_executions-1] && processes[j].arrival_time <= current_time && processes[j].remaining_time > 0) {
                    ready_queue[rear++] = j;
                }
            }
        }
    }

    PrintGanttChart(execution_order, execution_times, num_executions, earliest_arrival_time);
    Evaluation();
}

// 평균 대기 시간과 평균 반환 시간을 계산하고 출력하는 함수
void Evaluation() {
    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    for (int i = 0; i < num_processes; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    float avg_waiting_time = total_waiting_time / num_processes;
    float avg_turnaround_time = total_turnaround_time / num_processes;

    printf("Average Waiting Time: %.1f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.1f\n", avg_turnaround_time);
}

int main() {
    srand(time(NULL));

    int choice;

    while (1) {
        printf("\n < 2023320128 CPU Scheduling Simulator > \n\n");
        printf("1. Create Processes\n");
        printf("2. Print Processes\n");
        printf("3. FCFS\n");
        printf("4. Non-Preemptive SJF\n");
        printf("5. Preemptive SJF\n");
        printf("6. Non-Preemptive Priority\n");
        printf("7. Preemptive Priority\n");
        printf("8. Round Robin\n");
        printf("9. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Create_Process();
                Print_Process();
                break;
            case 2:
                if (num_processes > 0) {
                    Print_Process();
                } else {
                    printf("No processes created yet.\n");
                }
                break;
            case 3:
                FCFS();
                break;
            case 4:
                NonPreemptiveSJF();
                break;
            case 5:
                PreemptiveSJF();
                break;
            case 6:
                NonPreemptivePriority();
                break;
            case 7:
                PreemptivePriority();
                break;
            case 8:
                printf("Enter time quantum: ");
                scanf("%d", &time_quantum);
                RR();
                break;
            case 9:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar(); getchar();
    }

    return 0;
}
