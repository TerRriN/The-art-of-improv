#include "merch.h"

option_t ioopm_merch_create(ioopm_hash_table_t *merch_table, elem_t name, elem_t description, elem_t price){
    if(!ioopm_hash_table_has_key(merch_table, name)){
        merch_t *new_merch = calloc(1, sizeof(merch_t));
        new_merch->name = name;
        new_merch->description = description;
        new_merch->price = price;
        new_merch->location = (elem_t){.point = ioopm_linked_list_create((cmp_func_t)compare_str)};

        ioopm_hash_table_insert(merch_table, name, (elem_t){.point = new_merch});
        return Success(name);
    }
    free(name.str_val);
    free(description.str_val);
    return Failure();
}

option_t ioopm_merch_destroy(ioopm_hash_table_t *merch_table, ioopm_hash_table_t *shelf_table, elem_t merch_name){
    option_t merch_exist = ioopm_hash_table_lookup(merch_table, merch_name);
    free(merch_name.str_val);
    if (merch_exist.success){
        merch_t *merch = merch_exist.value.point;
        ioopm_hash_table_remove(merch_table, merch->name);

        ioopm_linked_list_t *locations = merch->location.point;
        /*
        Keep out til shelf.c is implemented
        for (int i = 0; i < ioopm_linked_list_size(merch->location.point); i++){
            ioopm_shelf_destroy(shelf_table, ioopm_linked_list_get(locations, i));
        }
        */
        
        ioopm_linked_list_destroy(locations);
        free(merch->name.str_val);
        free(merch->description.str_val);
        free(merch);
        return Success(ptr_elem("Merchandise removed"));
    }
    return Failure();
}

/**
 * @brief Help function for ioopm_edit_merch(). Edits the name of a merchandise
 *
 * @param merch_table the ht containing all merchandise
 * @param merch the merchandise that will be edited
 * @param new_name the new name of the merchandise
 * @return option_t Success if the name was changed, Failure if the name already exists
 */
option_t edit_merch_name(ioopm_hash_table_t *merch_table, merch_t *merch, elem_t new_name){
    if (!ioopm_hash_table_lookup(merch_table, new_name).success){ // Check that new_name dont exist
        ioopm_hash_table_remove(merch_table, merch->name);
        free(merch->name.str_val);
        merch->name = new_name;
        ioopm_hash_table_insert(merch_table, merch->name, (elem_t){.point = merch});
        return Success(new_name);
    }
    free(new_name.str_val);
    return Failure();
}

/**
 * @brief Help function for ioopm_edit_merch(). Edits the description of a merchandise
 *
 * @param merch the merchandise that will be edited
 * @param new_description the new description of the merchandise
 * @return option_t Success if the description was changed
 */
option_t edit_merch_description(merch_t *merch, elem_t new_description)
{
    free(merch->description.str_val);
    merch->description = new_description;
    return Success(new_description);
}

/**
 * @brief Help function for ioopm_edit_merch(). Edits the price of a merchandise
 *
 * @param merch the merchandise that will be edited
 * @param new_price the new price of the merchandise
 * @return option_t Success if the price was changed
 */
option_t edit_merch_price(merch_t *merch, elem_t new_price)
{
    merch->price = new_price;
    return Success(new_price);
}

option_t ioopm_merch_edit(ioopm_hash_table_t *merch_table, elem_t merch_name, elem_t path_selector, elem_t new_value){
    option_t o = ioopm_hash_table_lookup(merch_table, merch_name);
    free(merch_name.str_val);
    if (o.success){
        merch_t *merch = o.value.point;
        if (path_selector.char_val == 'A'){
            return edit_merch_name(merch_table, merch, new_value);
        }else if (path_selector.char_val == 'B'){
            return edit_merch_description(merch, new_value);
        }else if (path_selector.char_val == 'C'){
            return edit_merch_price(merch, new_value);
        }
    }
    free(new_value.str_val);
    return Failure();
}

void ioopm_list_merchandise(ioopm_hash_table_t *merch_table){
    ioopm_linked_list_t *list = ioopm_hash_table_values(merch_table);
    for (int i = 0; i < ioopm_linked_list_size(list); i++){
        merch_t *m = ioopm_linked_list_get(list, i).point;
        printf("  %d) Name: %s, Description: %s, Price: %d\n", (i + 1), m->name.str_val, m->description.str_val, m->price.int_val);
    }
    ioopm_linked_list_destroy(list);
}