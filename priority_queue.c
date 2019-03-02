#include "priority_queue.h"

struct queue_element *head, *tail;

int queue_size = 0;

struct queue_element {
    size_t priority;
    void *body;
    struct queue_element *next;
    struct queue_element *previous;
};

void push(size_t priority, void *body) {
    struct queue_element *new_element = malloc(sizeof(struct queue_element));
    new_element->priority = priority;
    new_element->body = body;
    queue_size++;

    if (!head && !tail) {
        head = malloc(sizeof(struct queue_element));
        tail = malloc(sizeof(struct queue_element));
        head->next = new_element;
        tail->previous = new_element;
        new_element->previous = head;
        new_element->next = tail;
        return;
    }

    struct queue_element *queue_element = head->next;
    while (queue_element != tail && queue_element->priority < new_element->priority) {
        queue_element = queue_element->next;
    }

    new_element->next = queue_element;
    new_element->previous = queue_element->previous;
    queue_element->previous->next = new_element;
    queue_element->previous = new_element;
}

void *pop() {
    if (!queue_size) {
        return 0;
    }
    queue_size--;

    struct queue_element *pop_element = head->next;
    head->next = pop_element->next;
    pop_element->next->previous = head;
    return pop_element->body;
}

int get_queue_size() {
    return queue_size;
}