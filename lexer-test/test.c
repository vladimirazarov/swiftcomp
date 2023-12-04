#include "../lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lexer.c"

int main (int argc, char *argv[]) {
    Token test = get_token();
    int ref[2] = {MULTILINE_STRING};
    Token array[9];
    int i = 0;
    do
    {
        array[i] = test;
        if ((int)array[i].type == ref[i]) {
            printf("PASS \n%s", array[i].value);
        }
        else {
            printf("FAIL \n%s", array[i].value);
        }
        i++;
        test = get_token();
    } while (test.type != END_OF_FILE);

    return 0;
    

}

