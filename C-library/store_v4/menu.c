#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "utilities/utils.h"
#include "store.h"
#include "cart.h"
#include "utilities/common.h"

void print_menu(){
    char *menu = "\n"
                "A.   List merchandiase\n"
                "B.   List warehouse\n"
                "C.   List carts\n"
                "\n"
                "D.   Create Merchandise\n"
                "E.   Edit Merchandise\n"
                "F.   Replenish stock\n"
                "G.   Remove Merchandise\n"
                "\n"
                "H.   Create cart\n"
                "I.   Add item to cart\n"
                "J.   Remove item from cart\n"
                "K.   List cart items\n"
                "L.   Check prize\n"
                "M.   Checkout\n"
                "\n"
                "Q.   Quit\n";
    puts(menu);
}

void edit_merch_alternatives(){
    char *menu = "\n"
                "A.   Edit name\n"
                "B.   Edit description\n"
                "C.   Edit price\n";
    puts(menu);
}

void event_loop(ioopm_store_t *store){
    
    while(true){
        print_menu();
        char selection = ask_question_menu("");
        if (selection == 'A'){ // View all Merchandias
            ioopm_list_merchandise(store);
        
        }else if (selection == 'B'){ // View warehouse
            ioopm_list_warehouse(store);
        
        }else if (selection == 'C'){ // View all carts
            ioopm_list_carts(store);
        
        }else if(selection == 'D'){ //Create merchandias
            char *new_name = ask_question_string("New name: ");
            char *new_desc = ask_question_string("New description: ");
            int new_price = ask_question_int("New price: ");
            option_t o = ioopm_add_merch(store, ptr_elem(new_name), ptr_elem(new_desc), int_elem(new_price));
            if(o.success){
                printf("\n%s was successfully added\n", o.value.str_val);
            }else{
                printf("\nMerch already exist\n");
            }

        }else if(selection == 'E'){ //Edit merchandias
            char *merch_name = ask_question_string("Type name of merchandise you want to edit: ");
            if(!ioopm_check_if_in_cart(store, ptr_elem(merch_name))){
                edit_merch_alternatives();
                char selection = ask_question_menu("");
                /**
                 * option_t o;
                 * Could be initialized here and then get a value from either case
                 * and then check if it was successful or not
                 * 
                 * In order to reduce if statements
                */

                if(selection == 'A'){
                    char *new_name = ask_question_string("New name: ");
                    option_t o = ioopm_edit_merch(store, ptr_elem(merch_name), (elem_t){.char_val = 'A'}, ptr_elem(new_name));
                    if(o.success){
                        printf("\n%s was successfully edited\n", o.value.str_val);
                    }else{
                        printf("\nCould not edit merch\n");
                    }
                }else if(selection == 'B'){
                    char *new_desc = ask_question_string("New description: ");
                    option_t o = ioopm_edit_merch(store, ptr_elem(merch_name), (elem_t){.char_val = 'B'}, ptr_elem(new_desc));
                    if(o.success){
                        printf("\n%s was successfully edited\n", o.value.str_val);
                    }else{
                        printf("\nCould not edit merch\n");
                    }
                }else if(selection == 'C'){
                    int new_price = ask_question_int("New price: ");
                    option_t o = ioopm_edit_merch(store, ptr_elem(merch_name), (elem_t){.char_val = 'C'}, int_elem(new_price));
                    if(o.success){
                        printf("\n%s was successfully edited\n", o.value.str_val);
                    }else{
                        printf("\nCould not edit merch\n");
                    }
                }
            }else{
                printf("\nMerchandise is in cart, can't edit\n");
            }
            /*char *merch_name = ask_question_string("Type name of merchandise you want to edit: ");
            char *new_name = ask_question_string("New name: ");
            char *new_desc = ask_question_string("New description: ");
            int new_price = ask_question_int("New price: ");

            // Check if in cart
            option_t o = ioopm_edit_merch(store, ptr_elem(merch_name), ptr_elem(new_name), ptr_elem(new_desc), int_elem(new_price));
            if(o.success){
                printf("\n%s was successfully edited\n", o.value.str_val);
            }else{
                printf("\nCould not edit merch\n");
            }*/

        }else if(selection == 'F'){ //Replenish merchandias
            char *merch_name = ask_question_string("Type name of merchandise you want to replenish: ");
            char *shelf = ask_question_string("Where? (Ex: A01): ");
            int quantity = ask_question_int("Quantity: ");
            option_t o = ioopm_replenish_stock(store, ptr_elem(merch_name), ptr_elem(shelf), int_elem(quantity));
            if(o.success){
                printf("\nQuantity was successfully added to stock\n");
            }else{
                printf("\ndidnt work\n");
            }

        }else if(selection == 'G'){ //Remove merchandias
            char *merch_name = ask_question_string("Type name of merchandise you want to remove: ");
            option_t o = ioopm_remove_merch(store, ptr_elem(merch_name));
            if(o.success){
                printf("\n%s was successfully removed\n", merch_name);
            }else{
                printf("\n%s could not be removed\n", merch_name);
            }
            free(merch_name);

        }else if(selection == 'H'){ //Create cart
            ioopm_create_cart(store);

        }else if(selection == 'I'){ //Add item to cart
            int cart_id = ask_question_int("Type cart_id: ");
            char *merch_name = ask_question_string("Type name of merchandise: ");
            int quantity = ask_question_int("Quantity: ");
            ioopm_add_cart_item(store, ptr_elem(merch_name), cart_id, quantity);

        }else if(selection == 'J'){ //Remove item from cart
            int cart_id = ask_question_int("Type cart_id: ");
            char *merch_name = ask_question_string("Type name of merchandise: ");
            ioopm_remove_item_from_cart(store, cart_id, ptr_elem(merch_name));

        }else if(selection == 'K'){ //List cart items
            int cart_id = ask_question_int("Type cart_id: ");
            ioopm_list_cart_items(store, cart_id);

        }else if(selection == 'L'){ //Check prize
            int cart_id = ask_question_int("Type cart_id: ");
            int price = ioopm_calculate_cost(store, cart_id);
            printf("Total price: %d\n", price);

        }else if(selection == 'M'){ //Checkout cart
            int cart_id = ask_question_int("Type cart_id: ");
            ioopm_checkout_cart(store, cart_id);

        }else if(selection == 'Q'){ //Quit
            ioopm_destroy_store(store);
            break;
        }
    }
}

ioopm_store_t *help_create_store()
{
    ioopm_store_t *store = ioopm_create_store();
    ioopm_add_merch(store, ptr_elem(strdup("BLUEBERRY")), ptr_elem(strdup("BLUE")), int_elem(15));
    ioopm_add_merch(store, ptr_elem(strdup("Ball")), ptr_elem(strdup("Round")), int_elem(125));
    ioopm_add_merch(store, ptr_elem(strdup("CORN")), ptr_elem(strdup("JUICY")), int_elem(3500));
    ioopm_add_merch(store, ptr_elem(strdup("ORANGE")), ptr_elem(strdup("ORANGE")), int_elem(7));
    ioopm_add_merch(store, ptr_elem(strdup("Car")), ptr_elem(strdup("FAST")), int_elem(3500000));
    ioopm_add_merch(store, ptr_elem(strdup("Computer")), ptr_elem(strdup("Smart")), int_elem(1000000000));

    ioopm_replenish_stock(store, ptr_elem(strdup("BLUEBERRY")), ptr_elem(strdup("A01")), int_elem(4));
    ioopm_replenish_stock(store, ptr_elem(strdup("Car")), ptr_elem(strdup("A02")), int_elem(4));
    ioopm_replenish_stock(store, ptr_elem(strdup("Computer")), ptr_elem(strdup("B17")), int_elem(4));
    ioopm_replenish_stock(store, ptr_elem(strdup("CORN")), ptr_elem(strdup("Z33")), int_elem(4));
    ioopm_replenish_stock(store, ptr_elem(strdup("Ball")), ptr_elem(strdup("G11")), int_elem(4));

    ioopm_create_cart(store);
    ioopm_create_cart(store);
    ioopm_create_cart(store);
    ioopm_create_cart(store);

    return store;
}

int main(void){
    ioopm_store_t *store = help_create_store();
    event_loop(store);

    //char test = ask_question_shelf("Ex: A01\n");
    //printf("%d\n", test);




    return 0;
}
/*
ask_question_shelf doesent work
*/