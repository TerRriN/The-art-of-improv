#include "shelf.h"

bool check_shelf_id(elem_t shelf_id){
    if(strlen(shelf_id.str_val) == 3){
        if(isalpha(shelf_id.str_val[0]) && isdigit(shelf_id.str_val[1]) && isdigit(shelf_id.str_val[2])){
            return true;
        }
    }
    return false;
}

bool shelf_is_pure(ioopm_hash_table_t *shelf_table, elem_t shelf_id, elem_t amount){
    if(amount.int_val <= 0) return false; // Unallowed amount 
    if(!check_shelf_id(shelf_id)) return false; // Wrong name format
    if(ioopm_hash_table_lookup(shelf_table, shelf_id).success) return false; // Shelf already exist
    return true;
}
    
//When listing A merch, list all shelfs and their amounts
option_t ioopm_shelf_create(ioopm_hash_table_t *shelf_table, elem_t shelf_id, elem_t amount){
    if(!shelf_is_pure(shelf_table, shelf_id, amount)){
        free(shelf_id.str_val);
        return Failure();
    }

    shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
    new_shelf->shelf_id = shelf_id;
    new_shelf->amount = amount;
    ioopm_hash_table_insert(shelf_table, shelf_id, (elem_t){.point = new_shelf});
    return Success(shelf_id);
}

option_t ioopm_shelf_destroy(ioopm_hash_table_t *shelf_table, elem_t shelf_id){
    option_t shelf_exist = ioopm_hash_table_lookup(shelf_table, shelf_id);
    free(shelf_id.str_val);
    if (shelf_exist.success){
        shelf_t *shelf = shelf_exist.value.point;
        ioopm_hash_table_remove(shelf_table, shelf->shelf_id);
        free(shelf->shelf_id.str_val);
        free(shelf);
        return Success(ptr_elem("Shelf removed"));
    }
    return Failure();
}

option_t ioopm_shelf_resupply(ioopm_hash_table_t *shelf_table, elem_t shelf_id, elem_t amount){
    if(amount.int_val <= 0){
        free(shelf_id.str_val);
        return Failure();
    }
    option_t shelf_exist = ioopm_hash_table_lookup(shelf_table, shelf_id);
    free(shelf_id.str_val);
    if (shelf_exist.success){
        shelf_t *shelf = shelf_exist.value.point;
        shelf->amount.int_val += amount.int_val;
        return Success(ptr_elem("Shelf resupplied"));
    }
    return Failure();
}

option_t ioopm_shelf_stock(ioopm_hash_table_t *shelf_table, elem_t shelf_id){
    option_t shelf_exist = ioopm_hash_table_lookup(shelf_table, shelf_id);
    free(shelf_id.str_val);
    if (shelf_exist.success){
        shelf_t *shelf = shelf_exist.value.point;
        return Success(shelf->amount);
    }
    return Failure();
}

void ioopm_list_warehouse(ioopm_hash_table_t *shelf_table){
    ioopm_linked_list_t *list = ioopm_hash_table_values(shelf_table);
    for(int i = 0; i < ioopm_linked_list_size(list); i++){
        shelf_t *shelf = ioopm_linked_list_get(list, i).point;
        printf("Shelf: %s, Amount: %d\n", shelf->shelf_id.str_val, shelf->amount.int_val);
    }
}