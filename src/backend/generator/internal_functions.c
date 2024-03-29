#include <stdio.h>

static void generate_internal_function_open_file(FILE *const output);
static void generate_internal_function_copy_buffer_content(FILE *const output);
static void generate_internal_function_copy_file_content(FILE *const output);
static void
generate_internal_function_get_list_of_files_in_dir(FILE *const output);
static void generate_internal_function_string_addition(FILE *const output);
static void generate_internal_function_string_substract(FILE *const output);
void generate_internal_function_compare_equality(FILE *const output);
void generate_internal_function_get_next_file(FILE *const output);
static void generate_internal_function_toString(FILE *const output);
static void generate_internal_function_at(FILE *const output);

extern void generate_allocation_error_msg(FILE *const output, char *ptr_name);

void generate_internal_functions(FILE *const output)
{
        generate_internal_function_open_file(output);
        generate_internal_function_copy_buffer_content(output);
        generate_internal_function_copy_file_content(output);
        generate_internal_function_get_list_of_files_in_dir(output);
        generate_internal_function_string_addition(output);
        generate_internal_function_string_substract(output);
        generate_internal_function_compare_equality(output);
        generate_internal_function_get_next_file(output);
        generate_internal_function_toString(output);
        generate_internal_function_at(output);
}

void generate_internal_functions_headers(FILE *const output)
{
        fprintf(output, "bool open_file(const char *name, const char *mode,"
                        "TexlerObject *tex_obj, const char *separators);");
        fprintf(output, "void copy_buffer_content(char *from, FILE *to);");
        fprintf(output, "void copy_file_content(FILE *from, FILE *to);");
        fprintf(output, "void copy_file_content_texler("
                        "TexlerObject *source,"
                        "TexlerObject *destination"
                        ");");
        fprintf(output, "long get_list_of_files_in_dir("
                        "char ***files, const char *path"
                        ");");
        fprintf(output, "char *string_substract(char *str1, char *str2);");
        fprintf(output, "char *string_addition(char *str1, char *str2);");
        fprintf(output, "bool compare_equality("
                        "TexlerObject *left, TexlerObject *right);");
        fprintf(output, "bool "
                        "compare_equality_constant_number_int(long left,"
                        "TexlerObject *right);");
        fprintf(output, "bool "
                        "compare_equality_constant_string(char* left,"
                        "TexlerObject *right);");
        fprintf(output, "TexlerObject *"
                        "get_next_file(TexlerObject *tex_obj, "
                        "const char* separators);");
        fprintf(output, "char *"
                        "toString(TexlerObject *tex_obj);");
        fprintf(output, "int "
                        "at(char *str, long pos);");
}

static void generate_internal_function_at(FILE *const output)
{
        fprintf(output, 
        "int at(char *str, long pos)"
        "{"
                "if (pos < 1 || pos > strlen(str)) {"
                        "fprintf(stderr, "
                        "\"\\nError al querer pasar un string con longitud erronea\\n\""
                        ");"
                        "return -1;"
                "} else {"
                        "return str[pos - 1];"
                "}"
        "}");
}


static void generate_internal_function_toString(FILE *const output)
{
        fprintf(output,
                "char *toString(TexlerObject *tex_obj)"
                "{"
                "char *to_return = calloc(1, BUFFER_SIZE * sizeof(char *));"
                "switch (tex_obj->type) {"
                "case TYPE_T_BOOLEAN:"
                "if (tex_obj->value.boolean) {"
                "to_return = strdup(\"True\");"
                "} else {"
                "to_return = strdup(\"False\");"
                "}"
                "break;"
                "case TYPE_T_REAL:"
                "to_return = strdup(\"\");"
                "sprintf(to_return, \"%%f\", tex_obj->value.real);"
                "break;"
                "case TYPE_T_INTEGER:"
                "to_return = strdup(\"\""
                ");"
                "sprintf(to_return, \"%%ld\", tex_obj->value.integer);"
                "break;"
                "case TYPE_T_STRING:"
                "return tex_obj->value.string;"
                "break;"
                "default:"
                "to_return = NULL;"
                "fprintf(stderr,"
                "\"\\nError al querer pasar a string"
                " algo que no es una variable, solo variables de tipo numerico, \""
                "\"booleanas o strings.\\n\");"
                "break;"
                "}"
                "return to_return;"
                "}");
}

static void generate_internal_function_open_file(FILE *const output)
{
        fprintf(output,
                "bool open_file("
                "const char *name,"
                "const char *mode,"
                "TexlerObject *tex_obj,"
                "const char *separators)"
                "{"
                "if (name == NULL || mode == NULL || tex_obj == NULL)"
                "{"
                "return false;"
                "}"

                "size_t error_msg_len ="
                "strlen(\"Error while opening file ''\") + strlen(name);"
                "char *error_msg = "
                "(char *)calloc(1 + error_msg_len, sizeof(char));");

        generate_allocation_error_msg(output, "error_msg");

        fprintf(output,
                "strcpy(error_msg, \"Error while opening file '\");"
                "strcat(error_msg, name);"
                "strcat(error_msg, \"'\");"

                /*caso donde es una carpeta
                "char *is_directory =  strrchr(name, '/');"
                "if (is_directory != NULL) {"
                "tex_obj->type = TYPE_T_FILE_LIST;"
                "tex_obj->value.file.n_files = get_list_of_files_in_dir("
                "&tex_obj->value.file.path_list, name);"
                "tex_obj->value.file.separators = separators;"
                "return true;"
                "}"*/

                "FILE *fptr = fopen(name, mode);"
                "if (fptr == NULL) {"
                "perror(error_msg);"
                "free(error_msg);"
                "return false;"
                "}"
                "free(error_msg);"

                "rewind(fptr);"

                "tex_obj->type = TYPE_T_FILEPTR;"
                "tex_obj->value.file.stream = fptr;"
                "if ("
                "fgetpos(tex_obj->value.file.stream, "
                "&tex_obj->value.file.pos)"
                ")"
                "{"
                "perror(\"Error while getting file position\");"
                "return false;"
                "}"
                "tex_obj->value.file.n_line = 1;"

                "if (separators == NULL)"
                "{"
                "tex_obj->value.file.separators = strdup(DEFAULT_SEPARATORS);"
                "}"
                "else"
                "{"
                "tex_obj->value.file.separators = strdup(separators);"
                "}"

                "return true;"
                "}");
}

static void generate_internal_function_copy_buffer_content(FILE *const output)
{
        fprintf(output, "void copy_buffer_content(char *from, FILE *to)"
                        "{"
                        "if (from == NULL || to == NULL || "
                        "(void*)from == (void*)to)"
                        "{"
                        "return;"
                        "}"
                        "fputs(from, to);"
                        "}");
}

static void generate_internal_function_copy_file_content(FILE *const output)
{
        fprintf(output, "void copy_file_content(FILE *from, FILE *to)"
                        "{"
                        "if (from == NULL || to == NULL || "
                        "(void*)from == (void*)to)"
                        "{"
                        "return;"
                        "}"
                        "char buffer[BUFFER_SIZE] = { 0 };"
                        "rewind(from);"
                        "while (!feof(from)) {"
                        "if (fgets(buffer, BUFFER_SIZE, from) == NULL)"
                        "{"
                        "break;"
                        "}"
                        "fputs(buffer, to);"
                        "}"
                        "}");

        fprintf(output,
                "void copy_file_content_texler("
                "TexlerObject *source,"
                "TexlerObject *destination"
                ")"
                "{"
                "if ("
                "destination->value.file.stream == stdout"
                "||"
                "strcmp("
                "destination->value.file.separators,"
                "DEFAULT_SEPARATORS"
                ") == 0)"
                "{"
                "copy_file_content(source->value.file.stream,"
                "destination->value.file.stream);"
                "}"
                "else"
                "{"
                "long source_separators_len ="
                "strlen(source->value.file.separators);"
                "long destination_separators_len ="
                "strlen(destination->value.file.separators);"

                "long line_len = BUFFER_SIZE;"
                "char *line = (char *)calloc(line_len, sizeof(char));");
        generate_allocation_error_msg(output, "line");

        fprintf(output,
                "while (line_len > 0)"
                "{"
                "line_len = lines(source, &line);"
                "if (line_len <= 0 || line == NULL) "
                "{"
                "break;"
                "}"
                "char *remaining = line;"
                "long int col_len = BUFFER_SIZE;"
                "char *column = (char *)calloc(col_len, sizeof(char));");

        generate_allocation_error_msg(output, "column");

        fprintf(output, "while (remaining != NULL)"
                        "{"
                        "int separator_char = 0;"
                        "col_len ="
                        "columns("
                        "&remaining,"
                        "source->value.file.separators,"
                        "&column,"
                        "&separator_char"
                        ");"
                        "if (column != NULL && col_len > 0) {"
                        "copy_buffer_content("
                        "column,"
                        "destination->value.file.stream);"
                        "}"
                        "if ("
                        "separator_char"
                        "&&"
                        "source_separators_len == destination_separators_len"
                        ")"
                        "{"
                        "for (long i = 0; i < source_separators_len; i++)"
                        "{"
                        "if ("
                        "source->value.file.separators[i] == separator_char"
                        ")"
                        "{"
                        "fputc("
                        "destination->value.file.separators[i],"
                        "destination->value.file.stream"
                        ");"
                        "}"
                        "}"
                        "}"
                        "else if (separator_char)"
                        "{"
                        "fputc("
                        "destination->value.file.separators[0],"
                        "destination->value.file.stream"
                        ");"
                        "}"
                        "}"
                        "free(column);"
                        "}"
                        "free(line);"
                        "}"
                        "}");
}

static void
generate_internal_function_get_list_of_files_in_dir(FILE *const output)
{
        fprintf(output, "long get_list_of_files_in_dir("
                        "char ***files, const char *path"
                        ")"
                        "{"
                        "if (files == NULL)"
                        "{"
                        "return -1;"
                        "}"

                        "char **files_list = *files;"

                        "DIR *dir_ptr = NULL;"
                        "struct dirent *dir = NULL;"
                        "long count_files = 0;"

                        "dir_ptr = opendir(path);"

                        "if (dir_ptr)"
                        "{"
                        "while ((dir = readdir(dir_ptr)) != NULL)"
                        "{"
                        "if (dir->d_type == DT_REG)"
                        "{"
                        "count_files++;"
                        "files_list = "
                        "(char **)realloc("
                        "files_list,"
                        "count_files * sizeof(char *)"
                        ");"
                        "files_list[count_files - 1] ="
                        "(char *)calloc("
                        "strlen(path)"
                        "+ 1"
                        "+ strlen(dir->d_name)"
                        "+ 1"
                        ","
                        "sizeof(char));"
                        "sprintf("
                        "files_list[count_files - 1]"
                        ","
                        "\"%%s/%%s\""
                        ","
                        "path, dir->d_name"
                        ");"
                        "}"
                        "}"
                        "closedir(dir_ptr);"
                        "}"
                        "*files = files_list;"
                        "return count_files;"
                        "}");
}

static void generate_internal_function_string_addition(FILE *const output)
{
        fprintf(output,
                "/* Concat str2 at the end of str1 and store it in str1 */"
                "char *string_addition(char *str1, char *str2)"
                "{"
                "if (str2 == NULL)"
                "{"
                "return str1;"
                "}"
                "else if (str1 == NULL)"
                "{"
                "return str2;"
                "}"
                "int str1_len = strlen(str1);"
                "int str2_len = strlen(str2);"

                "if (str1_len > 1 && str1[str1_len - 1] == '\\n')"
                "{"
                "str1[str1_len - 1] = '\\0';"
                "}"

                "int aux_len = 1 + str1_len + str2_len;"
                "char *aux = (char *)realloc(str1, aux_len * sizeof(char));");

        generate_allocation_error_msg(output, "aux");

        fprintf(output, "strncat(aux, str2, aux_len);"
                        "aux[aux_len - 1] = '\\0';"
                        "str1 = aux;"
                        "return str1;"
                        "}");
}

static void generate_internal_function_string_substract(FILE *const output)
{
        fprintf(output,
                "char *string_substract(char *str1, char *str2)"
                "{"
                "int str1_len = strlen(str1);"
                "int str2_len = strlen(str2);"

                "if (str1_len < str2_len)"
                "{"
                "return str1;"
                "}"

                "int i = 0;"
                "int j = str2_len;"
                "while (str2[i] == str1[str1_len - j] && str2_len > i)"
                "{"
                "i++;"
                "j--;"
                "}"

                "if (str2[i] == str1[str1_len - j] && j < 1)"
                "{"
                "memset(str1 + str1_len - str2_len, 0, str2_len);"
                "int aux_len = 1 + strlen(str1);"
                "char *aux = (char *)realloc(str1, aux_len * sizeof(char));");
        generate_allocation_error_msg(output, "aux");
        fprintf(output, "aux[aux_len - 1] = '\\0';"
                        "str1 = aux;"
                        "}"
                        "return str1;"
                        "}");
}

void generate_internal_function_compare_equality(FILE *const output)
{
        fprintf(output,
                "bool "
                "compare_equality(TexlerObject *left, TexlerObject *right)"
                "{"
                "if ("
                "(left->type == TYPE_T_BOOLEAN"
                "||"
                "left->type == TYPE_T_INTEGER)"
                "&&"
                "(right->type == TYPE_T_BOOLEAN"
                "||"
                "right->type == TYPE_T_INTEGER)"
                ")"
                "{"
                "return (left->value.integer - right->value.integer) == 0;"
                "}"
                "else if ("
                "("
                "left->type == TYPE_T_REAL &&"
                "(right->type == TYPE_T_BOOLEAN ||"
                "right->type == TYPE_T_INTEGER ||"
                "right->type == TYPE_T_REAL)"
                ")"
                "||"
                "(right->type == TYPE_T_REAL &&"
                "(left->type == TYPE_T_BOOLEAN ||"
                "left->type == TYPE_T_INTEGER ||"
                "left->type == TYPE_T_REAL)"
                ")"
                ")"
                "{"
                "return fabs(left->value.real - right->value.real) < DBL_EPSILON;"
                "}"
                "else if"
                "(left->type == TYPE_T_STRING && right->type == TYPE_T_STRING)"
                "{"
                "return strcmp(left->value.string, right->value.string) == 0;"
                "}"

                "return false;"
                "}");

        fprintf(output,
                "bool "
                "compare_equality_constant_number_int(long left,"
                "TexlerObject *right)"
                "{"
                "if ("
                "right->type == TYPE_T_BOOLEAN"
                "||"
                "right->type == TYPE_T_INTEGER"
                ")"
                "{"
                "return (left - right->value.integer) == 0;"
                "}"

                "else if (right->type == TYPE_T_REAL)"
                "{"
                "return fabs((double) left - right->value.real) < DBL_EPSILON;"
                "}"

                "return false;"
                "}");

        fprintf(output, "bool "
                        "compare_equality_constant_string(char* left,"
                        "TexlerObject *right)"
                        "{"
                        "if ("
                        "right->type == TYPE_T_STRING"
                        ")"
                        "{"
                        "return strcmp(left, right->value.string) == 0;"
                        "}"

                        "return false;"
                        "}");
}

void generate_internal_function_get_next_file(FILE *const output)
{
        fprintf(output, "TexlerObject *"
                        "get_next_file(TexlerObject *tex_obj, "
                        "const char* separators){"
                        "TexlerObject *input_file = NULL;"

                        "if (tex_obj == NULL) {"
                        "return NULL;"
                        "}"
                        "if (tex_obj->type == TYPE_T_FILE_LIST) {"
                        "input_file = "
                        "(TexlerObject *)calloc(1, sizeof(TexlerObject));");

        generate_allocation_error_msg(output, "input_file");

        fprintf(output, "if (open_file("
                        "tex_obj->value.file.path_list["
                        "tex_obj->value.file.next_open_file"
                        "]"
                        ", \"r\", input_file,"
                        "separators) == false) {"
                        "free_texlerobject(input_file);"
                        "free_texlerobject(tex_obj);"
                        "return NULL;"
                        "}"
                        "tex_obj->value.file.next_open_file++;"
                        "}"
                        "else if (tex_obj->type == TYPE_T_FILEPTR)"
                        "{"
                        "input_file = tex_obj;"
                        "}"

                        "return input_file;"
                        "}");
}
