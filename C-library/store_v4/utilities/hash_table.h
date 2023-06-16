#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"
/**
 * @file hash_table.h
 * @author Oskar Nylin, Johan Andersson Östling
 * @date 10 Oct 2022
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see https://uppsala.instructure.com/courses/68435/assignments/130155
 */
typedef int (*ioopm_hash_function)(elem_t key);
typedef bool (*ioopm_predicate)(cmp_func_t key_cmp, cmp_func_t val_func, elem_t key, elem_t value, void *args);
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

typedef struct entry
{
  elem_t key;         // holds the key
  elem_t value;       // holds the value
  struct entry *next; // points to the next entry (possibly NULL)
} ioopm_entry_t;

typedef struct hash_table
{
  ioopm_entry_t buckets[NO_BUCKETS];
  ioopm_hash_function hash_func;
  cmp_func_t key_cmp_func;
  cmp_func_t val_cmp_func;

} ioopm_hash_table_t;

/// @brief Create a new hash table
/// @return A new empty hash table
/// @param hash_function the function used to get hash values from keys
/// @param key_cmp_func function to compare keys. Can not be NULL
/// @param val_cmp_func function to compare values. Can not be NULL
/// @return Returns an empty hash table with assigned functions (can be NULL)
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, cmp_func_t key_cmp_func, cmp_func_t val_cmp_func);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return Returns struct option_t containing whether lookup was succesfull and the looked up value if succesfull
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return Returns struct option_t containing whether remove was succesfull and the removed value if succesfull
option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true if size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return an array of keys for hash table h
ioopm_linked_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return an array of values for hash table h
ioopm_linked_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_func, void *arg);