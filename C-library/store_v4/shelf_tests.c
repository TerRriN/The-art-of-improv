#include <CUnit/Basic.h>
#include "utilities/common.h"
#include "utilities/hash_table.h"
#include "utilities/linked_list.h"
#include "shelf.h"
//CUnit cant print number of passed suites
int init_suite(void){return 0;}
int clean_suite(void){return 0;}

void test_create_destroy_shelf(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);

    CU_ASSERT_TRUE_FATAL(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);

    elem_t tmp = {.str_val = strdup("A01")};
    shelf_t *s = ioopm_hash_table_lookup(shelf_ht, tmp).value.point;
    CU_ASSERT_TRUE_FATAL(strcmp(s->shelf_id.str_val, "A01") == 0);
    CU_ASSERT_TRUE_FATAL(s->amount.int_val == 10);

    CU_ASSERT_TRUE_FATAL(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("A01")}).success);

    free(tmp.str_val);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_create_shelf_wrong_id(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);

    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0A")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0A0")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0A0A")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0A0A0")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0A0A0A")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A0A0A0A0")}, (elem_t){.int_val = 10}).success);

    ioopm_hash_table_destroy(shelf_ht);
}

void test_create_shelf_twice(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);

    CU_ASSERT_TRUE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);

    ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("A01")});
    ioopm_hash_table_destroy(shelf_ht);
}

void test_create_negative_shelf(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);

    CU_ASSERT_FALSE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = -10}).success);

    ioopm_hash_table_destroy(shelf_ht);
}

void test_create_destroy_multiple_shelves(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);

    CU_ASSERT_TRUE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);
    CU_ASSERT_TRUE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("B17")}, (elem_t){.int_val = 20}).success);
    CU_ASSERT_TRUE(ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("X22")}, (elem_t){.int_val = 30}).success);

    elem_t tmp = {.str_val = strdup("A01")};
    shelf_t *s = ioopm_hash_table_lookup(shelf_ht, tmp).value.point;
    CU_ASSERT_TRUE(strcmp(s->shelf_id.str_val, "A01") == 0);
    CU_ASSERT_TRUE(s->amount.int_val == 10);
    free(tmp.str_val);

    tmp.str_val = strdup("B17");
    s = ioopm_hash_table_lookup(shelf_ht, tmp).value.point;
    CU_ASSERT_TRUE(strcmp(s->shelf_id.str_val, "B17") == 0);
    CU_ASSERT_TRUE(s->amount.int_val == 20);
    free(tmp.str_val);

    tmp.str_val = strdup("X22");
    s = ioopm_hash_table_lookup(shelf_ht, tmp).value.point;
    CU_ASSERT_TRUE(strcmp(s->shelf_id.str_val, "X22") == 0);
    CU_ASSERT_TRUE(s->amount.int_val == 30);
    free(tmp.str_val);

    CU_ASSERT_TRUE(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("A01")}).success);
    CU_ASSERT_TRUE(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("B17")}).success);
    CU_ASSERT_TRUE(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("X22")}).success);

    ioopm_hash_table_destroy(shelf_ht);
}

void test_destroy_non_existing_shelf(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);

    CU_ASSERT_FALSE_FATAL(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("A01")}).success);

    ioopm_hash_table_destroy(shelf_ht);
}

void test_resupply_shelf(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});

    CU_ASSERT_TRUE(ioopm_shelf_resupply(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10}).success);

    elem_t tmp = {.str_val = strdup("A01")};
    shelf_t *s = ioopm_hash_table_lookup(shelf_ht, tmp).value.point;
    CU_ASSERT_TRUE(strcmp(s->shelf_id.str_val, "A01") == 0);
    CU_ASSERT_TRUE(s->amount.int_val == 20);

    CU_ASSERT_TRUE(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("A01")}).success);

    free(tmp.str_val);
    ioopm_hash_table_destroy(shelf_ht);
}

void test_reupply_shelf_to_negative_value(){
    ioopm_hash_table_t *shelf_ht = ioopm_hash_table_create(hash_function, compare_str, compare_str);
    ioopm_shelf_create(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = 10});

    CU_ASSERT_FALSE(ioopm_shelf_resupply(shelf_ht, (elem_t){.str_val = strdup("A01")}, (elem_t){.int_val = -20}).success);

    elem_t tmp = {.str_val = strdup("A01")};
    shelf_t *s = ioopm_hash_table_lookup(shelf_ht, tmp).value.point;
    CU_ASSERT_TRUE(strcmp(s->shelf_id.str_val, "A01") == 0);
    CU_ASSERT_TRUE(s->amount.int_val == 10);

    CU_ASSERT_TRUE(ioopm_shelf_destroy(shelf_ht, (elem_t){.str_val = strdup("A01")}).success);

    free(tmp.str_val);
    ioopm_hash_table_destroy(shelf_ht);
}

int main(){
    if(CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();
    CU_pSuite shelf_test_suite = CU_add_suite("Shelf actions test", init_suite, clean_suite);
    if(shelf_test_suite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(
        (CU_add_test(shelf_test_suite, "Create and destroy shelf", test_create_destroy_shelf) == NULL) ||
        (CU_add_test(shelf_test_suite, "Create shelf wit wrong id", test_create_shelf_wrong_id) == NULL) ||
        (CU_add_test(shelf_test_suite, "Create shelf with existing id", test_create_shelf_twice) == NULL) ||
        (CU_add_test(shelf_test_suite, "Create shelf with negative amount", test_create_negative_shelf) == NULL) ||
        (CU_add_test(shelf_test_suite, "Create and destroy multiple shelves", test_create_destroy_multiple_shelves) == NULL) ||
        (CU_add_test(shelf_test_suite, "Destroy non existing shelf", test_destroy_non_existing_shelf) == NULL) ||
        (CU_add_test(shelf_test_suite, "Resupply shelf", test_resupply_shelf) == NULL) ||
        (CU_add_test(shelf_test_suite, "Resupply shelf to negative value", test_reupply_shelf_to_negative_value) == NULL) ||
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