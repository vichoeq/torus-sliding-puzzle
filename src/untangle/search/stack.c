#include "stack.h"
#include <stdlib.h>
#include "../watcher.h"

/** Inicializa un stack de nodos */
Stack* stack_init()
{
	Stack* stack = malloc(sizeof(Stack));
	// stack -> count = 0;

	Node* empty = malloc(sizeof(Node));
	empty -> empty = true;
	empty -> next = NULL;
	empty -> prev = NULL;

	stack -> head = empty;
	stack -> tail = empty;
	stack -> last = empty;

	return stack;
}

/** Inserta una operación al final del stack */
void stack_push(Stack* stack, Operation op)
{
	stack -> tail -> op = op;
	stack -> tail -> empty = false;

	if(stack -> last == stack -> tail)
	{
		Node* new = malloc(sizeof(Node));

		new -> next = NULL;
		new -> empty = true;
		new -> prev = stack -> last;

		stack -> last -> next = new;

		stack -> last = new;
		stack -> tail = new;
	}
	else
	{
		stack -> tail = stack -> tail -> next;
	}

	// stack -> count++;
}
/** Saca la última operación insertada */
Operation stack_pop(Stack* stack)
{
	Operation op = stack -> tail -> prev -> op;

	stack -> tail = stack -> tail -> prev;
	stack -> tail -> empty = true;

	// stack -> count--;

	return op;
}

/** Indica si el stack está o no vacío */
bool stack_is_empty(Stack* stack)
{
	return stack -> head -> empty;
}

/** Comunica al watcher la lista de operaciones */
void stack_watch   (Stack* stack)
{
	for(Node* n = stack -> head; !n -> empty; n = n -> next)
	{
		watcher_apply_operation(n -> op);
	}
}
/** Imprime la lista de operaciones */
void stack_print   (Stack* stack, void* stream)
{
	for(Node* n = stack -> head; !n -> empty; n = n -> next)
	{
		operation_print(n -> op, stream);
	}
}

/** Libera recursivamente todos los nodos del stack */
static void node_destroy(Node* node)
{
	if(node -> next)
	{
		node_destroy(node -> next);
	}
	free(node);
}

/** Libera los recursos asociados al stack */
void stack_destroy (Stack* stack)
{
	node_destroy(stack -> head);
	free(stack);
}

/** Counts the amount of elements in the stack */
int stack_count(Stack* stack)
{
	int count = 0;
	for(Node* n = stack -> head; !n -> empty; n = n -> next)
	{
		count++;
	}
	return count;
}
