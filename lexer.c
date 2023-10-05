#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
            { // let and var, multiline string handling missing
                State = string;
            }
            else if (a >= 48 && a <= 57)
            {
                State = Int;
            }
            else if (a == '(')
            {
                return make_token(lbracket, "(");
            }
            else if (a == ':')
            {
                return make_token(colon, ":");
            }
            else if (a == ')')
            {
                return make_token(rbracket, ")");
            }
            else if (a == '+')
            {
                return make_token(plus, "+");
            }
            else if (a == '-')
            {
                State = mins;
            }
            else if (a == '*')
            {
                return make_token(mul, "*");
            }
            else if (a == '/')
            {
                return make_token(division, "/");
            }
            else if (a == '{')
            {
                return make_token(lsetbracket, "{");
            }
            else if (a == '}')
            {
                return make_token(rsetbracket, "}");
            }
            else if (a == ';')
            {
                return make_token(semicolon, ";");
            }
            else if (a == ',')
            {
                return make_token(comma, ",");
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
                return make_token(eof, NULL);
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

            // KEYWORD TESTS
            if (strcmp(str, "String") == 0 || strcmp(str, "Int") == 0 || strcmp(str, "Double") == 0 ||
                strcmp(str, "Double?") == 0 || strcmp(str, "Int?") == 0 || strcmp(str, "String?") == 0)
            {
                return make_token(type, str);
            }
            if (strcmp(str, "var") == 0)
            {
                return make_token(varkw, str);
            }
            if (strcmp(str, "let") == 0)
            {
                return make_token(letkw, str);
            }
            if (strcmp(str, "return") == 0)
            {
                return make_token(funreturn, str);
            }
            if (strcmp(str, "else") == 0)
            {
                return make_token(funelse, str);
            }
            if (strcmp(str, "func") == 0)
            {
                return make_token(function, str);
            }
            if (strcmp(str, "if") == 0)
            {
                return make_token(funif, str);
            }
            if (strcmp(str, "nil") == 0)
            {
                return make_token(nil, str);
            }
            if (strcmp(str, "while") == 0)
            {
                return make_token(funwhile, str);
            }

            return make_token(identifiertoken, str);
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
            return make_token(integer, str);
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
                    return make_token(doubletoken, str);
                }
                else
                {
                    ungetc(a, stdin);
                    return make_token(doubletoken, str);
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
                return make_token(doubletoken, str);
            }
            break;

        case string:
            // z = 0;
            str = "";
            i = 0;
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
                    return make_token(stringtoken, NULL);
                    break;
                }
            }
            while (a != '"' && a > 31)
            {
                int cflag = 0;
                z = 0;
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
                        /* \u handling not functional
                        a = getchar();
                        if (a == '{') {
                            while ((a <= 57 && a >= 48) || (a >= 65 && a<= 70) || (a >= 97 && a <= 102) || z != 8) {
                                a = getchar();
                                z++;
                            }
                            if (a != '}') {

                            }*/
                    }
                }
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
            if (a == '"')
            {
                State = stringvalid;
            }
            else
            {
                fprintf(stderr, "missing quotes");
                exit(1);
            }
            break;

        case stringvalid:
            return make_token(stringtoken, str);
            break;

        case multilinestring:
            // TBD
            break;

        case identity1:
            a = getchar();
            if (a != '=')
            {
                ungetc(a, stdin);
                return make_token(assign, "=");
            }
            else
            {
                return make_token(identity, "==");
            }
            break;

        case nidentity1:
            a = getchar();
            if (a != '=')
            {
                fprintf(stderr, "unexpected symbol: !");
                exit(1);
            }
            else
            {
                return make_token(nidentity, "!=");
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
                return make_token(lt, "<");
            }
            else
            {
                return make_token(lte, "<=");
            }
            break;

        case mt0:
            a = getchar();
            if (a != '=')
            {
                ungetc(a, stdin);
                return make_token(mt, ">");
            }
            else
            {
                return make_token(mte, ">=");
            }
            break;

        case mins:
            a = getchar();
            if (a != '>')
            {
                ungetc(a, stdin);
                return make_token(minus, "-");
            }
            else
            {
                return make_token(arrow, "->");
            }
            break;
        }
    }
}
