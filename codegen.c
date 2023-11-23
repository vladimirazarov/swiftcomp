#include "codegen.h"

#include <stdio.h>



void print_gf()
{
printf("GF@");
}
void print_lf()
{
printf("LF@");
}
void print_tf()
{
printf("TF@");
}
void print_int()
{
printf("int@");
}
void print_float()
{
printf("float@");
}
void print_string()
{
printf("string@");
}
void print_nil()
{
printf("string@nil");
}


// Function to print variable assignment instructions
void print_assignment(const char* destination_prefix, const char* destination_suffix, const char* source_prefix, const char* source_suffix) {
    printf("MOVE %s%s %s%s\n", destination_prefix, destination_suffix, source_prefix, source_suffix);
}

// Function to print variable declaration instructions
void print_declaration(const char* frame, const char *name) {
    printf("DEFVAR %s%s\n", frame, name);
}

// Function to print label creation instructions
void print_label(const char* frame, const char* name) {
    printf("LABEL %s%s\n", frame, name);
}

void print_label_function_end(const char* frame, const char* function_name) {
    printf("LABEL %s%s_end\n", frame, function_name);
}
// Function to print an unconditional jump instruction
void print_jump(const char* prefix, const char* suffix) {
    printf("JUMP %s%s\n", prefix, suffix);
}

// Function to print an unconditional jump instruction
void print_jump_to_function_end(const char* prefix, const char* function_name) {
    printf("JUMP %s%s_end\n", prefix, function_name);
}

// Function to print a conditional jump instruction based on equality
void print_jumpifeq(const char* label, const char* op1, const char* op2) {
    printf("JUMPIFEQ %s %s %s\n", label, op1, op2);
}

// Function to print a conditional jump instruction based on inequality
void print_jumpifneq(const char* label, const char* op1, const char* op2) {
    printf("JUMPIFNEQ %s %s %s\n", label, op1, op2);
}

// Function to print the addition of two numbers
void print_add(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix) {
    printf("ADD %s%s %s%s %s%s\n", destination_prefix, destination_suffix, op1_prefix, op1_suffix, op2_prefix, op2_suffix);
}
void print_add_int(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, int op1_suffix, const char* op2_prefix, int op2_suffix) {
    printf("ADD %s%s %s%d %s%d\n", destination_prefix, destination_suffix, op1_prefix, op1_suffix, op2_prefix, op2_suffix);
}

// Function to print the subtraction of two numbers
void print_sub(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix) {
    printf("SUB %s%s %s%s %s%s\n", destination_prefix, destination_suffix, op1_prefix, op1_suffix, op2_prefix, op2_suffix);
}

// Function to print the multiplication of two numbers
void print_mul(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix) {
    printf("MUL %s%s %s%s %s%s\n", destination_prefix, destination_suffix, op1_prefix, op1_suffix, op2_prefix, op2_suffix);
}

// Function to print the division of two numbers
void print_div(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix) {
    printf("DIV %s%s %s%s %s%s\n", destination_prefix, destination_suffix, op1_prefix, op1_suffix, op2_prefix, op2_suffix);
}

// Function to create a new temporary frame
void print_createframe() {
    printf("CREATEFRAME\n");
}

// Function to push a temporary frame onto the frame stack
void print_pushframe() {
    printf("PUSHFRAME\n");
}

// Function to pop the current frame from the frame stack
void print_popframe() {
    printf("POPFRAME\n");
}

// Function to push a value onto the data stack
void print_pushs(const char* prefix, const char* suffix) {
    printf("PUSHS %s%s\n", prefix, suffix);
}
void print_pushs_size_t(const char* prefix, const int suffix) {
    printf("PUSHS %s%d\n", prefix, suffix);
}

void print_call(const char* prefix, const char* suffix) {
    printf("CALL %s%s\n", prefix, suffix);
}

// Function to pop a value from the data stack
void print_pops(const char* prefix, const char *suffix) {
    printf("POPS %s%s\n", prefix, suffix);
}

// Function to clear the data stack
void print_clears() {
    printf("CLEARS\n");
}

// Function to perform an integer division
void print_idiv(const char* destination, const char* op1, const char* op2) {
    printf("IDIV %s %s %s\n", destination, op1, op2);
}

// Function to compare if one value is less than another
void print_lt(const char* destination, const char* op1, const char* op2) {
    printf("LT %s %s %s\n", destination, op1, op2);
}

// Function to compare if one value is greater than another
void print_gt(const char* destination, const char* op1, const char* op2) {
    printf("GT %s %s %s\n", destination, op1, op2);
}

// Function to compare if two values are equal
void print_eq(const char* destination, const char* op1, const char* op2) {
    printf("EQ %s %s %s\n", destination, op1, op2);
}

// Function to perform a logical AND operation
void print_and(const char* destination, const char* op1, const char* op2) {
    printf("AND %s %s %s\n", destination, op1, op2);
}

// Function to perform a logical OR operation
void print_or(const char* destination, const char* op1, const char* op2) {
    printf("OR %s %s %s\n", destination, op1, op2);
}

// Function to perform a logical NOT operation
void print_not(const char* destination, const char* operand) {
    printf("NOT %s %s\n", destination, operand);
}

// Function to convert an integer to a float
void print_int2float(const char* destination, const char* operand) {
    printf("INT2FLOAT %s %s\n", destination, operand);
}

// Function to convert a float to an integer
void print_float2int(const char* destination, const char* operand) {
    printf("FLOAT2INT %s %s\n", destination, operand);
}

// Function to convert an integer to a character
void print_int2char(const char* destination, const char* operand) {
    printf("INT2CHAR %s %s\n", destination, operand);
}

// Function to get the ordinal value of a character in a string
void print_stri2int(const char* destination, const char* string, const char* index) {
    printf("STRI2INT %s %s %s\n", destination, string, index);
}

// Function to read a value from standard input
void print_read(const char* variable, const char* type) {
    printf("READ %s %s\n", variable, type);
}

// Function to write a value to standard output
void print_write(const char* symbol) {
    printf("WRITE %s\n", symbol);
}

// Function to concatenate two strings
void print_concat(const char* destination, const char* string1, const char* string2) {
    printf("CONCAT %s %s %s\n", destination, string1, string2);
}

// Function to get the length of a string
void print_strlen(const char* destination, const char* string) {
    printf("STRLEN %s %s\n", destination, string);
}

// Function to get a character from a string
void print_getchar(const char* destination, const char* string, const char* index) {
    printf("GETCHAR %s %s %s\n", destination, string, index);
}

// Function to set a character in a string
void print_setchar(const char* destination, const char* index, const char* character) {
    printf("SETCHAR %s %s %s\n", destination, index, character);
}
void print_return(){
    printf("RETURN\n");
}
