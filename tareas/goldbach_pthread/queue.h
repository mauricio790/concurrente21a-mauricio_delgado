/*
 * Copyright 2021 Jeisson Hidalgo-Cespedes - Universidad de Costa Rica
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct queue_node {
  int64_t product_number;
  struct queue_node* next;
} queue_node_t;

/**
 * Thread-safe queue
 */
typedef struct queue {
  queue_node_t* head;
  queue_node_t* tail;
  pthread_mutex_t can_access_queue;
} queue_t;

int queue_init(queue_t* queue);
int queue_destroy(queue_t* queue);
bool queue_is_empty(queue_t* queue);
int queue_append(queue_t* queue, int64_t data);
/**
 * @remarks Queue must be not empty, otherwise it will crash
 */
int64_t queue_dequeue(queue_t* queue);
int queue_free(queue_t* queue);

#endif