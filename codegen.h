#ifndef CODEGEN_H
#define CODEGEN_H

#define LAST_RETURN_VALUE "$$LAST_RETURN_VALUE"

#define FUNCTION_READSTRING                                                  \
    "\n# Built-in function readString"                                       \
    "\nLABEL $readString"                                                    \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nREAD LF@%retval string"                                               \
    "\nPOPFRAME"                                                             \
    "\nRETURN"

#define FUNCTION_READINT                                                     \
    "\n# Built-in function readInt"                                          \
    "\nLABEL $readInt"                                                       \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nREAD LF@%retval int"                                                  \
    "\nDEFVAR LF@intCheck"                                                   \
    "\nTYPE LF@intCheck LF@%retval"                                          \
    "\nJUMPIFNEQ $readInt$fail LF@intCheck string@int"                      \
    "\nPOPFRAME"                                                             \
    "\nRETURN"                                                               \
    "\nLABEL $readInt$fail"                                                  \
    "\nMOVE LF@%retval nil@nil"                                              \
    "\nPOPFRAME"                                                             \
    "\nRETURN"

#define FUNCTION_READDOUBLE                                                  \
    "\n# Built-in function readDouble"                                       \
    "\nLABEL $readDouble"                                                    \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nREAD LF@%retval float"                                                \
    "\nDEFVAR LF@floatCheck"                                                 \
    "\nTYPE LF@floatCheck LF@%retval"                                        \
    "\nJUMPIFNEQ $readDouble$fail LF@floatCheck string@float"               \
    "\nPOPFRAME"                                                             \
    "\nRETURN"                                                               \
    "\nLABEL $readDouble$fail"                                               \
    "\nMOVE LF@%retval nil@nil"                                              \
    "\nPOPFRAME"                                                             \
    "\nRETURN"

#define FUNCTION_WRITE \
"LABEL $write\n" \
"DEFVAR LF@arg_count\n" \
"DEFVAR LF@temp_arg\n" \
"DEFVAR LF@temp_arg_type\n" \
"POPS LF@arg_count\n" \
"LABEL $write_loop\n" \
"JUMPIFEQ $write_end LF@arg_count int@0\n" \
"SUB LF@arg_count LF@arg_count int@1\n" \
"POPS LF@temp_arg\n" \
"TYPE LF@temp_arg_type LF@temp_arg\n" \
"JUMPIFEQ $write_int LF@temp_arg_type string@int\n" \
"JUMPIFEQ $write_string LF@temp_arg_type string@string\n" \
"JUMPIFEQ $write_float LF@temp_arg_type string@float\n" \
"JUMP $write_loop\n" \
"LABEL $write_int\n" \
"WRITE LF@temp_arg\n" \
"JUMP $write_loop\n" \
"LABEL $write_string\n" \
"WRITE LF@temp_arg\n" \
"JUMP $write_loop\n" \
"LABEL $write_float\n" \
"WRITE LF@temp_arg\n" \
"JUMP $write_loop\n" \
"LABEL $write_end\n" \
"RETURN\n"

#define FUNCTION_INT2DOUBLE                                                  \
    "\n# Built-in function Int2Double"                                       \
    "\nLABEL $int2double"                                                    \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nMOVE LF@%retval double@0x0p+0"                                        \
    "\nDEFVAR LF@%term"                                                      \
    "\nMOVE LF@%term LF@%0"                                                  \
    "\nTYPE LF@%type LF@%term"                                               \
    "\nJUMPIFEQ $int2double$error LF@%type string@int"   

#define FUNCTION_DOUBLE2INT                                                  \
    "\n# Built-in function Double2Int"                                       \
    "\nLABEL $double2int"                                                    \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nMOVE LF@%retval int@0"                                                \
    "\nDEFVAR LF@%term"                                                      \
    "\nMOVE LF@%term LF@%0"                                                  \
    "\nTYPE LF@%type LF@%term"                                               \
    "\nJUMPIFEQ $double2int$error LF@%type string@double"                    \
    "\nFLOAT2INT LF@%retval LF@%term"                                        \
    "\nLABEL $double2int$return"                                             \
    "\nPOPFRAME"                                                             \
    "\nRETURN"                                                               \
    "\nLABEL $double2int$error"                                              \
    "\nEXIT int@4"                                                           \
    "\nRETURN"

#define FUNCTION_LENGTH                                                      \
    "\n# Built-in function length"                                           \
    "\nLABEL $length"                                                        \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nMOVE LF@%retval int@0"                                                \
    "\nDEFVAR LF@%s"                                                         \
    "\nMOVE LF@%s LF@%0"                                                     \
    "\nTYPE LF@%type LF@%s"                                                  \
    "\nJUMPIFEQ $length$error LF@%type string@string"                        \
    "\nSTRLEN LF@%retval LF@%s"                                              \
    "\nLABEL $length$return"                                                 \
    "\nPOPFRAME"                                                             \
    "\nRETURN"                                                               \
    "\nLABEL $length$error"                                                  \
    "\nEXIT int@4"                                                           \
    "\nRETURN"

#define FUNCTION_SUBSTRING                                                   \
    "\n# Built-in function substring"                                        \
    "\nLABEL $substring"                                                     \
    "\nPUSHFRAME"                                                            \
    "\nDEFVAR LF@%retval"                                                    \
    "\nMOVE LF@%retval nil@nil"                                              \
    "\nDEFVAR LF@%s"                                                         \
    "\nMOVE LF@%s LF@%0"                                                     \
    "\nDEFVAR LF@%i"                                                         \
    "\nMOVE LF@%i LF@%1"                                                     \
    "\nDEFVAR LF@%j"                                                         \
    "\nMOVE LF@%j LF@%2"                                                     \
    "\nDEFVAR LF@%length"                                                    \
    "\nCREATEFRAME"                                                          \
    "\nDEFVAR TF@%0"                                                         \
    "\nMOVE TF@%0 LF@%s"                                                     \
    "\nCALL $length"                                                         \
    "\nMOVE LF@%length TF@%retval"                                           \
    "\nLT LF@%cond LF@%i int@0"                                              \
    "\nJUMPIFEQ $substring$return LF@%cond bool@true"                        \
    "\nLT LF@%cond LF@%j int@0"                                              \
    "\nJUMPIFEQ $substring$return LF@%cond bool@true"                        \
    "\nGE LF@%cond LF@%i LF@%j"                                              \
    "\nJUMPIFEQ $substring$return LF@%cond bool@true"                        \
    "\nGE LF@%cond LF@%i LF@%length"                                         \
    "\nJUMPIFEQ $substring$return LF@%cond bool@true"                        \
    "\nGT LF@%cond LF@%j LF@%length"                                         \
    "\nJUMPIFEQ $substring$return LF@%cond bool@true"                        \
    "\nDEFVAR LF@%substr"                                                    \
    "\nMOVE LF@%substr string@\"\""                                          \
    "\nLABEL $substring$loop"                                                \
    "\nLT LF@%cond LF@%i LF@%j"                                              \
    "\nJUMPIFEQ $substring$end LF@%cond bool@false"                          \
    "\nGETCHAR LF@%char LF@%s LF@%i"                                         \
    "\nCONCAT LF@%substr LF@%substr LF@%char"                                \
    "\nADD LF@%i LF@%i int@1"                                                \
    "\nJUMP $substring$loop"                                                 \
    "\nLABEL $substring$end"                                                 \
    "\nMOVE LF@%retval LF@%substr"                                           \
    "\nLABEL $substring$return"                                              \
    "\nPOPFRAME"                                                             \
    "\nRETURN"


#define FUNCTION_ORD                                                          \
    "\n# Built-in function ord"                                               \
    "\nLABEL $ord"                                                            \
    "\nPUSHFRAME"                                                             \
    "\nDEFVAR LF@%retval"                                                     \
    "\nMOVE LF@%retval int@0"                                                 \
    "\nDEFVAR LF@%s"                                                          \
    "\nMOVE LF@%s LF@%0"                                                      \
    "\nDEFVAR LF@%length"                                                     \
    "\nSTRLEN LF@%length LF@%s"                                               \
    "\nJUMPIFEQ $ord$empty LF@%length int@0"                                  \
    "\nDEFVAR LF@%char"                                                       \
    "\nSTRI2INT LF@%char LF@%s int@0"                                         \
    "\nMOVE LF@%retval LF@%char"                                              \
    "\nLABEL $ord$empty"                                                      \
    "\nPOPFRAME"                                                              \
    "\nRETURN"

#define FUNCTION_CHR                                                          \
    "\n# Built-in function chr"                                               \
    "\nLABEL $chr"                                                            \
    "\nPUSHFRAME"                                                             \
    "\nDEFVAR LF@%retval"                                                     \
    "\nMOVE LF@%retval string@\"\""                                           \
    "\nDEFVAR LF@%i"                                                          \
    "\nMOVE LF@%i LF@%0"                                                      \
    "\nDEFVAR LF@%valid_range"                                                \
    "\nLT LF@%valid_range LF@%i int@0"                                        \
    "\nGT LF@%valid_range LF@%i int@255"                                      \
    "\nOR LF@%valid_range LF@%valid_range LF@%valid_range"                    \
    "\nJUMPIFEQ $chr$invalid_range LF@%valid_range bool@true"                 \
    "\nINT2CHAR LF@%retval LF@%i"                                             \
    "\nLABEL $chr$invalid_range"                                              \
    "\nEXIT int@4"                                                            \
    "\nLABEL $chr$end"                                                        \
    "\nPOPFRAME"                                                              \
    "\nRETURN"


void print_gf();
void print_lf();
void print_tf();
void print_int();
void print_float();
void print_string();
void print_nil();

void print_assignment(const char *destination_prefix,const char *destination_suffix, const char *prefix, const char *suffix);

// Function to print variable declaration instructions
void print_declaration(const char *frame, const char *name);

void print_jump_to_function_end(const char* prefix, const char* function_name);
void print_label_function_end(const char* frame, const char* function_name);


void print_add_int(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, int op1_suffix, const char* op2_prefix, int op2_suffix);

// Function to print label creation instructions
void print_label(const char *frame, const char *name);

void print_pushs_size_t(const char* prefix, const int suffix);
void print_call(const char *prefix, const char *suffix);
// Function to print an unconditional jump instruction
void print_jump(const char *prefix, const char *suffix);
// Function to print a conditional jump instruction based on equality
void print_jumpifeq(const char *label, const char *op1, const char *op2);

// Function to print a conditional jump instruction based on inequality
void print_jumpifneq(const char *label, const char *op1, const char *op2);
void print_return();

// Function to print the addition of two numbers
void print_add(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix);

// Function to print the subtraction of two numbers
void print_sub(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix);

void print_div(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix);

// Function to print the multiplication of two numbers
void print_mul(const char* destination_prefix, const char* destination_suffix, const char* op1_prefix, const char* op1_suffix, const char* op2_prefix, const char* op2_suffix);

// Function to create a new temporary frame
void print_createframe();

// Function to push a temporary frame onto the frame stack
void print_pushframe();

// Function to pop the current frame from the frame stack
void print_popframe();

// Function to push a value onto the data stack
void print_pushs(const char *prefix, const char *sufix);

// Function to pop a value from the data stack
void print_pops(const char *prefix, const char *sufix);


// Function to clear the data stack
void print_clears();

// Function to perform an integer division
void print_idiv(const char *destination, const char *op1, const char *op2);

// Function to compare if one value is less than another
void print_lt(const char *destination, const char *op1, const char *op2);
// Function to compare if one value is greater than another
void print_gt(const char *destination, const char *op1, const char *op2);

// Function to compare if two values are equal
void print_eq(const char *destination, const char *op1, const char *op2);

// Function to perform a logical AND operation
void print_and(const char *destination, const char *op1, const char *op2);

// Function to perform a logical OR operation
void print_or(const char *destination, const char *op1, const char *op2);

// Function to perform a logical NOT operation
void print_not(const char *destination, const char *operand);

// Function to convert an integer to a float
void print_int2float(const char *destination, const char *operand);
// Function to convert a float to an integer
void print_float2int(const char *destination, const char *operand);

// Function to convert an integer to a character
void print_int2char(const char *destination, const char *operand);

// Function to get the ordinal value of a character in a string
void print_stri2int(const char *destination, const char *string, const char *index);

// Function to read a value from standard input
void print_read(const char *variable, const char *type);

// Function to write a value to standard output
void print_write(const char *symbol);
// Function to concatenate two strings
void print_concat(const char *destination, const char *string1, const char *string2);
// Function to get the length of a string
void print_strlen(const char *destination, const char *string);

// Function to get a character from a string
void print_getchar(const char *destination, const char *string, const char *index);
// Function to set a character in a string
void print_setchar(const char *destination, const char *index, const char *character);

#endif // CODEGEN_H