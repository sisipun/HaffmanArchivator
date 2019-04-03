#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

void push(size_t priority, void *body);

void *pop();

int get_queue_size();

#endif