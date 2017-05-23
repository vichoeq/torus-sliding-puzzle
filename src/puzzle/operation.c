#include "operation.h"
#include "puzzle.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* Los primeros dos bytes representan la operación en sí */
#define OpR 0b00000000 //00 : 0
#define OpU 0b01000000 //01 : 1
#define OpD 0b10000000 //10 : 2
#define OpL 0b11000000 //11 : 3
/* Los otros 6 son el indice de la fila / columna de la operación */

/* L es inverso de R, U es inverso de D. Al hacer XOR con 11000000 sale eso */
#define inverse(op) ((op) ^ 0b11000000)
/* La operación son sólo los primeros 2 bits, asique queremos esos 2 bits */
#define operation(op) (((op) & 0b11000000) >> 6)
/* El parametro son los últimos 6 bits, asi que queremos sólo esos 6 bits */
#define parameter(op) ((op) & 0b00111111)

/** Códigos corespondientes a las 4 operaciones */
static const char codes[4] =
{
	'R',
	'U',
	'D',
	'L'
};

/** Funciones de desplazamiento para el puzzle */
static const shift_fn_t functions[4] =
{
	puzzle_shift_right, //0 : 00
	puzzle_shift_up,    //1 : 01
	puzzle_shift_down,  //2 : 10
	puzzle_shift_left   //3 : 11
};

/** Crea una nueva operación segun su tipo e índice*/
static Operation operation_create(char code, uint8_t index)
{
	/* Asegurarse de que el indice cabe en 6 bits */
	assert(index < 0b01000000);
	/* Une ambos datos en un solo byte */
	switch(code)
	{
		case 'R': return OpR | index;
		case 'U': return OpU | index;
		case 'L': return OpL | index;
		case 'D': return OpD | index;
		default: fprintf(stderr, "Invalid op type: %c\n", code); abort();
	}
}

/** Crea de una sola vez todas las operaciones con un símbolo dado */
static int mass_produce(char code, uint8_t limit, Operation* ops, int count)
{
	for(uint8_t index = 0; index < limit; index++)
	{
		ops[count++] = operation_create(code, index);
	}
	return count;
}

/** Genera todas las operaciones válidas para un puzzle */
Operation* operation_generate_all(Puzzle* puz)
{
	Operation* ops = calloc(2*puz -> height + 2*puz -> width, sizeof(uint8_t));

	int count = 0;
	count = mass_produce('R', puz -> height, ops, count);
	count = mass_produce('U', puz -> width,  ops, count);
	count = mass_produce('D', puz -> width,  ops, count);
	count = mass_produce('L', puz -> height, ops, count);

	return ops;
}

/** Ejecuta la operación sobre el puzzle, cambiando su estado */
void operation_execute(Puzzle* puz, Operation op)
{
	functions[operation(op)](puz, parameter(op));
}

/** Invierte la operación sobre el puzzle, volviendo al estado anterior */
void operation_revert(Puzzle* puz, Operation op)
{
	operation_execute(puz, inverse(op));
}

/** Obtiene la letra correspondiente al tipo de operación */
char operation_type(Operation op)
{
	return codes[operation(op)];
}

/** Imprime la operacion en un formato legible en el canal especificado */
int operation_print(Operation op, void* stream)
{
	return fprintf(stream, "%c %d\n", operation_type(op), parameter(op));
}

/** Obtiene el índice sobre el cual se efectua la operación */
uint8_t operation_index(Operation op)
{
	return parameter(op);
}

/** Obtiene la dirección de la operación: 'L', 'R', 'U' o 'D' */
char operation_direction(Operation op)
{
	return operation_type(op);
}
