#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"
#include "store.h"

typedef struct cart {
  ioopm_hash_table_t *items;
} cart_t;

typedef struct cart_item {
  elem_t name;
  elem_t amount;
} cart_item_t;

/**
 * @brief Creates a new cart and adds it to the store
 *
 * @param store struct containgin all ht
 */
void ioopm_create_cart(ioopm_store_t *store);

/**
  * @brief Adds a number of items to the cart
  *
  * @param store struct containing all ht
  * @param merch_name the sought merchandise
  * @param cart_id the sought cart
  * @param amount the amount of merch to be added
 */
void ioopm_add_cart_item(ioopm_store_t *store, elem_t merch_name, int cart_id, int amount);

/**
 * @brief Removes a item from the cart and returns all its reserved stock to warehouse
 *
 * @param store struct containing all ht
 * @param current_cart the cart operated upon
 * @param current_item the item that will be removed
 */
void ioopm_remove_item_from_cart(ioopm_store_t *store, int cart_id, elem_t merch_name);

/**
 * @brief Removes the cart and all its content
 *
 * @param store struct containing all ht
 * @param cart_id the sought cart
 */
void ioopm_remove_cart(ioopm_store_t *store, int cart_id);

/**
 * @brief Prints a list of all carts
 *
 * @param store struct containing all ht
 */
void ioopm_list_carts(ioopm_store_t *store);

/**
 * @brief Prints a list of all items in a cart
 */
void ioopm_list_cart_items(ioopm_store_t *store, int cart_id);

/**
 * @brief Checksout the cart and prints a list of its content and total cost
 *
 * @param store struct containing all ht
 * @param cart_id the sought cart
 */
void ioopm_checkout_cart(ioopm_store_t *store, int cart_id);

/**
 * @brief Get the total cost of all items in cart
 *
 * @param store struct containing all ht
 * @param cart_id the sought cart
 * @return cost as int
 */
int ioopm_calculate_cost(ioopm_store_t *store, int cart_id);

/**
 * @brief Get the cart with the given id
 * 
 * @param store struct containing all ht
 * @param cart_id the sought cart
 * @return cart_t
 * @return NULL if cart not found
*/
cart_t *get_cart(ioopm_store_t *store, int cart_id);

/**
 * @brief Get the cart item with the given name
 * 
 * @param cart the cart to search in
 * @param merch_name the sought merchandise
 * @return cart_item_t
 * @return NULL if cart item not found
*/
cart_item_t *get_cart_item(cart_t *cart, elem_t merch_name);