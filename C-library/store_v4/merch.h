#pragma once
#include <stdio.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"

typedef struct merch{
  elem_t name;
  elem_t description;
  elem_t price;
  elem_t location;
} merch_t;

/**
 * @brief Creates a new merch and adds it to the merch table
 * 
 * @param merch_table The hash table containing all merch
 * @param name The name of the merch
 * @param description The description of the merch
 * @param price The price of the merch
 * @return Success if merch was successfully created, otherwise Failure
*/
option_t ioopm_merch_create(ioopm_hash_table_t *merch_table, elem_t name, elem_t description, elem_t price);

/**
 * @brief Destroys a merch
 * 
 * @param merch_table The hash table containing all merch
 * @param shelf_table The hash table containing all shelfs
 * @param name The name of the merch to destroy
 * @return Success if merch was successfully destroyed, otherwise Failure
*/
option_t ioopm_merch_destroy(ioopm_hash_table_t *merch_table, ioopm_hash_table_t *shelf_table, elem_t merch_name);


/**
 * @brief Edits a value of a specified merchandise
 * 
 * @param merch_table the hash table containing all merchandise 
 * @param merch_name the sought merchandise
 * @param path_selector the sought value to be changed
 * @param new_value the new value
 * @return Success if merch was successfully edited, otherwise Failure
*/
option_t ioopm_merch_edit(ioopm_hash_table_t *merch_table, elem_t merch_name, elem_t path_selector, elem_t new_value);

/**
 * @brief Prints a list of all merchandise
 *
 * @param merch_table the hash table containing all merchandise
 */
void ioopm_merch_list(ioopm_hash_table_t *merch_table);