struct queue_element {
    char value;
    int priority;
    struct queue_element *next;
    struct queue_element *previous;
};

void push(struct queue_element *new_element);

struct queue_element * pop();