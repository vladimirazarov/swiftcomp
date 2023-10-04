#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    SymbolTable* table = initSymbolTable();

    Token varToken;
    varToken.type = identifier;
    varToken.value = strdup("int");

    Symbol varA;
    varA.name = "a";
    varA.data = &varToken;
    varA.is_defined = false;
    varA.scope = LOCAL;
    varA.modifier = VARIABLE;
    varA.functionInfo = NULL;  

    insertSymbol(table, varA);

    Token funcToken;
    funcToken.type = identifier;
    funcToken.value = strdup("void");

    Symbol funcB;
    funcB.name = "funcB";
    funcB.data = &funcToken;
    funcB.is_defined = true;
    funcB.scope = GLOBAL;
    funcB.modifier = CONSTANT;

    FunctionInfo funcInfo;
    funcInfo.func_pos = 0;
    funcInfo.func_body = 0;
    funcInfo.func_end = 0;
    funcInfo.parameterCount = 2;
    funcInfo.returnType = "int";

    Parameter param1 = {"param1", "int", NULL};
    Parameter param2 = {"param2", "float", NULL};
    param1.next = &param2;  

    funcInfo.parameters = &param1;
    funcB.functionInfo = &funcInfo;

    insertSymbol(table, funcB);

    printf("Contents of the Symbol Table:\n");
    printf("-------------------------------\n");

    Symbol* symbol = findSymbol(table, "a");
    if (symbol) {
        printf("Name: %s, Type: %s, Defined: %s\n", symbol->name, symbol->data->value, symbol->is_defined ? "Yes" : "No");
    }

    symbol = findSymbol(table, "funcB");
    if (symbol && symbol->functionInfo) {
        printf("Function Name: %s, Return Type: %s, Parameter Count: %d\n",
               symbol->name, symbol->functionInfo->returnType, symbol->functionInfo->parameterCount);
    }

    freeSymbolTable(table);

    free(varToken.value);
    free(funcToken.value);

    return 0;
}