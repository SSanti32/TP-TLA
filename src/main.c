// #include "backend/code-generation/generator.h"
#include <stdio.h>
#include <stdlib.h>

#include "backend/error.h"
#include "backend/logger.h"
#include "backend/shared.h"
#include "backend/mem_management.h"

#include "frontend/syntactic-analysis/bison-actions.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include "backend/symbols.h"
#include "backend/code-generator.h"

//Estado de la aplicación.
CompilerState state;

extern void free_yylval();

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char **arguments)
{
        // Inicializar estado de la aplicación.
        state.result = 0;
        state.succeed = false;

        // Mostrar parámetros recibidos por consola.
        for (int i = 0; i < argumentCount; ++i) {
                LogInfo("Argumento %d: '%s'", i, arguments[i]);
        }

        program_t *root = new_program();

        // Compilar el programa de entrada.
        LogInfo("Compilando...\n");
        const int result = yyparse(root);

        switch (result) {
        case COMPILER_STATE_RESULTS_FINISHED:
                if (state.succeed) {
                        LogInfo("La compilación fue exitosa.");
                        // generate_code(root);
                } else {
                        LogError("Se produjo un error en la aplicacion.");
                        return -1;
                }
                break;
        case COMPILER_STATE_RESULTS_SYNTAX_ERROR:
                LogError("Bison finalizó debido a un error de sintaxis.");
                break;
        case COMPILER_STATE_RESULTS_DANGLING_VARIABLES:
                error_dangling_variable_found();
                break;
        case COMPILER_STATE_RESULTS_OUT_OF_MEMORY:
                LogError(
                        "Bison finalizó abruptamente debido a que ya no hay memoria disponible.");
                break;
        default:
                LogError(
                        "Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).",
                        result);
        }
        LogInfo("Fin.");

        free_program(root);

        return result;
}
