// Copyright Serban Bianca-Sanziana 2021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t *list;

    list = malloc(sizeof(*list));
    DIE(list == NULL, "Allocation error");

    list->head = NULL;
    list->data_size = data_size;
    list->size = 0;

    return list;
}

static ll_node_t *get_nth_node(linked_list_t *list, unsigned int n)
{
    unsigned int len = list->size - 1;
    unsigned int i;
    ll_node_t *node = list->head;

    // punem in variabila n minimul intre len si n
    n = ((n) < (len) ? (n) : (len));

    for (i = 0; i < n; ++i)
        node = node->next;

    return node;
}

static ll_node_t *create_node(const void *new_data, unsigned int data_size)
{
    ll_node_t *node = calloc(1, sizeof(*node));
    DIE(!node, "calloc node");

    node->data = malloc(data_size);
    DIE(!node->data, "malloc data");

    memcpy(node->data, new_data, data_size);

    return node;
}

// functie care adauga un nod pe pozitia n
void ll_add_nth_node(linked_list_t *list, unsigned int n, const void *new_data)
{
    ll_node_t *new_node, *prev_node;

    if (list == NULL)
        return;

    new_node = create_node(new_data, list->data_size);

    if (!n || !list->size)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        prev_node = get_nth_node(list, n - 1);
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }

    ++list->size;
}

// functie care elimina un nod de pe pozitia n
ll_node_t *
ll_remove_nth_node(linked_list_t *list, unsigned int n)
{
    ll_node_t *prev_node, *removed_node;

    if (!list || !list->size)
        return NULL;

    if (!n)
    {
        removed_node = list->head;
        list->head = removed_node->next;
        removed_node->next = NULL;
    }
    else
    {
        prev_node = get_nth_node(list, n - 1);
        removed_node = prev_node->next;
        prev_node->next = removed_node->next;
        removed_node->next = NULL;
    }

    --list->size;

    return removed_node;
}

// Functia intoarce numarul de noduri din lista
unsigned int
ll_get_size(linked_list_t *list)
{
    if (list == NULL)
    {
        return -1;
    }

    return list->size;
}

// funtie pentru eliberarea memoriei
void ll_free(linked_list_t **pp_list)
{
    linked_list_t *list = *pp_list;
    ll_node_t *temp = NULL, *current = NULL;
    current = list->head;

    // parcugem lista, si punem intr-un nod temporar
    // fiecare nod si dupa eliberam memoria
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    free(list);
    list = NULL;
}

// functie care afiseaza toate valorile int
void ll_print_int(linked_list_t *list)
{
    ll_node_t *curr;

    if (list == NULL)
    {
        return;
    }

    curr = list->head;
    while (curr != NULL)
    {
        printf("%d ", *((int *)curr->data));
        curr = curr->next;
    }

    printf("\n");
}

// functie care afiseaza toate valorile de tip string
void ll_print_string(linked_list_t *list)
{
    ll_node_t *curr;

    if (list == NULL)
    {
        return;
    }
    curr = list->head;
    while (curr != NULL)
    {
        printf("%s ", (char *)curr->data);
        curr = curr->next;
    }

    printf("\n");
}
