struct list{
    size_t size;
    cmp_func_t cmp_func;
    ioopm_list_node_t *head;
    ioopm_list_node_t *tail;
};

struct list_node{
    elem_t value;
    ioopm_list_node_t *next;
};