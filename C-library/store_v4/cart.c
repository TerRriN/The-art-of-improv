#include "cart.h"
#include "utilities/common.h"
#include "store.h"

void ioopm_create_cart(ioopm_store_t *store){
    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->items = ioopm_hash_table_create(hash_function,compare_str,compare_int);
    ioopm_linked_list_append(store->cart_ls, (elem_t){.point = cart});
}
//sadssd
cart_item_t *get_cart_item(cart_t *cart, elem_t merch_name){
    option_t o = ioopm_hash_table_lookup(cart->items, merch_name);
    free(merch_name.str_val);
    if (o.success){
        return o.value.point;
    }
    return NULL;
}

cart_t *get_cart(ioopm_store_t *store, int cart_id){
    if (cart_id < 0 || cart_id >= ioopm_linked_list_size(store->cart_ls)){
        return NULL;
    }
    return ioopm_linked_list_get(store->cart_ls, cart_id).point;
}

bool merch_exist(ioopm_store_t *store, elem_t merch_name){
    return ioopm_hash_table_lookup(store->merchandise_ht, merch_name).success;
}

void update_stock(ioopm_store_t *store, elem_t merch_name, int amount){ //Double checks everything before updating stock
    if(merch_exist(store, merch_name)){
        merch_t *m = ioopm_hash_table_lookup(store->merchandise_ht, merch_name).value.point;
        ioopm_linked_list_t *ls = m->location.point;

        for (int i = 0; i < ioopm_linked_list_size(ls); i++){
            shelf_t *s = ioopm_hash_table_lookup(store->warehouse_ht, ioopm_linked_list_get(ls, i)).value.point;

            if(s->stock.int_val >= amount){
                s->stock.int_val = s->stock.int_val-amount;
                return;
            }else{
                amount = amount - s->stock.int_val;
                s->stock.int_val = 0;
            }
        }
    }
}

cart_item_t *create_cart_item(ioopm_store_t *store, cart_t *cart, elem_t merch_name, int amount){
    cart_item_t *cart_item = calloc(1, sizeof(cart_item_t));
    cart_item->name = merch_name;
    cart_item->amount = int_elem(amount);
    return cart_item;
}

void ioopm_add_cart_item(ioopm_store_t *store, elem_t merch_name, int cart_id, int amount){
    if(ioopm_in_stock(store, (elem_t){.str_val = strdup(merch_name.str_val)}, amount) && amount > 0){ //Check if there is enough stock and if the merch exist
        cart_t *cart = get_cart(store, cart_id); 
        cart_item_t *cart_item = get_cart_item(cart, (elem_t){.str_val = strdup(merch_name.str_val)});

        if(cart_item == NULL){ //Item dont exist in cart
            cart_item = create_cart_item(store, cart, merch_name, amount);
            ioopm_hash_table_insert(cart->items, merch_name, (elem_t){.point = cart_item});
            update_stock(store, merch_name, amount);
            return;
        }else{
            cart_item->amount.int_val += amount;
            update_stock(store, merch_name, amount);
        } 
    }
    free(merch_name.str_val);
}


void ioopm_remove_item_from_cart(ioopm_store_t *store, int cart_id, elem_t merch_name){
    cart_t *cart = get_cart(store, cart_id);
    if(cart == NULL){
        free(merch_name.str_val);
        return;
    }
    cart_item_t *cart_item = get_cart_item(cart, (elem_t){.str_val = strdup(merch_name.str_val)});
    if(cart_item != NULL){
        update_stock(store, merch_name, -(cart_item->amount.int_val)); //OBS this might not work
        ioopm_hash_table_remove(cart->items, merch_name);
        free(cart_item->name.str_val);
        free(cart_item);
    }
    free(merch_name.str_val);
} 

void ioopm_remove_cart(ioopm_store_t *store, int cart_id){
    cart_t *cart = get_cart(store, cart_id);
    ioopm_linked_list_t *all_item_names = ioopm_hash_table_keys(cart->items);

    for(int i = 0; i < ioopm_linked_list_size(all_item_names); i++){
        cart_item_t *cart_item = get_cart_item(cart, (elem_t){.str_val = strdup(ioopm_linked_list_get(all_item_names, i).str_val)});
        if(cart_item != NULL){
            ioopm_remove_item_from_cart(store, cart_id, (elem_t){.str_val = strdup(cart_item->name.str_val)});
        }
    }
    ioopm_linked_list_destroy(all_item_names);
    ioopm_hash_table_destroy(cart->items);
    ioopm_linked_list_remove(store->cart_ls, cart_id);
    free(cart);

}

void ioopm_list_carts(ioopm_store_t *store){ //Prints all carts and their items (might work)
    for(int i = 0; i < ioopm_linked_list_size(store->cart_ls); i++){
        cart_t *cart = ioopm_linked_list_get(store->cart_ls, i).point;
        printf("Cart %d\n", i);
        ioopm_linked_list_t *keys = ioopm_hash_table_keys(cart->items);
        for(int j = 0; j < ioopm_linked_list_size(keys); j++){
            cart_item_t *c_item = ioopm_hash_table_lookup(cart->items, ioopm_linked_list_get(keys, j)).value.point;
            printf("Item: %s, amount: %d\n", c_item->name.str_val, c_item->amount.int_val);
        }
        ioopm_linked_list_destroy(keys);
    }
}

//Prints all items in a cart (might work) (might also be obsolete)
void ioopm_list_cart_items(ioopm_store_t *store, int cart_id){
    cart_t *cart = get_cart(store, cart_id);
    ioopm_linked_list_t *keys = ioopm_hash_table_keys(cart->items);
    for(int i = 0; i < ioopm_linked_list_size(keys); i++){
        cart_item_t *c_item = ioopm_hash_table_lookup(cart->items, ioopm_linked_list_get(keys, i)).value.point;
        printf("Item: %s, amount: %d\n", c_item->name.str_val, c_item->amount.int_val);
    }
    ioopm_linked_list_destroy(keys);
}

int ioopm_calculate_cost(ioopm_store_t *store, int cart_id){
    cart_t *cart = get_cart(store, cart_id);
    ioopm_linked_list_t *all_items = ioopm_hash_table_values(cart->items);
    int total_price = 0;

    for(int i = 0; i < ioopm_linked_list_size(all_items); i++){
        cart_item_t *c_item = ioopm_linked_list_get(all_items, i).point;
        merch_t *merch = ioopm_hash_table_lookup(store->merchandise_ht, c_item->name).value.point;
        total_price += merch->price.int_val * c_item->amount.int_val;
    }
    ioopm_linked_list_destroy(all_items);
    return total_price;
}

void ioopm_checkout_cart(ioopm_store_t *store, int cart_id){
    printf("\n\n");
    ioopm_list_cart_items(store, cart_id);
    printf("Total price: %d\n", ioopm_calculate_cost(store, cart_id));
    ioopm_remove_cart(store, cart_id);
}