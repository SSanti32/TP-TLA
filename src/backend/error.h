#ifndef ERROR_H
#define ERROR_H

void error_no_memory();
void error_multiple_declaration(const char *name);
void error_variable_not_found(const char *name);
void error_dangling_variable_found();
void error_invalid_separator_in_file_decalration_bad_type();
void error_invalid_separator_in_file_decalration_bad_len(const char * sep);
void error_function_not_found(const char *name);
void error_invalid_range(const char *from, const char *to);
void error_invalid_function_arguments();
void error_in_function(const char *name);
void error_invalid_file_variable_name(const char *name);
void error_invalid_node_file_handler(const char *function);
void error_invalid_byIndex_argument();
void error_invalid_multiplication_type();
void error_loop_iterable_of_invalid_type();

#endif
