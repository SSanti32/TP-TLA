#include <stdbool.h> /* bool type */
#include <stdlib.h> /* malloc, calloc */
#include <string.h> /* strlen, strcpy */

/* Backend */
#include "../../backend/logger.h" /* LogDebug */

/* Frontend */
#include "../lexical-analysis/flex-actions.h" /* enum TokenID; free_yylval */

/* This file */
#include "bison-actions.h"

/* Prototypes */
// static bool const_to_expr_cp(const char* value,
//                              node_expression* expression);

/**
 * Implementación de "bison-grammar.h".
 */

void yyerror(const char *string)
{
        const int length = strlen(yytext);

        LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext,
                 yylineno);
        LogError("Mensaje: '%s.", string);
        LogError("En ASCII es:");
        LogErrorRaw("\t");

        for (int i = 0; i < length; ++i) {
                LogErrorRaw("[%d]", yytext[i]);
        }

        LogErrorRaw("\n\n");
}

program* grammar_program(const node_function *value)
{
        LogDebug("%s(%p)\n", __func__, value);
        program* p = calloc(1,sizeof(program));
        p->value = calloc(1,sizeof(node_function));
        p->value = value;
        state.succeed = true;
        
        return p;
}

node_function *grammar_new_function(const node_expression *expr)
{
        LogDebug("%s(%p)\n", __func__, expr);

        node_function * newFuncNode = calloc(1,sizeof(node_function));
        newFuncNode->expr = calloc(1,sizeof(node_expression));
        newFuncNode->expr = expr;

        return newFuncNode;
}

node_expression *grammar_expression_from_list(const list *list)
{
        LogDebug("%s(%p)\n", __func__, list);
        node_expression * newExprList = calloc(1, sizeof(node_expression));
        if(list != NULL){
                node_list *iter = list->head;
                newExprList->listExpr = calloc(1, sizeof(list));
                node_list *retIter = newExprList->listExpr->head;
                newExprList->listExpr->head = iter->elem;
                newExprList->listExpr->type = iter->type;
                while(iter!=NULL){
                        if(iter!=COMMA){
                                retIter->next->elem = iter->elem;
                                retIter->next->type = iter->type;
                                iter = list->head->next;
                        }
                        retIter->next = newExprList->listExpr->next->next;
                }
                retIter->next = NULL;
        } else {
                return NULL;
        }

        return newExprList;
}

node_expression *
grammar_expression_from_funcall(const node_function_call *fun_list)
{
        LogDebug("%s(%p, %p)\n", __func__, fun_list);

        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        
        node->var->type = node->g_boolean_type;
        
        return node;
}

node_expression *
grammar_expression_from_filehandler(const node_file_block *fhandler)
{
        LogDebug("%s(%p)\n", __func__, fhandler);

        node_expression * fileExpr = calloc(1,sizeof(node_expression));
        fileExpr->fileHandler = fhandler;
        
        return fileExpr;
}

node_expression *
grammar_expression_from_assignment(const node_expression *assignment)
{
        LogDebug("%s(%p)\n", __func__, assignment);


        return assignment;
}

node_function_call *
grammar_concat_function_call(const node_function_call *fun_list,
                             const node_function_call *fun_append)
{
        LogDebug("%s(%p, %p)\n", __func__, fun_append);

        node_function_call* node = calloc(1, sizeof(node_function_call));
        node->function_call = fun_list;
        node->function_append = fun_append;

        return node;
}

node_function_call *
grammar_function_call(const node_expression *fun_id,
                        const node_list *args)
{
        LogDebug("%s(%p, %p)\n", __func__, fun_id, args);

        node_function_call* node = calloc(1, sizeof(node_function_call));
        node->id = fun_id;
        node->node_list = args;

        return node;
}

node_function_call *
grammar_function_call_from_id(const node_expression *id,
                                const node_function_call *fun)
{
        LogDebug("%s(%p, %p)\n", __func__, id, fun);

        node_function_call* node = calloc(1, sizeof(node_function_call));
        node->id = id;
        node->function_call = fun;

        return node;
}

node_expression *
grammar_new_declaration_file_node(const variable *fpath,
                                const node_expression *id,
                                const node_list *separators)
{
        LogDebug("%s(%p, %p, %p)callocfunc__, fpath, id, separators");

        node_expression* node = calloc(1,sizeof(node_expression));
        if (separators == NULL)
        {
                node->f_declaration_type = FILE_STRING_TYPE;
        } else {
                node->f_declaration_type = FILE_FSTREAM_STDOUT_TYPE;
        }
        
        node->node_list = separators;
        node->var = fpath;       

        return node;
}

node_expression *
grammar_new_declaration_stdout_node(const node_expression *id,
                                    const node_list *separators)
{
        LogDebug("%s(%p, %p)\n", __func__, id, separators);

        node_expression* node = calloc(1,sizeof(node_expression));
        if (separators == NULL)
        {
                node->f_declaration_type = STDOUT_STRING_TYPE;
        } else {
                node->f_declaration_type = STDOUT_FSTREAM_STDOUT_TYPE;
        }
        node->node_list = separators;

        return node;
}

node_file_block *
grammar_using_file(const node_expression *id,
                const node_expression *expr)
{
        LogDebug("%s(%p, %p)\n", __func__, id, expr);

        node_file_block* node_file_block = calloc(1, sizeof(node_file_block));
        // node_file_block->id = calloc(1,sizeof(node_file_block));
        // node_file_block->expr = calloc(1,sizeof(node_file_block));
        node_file_block->id = id;
        node_file_block->expr = expr;

        return NULL;
}

node_expression *
grammar_new_variable(const node_expression *id,
                        const node_expression *expr)
{
        LogDebug("%s(%p, %p)\n", __func__, id, expr);

        node_expression* node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->var = expr->var;
        node->var->type = id->var->type;

        return node;
}

conditional_node *
grammar_new_conditional(const node_expression *condition,
                        const node_expression *expr_true,
                        const node_expression *expr_false)
{
        LogDebug("%s(%p, %p, %p)\n", __func__, condition, expr_true, expr_false);
        
        conditional_node* node = calloc(1, sizeof(conditional_node));
        node->condition = calloc(1,sizeof(node_expression));
        node->condition = condition;
        node->expr_true = calloc(1,sizeof(node_expression));
        node->expr_true = expr_true;
        node->expr_false = calloc(1,sizeof(node_expression));
        node->expr_false = expr_false;
        
        return node;
}

loop_node *
grammar_new_loop(const node_expression *id,
                const node_expression *iterable,
                const node_expression *action)
{
        LogDebug("%s(%p, %p, %p)\n", __func__, id, iterable, action);

        loop_node * loop = calloc(1,sizeof(loop_node));
        loop->id = calloc(1,sizeof(node_expression));
        loop->iterable = calloc(1,sizeof(node_expression));
        loop->action = calloc(1,sizeof(node_expression));

        return loop;
}

node_list *
grammar_new_list(const node_expression *expr)
{
        LogDebug("%s(%p)\n", __func__, expr);
        /* 
        list * list = calloc(1,sizeof(list));
        list->head = calloc(1,sizeof(node_list));
        list->head->elem = calloc(1,sizeof(node_expression));

        list->head->elem = expr;
        if(expr != NULL) {
                list->head->elem->var->type = BLANK_TYPE;
                list->head->next = NULL;
        }
        list->head->next = NULL; */

        node_list* lst = (node_list*) malloc(sizeof(node_list));

        if (expr == NULL) {
                lst->exprs = NULL;
                lst->len = 0;
                lst->type = LIST_BLANK_TYPE;
        } else {
                lst->len = 1;
                lst->type = LIST_EXPRESSION_TYPE;

                lst->exprs = (node_expression**) malloc(sizeof(node_expression*));
                lst->exprs[0] = expr;
        }

        return lst; 
        /*
        Devuelve l->{elem (expr), next-> NULL}
        */
}

node_list *
grammar_concat_list_expr(const node_list *list,
                        const node_expression *expr)
{
        LogDebug("%s(%p, %p)\n", __func__, list, expr);

        /* list* new_list = calloc(1,sizeof(node_list));
        new_list->type = LIST_COMMA_EXPRESSION_TYPE;
        new_list->head = calloc(1,sizeof(node_list));
        new_list->head = list;
        node_list* new_next = calloc(1,sizeof(node_list));
        new_next->head = expr;
        new_next->next = NULL; */
        
        node_expression ** expanded = (node_expression **) realloc(list->exprs, (list->len + 1) * sizeof(node_expression*));
        expanded[list->len] = expr;

        list->exprs = expanded;
        list->len += 1;

        return list;

}

node_list *
recursive_list_add(const variable_value new_var,
                const node_list* first)
{
        if (first == NULL)
        {
                node_list* new = calloc(1,sizeof(node_list));
                new->type = LIST_RANGE_TYPE;
                new->elem = calloc(1,sizeof(node_expression));
                new->elem->var = calloc(1,sizeof(variable));
                new->elem->var->value = new_var;
                // agragar bien los types mas abajo en los nodes
                return new;
        }
        first->next = recursive_list_add(new_var, first->next);
        return first;

}


node_list *
grammar_concat_list_list(const node_list *head_list,
                        const node_list *tail_list)
{
        LogDebug("%s(%p, %p)\n", __func__, head_list, tail_list);

        /* list* new_list = calloc(1,sizeof(list));
        new_list->type = LIST_COMMA_RANGE_TYPE;
        // new_list->head = head_list->head;
        node_list* j = head_list->head;

        while (j->next != NULL) {
                new_list->head = recursive_list_add(j->elem->var->value, new_list->head);
                j->next = j->next->next;
        }

        j->next = tail_list->head;
        while (j->next != NULL) {
                new_list->head = recursive_list_add(j->elem->var->value, new_list->head);
                j->next = j->next->next;
        }

        // node_list* i = tail_list->head;
        // while (i->next != NULL) {
        //         i->next = i->next->next;
        // } */

        node_list * concat = realloc(head_list, head_list->len + tail_list->len);

        for (size_t i = 0, size_t last_head = head_list->len; i < tail_list->len; i++) {
                concat->exprs[last_head + i] = tail_list->exprs[i];
        }

        head_list->len += tail_list->len;
        head_list = concat;
        return concat;
        // return grammar_new_list_from_range(head_list, tail_list);
}

node_list *
grammar_new_list_from_range(const variable *start,
                        const variable *end)
{
        LogDebug("%s(%p, %p)\n", __func__, start, end);

        /* // node_expression* first = calloc(1,sizeof(node_expression));
        // first->var = calloc(1,sizeof(variable));
        // first->var = *start;

        list * to_return = (list*) calloc(1,sizeof(list));
        variable * i = start;
        variable * until = end;
        // to_return->head = recursive_list_add(i->value, to_return->head);
        while (i->value < until->value) {
                to_return->head = recursive_list_add(i->value, to_return->head);
                (i->value)++;
        }
        
        to_return->type = LIST_RANGE_TYPE; */

        if (start->type != TYPE_NUMBER || end->type != TYPE_NUMBER) {
                LogError("Invalid type for range.\n");
                return NULL;
        }

        const long long vstart = start->value.number;
        const long long vend = end->value.number;
        const long long diff = (vend > vstart) ? vend - vstart : vstart - vend;

        node_list * lst = (node_list*) malloc(sizeof(node_list));
        lst->exprs = (node_expression**) malloc(diff * sizeof(node_expression*));

        for(long long i = 0; i < diff; i++) {
                node_expression* expr = (node_expression*) malloc(sizeof(node_expression));
                variable * var = (variable*) malloc(sizeof(variable));

                var->type = TYPE_NUMBER;
                var->value.number = i;

                expr->var = var;
                expr->type = NUMBER_TYPE;

                lst->exprs[i] = expr;
        }

        list->type = LIST_RANGE_TYPE;
        list->len = diff;

        return to_return;
}

node_list *
grammar_new_list_args(const node_expression *expr)
{
        LogDebug("%s(%p)\n", __func__, expr);

        /*node_list * list = (node_list *) calloc(1,sizeof(node_list));
        if (expr == NULL) {
                list->elem = calloc(1,sizeof(node_expression));
                // list->head->var = calloc(1,sizeof(variable));

              list->type = BLANK_TYPE;
                // list->head->var = expr;
        } else {
                list->type = EXPRESSION_TYPE;
        }
        list->elem = expr;
        list->next = NULL;*/

        node_list * lst = (node_list*) malloc(sizeof(node_list));
        
        if (expr == NULL)
                lst->exprs = NULL;
                list->type = BLANK_TYPE;
                lst->len = 0;
        else {
                lst->exprs = (node_expression**) malloc(sizeof(node_expression*));

                lst->exprs[0] = expr;
                lst->len = 1;
                lst->type = EXPRESSION_TYPE;
        }
        return lst;
}

node_list*
grammar_concat_list_args(const node_list *list,
                        const node_expression *expr)
{
        LogDebug("%s(%p, %p)\n", __func__, list, expr);

        if (expr == NULL)
                return list;

        list->exprs = (node_expression**) realloc(list->exprs, (1 + list->len) * sizeof(node_expression*));

        list->exprs[list->len] = expr;
        list->len += 1;
        list->type = EXPRESSION_TYPE;


        /*
        node_list * new_list = (node_list *) calloc(1,sizeof(node_list));
        new_list->head = list;
        new_list->head->var->type = COMMA_TYPE;
        new_list->head->next = expr;
        list->head->next->next = NULL;
        */

        return list;
}

node_expression *
grammar_expression_arithmetic_num_add(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->var->type = NUMBER_ARITHMETIC_ADD;
        strcpy(node->var->name, "NUMBER_ARITHMETIC_ADD");
        node->var->value.number = lvalue->var->value.number + rvalue->var->value.number;

        return (node_expression *) node;
}

node_expression *
grammar_expression_arithmetic_num_sub(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->var->type = NUMBER_ARITHMETIC_SUB;
        strcpy(node->var->number->name, "NUMBER_ARITH->numberMETIC_SUB");
        node->var->value = lvalue->var->value.number - rvalue->var->value.number;

        return (node_expression *) node;
}

node_expression *
grammar_expression_arithmetic_num_mul(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->var->type = NUMBER_ARITHMETIC_MUL;
        strcpy(node->var->name, "NUMBER_ARITHMETIC_MUL");
        node->var->value.number = lvalue->var->value.number * rvalue->var->value.number;
 
        return node;
}

node_expression *
grammar_expression_arithmetic_num_div(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->var->type = NUMBER_ARITHMETIC_DIV;
        strcpy(node->var->name, "NUMBER_ARITHMETIC_DIV");
        node->var->value.number = lvalue->var->value.number / rvalue->var->value.number;

        return node;
}

node_expression *
grammar_expression_arithmetic_num_mod(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->var->type = NUMBER_ARITHMETIC_MOD;
        strcpy(node->var->name, "NUMBER_ARITHMETIC_MOD");
        node->var->value.number = (double) ((long long) lvalue->var->value.number) % ((long long) rvalue->var->value.number);
 
        return node;
}

node_expression *
grammar_expression_arithmetic_str_add(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        const size_t a = strlen(lvalue->var->value.string);
        const size_t b = strlen(rvalue->var->value.string);
        const size_t size_ab = a + b + 1;
        
        node_expression * concat = (node_expression *) malloc(sizeof(char*) * size_ab);

        memcpy(concat, lvalue->value.string, a);
        memcpy(concat + a, rvalue->value.string, b + 1);

        concat->var->type = ARITHMETIC_ADD;

        return (node_expression *) concat;
}

node_expression *
grammar_expression_arithmetic_str_sub(const node_expression *lvalue,
                                      const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        char* *p, *q, *r;
        char* str = lvalue->var->value.string;
        char* sub = rvalue->var->value.string;
        if (*sub && (q = r = strstr(str, sub)) != NULL) {
                size_t len = strlen(sub);
                while ((r = strstr(p = r + len, sub)) != NULL) {
                        memmove(q, p, r - p);
                        q += r - p;
                }
                memmove(q, p, strlen(p) + 1);
        }

        lvalue->var->type = ARITHMETIC_SUB;

        return (node_expression *) lvalue;
}

node_expression *
grammar_expression_bool_and(const node_expression *lvalue,
                            const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->g_boolean_type = BOOL_TYPE_AND;
        node->var = (variable *) calloc(1,sizeof(variable));
        
        node->var->value.boolean = (lvalue->var->value.boolean && rvalue->var->value.boolean) ? true : false;

        node->var->type = node->g_boolean_type;

        return node;
}

node_expression *
grammar_expression_bool_or(const node_expression *lvalue,
                        const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = (node_expression *) calloc(1,sizeof(node_expression));
        node->g_boolean_type = BOOL_TYPE_OR;
        node->var = (variable *) calloc(1,sizeof(variable));
        
        node->var->value.boolean = (lvalue->var->value.boolean || rvalue->var->value.boolean) ? true : false;

        node->var->type = node->g_boolean_type;

        return node;
}

node_expression *
grammar_expression_bool_not(const node_expression *value)
{        
        LogDebug("%s(%p)\n", __func__, value);

        node_expression * node = calloc(1,sizeof(node_expression));
        node->g_boolean_type = BOOL_TYPE_NOT;
        node->var = (variable *) calloc(sizeof(variable));
        
        node->var->value.boolean = ! value->var->value.boolean;

        node->var->type = node->g_boolean_type;

        return node;

}

node_expression *
grammar_expression_cmp_equals(const node_expression *lvalue,
                                const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);


        node_expression * node = calloc(1,sizeof(node_expression));
        node->g_expression_cmp_type = CMP_EXP_EQUALS;
        node->var = calloc(1,sizeof(variable));
        node->var->type = node->g_expression_cmp_type;
        switch(node->var->type) {
                case TYPE_NUMBER:
                        if (lvalue->var->value.number == rvalue->var->value.number) {
                                node->var->value.boolean = true;
                        } else{
                                node->var->value.boolean = false;
                        }
                        break;
                case TYPE_STRING:
                        if (strcmp(lvalue->var->value.string, rvalue->var->value.string) == 0) {
                                node->var->value.boolean = true;
                        } else {
                                node->var->value.boolean = false;
                        }
                        break;
                default:
                        break;
        }
        
        return node;
}

node_expression *
grammar_expression_cmp_not_equals(const node_expression *lvalue,
                                  const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);
        

        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->g_expression_cmp_type = CMP_EXP_NOT_EQUALS;
        node->var->type = node->g_expression_cmp_type;

        switch(node->var->type) {
                case TYPE_NUMBER:
                        if (lvalue->var->value.number != rvalue->var->value.number) {
                                node->var->value.boolean = true;
                        } else{
                                node->var->value.boolean = false;
                        }
                        break;
                case TYPE_STRING:
                        if (! strcmp(lvalue->var->value.string, rvalue->var->value.string) == 0) {
                                node->var->value.boolean = true;
                        } else {
                                node->var->value.boolean = false;
                        }
                        break;
                default:
                        break;
        }

        return node;
}

node_expression *
grammar_expression_cmp_greater_than(const node_expression *lvalue,
                                    const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->g_expression_cmp_type = CMP_EXP_GREATER_THAN;
        node->var->type = node->g_expression_cmp_type;
        switch(node->var->type) {
                case TYPE_NUMBER:
                        if (lvalue->var->value.number > rvalue->var->value.number) {
                                node->var->value.boolean = true;
                        } else{
                                node->var->value.boolean = false;
                        }
                        break;
                case TYPE_STRING:
                        if (strcmp(lvalue->var->value.string, rvalue->var->value.string) > 0) {
                                node->var->value.boolean = true;
                        } else {
                                node->var->value.boolean = false;
                        }
                        break;
                default:
                        break;
        }
        return node;
}

node_expression *
grammar_expression_cmp_greater_equal(const node_expression *lvalue,
                                     const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->g_expression_cmp_type = CMP_EXP_GREATER_EQUAL;
        node->var->type = node->g_expression_cmp_type;
        switch(node->var->type) {
                case TYPE_NUMBER:
                        if (lvalue->var->value.number >= rvalue->var->value.number) {
                                node->var->value.boolean = true;
                        } else{
                                node->var->value.boolean = false;
                        }
                        break;
                case TYPE_STRING:
                        if (strcmp(lvalue->var->value.string, rvalue->var->value.string) >= 0) {
                                node->var->value.boolean = true;
                        } else {
                                node->var->value.boolean = false;
                        }
                        break;
                default:
                        break;
        }
        return node;
}

node_expression *
grammar_expression_cmp_less_than(const node_expression *lvalue,
                                        const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->g_expression_cmp_type = CMP_EXP_LESS_THAN;
        node->var->type = node->g_expression_cmp_type;
        
        return node;
}

node_expression *
grammar_expression_cmp_less_equal(const node_expression *lvalue,
                                  const node_expression *rvalue)
{
        LogDebug("%s(%p, %p)\n", __func__, lvalue, rvalue);

        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable_value));
        node->g_expression_cmp_type = CMP_EXP_LESS_EQUAL;
        node->var->type = node->g_expression_cmp_type;
        switch(node->var->type) {
                case TYPE_NUMBER:
                        if (lvalue->var->value.number <= rvalue->var->value.number){
                                node->var->value.boolean = true;
                        } else{
                                node->var->value.boolean = false;
                        }
                        break;
                case TYPE_STRING:
                        if (strcmp(lvalue->var->value.string, rvalue->var->value.string) <= 0) {
                                node->var->value.boolean = true;
                        } else {
                                node->var->value.boolean = false;
                        }
                        break;
                default:
                        break;
        }
        
        return node;
}

node_expression *
grammar_expression_cmp_by_type(const node_expression *expr,
                                const token_t type)
{
        LogDebug("%s(%p, %d)\n", __func__, expr, type);
        node_expression * node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable_value));
        node->g_expression_cmp_type = CMP_EXP_IS;
        node->var->type = node->g_expression_cmp_type;
        node->var->value.boolean = (expr->var->type == type) ? true : false;

        return node;
}

node_expression *
grammar_expression_from_funcall(node_function_call* fn_calls) {
        
        LogDebug("%s(%p)\n", __func__, fn_calls);
        
        node_expression* node = (node_expression*) calloc(1, sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->function_call_pointer = fn_calls;
        
        return node;
}

node_expression *
grammar_new_return_node(const node_expression *id)
{
        LogDebug("%s(%p)\n", __func__, id);

        node_expression * node = calloc(1,sizeof(node_expression));
        if(id != NULL){
                node->var = calloc(1,sizeof(variable));
                node->var->name = calloc(1,strlen(id->var->name)+1);
                strcpy(node->var->name, id->var->name);
                node->var->type = id->var->type;
                node->var->value = id->var->value;
                
                node->type = RETURN__ID;
        } else {
                node->var = calloc(1,sizeof(variable));
                char * name = "RETURN_NO_ID";
                node->var->name = calloc(1,strlen(name)+1);
                strcpy(node->var->name, name);

                node->type = RETURN_NO_ID;
        }

        return node;
}


node_expression *
grammar_new_assignment_expression(const node_expression *expr,
                                  const node_expression *id)
{
        LogDebug("%s(%p, %p)\n", __func__, expr, id);
        expr->var->type = id->var->type;
        expr->type = id->type;
        strcpy(expr->var->name, id->var->name);

        return expr;
}

node_expression *
grammar_identifier(const variable_value * id)
{
        LogDebug("%s(%p)\n", __func__, id);

        node_expression* node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->var->type = id;
        node->var->name = calloc(1,strlen(id)+1);
        strcpy(node->var->name, id);

        node->type = -1;

        return node;
}

node_expression *
grammar_constant_bool(const char *value)
{
        LogDebug("%s(%s)\n", __func__, value);

        node_expression * newNode = calloc(1,sizeof(node_expression));
        newNode->var = calloc(1, sizeof(variable));
        newNode->var->type = BOOL_TYPE;
        if(strcmp(value, "TRUE") == 0) {
                newNode->var->value = (bool) true;
        } else {
                newNode->var->value = (bool) false;
        }
        char * name = "BOOL_TYPE";
        newNode->var->name = calloc(1,strlen(name)+1);
        strcpy(newNode->var->name, name);

        newNode->type = BOOL_TYPE;

        return newNode;
}

node_expression *
grammar_constant_number(const char *value)
{
        LogDebug("%s(%s)\n", __func__, value);

        node_expression* node = calloc(1,sizeof(node_expression));
        node->var = calloc(1,sizeof(variable));
        node->var->value = atoi(value);
        node->var->type = NUMBER_TYPE;
        char * name = "NUMBER_TYPE";
        node->var->name = calloc(1,strlen(name)+1);
        strcpy(node->var->name, name);

        node->type = NUMBER_TYPE;

        return node;
}

node_expression *
grammar_constant_string(const char *value)
{
        LogDebug("%s(%s)\n", __func__, value);
        
        node_expression * newNode = calloc(1,sizeof(node_expression));
        newNode->var = calloc(1,sizeof(variable));
        newNode->var->type = STRING_TYPE;
        char * name = "STRING_TYPE";
        newNode->var->name = calloc(1,strlen(name)+1);
        strcpy(newNode->var->name, name);
        newNode->var->value = (variable_value) malloc(strlen(value)+1);
        strcpy(newNode->var->value.string, value);

        newNode->type = STRING_TYPE;

        return newNode;
}

node_expression *
grammar_concat_expressions_filehandler(const node_expression *exprs,
                                       const node_file_block *fhandler)
{
        LogDebug("%s(%p, %p)\n", __func__, exprs, fhandler);

        return NULL;
}

node_expression *
grammar_new_assignment_identifier(const node_expression *from_id,
                                  const node_expression *to_id)
{
        LogDebug("%s(%p, %p)\n", __func__, from_id, to_id);

        return NULL;
}

/*
void
free_programblock(struct node_block* program)
{
        free_yylval();

        if (program == NULL)
                return;

        free(program->exp);
        free(program);
}

static bool const_to_expr_cp(const char* value, node_expression* expression)
{
        size_t len = strlen(value);

        expression->value = calloc(sizeof(char), len);
        if (expression->value == NULL) {
                log_error_no_mem();
                return false;
        }

        expression->length = len;
        strcpy(expression->value, value);
        return true;
}
*/
