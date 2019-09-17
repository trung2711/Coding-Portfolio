#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void printEvent(event *k, FILE *fout){
    fprintf(fout,"%-5d %s %-4d %s\n", k->time, "Job", k->jobNum, k->description);
}

struct event * newEvent (int x, char arr[EVENT_DESCRIPTION], int y){
    event *temp = (event*)malloc(sizeof(event));
    temp->jobNum = x;
    strcpy(temp->description, arr);
    temp->time = y;
    return temp;
}

struct QNode * newNode(event *k){
    QNode *temp = (QNode*)malloc(sizeof(QNode));
    temp->event = k;
    temp->next = NULL;
    return temp;
}

struct queue * createQueue()
{
    queue *q = (queue*)malloc(sizeof(queue));
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

void enqueuePriority (queue *q, event *k){
    QNode *temp = newNode(k);
    if(q->tail==NULL){
        q->head=q->tail=temp;
    } else {
        if(q->head->event->time> temp->event->time){
        temp->next=q->head;
        q->head = temp;
        } else {
            QNode *prev = q->head;
            QNode *current = prev->next;
            while(prev!=NULL&&current!=NULL){
                if(temp->event->time<current->event->time){
                    prev->next=temp;
                    temp->next=current;
                    q->size++;
                    return;
                } else {
                    prev = prev->next;
                    current = prev->next;
                }
            }
            q->tail->next=temp;
            q->tail=temp;
        }
    }
    q->size++;
}

int size(queue *q){
    return q->size;
}

void enqueue (queue *q, event *k){
    struct QNode *temp = newNode(k);
    if(q->tail==NULL){
        q->head=q->tail=temp;
    } else {
        q->tail->next=temp;
        q->tail=temp;
    }
    q->size++;
}

struct QNode * dequeue (queue *q) {
    if(q->head==NULL){
        return NULL;
    }
    QNode *temp = q->head;
    q->head = q->head->next;

    if (q->head == NULL){
       q->tail = NULL;
    }
    q->size--;
    return temp;
}
