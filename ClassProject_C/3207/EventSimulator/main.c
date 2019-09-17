#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h" //Queue header file

//Function prototypes
void simulation(int confVar[NUM_PARAMS], FILE *fout);

void disk1Handler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue, queue *CPU, queue *disk1, int *CPUIdlePtr, int *Disk1IdlePtr, int *maxCPUPtr, int *totalDisk1Ptr,
                  int *Disk1ProcessPtr, int *CPUResponsePtr, int *Disk1ResponsePtr, int *totalCPUResponsePtr, int *totalDisk1ResponsePtr);
void disk2Handler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue, queue *CPU, queue *disk2, int *CPUIdlePtr, int *Disk2IdlePtr, int *maxCPUPtr, int *totalDisk2Ptr,
                  int *Disk2ProcessPtr, int *CPUResponsePtr, int *Disk2ResponsePtr, int *totalCPUResponsePtr, int *totalDisk2ResponsePtr);

void arrivalHandler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue, queue *CPU, int *CPUIdlePtr, int *maxCPUPtr, int *CPUResponsePtr, int *totalCPUResponsePtr);

void CPUHandler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue,queue *CPU,queue *disk1,queue *disk2, int *CPUIdlePtr, int *Disk1IdlePtr, int *Disk2IdlePtr,
                int *maxDisk1Ptr, int *maxDisk2Ptr, int *totalCPUPtr, int *CPUProcessPtr, int *CPUResponsePtr, int *Disk1ResponsePtr, int *Disk2ResponsePtr,
                int *totalCPUResponsePtr, int *totalDisk1ResponsePtr, int *totalDisk2ResponsePtr);


//To make accessing arrays easier
enum {SEED, INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, QUIT_PROB, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX};

int main()
{
    int confVar[NUM_PARAMS] = {};
    const char *conf_types[NUM_PARAMS] = {"SEED", "INIT_TIME", "FIN_TIME", "ARRIVE_MIN" , "ARRIVE_MAX", "QUIT_PROB", "CPU_MIN", "CPU_MAX", "DISK1_MIN", "DISK1_MAX", "DISK2_MIN", "DISK2_MAX"};

    //Opening the input file
    FILE *fin = fopen("config.txt", "r");

    if (fin == NULL)
    {
        printf("Cannot open input file \n");
        exit(0);
    }

    char line[20];
    char search_str[20];
    int i = 0; //get value
    //Scan the appropriate configurations from input file to an array for future use
    while(fgets(line, 20, fin)!= NULL){
            strcpy( search_str, conf_types[i] );
            strcat( search_str, " %d\n" );
            sscanf( line, search_str, &confVar[i] );
            i++;
    }

    i = 0;

    //Creating a pointer to the output file
    FILE *fout = fopen("log.txt", "w");
    if (fout == NULL) {
        printf("Cannot open output file \n");
        exit(0);
    }

    //Write the configurations to the log.txt file
    for(;i<NUM_PARAMS;i++){
        fprintf(fout, "%s %s %d\n", conf_types[i], ": ", confVar[i]);
    }
    simulation(confVar, fout); //The simulation function

    fclose(fin);
    fclose(fout);
}

void simulation(int confVar[NUM_PARAMS], FILE *fout){
    queue *eventQueue = createQueue();
    queue *CPU = createQueue();
    queue *disk1 = createQueue();
    queue *disk2 = createQueue();

    srand((int) confVar[SEED]);

    int currentTime = 0;

    int CPUIsIdle = 1;
    int Disk1IsIdle = 1;
    int Disk2IsIdle = 1;

    int maxCPUSize = 0;
    int maxDisk1Size = 0;
    int maxDisk2Size = 0;

    int totalCPUSize = 0;
    int totalDisk1Size = 0;
    int totalDisk2Size = 0;

    int totalCPUProcesses = 0;
    int totalDisk1Processes = 0;
    int totalDisk2Processes = 0;

    int maxCPUResponse = 0;
    int maxDisk1Response = 0;
    int maxDisk2Response = 0;

    int totalCPUResponse = 0;
    int totalDisk1Response = 0;
    int totalDisk2Response = 0;

    enqueuePriority(eventQueue,newEvent(1,"arrives",confVar[INIT_TIME])); //Enqueue first event to get everything going

    //The main loop
    while((currentTime < confVar[FIN_TIME])&&(size(eventQueue)>0)){
            QNode *toBeProcessed = dequeue(eventQueue);
            currentTime = toBeProcessed->event->time;

            //Write the most recent event to the disk and handle it
            printEvent(toBeProcessed->event, fout);
            //Using strcmp(), compare the description to decide which handler function to use
            if(strcmp(toBeProcessed->event->description,"arrives")==0){
                arrivalHandler(confVar, toBeProcessed, eventQueue, CPU, &CPUIsIdle, &maxCPUSize, &maxCPUResponse, &totalCPUResponse);
            } else if (strcmp(toBeProcessed->event->description,"finishes at CPU")==0){
                CPUHandler(confVar, toBeProcessed, eventQueue, CPU, disk1, disk2, &CPUIsIdle, &Disk1IsIdle, &Disk2IsIdle, &maxDisk1Size, &maxDisk2Size, &totalCPUSize,
                            &totalCPUProcesses, &maxCPUResponse, &maxDisk1Response, &maxDisk2Response, &totalCPUResponse, &totalDisk1Response, &totalDisk2Response);
            } else if (strcmp(toBeProcessed->event->description,"finishes at Disk 1")==0){
                disk1Handler(confVar, toBeProcessed, eventQueue, CPU, disk1, &CPUIsIdle, &Disk1IsIdle, &maxCPUSize, &totalDisk1Size,
                             &totalDisk1Processes, &maxCPUResponse, &maxDisk1Response, &totalCPUResponse, &totalDisk1Response);
            } else if (strcmp(toBeProcessed->event->description,"finishes at Disk 2")==0){
                disk2Handler(confVar, toBeProcessed, eventQueue, CPU, disk2, &CPUIsIdle, &Disk2IsIdle, &maxCPUSize, &totalDisk2Size,
                             &totalDisk2Processes, &maxCPUResponse, &maxDisk2Response, &totalCPUResponse, &totalDisk2Response);
            }
    }
    fprintf(fout, "%-5d %s\n", confVar[FIN_TIME], "Simulation finishes"); //Write the unique event "Simulation finishes" to the log file

    //Print out all statistics
    printf("%s %d\n", "Max CPU Queue Size: ", maxCPUSize);
    printf("%s %f\n", "Utilization of CPU: ", (float) totalCPUResponse/(confVar[FIN_TIME]-confVar[INIT_TIME]));
    if(totalCPUProcesses>0){
    printf("%s %d\n", "Average CPU Queue Size: ", totalCPUSize/totalCPUProcesses);
    printf("%s %d\n", "Average CPU Response Time: ", totalCPUResponse/totalCPUProcesses);
    } else {
    printf("%s %d\n", "Average CPU Queue Size: ", maxCPUSize);
    printf("%s %d\n", "Average CPU Response Time: ", maxCPUResponse);
    }
    printf("%s %d\n", "Max CPU Response Time: ", maxCPUResponse);
    printf("%s %d\n", "Total CPU Processes: ", totalCPUProcesses);
    printf("%s %f\n\n", "Throughput of CPU: ", (float) totalCPUProcesses/(confVar[FIN_TIME]-confVar[INIT_TIME]));


    printf("%s %d\n", "Max Disk 1 Queue Size: ", maxDisk1Size);
    printf("%s %f\n", "Utilization of Disk 1: ", (float) totalDisk1Response/(confVar[FIN_TIME]-confVar[INIT_TIME]));
    if(totalDisk1Processes>0){
    printf("%s %d\n", "Average Disk 1 Queue Size: ", totalDisk1Size/totalDisk1Processes);
    printf("%s %d\n", "Average Disk 1 Response Time: ", totalDisk1Response/totalDisk1Processes);
    } else {
    printf("%s %d\n", "Average Disk 1 Queue Size: ", maxDisk1Size);
    printf("%s %d\n", "Average Disk 1 Response Time: ", maxDisk1Response);
    }
    printf("%s %d\n", "Max Disk 1 Response Time: ", maxDisk1Response);
    printf("%s %d\n", "Total Disk 1 Processes: ", totalDisk1Processes);
    printf("%s %f\n\n", "Throughput of Disk 1: ", (float) totalDisk1Processes/(confVar[FIN_TIME]-confVar[INIT_TIME]));


    printf("%s %d\n", "Max Disk 2 Queue Size: ", maxDisk2Size);
    printf("%s %f\n", "Utilization of Disk 2: ", (float) totalDisk2Response/(confVar[FIN_TIME]-confVar[INIT_TIME]));
    if(totalDisk2Processes>0){
    printf("%s %d\n", "Average Disk 2 Queue Size: ", totalDisk2Size/totalDisk2Processes);
    printf("%s %d\n", "Average Disk 2 Response Time: ", totalDisk2Response/totalDisk2Processes);
    } else {
    printf("%s %d\n", "Average Disk 2 Queue Size: ", maxDisk2Size);
    printf("%s %d\n", "Average Disk 2 Response Time: ", maxDisk2Response);
    }
    printf("%s %d\n", "Max Disk 2 Response Time: ", maxDisk2Response);
    printf("%s %d\n", "Total Disk 2 Processes: ", totalDisk2Processes);
    printf("%s %f\n\n", "Throughput of Disk 2: ", (float) totalDisk2Processes/(confVar[FIN_TIME]-confVar[INIT_TIME]));
}

void arrivalHandler(int confVar[NUM_PARAMS],QNode *toBeProcessed, queue *eventQueue, queue *CPU, int *CPUIdlePtr, int *maxCPUPtr, int *CPUResponsePtr, int *totalCPUResponsePtr){
    //Create a new arrival event
    int currentTime = toBeProcessed->event->time;
    int randomTimeArrival = (rand()%(confVar[ARRIVE_MAX]-confVar[ARRIVE_MIN])+confVar[ARRIVE_MIN]);

    if(currentTime+randomTimeArrival<confVar[FIN_TIME]){
        event *jobArrives = newEvent(toBeProcessed->event->jobNum+1, "arrives", currentTime+randomTimeArrival);
        enqueuePriority(eventQueue, jobArrives);
    }

    //Create a CPU event
    int randomTimeCPU = rand()%(confVar[CPU_MAX]-confVar[CPU_MIN])+confVar[CPU_MIN];

    if(currentTime+randomTimeCPU<confVar[FIN_TIME]){
        event *finishedAtCPU = newEvent(toBeProcessed->event->jobNum, "finishes at CPU", currentTime+randomTimeCPU);
        if(*CPUIdlePtr){ //If CPU is idle, begin the job right away by enqueueing it to the event queue
            enqueuePriority(eventQueue, finishedAtCPU);
            *CPUIdlePtr=0;
            if (*CPUResponsePtr < randomTimeCPU){
                *CPUResponsePtr = randomTimeCPU;
            }
            *totalCPUResponsePtr+=randomTimeCPU;
        } else { //If CPU is busy, enqueue event to CPU queue, and see if the new CPU size is the maximum size
            enqueue(CPU, finishedAtCPU);
            if(*maxCPUPtr<size(CPU)){
                *maxCPUPtr=size(CPU);
            }
        }
    }
}

void CPUHandler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue,queue *CPU,queue *disk1,queue *disk2, int *CPUIdlePtr, int *Disk1IdlePtr, int *Disk2IdlePtr, int *maxDisk1Ptr, int *maxDisk2Ptr, int *totalCPUPtr,
                int *CPUProcessPtr, int *CPUResponsePtr, int *Disk1ResponsePtr, int *Disk2ResponsePtr, int *totalCPUResponsePtr, int *totalDisk1ResponsePtr, int *totalDisk2ResponsePtr){
    int jobNum = toBeProcessed->event->jobNum;
    int time = toBeProcessed->event->time;

    *CPUProcessPtr+=1;
    *totalCPUPtr+=size(CPU);
    //Either the event finishes or is put to a disk
    if(rand()%100+1<confVar[QUIT_PROB]){
        enqueuePriority(eventQueue,newEvent(jobNum, "finishes", time));
    } else {
    if((disk1->size<disk2->size)||((disk1->size==disk2->size)&&(rand()%100+1>50))){
        int randomTimeDisk = rand()%(confVar[DISK1_MAX]-confVar[DISK1_MIN])+confVar[DISK1_MIN];

        if(time+randomTimeDisk<confVar[FIN_TIME]){
            event *finishedAtDisk1 = newEvent(jobNum,"finishes at Disk 1", time+randomTimeDisk);
            if(*Disk1IdlePtr){
                enqueuePriority(eventQueue,finishedAtDisk1);
                *Disk1IdlePtr = 0;
                if (*Disk1ResponsePtr < randomTimeDisk){
                    *Disk1ResponsePtr = randomTimeDisk;
                }
                *totalDisk1ResponsePtr+=randomTimeDisk;
            } else {
                enqueue(disk1,finishedAtDisk1);
                if(*maxDisk1Ptr<size(disk1)){
                    *maxDisk1Ptr=size(disk1);
                }
            }
        }
    } else {
        int randomTimeDisk = rand()%(confVar[DISK2_MAX]-confVar[DISK2_MIN])+confVar[DISK2_MIN];

        if(time+randomTimeDisk<confVar[FIN_TIME]){
            event *finishedAtDisk2 = newEvent(jobNum,"finishes at Disk 2", time+randomTimeDisk);
            if(*Disk2IdlePtr){
                enqueuePriority(eventQueue,finishedAtDisk2);
                *Disk2IdlePtr = 0;
                if (*Disk2ResponsePtr < randomTimeDisk){
                    *Disk2ResponsePtr = randomTimeDisk;
                }
                *totalDisk2ResponsePtr+=randomTimeDisk;
            } else {
                enqueue(disk2,finishedAtDisk2);
                if(*maxDisk2Ptr<size(disk2)){
                    *maxDisk2Ptr=size(disk2);
                }
            }
        }
    }
    }

    if(CPU->size==0){ //if CPU queue is empty after processing the event, switch the variable to 1 to indicate idleness.
        *CPUIdlePtr=1;
    } else { //If there is still work to do, dequeue the next event and process it
        event *next = (dequeue(CPU))->event;
        int timeNext = toBeProcessed->event->time;
        int jobNext = next->jobNum;
        int randomTimeCPU = rand()%(confVar[CPU_MAX]-confVar[CPU_MIN])+confVar[CPU_MIN];
        if(timeNext+randomTimeCPU<confVar[FIN_TIME]){
        event *finishedAtCPU = newEvent(jobNext, "finishes at CPU", timeNext+randomTimeCPU);
        enqueuePriority(eventQueue, finishedAtCPU);
        if (*CPUResponsePtr < randomTimeCPU){
            *CPUResponsePtr = randomTimeCPU;
        }
        *totalCPUResponsePtr+=randomTimeCPU;
        }
    }
}

void disk1Handler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue, queue *CPU, queue *disk1, int *CPUIdlePtr, int *Disk1IdlePtr, int *maxCPUPtr, int *totalDisk1Ptr,
                  int *Disk1ProcessPtr, int *CPUResponsePtr, int *Disk1ResponsePtr, int *totalCPUResponsePtr, int *totalDisk1ResponsePtr){
    int jobNum = toBeProcessed->event->jobNum;
    int time = toBeProcessed->event->time;
    int randomTimeCPU = rand()%(confVar[CPU_MAX]-confVar[CPU_MIN])+confVar[CPU_MIN];

    //put the job back to the CPU
    if(time+randomTimeCPU<confVar[FIN_TIME]){
        event *finishedAtCPU = newEvent(jobNum, "finishes at CPU", time+randomTimeCPU);
        if(*CPUIdlePtr){
            enqueuePriority(eventQueue, finishedAtCPU);
            *CPUIdlePtr = 0;
            if (*CPUResponsePtr < randomTimeCPU){
                *CPUResponsePtr = randomTimeCPU;
            }
            *totalCPUResponsePtr+=randomTimeCPU;
        } else {
            enqueue(CPU, finishedAtCPU);
            if(*maxCPUPtr<size(CPU)){
                *maxCPUPtr=size(CPU);
            }
        }
    }

    *Disk1ProcessPtr+=1;
    *totalDisk1Ptr+=size(disk1);

    //If Disk is idle, switch the variable to 1(idle), otherwise dequeue and process the next event by enqueueing it to the event queue
    if(size(disk1)==0){
        *Disk1IdlePtr = 1;
    } else {
        event *next = (dequeue(disk1))->event;
        int timeNext = toBeProcessed->event->time;
        int jobNext = next->jobNum;
        int randomTimeDisk = rand()%(confVar[DISK1_MAX]-confVar[DISK1_MIN])+confVar[DISK1_MIN];
        if(timeNext+randomTimeDisk<confVar[FIN_TIME]){
            event *finishedAtDisk = newEvent(jobNext, "finishes at Disk 1", timeNext+randomTimeDisk);
            enqueuePriority(eventQueue, finishedAtDisk);
            if (*Disk1ResponsePtr < randomTimeDisk){
                *Disk1ResponsePtr = randomTimeDisk;
            }
            *totalDisk1ResponsePtr+=randomTimeDisk;
        }
    }
}

//Similar to Disk 1 function
void disk2Handler(int confVar[NUM_PARAMS], QNode *toBeProcessed, queue *eventQueue, queue *CPU, queue *disk2, int *CPUIdlePtr, int *Disk2IdlePtr, int *maxCPUPtr, int *totalDisk2Ptr,
                  int *Disk2ProcessPtr, int *CPUResponsePtr, int *Disk2ResponsePtr, int *totalCPUResponsePtr, int *totalDisk2ResponsePtr){
    int jobNum = toBeProcessed->event->jobNum;
    int time = toBeProcessed->event->time;
    int randomTimeCPU = rand()%(confVar[CPU_MAX]-confVar[CPU_MIN])+confVar[CPU_MIN];
    if(time+randomTimeCPU<confVar[FIN_TIME]){
        event *finishedAtCPU = newEvent(jobNum, "finishes at CPU", time+randomTimeCPU);
        if(*CPUIdlePtr){
            enqueuePriority(eventQueue, finishedAtCPU);
            *CPUIdlePtr = 0;
        } else {
            enqueue(CPU, finishedAtCPU);
            if(*maxCPUPtr<size(CPU)){
                *maxCPUPtr=size(CPU);
            }
        }
    }

    *Disk2ProcessPtr+=1;
    *totalDisk2Ptr+=size(disk2);

    if(size(disk2)==0){
        *Disk2IdlePtr = 1;
    } else {
        event *next = (dequeue(disk2))->event;
        int timeNext = toBeProcessed->event->time;
        int jobNext = next->jobNum;
        int randomTimeDisk = rand()%(confVar[DISK2_MAX]-confVar[DISK2_MIN])+confVar[DISK2_MIN];
        if(timeNext+randomTimeDisk<confVar[FIN_TIME]){
            event *finishedAtDisk = newEvent(jobNext, "finishes at Disk 2", timeNext+randomTimeDisk);
            enqueuePriority(eventQueue, finishedAtDisk);
            if (*Disk2ResponsePtr < randomTimeDisk){
                *Disk2ResponsePtr = randomTimeDisk;
            }
            *totalDisk2ResponsePtr+=randomTimeDisk;
        }
    }
}



