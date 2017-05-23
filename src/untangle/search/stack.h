#pragma once

#include "../../puzzle/operation.h"

/** Represents a doubly-linked node for a dynamic allocating stack */
struct operation_node;

/** Represents a doubly-linked node for a dynamic allocating stack */
typedef struct operation_node Node;

/** Represents a doubly-linked node for a lazy allocating stack */
struct operation_node
{
	/** Whether the node is empty */
	bool empty;
	/** The operation stored in this node */
	Operation op;
	/** Previous node on the list */
	Node* prev;
	/** Next node on the list */
	Node* next;
};

/** Represents a doubly-linked list to use as a lazy allocating stack */
struct operation_list
{
	/** First node with something stored */
	Node* head;
	/** First empty node */
	Node* tail;
	/** Last empty node */
	Node* last;
	/** Amount of elements in this stack */
	// int count;
};

typedef struct operation_list Stack;

/** Initializes a node-based stack */
Stack*    stack_init     ();
/** Inserts an operation at the end of the stack */
void      stack_push     (Stack* stack, Operation op);
/** Removes the operation at the end of the stack */
Operation stack_pop      (Stack* stack);
/** Visualizes the operations on the watcher */
void      stack_watch    (Stack* stack);
/** Prints the operation list */
void      stack_print    (Stack* stack, void* stream);
/** Indicates whether the stack is empty */
bool      stack_is_empty (Stack* stack);
/** Counts the amount of elements in the stack */
int       stack_count    (Stack* stack);
/** Frees the resources used up by the stack */
void      stack_destroy  (Stack* stack);
