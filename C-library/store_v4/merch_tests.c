#include <CUnit/Basic.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"
#include "merch.h"
//CUnit cant print number of passed suites
int init_suite(void){return 0;}
int clean_suite(void){return 0;}

void test_create_destroy_merch(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    
    CU_ASSERT_TRUE_FATAL(ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000}).success);

    elem_t tmp = {.str_val = strdup("Car")};
    merch_t *m = ioopm_hash_table_lookup(merchandise_ht, tmp).value.point;
    CU_ASSERT_TRUE_FATAL(strcmp(m->name.str_val, "Car") == 0);
    CU_ASSERT_TRUE_FATAL(strcmp(m->description.str_val, "Fast") == 0);
    CU_ASSERT_TRUE_FATAL(m->price.int_val == 1000);    
    
    CU_ASSERT_TRUE_FATAL(ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")}).success);

    free(tmp.str_val);
    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_create_merch_twice(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    
    CU_ASSERT_TRUE_FATAL(ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000}).success);
    CU_ASSERT_FALSE_FATAL(ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000}).success);

    ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")});
    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_create_destroy_multiple_merch(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    
    CU_ASSERT_TRUE_FATAL(ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000}).success);
    CU_ASSERT_TRUE_FATAL(ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Bike")}, (elem_t){.str_val = strdup("Yellow")}, (elem_t){.int_val = 500}).success);
    CU_ASSERT_TRUE_FATAL(ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Boat")}, (elem_t){.str_val = strdup("Big")}, (elem_t){.int_val = 10000}).success);

    CU_ASSERT_TRUE_FATAL(ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")}).success);
    CU_ASSERT_TRUE_FATAL(ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Bike")}).success);
    CU_ASSERT_TRUE_FATAL(ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Boat")}).success);

    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_destroy_non_existing_merch(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    
    CU_ASSERT_FALSE(ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")}).success);

    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_destroy_merch_with_shelfs(){
    return;
}

void test_edit_non_existing_merch(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    
    CU_ASSERT_FALSE(ioopm_merch_edit(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.char_val = 'A'}, (elem_t){.str_val = strdup("Boat")}).success);

    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_edit_merch_name(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000});

    CU_ASSERT_TRUE(ioopm_merch_edit(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.char_val = 'A'}, (elem_t){.str_val = strdup("Boat")}).success);
    elem_t tmp = {.str_val = strdup("Boat")};
    merch_t *m = ioopm_hash_table_lookup(merchandise_ht, tmp).value.point;
    CU_ASSERT_TRUE_FATAL(strcmp(m->name.str_val, "Boat") == 0);
    CU_ASSERT_TRUE_FATAL(strcmp(m->description.str_val, "Fast") == 0);
    CU_ASSERT_TRUE_FATAL(m->price.int_val == 1000);

    free(tmp.str_val);
    ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Boat")});
    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_edit_merch_description(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000});

    CU_ASSERT_TRUE(ioopm_merch_edit(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.char_val = 'B'}, (elem_t){.str_val = strdup("Big")}).success);
    
    elem_t tmp = {.str_val = strdup("Car")};
    merch_t *m = ioopm_hash_table_lookup(merchandise_ht, tmp).value.point;
    CU_ASSERT_TRUE_FATAL(strcmp(m->name.str_val, "Car") == 0);
    CU_ASSERT_TRUE_FATAL(strcmp(m->description.str_val, "Big") == 0);
    CU_ASSERT_TRUE_FATAL(m->price.int_val == 1000);

    free(tmp.str_val);
    ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")});
    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_edit_merch_price(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000});

    CU_ASSERT_TRUE(ioopm_merch_edit(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.char_val = 'C'}, (elem_t){.int_val = 2000}).success);
    
    elem_t tmp = {.str_val = strdup("Car")};
    merch_t *m = ioopm_hash_table_lookup(merchandise_ht, tmp).value.point;
    CU_ASSERT_TRUE_FATAL(strcmp(m->name.str_val, "Car") == 0);
    CU_ASSERT_TRUE_FATAL(strcmp(m->description.str_val, "Fast") == 0);
    CU_ASSERT_TRUE_FATAL(m->price.int_val == 2000);

    free(tmp.str_val);
    ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")});
    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_edit_destroyed_merch(){
    ioopm_hash_table_t *merchandise_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_merch_create(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.str_val = strdup("Fast")}, (elem_t){.int_val = 1000});
    ioopm_merch_destroy(merchandise_ht, shelf_ht, (elem_t){.str_val = strdup("Car")});

    CU_ASSERT_FALSE(ioopm_merch_edit(merchandise_ht, (elem_t){.str_val = strdup("Car")}, (elem_t){.char_val = 'A'}, (elem_t){.str_val = strdup("Boat")}).success);

    ioopm_hash_table_destroy(merchandise_ht);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_edit_merch_with_stock(){
    return;
}


int main()
{
    if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();

    CU_pSuite merch_test_suite = CU_add_suite("Merch actions test", init_suite, clean_suite);
    if (merch_test_suite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    //(CU_add_test(store_test_suite, "Create and destroy store", test_create_destroy_store) == NULL) ||
    if (
        (CU_add_test(merch_test_suite, "Create and destroy merch", test_create_destroy_merch) == NULL) ||
        (CU_add_test(merch_test_suite, "Create same merch twice", test_create_merch_twice) == NULL) ||
        (CU_add_test(merch_test_suite, "Create and destroy multiple merch", test_create_destroy_merch) == NULL) ||
        (CU_add_test(merch_test_suite, "Destroy non existing merch", test_destroy_non_existing_merch) == NULL) ||
        (CU_add_test(merch_test_suite, "Destroy merch with shelfs", test_destroy_merch_with_shelfs) == NULL) ||
        (CU_add_test(merch_test_suite, "Edit non existing merch", test_edit_non_existing_merch) == NULL) ||
        (CU_add_test(merch_test_suite, "Edit merch name", test_edit_merch_name) == NULL) ||
        (CU_add_test(merch_test_suite, "Edit merch desc", test_edit_merch_description) == NULL) ||
        (CU_add_test(merch_test_suite, "Edit merch price", test_edit_merch_price) == NULL) ||
        (CU_add_test(merch_test_suite, "Test edit removed merch", test_edit_destroyed_merch) == NULL) ||
        (CU_add_test(merch_test_suite, "Edit merch with stock", test_edit_merch_with_stock) == NULL) ||
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