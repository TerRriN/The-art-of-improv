#include "linked_list.h"
#include "list_common.h"

ioopm_list_node_t *create_node(elem_t value)
{
    ioopm_list_node_t *new_node = calloc(1, sizeof(ioopm_list_node_t));
    new_node->value = value;
    return new_node;
}

ioopm_list_node_t *get_node(ioopm_list_t *list, int index)
{
    assert(list->size >= index && index >= 0);
    ioopm_list_node_t *current = list->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current;
}

ioopm_list_t *ioopm_linked_list_create(cmp_func_t cmp_func)
{
    ioopm_list_t *ls = calloc(1, sizeof(ioopm_list_t));
    ls->cmp_func = cmp_func;
    ls->size = 0;
    return ls;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    while (ioopm_linked_list_size(list) > 0)
    {
        ioopm_linked_list_remove(list, 0);
    }
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    ioopm_list_node_t *new_node = create_node(value);
    if (ioopm_linked_list_is_empty(list))
    {
        list->head = new_node;
    }
    else
    {
        list->tail->next = new_node;
    }

    list->tail = new_node;
    list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    ioopm_list_node_t *new_head = create_node(value);
    if (ioopm_linked_list_is_empty(list))
    {
        list->tail = new_head;
    }
    else
    {
        new_head->next = list->head;
    }
    list->head = new_head;
    list->size++;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{

    if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
        return;
    }
    else if (index == list->size)
    {
        ioopm_linked_list_append(list, value);
        return;
    }

    ioopm_list_node_t *current = get_node(list, index - 1);
    ioopm_list_node_t *next = current->next;
    ioopm_list_node_t *new_node = create_node(value);
    current->next = new_node;
    new_node->next = next;

    list->size++;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    return get_node(list, index)->value;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

void print_list(ioopm_list_t *list)
{
    ioopm_list_node_t *current = list->head;
    puts("\n");
    while (current)
    {
        printf("%d--->", current->value.int_val);
        current = current->next;
    }
    puts("\n");
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    assert(list->size >= index && index >= 0);

    ioopm_list_node_t *remove_node;
    if (list->size == 0)
    {
        remove_node = list->head;
        list->head = NULL;
        list->tail = NULL;
    }
    else if (index == 0)
    {
        remove_node = list->head;
        list->head = list->head->next;
    }
    else if (index == list->size)
    {
        remove_node = list->tail;
        ioopm_list_node_t *prev_node = get_node(list, (index - 1));
        list->tail = prev_node;
    }
    else
    {
        ioopm_list_node_t *prev_node = get_node(list, (index - 1));
        remove_node = prev_node->next;
        prev_node->next = remove_node->next;
    }
    elem_t return_elem = remove_node->value;
    free(remove_node);
    list->size--;
    return return_elem;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, action_func_t func, void *extra)
{
    ioopm_list_node_t *current = list->head;
    while (current)
    {
        func(current->value, extra);
        current = current->next;
    }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_list_predicate pred, void *extra)
{
    ioopm_list_node_t *current = list->head;
    while (current)
    {
        if (pred(list->cmp_func, current->value, extra) == false)
        {
            return false;
        }
        current = current->next;
    }
    return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_list_predicate pred, void *extra)
{
    ioopm_list_node_t *current = list->head;

    while (current)
    {
        if (pred(list->cmp_func, current->value, extra) == true)
            return true;
        current = current->next;
    }
    return false;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{

    return ioopm_linked_list_any(list, (ioopm_list_predicate)list_equality, (void *)&element);
}
