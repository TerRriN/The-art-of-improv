extern char *strdup(const char *);
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include "../utils.h"

void clear_input_buffer()
{
    int c;
    do
    {
        c = getchar(); // (inbyggd funktion)lagar alla character ett åt gången i c
    } while (c != '\n' && c != EOF);
}

bool not_empty(char *str)
{
    return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
    int buf_siz = 255;
    char buf[buf_siz];
    int kontroll = 0;

    while (kontroll == 0) // (true)
    {
        printf("%s", question);
        read_string(buf, buf_siz);
        if (check(buf)) // om det är sant
        {
            kontroll = 1; // break;
        }
    }
    answer_t resul = convert(buf);
    return resul;
}

void print(char *moz)
{
    int acc = 0;
    while (moz[acc] != '\0')
    {
        putchar(moz[acc]);
        acc++;
    }
}

void println(char *alu)
{
    print(alu);
    putchar('\n');
}
bool is_number(char *str)
{
    int length = strlen(str);
    if (length == 0) // Returns false if nothing is entered
    {
        return false;
    }

    // printf("Length is %d\n", length);
    for (int i = 0; i < length; i++)
    {
        // printf("%c\n", str[i]);
        if (isdigit(str[i]) == false)
        {
            if (i == 0 && str[i] == '-' && length > 1)
            {
            }
            // Exception, first char is not a number but it's a '-' sign, then loop continues
            else
            {
                return false;
            }
        }
    }
    return true;
}
bool is_shelf(char *str)
{
    int length = strlen(str);
    if (length != 3) // Returns false if less than 2 char/digits are entered
    {
        return false;
    }
    else if ((str[0] >= 97 && str[0] <= 122) || (str[0] >= 65 && str[0] <= 90))
    {
        if (isdigit(str[1]) == true && isdigit(str[2] == true))
        {
            return true;
        }
    }
    return false;
}

bool is_menu(char *str)
{
    int length = strlen(str);
    if (length != 1) // Returns false if less than 2 char/digits are entered
    {
        return false;
    }
    else if ((str[0] >= 65 && str[0] <= 90) || (str[0] >= 97 && str[0] <= 122))
    {
        return true;
    }
    // if first digit is a-z or A-Z and it's followed by at least 1 digit
    else
    {
        return false;
    }
}
char to_upper(char *str)
{
    char inpC = toupper(str[0]);
    return inpC;
}
int read_string(char *buf, int buf_siz)
{
    int c;
    int count = 0;

    for (;; count++)
    {
        c = getchar(); // Temp stores input character in c

        if (c == '\n')
        {
            buf[count] = '\0';
            return count;
        }
        else
        {
            if (count >= buf_siz - 1)
            {
                buf[count] = '\0';
                clear_input_buffer();
                return count;
            }
            else
            {
                buf[count] = c; // Stores each read character in c unless 'Enter' is pressed
            }
        }
    }
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func)strdup).string_value;
}
char ask_question_menu(char *question)
{
    return ask_question(question, is_menu, (convert_func)to_upper).str_value;
}
char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func)strdup).string_value;
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func)atoi);
    return answer.int_value; // svaret som ett heltal
}