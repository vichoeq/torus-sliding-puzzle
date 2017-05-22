#pragma once

#include "puzzle.h"

/** Encapsula en un byte una operación de desplazamiento y su parámetro */
typedef uint8_t Operation;

/** Genera todas las operaciones válidas para un puzzle */
Operation* operation_generate_all (Puzzle* puz);
/** Ejecuta la operación sobre el puzzle, cambiando su estado */
void       operation_execute      (Puzzle* puz, Operation op);
/** Invierte la operación sobre el puzzle, volviendo al estado anterior */
void       operation_revert       (Puzzle* puz, Operation op);
/** Imprime la operacion en un formato legible en el canal especificado */
int        operation_print        (Operation op, void* stream);
