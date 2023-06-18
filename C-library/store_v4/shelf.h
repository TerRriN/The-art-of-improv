#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"

typedef struct shelf
{
  elem_t shelf_id;
  elem_t amount;
} shelf_t;

/**
 * @brief Creates a shelf with a given shelf id and stock
 * 
 * @param shelf_table hash table containing all shelves
 * @param shelf_id the shelf id
 * @param amount the stock
 * @return Success if the shelf was created, Failure if the shelf already exists
 */
option_t ioopm_shelf_create(ioopm_hash_table_t *shelf_table, elem_t shelf_id, elem_t amount);

/**
 * @brief Removes a shelf with a given shelf id
 * 
 * @param shelf_table hash table containing all shelves
 * @param shelf_id the shelf id
 * @return Success if the shelf was removed, Failure if the shelf does not exist
 */
option_t ioopm_shelf_destroy(ioopm_hash_table_t *shelf_table, elem_t shelf_id);

/**
 * @brief Adds stock to a shelf with a given shelf id
 * 
 * @param shelf_table hash table containing all shelves
 * @param shelf_id the shelf id
 * @param amount the amount to add
 * @return Success if the stock was added, Failure if the shelf does not exist
 */
option_t ioopm_shelf_resupply(ioopm_hash_table_t *shelf_table, elem_t shelf_id, elem_t amount);

/**
 * @brief Returns the stock of a shelf with a given shelf id
 * 
 * @param shelf_table hash table containing all shelves
 * @param shelf_id the shelf id
 * @return Success if the shelf was found, Failure if the shelf does not exist
*/
option_t ioopm_shelf_stock(ioopm_hash_table_t *shelf_table, elem_t shelf_id);

/**
 * @brief Lists all shelves
 * 
 * @param shelf_table hash table containing all shelves
 * @return void
*/
void ioopm_shelf_list(ioopm_hash_table_t *shelf_table);