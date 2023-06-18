#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"

typedef struct store
{
  ioopm_hash_table_t *merchandise_ht;
  ioopm_hash_table_t *warehouse_ht; // Shelfs
  ioopm_linked_list_t *cart_ls;
} ioopm_store_t;

typedef struct merch
{
  elem_t name;
  elem_t description;
  elem_t price;
  elem_t location;
} merch_t;

typedef struct shelf
{
  elem_t shelf;
  elem_t stock;
} shelf_t;

/**
 * @brief Creates the store, containing three hash tables
 * @param merchandise_ht contains all merchandise created by the admin
 * @param warehouse_ht contains all shelfs where merchandise can be stored
 * @param cart_ht contains all existing carts. Every customer gets a unique cart generated when
 * entering the store
 * @param cart_id keeps track of how many carts that exists and gives every cart a unique id
 * @return ioopm_store_t* the struct containgin all ht
 */
ioopm_store_t *ioopm_create_store();

/**
 * @brief Creates a new merchandise and adds it the store
 *
 * @param store struct containgin all ht
 * @param name of merchandise
 * @param description of merchandise
 * @param price of merchandise
 * @return Success if merch was successfully created and added to merchandise_ht
 * @return Failure if merch already existed in merchandise_ht
 */
option_t ioopm_add_merch(ioopm_store_t *store, elem_t name, elem_t description, elem_t price);

/**
 * @brief Checks if a merchandise is added to a cart
 * 
 * @param store struct containgin all ht
 * @param name of merchandise
 * @return true if merch is in cart
 * @return false if merch is NOT in cart
*/
bool ioopm_check_if_in_cart(ioopm_store_t *store, elem_t name);

/**
 * @brief Removes a merchandise from the store
 *
 * @param store struct containgin all ht
 * @param name of the sought merchandise
 * @return Success if merch and all shelfs containing that merch was removed
 * @return Failure if merch dont exist
 */
option_t ioopm_remove_merch(ioopm_store_t *store, elem_t name);

/**
 * @brief Takes down store, frees all memory
 *
 * @param store struct containgin all ht
 */
void ioopm_destroy_store(ioopm_store_t *store);

/**
 * @brief Edits a value of a specified merchandise
 * 
 * @param store struct containgin all ht
 * @param merch_name the sought merchandise
 * @param path_selector the sought value to be changed
 * @param new_value the new value
 * @return Success if merch was successfully edited, otherwise Failure
*/
option_t ioopm_edit_merch(ioopm_store_t *store, elem_t merch_name, elem_t path_selector, elem_t new_value);

/**
 * @brief Prints a list of all shelfs in the warehouse
 *
 * @param store struct containgin all ht
 */
void ioopm_list_warehouse(ioopm_store_t *store);

/**
 * @brief Prints a list of all merchandise
 *
 * @param store struct containgin all ht
 */
void ioopm_list_merchandise(ioopm_store_t *store);

/**
 * @brief Checks if there is n items for a specified merchandise
 *
 * @param store struct containgin all ht
 * @param merch_name the sought merchandise
 * @return true if stock >= n
 * @return false if stock < n
 */
bool ioopm_in_stock(ioopm_store_t *store, elem_t merch_name, int amount);

/**
 * @brief Creates a new shelf
 *
 * @param shelf_id name of the shelf. Must be uppercase letter followed by two numbers e.g. "A01"
 * @param amount sets the stock for said product
 * @return shelf_t* pointer to the new shelf
 */
shelf_t *ioopm_create_shelf(elem_t shelf_id, elem_t amount);

/**
 * @brief Increase the stock on a specified shelf.
 * If the shelf doesent exist a new will be created.
 *
 * @param store struct containgin all ht
 * @param name the merchandise operated upon
 * @param shelf the sought shelf
 * @param amount the amount added to the stock(positiv/negativ)
 * @return Success if stock was successfully changed otherwise Failure
 */
option_t ioopm_replenish_stock(ioopm_store_t *store, elem_t name, elem_t shelf, elem_t amount);