#ifndef QUEUE_H
#define QUEUE_H

#define EVENT_DESCRIPTION 30
#define NUM_PARAMS 12

typedef struct event{
    int jobNum;
    char description[EVENT_DESCRIPTION];
    int time;
} event;

typedef struct QNode{
    struct event *event;
    struct QNode *next;
} QNode;

typedef struct queue{
    struct QNode *head;
    struct QNode *tail;
    int size;
} queue;

struct QNode * newNode(event *k);
struct queue * createQueue();
void enqueue (queue *q, event *k);
struct QNode * dequeue (queue *q);
struct event * newEvent (int x, char arr[EVENT_DESCRIPTION], int y);
int size(queue *q);
void enqueuePriority (queue *q, event *k);
void printEvent(event *k, FILE *fout);

#endif
