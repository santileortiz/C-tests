//gcc -g -o cyclic_linked_list cyclic_linked_list.c
/*
 * Copiright (C) 2018 Santiago León O.
 */

#include <stdio.h>
#include <stdlib.h>

struct node_t {
    struct node_t *next;
    int val;
    int id;
};

struct list_t {
    int curr_id;
    struct node_t *end;
};

void push_node (struct list_t *list, int val) {
    struct node_t *new_node = malloc (sizeof(struct node_t));
    new_node->val = val;
    new_node->id = list->curr_id;
    list->curr_id++;

    if (list->end != NULL) {
        new_node->next = list->end->next;
        list->end->next = new_node;
    } else {
        new_node->next = new_node;
    }

    list->end = new_node;
}

void print_from_node (struct node_t *node)
{
    struct node_t *curr_node = node;

    while (curr_node->next != node) {
        printf ("(%d, %d) ", curr_node->val, curr_node->id);
        curr_node = curr_node->next;
    }
    printf ("(%d, %d) ", curr_node->val, curr_node->id);
    printf ("\n");
}

void print_list (struct list_t *list)
{
    struct node_t *curr_node = list->end->next;

    do {
        printf ("(%d, %d) ", curr_node->val, curr_node->id);
        curr_node = curr_node->next;
    } while (curr_node != list->end->next);
    printf ("\n");
}

void print_list_1 (struct list_t *list)
{
    struct node_t *curr_node = list->end->next;

    list->end->next = NULL;
    struct node_t *tmp_node = curr_node;
    while (curr_node != NULL) {
        printf ("(%d, %d) ", curr_node->val, curr_node->id);
        curr_node = curr_node->next;
    }
    printf ("\n");
    list->end->next = tmp_node;
}

// This takes the cyclic list and puts all nodes that satisfy CONDITION at the
// beginning of the list.
void put_at_beginning (struct list_t *list)
{
    struct node_t *list_1 = NULL;
    struct node_t *list_2 = NULL;

    struct node_t dummy;
    dummy.next = list->end->next;
    list->end->next = &dummy;

    struct node_t *prev_node = &dummy;
    struct node_t *curr_node = dummy.next;
    while (curr_node != prev_node) {
        struct node_t *curr_node_next = curr_node->next;
        if (curr_node->val == 1) { // <----- CONDITION
            if (list_1 != NULL) {
                curr_node->next = list_1->next;
                list_1->next = curr_node;
            } else {
                curr_node->next = curr_node;
            }
            list_1 = curr_node;

        } else {
            if (list_2 != NULL) {
                curr_node->next = list_2->next;
                list_2->next = curr_node;
            } else {
                curr_node->next = curr_node;
            }
            list_2 = curr_node;
        }

        prev_node->next = curr_node_next;
        curr_node = curr_node_next;
    }

    struct node_t *tmp = list_2->next;
    list_2->next = list_1->next;
    list_1->next = tmp;

    list->end = list_1;
}

int main (void)
{
    struct list_t list = {0};
    push_node (&list, 1);
    push_node (&list, 2);
    push_node (&list, 2);
    push_node (&list, 1);
    push_node (&list, 2);
    push_node (&list, 1);
    push_node (&list, 2);

    print_list (&list);
    put_at_beginning (&list);
    print_list (&list);
}
