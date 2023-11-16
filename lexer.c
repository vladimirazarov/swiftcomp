#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int hextoint(char k)
{
    int a = k;
    if (a >= 48 && a <= 57)
    {
        return a - 48;
    }
    if (a >= 65 && a <= 70)
    {
        return a - 55;
    }
    if (a >= 97 && a <= 102)
    {
        return a - 87;
    }
    return -1;
}

int pwr(int a, int b)
{
    int x = 1;
    for (int i = 0; i < b; i++)
    {
        x = x * a;
    }
    return x;
}

Token make_token(TokenType typ, char *hodnota)
{
    Token lex;
    lex.type = typ;
    lex.value = hodnota;
    return lex;
}

Token get_token()
{
    State State = start;
    char a;
    char b;
    char c;
    char *str;
    int i;
    int z;
    int k;
    bool first;
    while (1)
    {
        switch (State)
        {
        case start:
            a = getchar();
            while (isspace(a) || a == '/')
            {
                if (a == '/')
                {
                    a = getchar();
                    if (a == '/')
                    {
                        while (a != '\n' && a != EOF)
                        {
                            a = getchar();
                        }
                        if (a == EOF)
                        {
                            ungetc(a, stdin);
                        }
                    }
                    else if (a == '*')
                    {
                        b = a;
                        a = getchar();
                        while ((a != '/' || b != '*') && a != EOF)
                        {
                            b = a;
                            a = getchar();
                        }
                        if (a == EOF)
                        {
                            fprintf(stderr, "unfinished block commentary");
                            exit(1);
                        }
                    }
                    else
                    {
                        ungetc(a, stdin);
                        a = '/';
                        break;
                    }
                }
                a = getchar();
            }
            if ((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90))
            {
                State = identifier;
            }
            else if (a == '"')
            { //multiline string handling missing
                State = string;
            }
            else if (a >= 48 && a <= 57)
            {
                State = Int;
            }
            else if (a == '(')
            {
                return make_token(LEFT_PARENTHESIS, "(");
            }
            else if (a == ':')
            {
                return make_token(COLON, ":");
            }
            else if (a == ')')
            {
                return make_token(RIGHT_PARENTHESIS, ")");
            }
            else if (a == '+')
            {
                return make_token(PLUS_OPERATOR, "+");
            }
            else if (a == '-')
            {
                State = mins;
            }
            else if (a == '*')
            {
                return make_token(MULTIPLICATION_OPERATOR, "*");
            }
            else if (a == '/')
            {
                return make_token(DIVISION_OPERATOR, "/");
            }
            else if (a == '{')
            {
                return make_token(LEFT_CURLY_BRACE, "{");
            }
            else if (a == '}')
            {
                return make_token(RIGHT_CURLY_BRACE, "}");
            }
            else if (a == ';')
            {
                return make_token(SEMICOLON, ";");
            }
            else if (a == ',')
            {
                return make_token(COMMA, ",");
            }
            else if (a == '<')
            {
                State = lt0;
            }
            else if (a == '>')
            {
                State = mt0;
            }
            else if (a == '=')
            {
                State = identity1;
            }
            else if (a == '!')
            {
                State = nidentity1;
            }
            else if (a == EOF)
            {
                return make_token(END_OF_FILE, NULL);
            }
            else if (a == '?')
            {
                State = qmark;
            }

            else
            {
                State = Error;
            }
            break;

        case identifier:
            str = "";
            i = 0;
            while ((a >= 97 && a <= 122) || a == '_' || (a >= 65 && a <= 90) || (a >= 48 && a <= 57) || (a == '?'))
            {
                size_t len = strlen(str);
                char *str2 = malloc(len + 1 + 1);
                strcpy(str2, str);
                if (i > 0)
                {
                    free(str);
                }
                str2[len] = a;
                str2[len + 1] = '\0';
                str = str2;
                a = getchar();
                i++;
            }
            if (i > 0)
            {
                ungetc(a, stdin);
            }
            if (strcmp(str, "String") == 0)
            {
                return make_token(STRING_KEYWORD, str);
            }
            else if (strcmp(str, "String?") == 0)
            {
                return make_token(STRING_NULLABLE_KEYWORD, str);
            }
            else if (strcmp(str, "Int") == 0)
            {
                return make_token(INT_KEYWORD, str);
            }
            else if (strcmp(str, "Int?") == 0)
            {
                return make_token(INT_NULLABLE_KEYWORD, str);
            }
            else if (strcmp(str, "Double") == 0)
            {
                return make_token(DOUBLE_KEYWORD, str);
            }
            else if (strcmp(str, "Double?") == 0)
            {
                return make_token(DOUBLE_NULLABLE_KEYWORD, str);
            }
            if (strcmp(str, "var") == 0)
            {
                return make_token(VAR_KEYWORD, str);
            }
            if (strcmp(str, "let") == 0)
            {
                return make_token(LET_KEYWORD, str);
            }
            if (strcmp(str, "return") == 0)
            {
                return make_token(RETURN_KEYWORD, str);
            }
            if (strcmp(str, "else") == 0)
            {
                return make_token(ELSE_KEYWORD, str);
            }
            if (strcmp(str, "func") == 0)
            {
                return make_token(FUNCTION_KEYWORD, str);
            }
            if (strcmp(str, "if") == 0)
            {
                return make_token(IF_KEYWORD, str);
            }
            if (strcmp(str, "nil") == 0)
            {
                return make_token(NIL_LITERAL, str);
            }
            if (strcmp(str, "while") == 0)
            {
                return make_token(WHILE_KEYWORD, str);
            }
            if (strcmp(str, "_") == 0)
            {
                return make_token(UNDERSCORE, str);
            }

            return make_token(IDENTIFIER, str);
            break;

        case Int:
            str = "";
            i = 0;
            // z = 0;
            while ((a >= 48 && a <= 57) || a == '.' || a == 'e' || a == 'E')
            {
                size_t len = strlen(str);
                char *str2 = malloc(len + 1 + 1);
                strcpy(str2, str);
                if (i > 0)
                {
                    free(str);
                }
                str2[len] = a;
                str2[len + 1] = '\0';
                str = str2;
                a = getchar();
                i++;
                if (a == '.' || a == 'e' || a == 'E')
                {
                    State = ddouble;
                    break;
                }
            }
            if (i > 0 && State != ddouble)
            {
                ungetc(a, stdin);
            }
            if (State == ddouble)
            {
                break;
            }
            return make_token(INTEGER_LITERAL, str);
            break;

        case ddouble: // work pls
            if (a == '.')
            {
                a = getchar();
                if (!(a >= 48 && a <= 57))
                { // check if number following char is a number
                    fprintf(stderr, "unexpected symbol: .");
                    exit(1);
                }
                size_t len = strlen(str);
                char *str2 = malloc(len + 1 + 1);
                strcpy(str2, str);
                if (i > 0)
                {
                    free(str);
                }
                str2[len] = '.';
                str2[len + 1] = '\0';
                str = str2; // add '.' to string

                while ((a >= 48 && a <= 57))
                { // add all following numbers to the string
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if (i > 0)
                    {
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                }

                if (a == 'e' || a == 'E')
                { // check for e/E
                    a = getchar();
                    if (a == '+' || a == '-')
                    {
                        size_t len = strlen(str);
                        char *str2 = malloc(len + 1 + 2);
                        strcpy(str2, str);
                        if (i > 0)
                        {
                            free(str);
                        }
                        str2[len] = 'e';
                        str2[len + 1] = a;
                        str2[len + 2] = '\0';
                        str = str2;
                        a = getchar();
                    }
                    else
                    {
                        size_t len = strlen(str);
                        char *str2 = malloc(len + 1 + 1);
                        strcpy(str2, str);
                        if (i > 0)
                        {
                            free(str);
                        }
                        str2[len] = 'e';
                        str2[len + 1] = '\0'; // add 'e/+/-' to string
                        str = str2;
                    }
                    if (!(a >= 48 && a <= 57))
                    {
                        fprintf(stderr, "missing value for e");
                        exit(1);
                    }
                    while ((a >= 48 && a <= 57))
                    {
                        size_t len = strlen(str);
                        char *str2 = malloc(len + 1 + 1);
                        strcpy(str2, str);
                        if (i > 0)
                        {
                            free(str);
                        }
                        str2[len] = a;
                        str2[len + 1] = '\0';
                        str = str2;
                        a = getchar();
                        i++;
                    }
                    ungetc(a, stdin);
                    return make_token(DOUBLE_LITERAL, str);
                }
                else
                {
                    ungetc(a, stdin);
                    return make_token(DOUBLE_LITERAL, str);
                }
            }
            else
            { // no '.' just e/E
                a = getchar();
                if (a == '+' || a == '-')
                {
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 2);
                    strcpy(str2, str);
                    if (i > 0)
                    {
                        free(str);
                    }
                    str2[len] = 'e';
                    str2[len + 1] = a;
                    str2[len + 2] = '\0';
                    str = str2;
                    a = getchar();
                }
                else
                {
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if (i > 0)
                    {
                        free(str);
                    }
                    str2[len] = 'e';
                    str2[len + 1] = '\0'; // add e/E/+/- to string
                    str = str2;
                }
                if (!(a >= 48 && a <= 57))
                { // check for e value
                    fprintf(stderr, "missing value for e");
                    exit(1);
                }
                while ((a >= 48 && a <= 57))
                { // load e value
                    size_t len = strlen(str);
                    char *str2 = malloc(len + 1 + 1);
                    strcpy(str2, str);
                    if (i > 0)
                    {
                        free(str);
                    }
                    str2[len] = a;
                    str2[len + 1] = '\0';
                    str = str2;
                    a = getchar();
                    i++;
                }
                ungetc(a, stdin);
                return make_token(DOUBLE_LITERAL, str);
            }
            break;

        case string:
            //z = 0;
            b = 0;
            //c = 0;
            str = "";
            i = 0;
            int cflag = 0;
            int bflag = 0;
            char arr[9];
            //memset(arr, 0, sizeof(arr));
            a = getchar();
            if (a == '"')
            { // if second " is detected its either multiline string or an empty one
                b = a;
                a = getchar();
                if (a == '"')
                {
                    State = stringmultiline;
                    break;
                }
                else
                {
                    return make_token(STRING_LITERAL, NULL);
                    break;
                }
            }
            while (a != '"' && a > 31)
            {
                int y;
                y = 0;
                z = 0;
                c = 0;
                k = 0;
                if (a == 92)
                {
                    a = getchar();
                    if (a == 'n')
                    {
                        a = '\n';
                    }
                    else if (a == 't')
                    {
                        a = '\t';
                    }
                    else if (a == '"')
                    {
                        a = '"';
                    }
                    else if (a == 'r')
                    {
                        a = '\r';
                    }
                    else if (a == 92)
                    {
                        a = 92;
                    }
                    else if (a == 'u')
                    {
                        //   \u handling not functional
                        a = getchar();
                        if (a == '{') {
                            a = getchar();
                            if (a == '"') { //handle case where quotes are placed directly behind the first bracket 
                                size_t len = strlen(str);
                                char *str2 = malloc(len + 1 + 3);
                                strcpy(str2, str);
                                if(i>0){
                                    free(str);
                                }
                                str2[len] = 92;
                                str2[len + 1] = 'u';
                                str2[len + 2] = '{';
                                str2[len + 3] = '\0';
                                str = str2;
                                i++;
                                State = stringvalid;
                                break;
                            }
                            while ((a <= 57 && a >= 48) || (a >= 65 && a<= 70) || (a >= 97 && a <= 102) || z <= 7) {
                                if (a != '}') {
                                    arr[z] = a;
                                    z++;
                                }
                                if (a == '}' && z == 0) {
                                size_t len = strlen(str);
                                char *str2 = malloc(len + 1 + 4);
                                strcpy(str2, str);
                                if(i>0){
                                    free(str);
                                }
                                str2[len] = 92;
                                str2[len + 1] = 'u';
                                str2[len + 2] = '{';
                                str2[len + 3] = '}';
                                str2[len + 4] = '\0';
                                str = str2;
                                i++;
                                cflag = 1;
                                break;
                            }
                                if (!(arr[0] <= 57 && arr[0] >= 48) && !(arr[0] >= 65 && arr[0]<= 70) && !(arr[0] >= 97 && arr[0] <= 102)) {
                                    size_t len = strlen(str);
                                    char *str2 = malloc(len + 1 + 4);
                                    strcpy(str2, str);
                                    if (i>0){
                                        free(str);
                                    }
                                    str2[len] = 92;
                                    str2[len + 1] = 'u';
                                    str2[len + 2] = '{';
                                    str2[len + 3] = arr[0];
                                    str2[len + 4] = '\0';
                                    str = str2;
                                    i++;
                                    cflag = 1;
                                    break;
                                }
                                if (z > 8) { 
                                    size_t len = strlen(str);
                                    char *str2 = malloc(len + 1 + 11);
                                    strcpy(str2, str);
                                    if (i>0){
                                        free(str);
                                    }
                                    str2[len] = 92;
                                    str2[len + 1] = 'u';
                                    str2[len + 2] = '{';
                                    str2[len + 3] = arr[0];
                                    str2[len + 4] = arr[1];
                                    str2[len + 5] = arr[2];
                                    str2[len + 6] = arr[3];
                                    str2[len + 7] = arr[4];
                                    str2[len + 8] = arr[5];
                                    str2[len + 9] = arr[6];
                                    str2[len + 10] = arr[7];
                                    str2[len + 11] = '\0';
                                    str = str2;
                                    i++;
                                    //cflag = 1;
                                    break;
                                }
                                a = getchar();
                                if ( a == '}') {
                                        k = z;
                                        while (z != 0) {
                                            z--;
                                            c += hextoint(arr[y]) * pwr(16, z);     //ronald mcnutt
                                            y++;
                                        }

                                        if (c != 0) {
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 1);
                                        strcpy(str2, str);
                                        if (i>0) {
                                            free(str);
                                        }
                                        str2[len] = c;
                                        str2[len + 1] = '\0';
                                        str = str2;
                                        i++;
                                        a = getchar();
                                        break;
                                        } 
                                        else if (c == 0) {
                                            size_t len = strlen(str);
                                            char *str2 = malloc(len + 1 + 4);
                                            strcpy(str2, str);
                                            if (i>0) {
                                            free(str);
                                            }
                                            str2[len] = 92;
                                            str2[len + 1] = 'u';
                                            str2[len + 2] = '{';
                                            str2[len + 3] = '}';
                                            str2[len + 4] = '\0';
                                            str = str2;
                                            i++;
                                            //a = getchar();
                                            break;
                                        
                                        }
                                    }
                               else if (!(a <= 57 && a >= 48) && !(a >= 65 && a<= 70) && !(a >= 97 && a <= 102) && !(a == '"')) {
                                        size_t len = strlen(str);
                                        char *str2 = malloc(len + 1 + 3);
                                        strcpy(str2, str);
                                        if(i>0){
                                            free(str);
                                        }
                                        str2[len] = 92;
                                        str2[len + 1] = 'u';
                                        str2[len + 2] = '{';
                                        str2[len + 3] = '\0';
                                        str = str2;
                                        i++;
                                        switch (z) {                //switch>loop

                                            case 1:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 2);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = a;
                                                str2[len + 2] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                                case 2:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 3);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = a;
                                                    str2[len + 3] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                                case 3:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 4);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = arr[2];
                                                    str2[len + 3] = a;
                                                    str2[len + 4] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                                case 4:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 5);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = arr[2];
                                                    str2[len + 3] = arr[3];
                                                    str2[len + 4] = a;
                                                    str2[len + 5] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                                case 5:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 6);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = arr[2];
                                                    str2[len + 3] = arr[3];
                                                    str2[len + 4] = arr[4];
                                                    str2[len + 5] = a;
                                                    str2[len + 6] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                                case 6:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 7);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = arr[2];
                                                    str2[len + 3] = arr[3];
                                                    str2[len + 4] = arr[4];
                                                    str2[len + 5] = arr[5];
                                                    str2[len + 6] = a;
                                                    str2[len + 7] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                                case 7:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 8);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = arr[2];
                                                    str2[len + 3] = arr[3];
                                                    str2[len + 4] = arr[4];
                                                    str2[len + 5] = arr[5];
                                                    str2[len + 6] = arr[6];
                                                    str2[len + 7] = a;
                                                    str2[len + 8] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                                case 8:
                                                    len = strlen(str);
                                                    str2 = malloc(len + 1 + 9);
                                                    strcpy(str2, str);
                                                    if (i>0){
                                                        free(str);
                                                    }
                                                    str2[len] = arr[0];
                                                    str2[len + 1] = arr[1];
                                                    str2[len + 2] = arr[2];
                                                    str2[len + 3] = arr[3];
                                                    str2[len + 4] = arr[4];
                                                    str2[len + 5] = arr[5];
                                                    str2[len + 6] = arr[6];
                                                    str2[len + 7] = arr[7];
                                                    str2[len + 8] = a;
                                                    str2[len + 9] = '\0';
                                                    str = str2;
                                                    i++;
                                                    cflag = 1;
                                                    break;

                                            
                                        }
                                        if (cflag == 1){
                                            break;
                                        }
                                    
                                } 
                                else if (a == '"') {
                                    size_t len = strlen(str);
                                    char *str2 = malloc(len + 1 + 3);
                                    strcpy(str2, str);
                                    if(i>0){
                                        free(str);
                                    }
                                    str2[len] = 92;
                                    str2[len + 1] = 'u';
                                    str2[len + 2] = '{';
                                    str2[len + 3] = '\0';
                                    str = str2;
                                    i++;
                                    switch (z) {                //switch>loop

                                        case 1:
                                            len = strlen(str);
                                            str2 = malloc(len + 1 + 2);
                                            strcpy(str2, str);
                                            if (i>0){
                                                free(str);
                                            }
                                            str2[len] = arr[0];
                                            str2[len + 1] = a;
                                            str2[len + 2] = '\0';
                                            str = str2;
                                            i++;
                                            cflag = 1;
                                            break;

                                            case 2:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 3);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = a;
                                                str2[len + 3] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            case 3:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 4);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = arr[2];
                                                str2[len + 3] = a;
                                                str2[len + 4] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            case 4:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 5);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = arr[2];
                                                str2[len + 3] = arr[3];
                                                str2[len + 4] = a;
                                                str2[len + 5] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            case 5:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 6);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = arr[2];
                                                str2[len + 3] = arr[3];
                                                str2[len + 4] = arr[4];
                                                str2[len + 5] = a;
                                                str2[len + 6] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            case 6:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 7);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = arr[2];
                                                str2[len + 3] = arr[3];
                                                str2[len + 4] = arr[4];
                                                str2[len + 5] = arr[5];
                                                str2[len + 6] = a;
                                                str2[len + 7] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            case 7:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 8);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = arr[2];
                                                str2[len + 3] = arr[3];
                                                str2[len + 4] = arr[4];
                                                str2[len + 5] = arr[5];
                                                str2[len + 6] = arr[6];
                                                str2[len + 7] = a;
                                                str2[len + 8] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            case 8:
                                                len = strlen(str);
                                                str2 = malloc(len + 1 + 9);
                                                strcpy(str2, str);
                                                if (i>0){
                                                    free(str);
                                                }
                                                str2[len] = arr[0];
                                                str2[len + 1] = arr[1];
                                                str2[len + 2] = arr[2];
                                                str2[len + 3] = arr[3];
                                                str2[len + 4] = arr[4];
                                                str2[len + 5] = arr[5];
                                                str2[len + 6] = arr[6];
                                                str2[len + 7] = arr[7];
                                                str2[len + 8] = a;
                                                str2[len + 9] = '\0';
                                                str = str2;
                                                i++;
                                                cflag = 1;
                                                break;

                                            
                                    }
                                    if (cflag == 1){
                                        break;
                                    }

                                }
                            }

                            } else {                            //braces missing, add u to the string
                                ungetc(a, stdin);
                                size_t len = strlen(str);
                                char *str2 = malloc(len + 1 + 2);
                                strcpy(str2, str);
                                if(i>0){
                                    free(str);
                                }
                                str2[len] = 92;
                                str2[len + 1] = 'u';
                                str2[len + 2] = '\0';
                                str = str2;
                                i++;
                                }
                    }
                }
                if (a == '"') {
                    State = stringvalid;
                    break;
                }
                if (cflag != 1){
                size_t len = strlen(str);
                char *str2 = malloc(len + 1 + 1);
                strcpy(str2, str);
                if (i > 0)
                {
                    free(str);
                }
                str2[len] = a;
                str2[len + 1] = '\0';
                str = str2;
                } else {
                cflag = 0;
                }
                a = getchar();
                i++;
            }
            if (a == '"')
            {
                State = stringvalid;
                break;
            }
            else
            {
                fprintf(stderr, "missing quotes");
                exit(1);
            }
            break;

        case stringvalid:
            return make_token(STRING_LITERAL, str);
            break;

        case MULTILINE_STRING:
            // TBD
            break;

        case identity1:
            a = getchar();
            if (a != '=')
            {
                ungetc(a, stdin);
                return make_token(ASSIGNMENT_OPERATOR, "=");
            }
            else
            {
                return make_token(EQUALITY_OPERATOR, "==");
            }
            break;

        case nidentity1:
            a = getchar();
            if (a != '=')
            {
                ungetc(a, stdin);
                return make_token(EXCLAMATION_MARK, "!");
            }
            else
            {
                return make_token(NOT_EQUAL_OPERATOR, "!=");
            }
            break;

        case Error:
            fprintf(stderr, "syntax error");
            exit(1);
            break;

        case lt0:
            a = getchar();
            if (a != '=')
            {
                ungetc(a, stdin);
                return make_token(LESS_THAN_OPERATOR, "<");
            }
            else
            {
                return make_token(LESS_THAN_EQUAL_OPERATOR, "<=");
            }
            break;

        case mt0:
            a = getchar();
            if (a != '=')
            {
                ungetc(a, stdin);
                return make_token(GREATER_THAN_OPERATOR, ">");
            }
            else
            {
                return make_token(GREATER_THAN_EQUAL_OPERATOR, ">=");
            }
            break;

        case mins:
            a = getchar();
            if (a != '>')
            {
                ungetc(a, stdin);
                return make_token(MINUS_OPERATOR, "-");
            }
            else
            {
                return make_token(ARROW, "->");
            }
            break;

        case qmark:
            a = getchar();
            if (a != '?')
            {
                fprintf(stderr, "unexpected symbol: ?");
                exit(1);
            }
            else
            {
                return make_token(DOUBLE_QMARK_OPERATOR, "??");
            }
            break;
        }
    }
}
