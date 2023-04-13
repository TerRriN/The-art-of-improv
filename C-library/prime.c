#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "linked_list.h"

int list_accesses;

/**
 * This program demonstrates the power of refactoring
 * by calculating all prime numbers between 1 and 50000
 * in different ways
 * OBS: The first version takes over 40s
*/

/************************************************************************/
/*                                  V1                                  */
/************************************************************************/
void v1_compare_prime_numbers(ioopm_list_t *prime_numbers, int x){
    for(int i = 0; i < ioopm_linked_list_size(prime_numbers); i++){
        int fetched_nr = ioopm_linked_list_get(prime_numbers,i).int_val;
        list_accesses++;
        if(x % fetched_nr == 0){ //if x (mod) prime_nr == 0, then x is not a prime number
            return;
        }
    }
    ioopm_linked_list_append(prime_numbers, int_elem(x));
}

void v1_find_prime_numbers(int interval){
    ioopm_list_t *ls = ioopm_linked_list_create((cmp_func_t)compare_int);
    ioopm_linked_list_append(ls, int_elem(2));
    list_accesses = 0;

    clock_t start = clock();
    for(int i = 3; i < interval; i++){
        v1_compare_prime_numbers(ls, i);
    }
    clock_t end = clock();
    double temp = end - start;
    double elapsed = temp/CLOCKS_PER_SEC;

    printf("\n");
    printf("Version 1\n");
    printf("List comparisons = %d\n",list_accesses);
    printf("Found prime numbers between 1 and %d = %ld\n", interval, (ioopm_linked_list_size(ls)-1));
    printf("Time measured: %.6f seconds.\n", elapsed);
    printf("\n");

    ioopm_linked_list_destroy(ls);
}
/************************************************************************/
/************************************************************************/

/************************************************************************/
/*                                  V2                                  */
/************************************************************************/
void v2_compare_prime_numbers(ioopm_list_t *prime_numbers, int x){
    for(int i = 0; i < ioopm_linked_list_size(prime_numbers); i++){
        int fetched_nr = ioopm_linked_list_get(prime_numbers,i).int_val;
        list_accesses++;
        if(x % fetched_nr == 0){
            return;
        }
    }
    ioopm_linked_list_append(prime_numbers, int_elem(x));
}

void v2_find_prime_numbers(int interval){
    ioopm_list_t *ls = ioopm_linked_list_create((cmp_func_t)compare_int);
    ioopm_linked_list_append(ls, int_elem(2));
    list_accesses = 0;

    clock_t start = clock();
    for(int i = 3; i < interval; i+=2){ //NEW
        int last_digit = i % 10; //NEW
        if(last_digit == 1 || last_digit == 3 || last_digit == 7 || last_digit == 9) v2_compare_prime_numbers(ls, i); //NEW
    }
    clock_t end = clock();
    double temp = end - start;
    double elapsed = temp/CLOCKS_PER_SEC;

    printf("\n");
    printf("Version 2\n");
    printf("List comparisons = %d\n",list_accesses);
    printf("Found prime numbers between 1 and %d = %ld\n", interval, ioopm_linked_list_size(ls));
    printf("Time measured: %.6f seconds.\n", elapsed);
    printf("\n");

    ioopm_linked_list_destroy(ls);
}
/************************************************************************/
/************************************************************************/

/************************************************************************/
/*                                  V3                                  */
/************************************************************************/
void v3_compare_prime_numbers(ioopm_list_t *prime_numbers, int x){
    for(int i = 0; i < ioopm_linked_list_size(prime_numbers); i++){
        int fetched_nr = ioopm_linked_list_get(prime_numbers,i).int_val;
        list_accesses++;
        if(x % fetched_nr == 0){
            return;
        } else if((x/2) < fetched_nr){ //NEW
            break;
        }
    }
    ioopm_linked_list_append(prime_numbers, int_elem(x));
}

void v3_find_prime_numbers(int interval){
    ioopm_list_t *ls = ioopm_linked_list_create((cmp_func_t)compare_int);
    ioopm_linked_list_append(ls, int_elem(2));
    list_accesses = 0;

    clock_t start = clock();
    for(int i = 3; i < interval; i+=2){ //NEW
        int last_digit = i % 10; //NEW
        if(last_digit == 1 || last_digit == 3 || last_digit == 7 || last_digit == 9) v3_compare_prime_numbers(ls, i); //NEW
    }
    clock_t end = clock();
    double temp = end - start;
    double elapsed = temp/CLOCKS_PER_SEC;

    printf("\n");
    printf("Version 3\n");
    printf("List comparisons = %d\n",list_accesses);
    printf("Found prime numbers between 1 and %d = %ld\n", interval, ioopm_linked_list_size(ls));
    printf("Time measured: %.6f seconds.\n", elapsed);
    printf("\n");

    ioopm_linked_list_destroy(ls);
}
/************************************************************************/
/************************************************************************/

/************************************************************************/
/*                                  v4                                  */
/************************************************************************/
void v4_compare_prime_numbers(ioopm_list_t *prime_numbers, int x){
    for(int i = 0; i < ioopm_linked_list_size(prime_numbers); i++){
        int fetched_nr = ioopm_linked_list_get(prime_numbers,i).int_val;
        list_accesses++;
        if(x % fetched_nr == 0){
            return;
        }else if(sqrt(x) < fetched_nr){ //NEW //NEW
            break;
        }
    }
    ioopm_linked_list_append(prime_numbers, int_elem(x));
}

void v4_find_prime_numbers(int interval){
    ioopm_list_t *ls = ioopm_linked_list_create((cmp_func_t)compare_int);
    ioopm_linked_list_append(ls, int_elem(2));
    list_accesses = 0;

    clock_t start = clock();
    for(int i = 3; i < interval; i+=2){ //NEW
        int last_digit = i % 10; //NEW
        if(last_digit == 1 || last_digit == 3 || last_digit == 7 || last_digit == 9) v4_compare_prime_numbers(ls, i); //NEW
    }
    clock_t end = clock();
    double temp = end - start;
    double elapsed = temp/CLOCKS_PER_SEC;

    printf("\n");
    printf("Version 4\n");
    printf("List comparisons = %d\n",list_accesses);
    printf("Found prime numbers between 1 and %d = %ld\n", interval, ioopm_linked_list_size(ls));
    printf("Time measured: %.6f seconds.\n", elapsed);
    printf("\n");

    ioopm_linked_list_destroy(ls);
}
/************************************************************************/
/************************************************************************/

void main(){
    v1_find_prime_numbers(50000);
    v2_find_prime_numbers(50000);
    v3_find_prime_numbers(50000);
    v4_find_prime_numbers(50000);
}