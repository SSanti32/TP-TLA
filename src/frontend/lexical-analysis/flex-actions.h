#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/logger.h"
#include "../../backend/shared.h"

/**
 * Se definen los diferentes IDs de cada token disponible para el scanner Flex.
 */
typedef enum TokenID {
    // Por defecto, el valor "0" hace fallar el analizador sintáctico.
    UNKNOWN = 0,

    // Código de error de Bison, que permite abortar el escaneo de lexemas cuando
    // se presente un patrón desconocido. El número "257" coincide con el valor
    // que Bison le otorga por defecto, pero además permite que el resto de
    // tokens continúen desde el valor "258" lo que permite proteger los IDs
    // internos que Bison reserva para crear "tokens literales":
    YYUNDEF = 257,

    /* Pipe operator */
    PIPE,

    /* Assignment */
    ASSIGN,

    /* Logic */
    OR,
    AND,
    NOT,

    /* Relational*/
    EQUALS,
    NOT_EQUALS,

    GREATER_THAN,
    GREATER_EQUAL,
    LESS_EQUAL,
    LESS_THAN,

    /* Arithmetic (numeric) */
    ADD,
    SUB,
    MOD,
    MUL,
    DIV,

    /* Arithmetic (string) */
    STR_SUB,
    STR_ADD,

    /* Brackets */
    OPEN_BRACKETS,
    CLOSE_BRACKETS,

    /* Parentheses */
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,

    /* Data types */
    NUMBER,
    STRING,
    FILE_TYPE,
} TokenID;

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 */

TokenID StringPatternAction(const char * lexeme);

TokenID NumberPatternAction(const char * lexeme);

void IgnoredPatternAction(const char * lexeme);

TokenID UnknownPatternAction(const char * lexeme);

#endif
