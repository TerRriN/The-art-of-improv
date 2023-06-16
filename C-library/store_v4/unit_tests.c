#include <CUnit/Basic.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"
#include "store.h"
#include "cart.h"

#define ioopm_add_merch_dup(store, name, desc, price) \
    ioopm_add_merch(store, (elem_t){.str_val = strdup(name)}, (elem_t){.str_val = strdup(desc)}, (elem_t){.int_val = price})

int init_suite(void)
{
    printf("\n");
    return 0;
}
int clean_suite(void) { return 0; }


///////////////////STORE TEST CASES///////////////////
void test_create_destroy_store()
{
    ioopm_store_t *store = ioopm_create_store();
    ioopm_destroy_store(store);
}

void test_crete_store_check_for_null()
{
    ioopm_store_t *store = ioopm_create_store();
    CU_ASSERT_PTR_NOT_NULL_FATAL(store);
    CU_ASSERT_PTR_NOT_NULL_FATAL(store->merchandise_ht);
    CU_ASSERT_PTR_NOT_NULL_FATAL(store->warehouse_ht);
    CU_ASSERT_PTR_NOT_NULL_FATAL(store->cart_ls);
    ioopm_destroy_store(store);
}

void test_add_merch(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_destroy_store(store);
}

void test_add_multiple_merch(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_add_merch_dup(store, "Apple", "Rod", 10);
    ioopm_add_merch_dup(store, "Apelsin", "Orange", 10);
    ioopm_destroy_store(store);
}

void test_add_remove_merch(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_remove_merch(store, (elem_t){.str_val = strdup("Banan")});
    ioopm_destroy_store(store);
}

void test_add_and_remove_multiple_merch(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_add_merch_dup(store, "Apple", "Rod", 10);
    ioopm_add_merch_dup(store, "Apelsin", "Orange", 10);
    ioopm_remove_merch(store, (elem_t){.str_val = strdup("Banan")});
    ioopm_remove_merch(store, (elem_t){.str_val = strdup("Apple")});
    ioopm_remove_merch(store, (elem_t){.str_val = strdup("Apelsin")});
    ioopm_destroy_store(store);
}

void test_replenish_stock(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_destroy_store(store);
}

void test_in_stock_wrong_name(){
    ioopm_store_t *store = ioopm_create_store();
    CU_ASSERT_FALSE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10));
    ioopm_destroy_store(store);
}

void test_in_stock_true_empty(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 0));
    ioopm_destroy_store(store);
}

void test_in_stock_false_empty(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    CU_ASSERT_FALSE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10));
    ioopm_destroy_store(store);
}

void test_ioopm_check_stock(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10));
    CU_ASSERT_FALSE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 11));
    ioopm_destroy_store(store);
}

void test_replenish_wrong_name(){
    ioopm_store_t *store = ioopm_create_store();
    CU_ASSERT_FALSE(ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);
    ioopm_destroy_store(store);
}

void test_replenish_wrong_shelf(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    CU_ASSERT_FALSE(ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("HEJ")}, (elem_t){.int_val = 10}).success);
    ioopm_destroy_store(store);
}

void test_replenish_negative_stock(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    CU_ASSERT_FALSE(ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = -10}).success);
    ioopm_destroy_store(store);
}

void test_replenish_existing_shelf(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    CU_ASSERT_TRUE(ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 20));
    ioopm_destroy_store(store);
}

void test_replenish_used_shelf(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_add_merch_dup(store, "Apple", "Rod", 10);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    CU_ASSERT_FALSE(ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);
    ioopm_destroy_store(store);
}

void test_replenish_multiple_shelfs(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);

    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A02")}, (elem_t){.int_val = 10});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A03")}, (elem_t){.int_val = 10});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A04")}, (elem_t){.int_val = 10});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A05")}, (elem_t){.int_val = 10});

    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 50));
    ioopm_destroy_store(store);
}

void test_replenish_multiple_merch(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 10);
    ioopm_add_merch_dup(store, "Apple", "Rod", 10);
    ioopm_add_merch_dup(store, "Apelsin", "Orange", 10);

    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("A02")}, (elem_t){.int_val = 10});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apelsin")}, (elem_t){.str_val = strdup("A03")}, (elem_t){.int_val = 10});

    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10));
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Apple")}, 10));
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Apelsin")}, 10));
    ioopm_destroy_store(store);
}
//option_t ioopm_edit_merch(ioopm_store_t *store, elem_t merch_name, elem_t path_selector, elem_t new_value);
void test_edit_non_existing_merch(){
    ioopm_store_t *store = ioopm_create_store();
    CU_ASSERT_FALSE(ioopm_edit_merch(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.char_val = 'A'}, (elem_t){.str_val = strdup("Apple")}).success);
    ioopm_destroy_store(store);
}

void test_edit_merch_name(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    CU_ASSERT_TRUE(ioopm_edit_merch(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.char_val = 'A'}, (elem_t){.str_val = strdup("Apple")}).success);
    ioopm_destroy_store(store);
}
/*
void test_edit_removed_merch(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_remove_merch(store, (elem_t){.str_val = strdup("Banan")});
    CU_ASSERT_FALSE(ioopm_edit_merch(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("Rod")}, (elem_t){.int_val = 15}).success);
    ioopm_destroy_store(store);   
}

void test_edit_merch_with_stock(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    CU_ASSERT_TRUE(ioopm_edit_merch(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("Rod")}, (elem_t){.int_val = 15}).success);
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Apple")}, 10));
    ioopm_destroy_store(store);   
}*/

//////////////////////////////////////////////////////
//////////////////CART TEST CASES/////////////////////
void test_create_destroy_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_create_cart(store);
    ioopm_destroy_store(store);
}

void test_create_cart_check_for_null(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_create_cart(store);
    CU_ASSERT_PTR_NOT_NULL_FATAL(get_cart(store, 0)); //0 = first cart
    ioopm_destroy_store(store);
}

void test_get_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_create_cart(store);
    cart_t *cart = get_cart(store, 0);
    CU_ASSERT_PTR_NOT_NULL_FATAL(cart);
    ioopm_destroy_store(store);
}

void test_add_nothing_to_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_create_cart(store);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 0);

    cart_t *cart = get_cart(store, 0);
    cart_item_t *item = get_cart_item(cart, (elem_t){.str_val = strdup("Banan")});
    CU_ASSERT_PTR_NULL_FATAL(item);

    ioopm_destroy_store(store);
}

void test_add_item_to_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_create_cart(store);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 1);

    cart_t *cart = get_cart(store, 0);
    CU_ASSERT_TRUE(ioopm_hash_table_size(cart->items) == 1);

    ioopm_destroy_store(store);
}

void test_get_cart_item(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_create_cart(store);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 10);

    cart_t *cart = get_cart(store, 0);
    cart_item_t *item = get_cart_item(cart, (elem_t){.str_val = strdup("Banan")});

    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    CU_ASSERT_TRUE(item->amount.int_val == 10);
    CU_ASSERT_TRUE(strcmp(item->name.str_val, "Banan") == 0);

    ioopm_destroy_store(store);
}


void test_add_multiple_items_to_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_add_merch_dup(store, "Apple", "Rod", 15);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 20});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("A02")}, (elem_t){.int_val = 15});
    ioopm_create_cart(store);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 10);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Apple")},0, 5);

    cart_t *cart = get_cart(store, 0);
    cart_item_t *item = get_cart_item(cart, (elem_t){.str_val = strdup("Banan")});
    cart_item_t *item2 = get_cart_item(cart, (elem_t){.str_val = strdup("Apple")});
    
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    CU_ASSERT_TRUE(item->amount.int_val == 10);
    CU_ASSERT_TRUE(strcmp(item->name.str_val, "Banan") == 0);

    CU_ASSERT_PTR_NOT_NULL_FATAL(item2);
    CU_ASSERT_TRUE(item2->amount.int_val == 5);
    CU_ASSERT_TRUE(strcmp(item2->name.str_val, "Apple") == 0);

    ioopm_destroy_store(store);
}


void test_remove_item_from_cart(){ // Should return all items to stock
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_create_cart(store);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 1);
    ioopm_remove_item_from_cart(store, 0, (elem_t){.str_val = strdup("Banan")});

    cart_t *cart = get_cart(store, 0);
    cart_item_t *item = get_cart_item(cart, (elem_t){.str_val = strdup("Banan")});
    CU_ASSERT_PTR_NULL_FATAL(item);
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10)); //Might leak memory

    ioopm_destroy_store(store);
}

void test_remove_multiple_items_from_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_add_merch_dup(store, "Apple", "Rod", 15);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 20});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("A02")}, (elem_t){.int_val = 15});
    ioopm_create_cart(store);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 10);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Apple")},0, 5);
    ioopm_remove_item_from_cart(store, 0, (elem_t){.str_val = strdup("Banan")});
    ioopm_remove_item_from_cart(store, 0, (elem_t){.str_val = strdup("Apple")});

    cart_t *cart = get_cart(store, 0);
    cart_item_t *item = get_cart_item(cart, (elem_t){.str_val = strdup("Banan")});
    cart_item_t *item2 = get_cart_item(cart, (elem_t){.str_val = strdup("Apple")});
    CU_ASSERT_PTR_NULL_FATAL(item);
    CU_ASSERT_PTR_NULL_FATAL(item2);
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 20)); //Might leak memory
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Apple")}, 15)); //Might leak memory

    ioopm_destroy_store(store);
}

void test_remove_item_from_empty_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});
    ioopm_create_cart(store);
    ioopm_remove_item_from_cart(store, 0, (elem_t){.str_val = strdup("Banan")});

    cart_t *cart = get_cart(store, 0);
    cart_item_t *item = get_cart_item(cart, (elem_t){.str_val = strdup("Banan")});
    CU_ASSERT_PTR_NULL_FATAL(item);
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10)); //Might leak memory

    ioopm_destroy_store(store);
}

void test_remove_empty_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_create_cart(store);
    ioopm_remove_cart(store, 0);
    CU_ASSERT_PTR_NULL_FATAL(get_cart(store, 0));
    ioopm_destroy_store(store);
}

void test_remove_filled_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_create_cart(store);
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_add_merch_dup(store, "Apple", "Rod", 15);
    
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 20});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("A02")}, (elem_t){.int_val = 15});

    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 10);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Apple")},0, 5);

    ioopm_remove_cart(store, 0);

    CU_ASSERT_PTR_NULL_FATAL(get_cart(store, 0));
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 20)); //Might leak memory
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Apple")}, 15)); //Might leak memory

    ioopm_destroy_store(store);
}

void test_checout_cart(){
    ioopm_store_t *store = ioopm_create_store();
    ioopm_create_cart(store);
    ioopm_add_merch_dup(store, "Banan", "Gul", 20);
    ioopm_add_merch_dup(store, "Apple", "Rod", 15);
    
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Banan")}, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 20});
    ioopm_replenish_stock(store, (elem_t){.str_val = strdup("Apple")}, (elem_t){.str_val = strdup("A02")}, (elem_t){.int_val = 15});
    
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Banan")},0, 10);
    ioopm_add_cart_item(store, (elem_t){.str_val = strdup("Apple")},0, 5);

    ioopm_checkout_cart(store, 0);
    
    CU_ASSERT_PTR_NULL_FATAL(get_cart(store, 0));
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Banan")}, 10)); //Might leak memory
    CU_ASSERT_TRUE(ioopm_in_stock(store, (elem_t){.str_val = strdup("Apple")}, 10)); //Might leak memory
    
    ioopm_destroy_store(store);
}
//////////////////////////////////////////////////////
int main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite store_test_suite = CU_add_suite("Store actions test", init_suite, clean_suite);
    CU_pSuite cart_test_suite = CU_add_suite("Cart actions test", init_suite, clean_suite);
    if ((store_test_suite == NULL) || (cart_test_suite == NULL))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (CU_add_test(store_test_suite, "Create and destroy store", test_create_destroy_store) == NULL) ||
        (CU_add_test(store_test_suite, "Create store and check for null", test_crete_store_check_for_null) == NULL) ||
        (CU_add_test(store_test_suite, "Add merchandise", test_add_merch) == NULL) ||
        (CU_add_test(store_test_suite, "Add multiple merchandise", test_add_multiple_merch) == NULL) ||
        (CU_add_test(store_test_suite, "Add and remove merchandise", test_add_remove_merch) == NULL) ||
        (CU_add_test(store_test_suite, "Add and remove multiple merchandise", test_add_and_remove_multiple_merch) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish stock", test_replenish_stock) == NULL) ||
        (CU_add_test(store_test_suite, "Test in stock wrong name", test_in_stock_wrong_name) == NULL) ||
        (CU_add_test(store_test_suite, "Test in stock empty", test_in_stock_true_empty) == NULL) ||
        (CU_add_test(store_test_suite, "Test in stock wrong ammount", test_in_stock_false_empty) == NULL) ||
        (CU_add_test(store_test_suite, "Test in stock", test_ioopm_check_stock) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish wrong name", test_replenish_wrong_name) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish wrong shelf", test_replenish_wrong_shelf) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish negative stock", test_replenish_negative_stock) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish existing shelf", test_replenish_existing_shelf) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish used shelf", test_replenish_used_shelf) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish multiple shelfs", test_replenish_multiple_shelfs) == NULL) ||
        (CU_add_test(store_test_suite, "Test replenish multiple merch", test_replenish_multiple_merch) == NULL) ||
        (CU_add_test(store_test_suite, "Edit non existing merch", test_edit_non_existing_merch) == NULL) ||
        (CU_add_test(store_test_suite, "Edit merch name", test_edit_merch_name) == NULL) ||

        (CU_add_test(cart_test_suite, "Creat and destroy cart", test_create_destroy_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Creat cart and check for null", test_create_cart_check_for_null) == NULL) ||
        (CU_add_test(cart_test_suite, "Get created cart", test_get_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Add nothing to cart", test_add_nothing_to_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Add item to cart", test_add_item_to_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Get item from cart", test_get_cart_item) == NULL) ||
        (CU_add_test(cart_test_suite, "Add multiple items to cart", test_add_multiple_items_to_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Remove item from cart", test_remove_item_from_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Remove multiple items from cart", test_remove_multiple_items_from_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Remove item from empty cart", test_remove_item_from_empty_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Remove empty cart", test_remove_empty_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Remove filled cart", test_remove_filled_cart) == NULL) ||
        (CU_add_test(cart_test_suite, "Checkout cart", test_checout_cart) == NULL) ||
        0)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}