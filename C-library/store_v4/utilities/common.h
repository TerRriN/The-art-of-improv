#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#define NO_BUCKETS 17
#define Success(v) (option_t){.success = true, .value = v};
#define Failure() (option_t){.success = false};
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)

#define int_elem(x) \
  (elem_t)          \
  {                 \
    .int_val = (x)  \
  }
#define ptr_elem(x) \
  (elem_t)          \
  {                 \
    .str_val = (x) \
  }

#define ioopm_int_str_ht_insert(ht, i, s) \
  ioopm_hash_table_insert(ht, int_elem(i), ptr_elem(s))

#define ioopm_int_str_ht_lookup(ht, i) \
  ioopm_hash_table_lookup(ht, int_elem(i))

#define ioopm_int_ht_has_key(ht, i) \
  ioopm_hash_table_has_key(ht, int_elem(i))

#define ioopm_int_ht_has_value(ht, s) \
  ioopm_hash_table_has_value(ht, ptr_elem(s))

#define ioopm_int_ht_remove(ht, i) \
  ioopm_hash_table_remove(ht, int_elem(i))

// typedef struct { union AU { int a; char b; } U1; } A;
typedef union elem
{
  int int_val;
  char *str_val;
  char char_val;
  bool bool_val;
  void *point;
} elem_t;

typedef struct option
{
  bool success;
  elem_t value;
} option_t;

/// @brief Compares the integer values of two elem_t
int compare_int(elem_t a, elem_t b);
/// @brief Compares the string values of two elem_t
int compare_str(elem_t a, elem_t b);
typedef int (*cmp_func_t)(elem_t a, elem_t b);

bool list_equality(cmp_func_t cmp_func, elem_t elem, void *extra);
bool table_key_equality(cmp_func_t key_cmp, cmp_func_t val_cmp, elem_t key, elem_t val, void *extra);
bool table_val_equality(cmp_func_t key_cmp, cmp_func_t val_cmp, elem_t value, elem_t val, void *extra);
void change_str(elem_t key, elem_t *value, void *extra);
int hash_function(elem_t name);