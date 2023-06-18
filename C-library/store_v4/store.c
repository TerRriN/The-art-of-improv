#include "store.h"
#include "utilities/common.h"
#include "cart.h"

ioopm_store_t *ioopm_create_store()
{
    ioopm_store_t *new_store = calloc(1, sizeof(ioopm_store_t));
    ioopm_hash_table_t *merchadise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *warehouse_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_linked_list_t *cart_ls = ioopm_linked_list_create((cmp_func_t)compare_str);
    new_store->merchandise_ht = merchadise_ht;
    new_store->warehouse_ht = warehouse_ht;
    new_store->cart_ls = cart_ls;

    return new_store;
}

/**
 * @brief Help function for ioopm_add_merch(). Allocates space for a new
 * merch and returns the pointer
 */
merch_t *create_merch(elem_t name, elem_t description, elem_t price)
{
    merch_t *new = calloc(1, sizeof(merch_t));
    new->name = name;
    new->description = description;
    new->price = price;
    new->location = (elem_t){.point = ioopm_linked_list_create((cmp_func_t)compare_str)};
    return new;
}

option_t ioopm_add_merch(ioopm_store_t *store, elem_t name, elem_t description, elem_t price)
{
    option_t merch_exist = ioopm_hash_table_lookup(store->merchandise_ht, name);
    if (!merch_exist.success)
    {
        merch_t *new_merch = create_merch(name, description, price);
        elem_t tmp = {.point = new_merch};
        ioopm_hash_table_insert(store->merchandise_ht, new_merch->name, tmp);
        return Success(name);
    }
    return Failure();
}

bool ioopm_check_if_in_cart(ioopm_store_t *store, elem_t name)
{
    for (int i = 0; i < ioopm_linked_list_size(store->cart_ls); i++)
    {
        cart_t *cart = ioopm_linked_list_get(store->cart_ls, i).point;
        option_t o_exist = ioopm_hash_table_lookup(cart->items, name);
        if (o_exist.success)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Help function for ioopm_remove_merch(). Removes all shelfs from warehouse
 * containing said merchandise
 *
 * @param store struct containing all ht
 * @param merch the merchandise that will be deleted
 */
void remove_merch_from_warehouse(ioopm_store_t *store, merch_t *merch)
{
    ioopm_linked_list_t *locations = merch->location.point;
    for (int i = 0; i < ioopm_linked_list_size(merch->location.point); i++)
    {
        // The shelf is removed from warehouse_ht
        // The pointer is returned and freed
        shelf_t *s = ioopm_hash_table_remove(store->warehouse_ht, ioopm_linked_list_get(locations, i)).value.point;
        free(s->shelf.str_val);
        free(s);
    }
    ioopm_linked_list_destroy(locations);
}

option_t ioopm_remove_merch(ioopm_store_t *store, elem_t name)
{
    option_t merch_exist = ioopm_hash_table_lookup(store->merchandise_ht, name);
    if (merch_exist.success)
    {
        merch_t *merch = merch_exist.value.point;
        if (!ioopm_check_if_in_cart(store, name))
        {
            remove_merch_from_warehouse(store, merch);
            ioopm_hash_table_remove(store->merchandise_ht, name);
            free(merch->name.str_val);
            free(merch->description.str_val);
            free(merch);
            free(name.str_val);
            return Success(name);
        }
    }
    return Failure();
}

/**
 * @brief Help function for ioopm_destroy_store(). Frees all merchandise
 * in store
 *
 * @param store struct containing all ht
 */
void destroy_merchandise(ioopm_store_t *store)
{
    // Frees all merchandise
    ioopm_linked_list_t *all_merch = ioopm_hash_table_values(store->merchandise_ht);
    ioopm_hash_table_destroy(store->merchandise_ht);
    for (int i = 0; i < ioopm_linked_list_size(all_merch); i++){
        merch_t *merch = ioopm_linked_list_get(all_merch, i).point;
        free(merch->name.str_val);
        free(merch->description.str_val);
        ioopm_linked_list_destroy(merch->location.point); // Cant free the actula shelfs since they are in warehouse_ht
        free(merch);
    }
    ioopm_linked_list_destroy(all_merch);
}

/**
 * @brief Help function for ioopm_destroy_store(). Frees all shelfs
 * in store
 *
 * @param store struct containing all ht
 */
void destroy_warehouse(ioopm_store_t *store)
{
    ioopm_linked_list_t *all_shelfs = ioopm_hash_table_values(store->warehouse_ht);
    ioopm_hash_table_destroy(store->warehouse_ht);
    for (int i = 0; i < ioopm_linked_list_size(all_shelfs); i++)
    {
        shelf_t *shelf = ioopm_linked_list_get(all_shelfs, i).point;
        free(shelf->shelf.str_val);
        free(shelf);
    }
    ioopm_linked_list_destroy(all_shelfs);
}

/**
 * @brief Help function for ioopm_destroy_store(). Frees all carts
 * in store
 *
 * @param store struct containing all ht
 */
void destroy_carts(ioopm_store_t *store)
{
    for (int i = 0; i < ioopm_linked_list_size(store->cart_ls); i++)
    {
        cart_t *cart = ioopm_linked_list_get(store->cart_ls, i).point;
        ioopm_linked_list_t *all_cart_items = ioopm_hash_table_values(cart->items);
        for (int j = 0; j < ioopm_linked_list_size(all_cart_items); j++)
        {
            cart_item_t *cart_item = ioopm_linked_list_get(all_cart_items, j).point;
            free(cart_item->name.str_val);
            free(cart_item);
        }
        ioopm_hash_table_destroy(cart->items);
        ioopm_linked_list_destroy(all_cart_items);
        free(cart);
    }
    ioopm_linked_list_destroy(store->cart_ls);
}

void ioopm_destroy_store(ioopm_store_t *store)
{
    destroy_merchandise(store);
    destroy_warehouse(store);
    destroy_carts(store);
    free(store);
}

/**
 * @brief Help function for ioopm_edit_merch(). Edits the name of a merchandise
 *
 * @param store struct containing all ht
 * @param merch the merchandise that will be edited
 * @param new_name the new name of the merchandise
 * @return option_t Success if the name was changed, Failure if the name already exists
 */
option_t edit_merch_name(ioopm_store_t *store, merch_t *merch, elem_t new_name){
    if (!ioopm_hash_table_lookup(store->merchandise_ht, new_name).success){ // Check that new_name dont exist
        ioopm_hash_table_remove(store->merchandise_ht, merch->name);
        free(merch->name.str_val);
        merch->name = new_name;
        ioopm_hash_table_insert(store->merchandise_ht, merch->name, (elem_t){.point = merch});
        return Success(new_name);
    }
    free(new_name.str_val);
    return Failure();
}

/**
 * @brief Help function for ioopm_edit_merch(). Edits the description of a merchandise
 *
 * @param store struct containing all ht
 * @param merch the merchandise that will be edited
 * @param new_description the new description of the merchandise
 * @return option_t Success if the description was changed
 */
option_t edit_merch_description(ioopm_store_t *store, merch_t *merch, elem_t new_description)
{
    free(merch->description.str_val);
    merch->description = new_description;
    return Success(new_description);
}

/**
 * @brief Help function for ioopm_edit_merch(). Edits the price of a merchandise
 *
 * @param store struct containing all ht
 * @param merch the merchandise that will be edited
 * @param new_price the new price of the merchandise
 * @return option_t Success if the price was changed
 */
option_t edit_merch_price(ioopm_store_t *store, merch_t *merch, elem_t new_price)
{
    merch->price = new_price;
    return Success(new_price);
}

// Assert that ioopm_check_if_in_cart() has been called before
option_t ioopm_edit_merch(ioopm_store_t *store, elem_t merch_name, elem_t path_selector, elem_t new_value){
    option_t o = ioopm_hash_table_lookup(store->merchandise_ht, merch_name);
    free(merch_name.str_val);
    if (o.success){
        merch_t *merch = o.value.point;
        if (path_selector.char_val == 'A'){
            return edit_merch_name(store, merch, new_value);
        }else if (path_selector.char_val == 'B'){
            return edit_merch_description(store, merch, new_value);
        }else if (path_selector.char_val == 'C'){
            return edit_merch_price(store, merch, new_value);
        }
    }
    free(new_value.str_val);
    return Failure();
}

void ioopm_list_warehouse(ioopm_store_t *store){
    ioopm_linked_list_t *list = ioopm_hash_table_values(store->warehouse_ht);
    for (int i = 0; i < ioopm_linked_list_size(list); i++){
        shelf_t *shelf = ioopm_linked_list_get(list, i).point;
        printf(" %d) Shelf: %s, amount: %d\n", (i + 1), shelf->shelf.str_val, shelf->stock.int_val);
    }
    ioopm_linked_list_destroy(list);
}

void ioopm_list_merchandise(ioopm_store_t *store){
    ioopm_linked_list_t *list = ioopm_hash_table_values(store->merchandise_ht);
    for (int i = 0; i < ioopm_linked_list_size(list); i++){
        merch_t *m = ioopm_linked_list_get(list, i).point;
        printf("  %d) Name: %s, Description: %s, Price: %d\n", (i + 1), m->name.str_val, m->description.str_val, m->price.int_val);
    }
    ioopm_linked_list_destroy(list);
}

int check_amount(ioopm_store_t *store, merch_t *m){
    ioopm_linked_list_t *list_of_shelfs = m->location.point;
    if (ioopm_linked_list_size(list_of_shelfs) == 0){
        return 0;
    }

    int total_stock = 0;
    for (int i = 0; i < ioopm_linked_list_size(list_of_shelfs); i++){
        shelf_t *s = ioopm_hash_table_lookup(store->warehouse_ht, ioopm_linked_list_get(list_of_shelfs, i)).value.point;
        total_stock += s->stock.int_val;
    }
    return total_stock;
}

bool in_stock_helper(ioopm_store_t *store, merch_t *m, int amount){
    int total_stock = check_amount(store, m);
    if (total_stock >= amount){
        return true;
    }else{
        return false;
    }
}

bool ioopm_in_stock(ioopm_store_t *store, elem_t merch_name, int amount){
    option_t o = ioopm_hash_table_lookup(store->merchandise_ht, merch_name);
    if (o.success){
        merch_t *m = o.value.point;
        free(merch_name.str_val);
        return in_stock_helper(store, m, amount);
    }
    free(merch_name.str_val);
    return false;
}

shelf_t *ioopm_create_shelf(elem_t shelf, elem_t amount)
{
    shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
    new_shelf->shelf = shelf;
    new_shelf->stock = amount;
    return new_shelf;
}

bool check_shelf_name(elem_t shelf)
{
    if (strlen(shelf.str_val) == 3)
    {
        if (isalpha(shelf.str_val[0]) && isdigit(shelf.str_val[1]) && isdigit(shelf.str_val[2]))
        {
            return true;
        }
    }
    return false;
}

bool check_shelf(ioopm_store_t *store, ioopm_linked_list_t *ls, elem_t shelf)
{
    if (!check_shelf_name(shelf))
        return false;

    if (!ioopm_hash_table_lookup(store->warehouse_ht, shelf).success)
    {
        return true;
    }
    else
    {
        if (ioopm_linked_list_contains(ls, shelf))
            return true;
    }
    return false;
}

option_t replenish_stock_helper(ioopm_store_t *store, elem_t shelf, elem_t amount, ioopm_linked_list_t *ls)
{
    if (ioopm_linked_list_contains(ls, shelf)) // If shelf exist in list, assert shelf exist in warehouse
    {
        shelf_t *s = ioopm_hash_table_lookup(store->warehouse_ht, shelf).value.point;
        if ((s->stock.int_val + amount.int_val) < 0) // If the difference < 0, set stock to 0
        {
            s->stock.int_val = 0;
        }
        else
        {
            s->stock.int_val += amount.int_val;
        }
        free(shelf.str_val);
        return Success(s->stock);
    }
    else if (!ioopm_hash_table_has_key(store->warehouse_ht, shelf)) // If key dont exist in ht, create new shelf
    {
        if (amount.int_val >= 0)
        { // Prevents creation of shelf with negativ amount
            ioopm_linked_list_append(ls, shelf);
            shelf_t *new_shelf = ioopm_create_shelf(shelf, amount);
            elem_t tmp = {.point = new_shelf};
            ioopm_hash_table_insert(store->warehouse_ht, shelf, tmp);

            return Success(new_shelf->stock);
        }
    }

    return Failure();
}

option_t ioopm_replenish_stock(ioopm_store_t *store, elem_t name, elem_t shelf, elem_t amount)
{
    if (amount.int_val > 0)
    {
        option_t o = ioopm_hash_table_lookup(store->merchandise_ht, name);
        if (o.success)
        {
            merch_t *m = o.value.point;
            ioopm_linked_list_t *ls = m->location.point;

            if (check_shelf(store, ls, shelf))
            {
                free(name.str_val);
                return replenish_stock_helper(store, shelf, amount, ls);
            }
        }
    }
    free(name.str_val);
    free(shelf.str_val);

    return Failure();
}
/*
mayby WRITE FUNCTIONs:

    get merch
        get name
        get description
        get price
    get shelf
        get merch on shelf
        get shelf
        get amount*/