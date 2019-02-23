#include "priority_queue.h"

struct queue_element *head, *tail;

void push(struct queue_element *new_element) {
    if (!head && !tail) {
        struct queue_element new_head = {0, 0, 0, 0};
        struct queue_element new_tail = {0, 0, 0, 0};
        head = &new_head;
        tail = &new_tail;
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
    queue_element->previous= new_element;
}

struct queue_element * pop() {
    struct queue_element *queue_element = head->next;
    if (queue_element == tail) {
        return 0;
    }

    head->next = queue_element->next;
    queue_element->next->previous = head;
    return queue_element;
}