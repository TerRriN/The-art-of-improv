#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int compare_int(elem_t a, elem_t b){
    return a.int_val - b.int_val;
}

int compare_str(elem_t a, elem_t b){
    assert(a.char_val && b.char_val);
    return strcmp(a.char_val, b.char_val);
}

bool list_equality(cmp_func_t cmp_func, elem_t elem, void *extra){
    return cmp_func(elem, *((elem_t*) extra)) == 0;
}

bool table_key_equality(cmp_func_t key_cmp, cmp_func_t val_cmp, elem_t key, elem_t val, void *extra){
  elem_t *cmp_val = (elem_t*) extra;
  return key_cmp(key, *cmp_val) == 0;
}

bool table_val_equality(cmp_func_t key_cmp, cmp_func_t val_cmp, elem_t key, elem_t val, void *extra){
    elem_t *cmp_val = (elem_t*) extra;
    return val_cmp(val, *cmp_val) == 0;
}

void change_str(elem_t key, elem_t *value, void *extra){
    value->char_val = (char*) extra;
}