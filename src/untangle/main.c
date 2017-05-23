#include <stdio.h>
#include <stdlib.h>
#include "watcher.h"
#include <unistd.h>
#include "search/search.h"
#include <time.h>
#include <string.h>

int main(int argc, char** argv)
{
	/* Si se recibe el flag opcional -s entonces el watcher no se abre */
	if(argc == 2)
	{
		if(strcmp(argv[1], "-s"))
		{
			abort();
		}
	}
	else
	{
		watcher_init();
	}

	/* Leer la paleta de colores y enviarla al watcher */
	watcher_read_color_palette(stdin);

	/* Leer los estados relevantes del problema */
	Puzzle* solution = puzzle_from_stream(stdin);
	Puzzle* initial = puzzle_from_stream(stdin);

	/* Cargarlo a la ventana */
	watcher_set_initial_state(initial);

	/* Resuelve el problema  */
	clock_t start = clock();
	Stack* stack = search(initial, solution);
	double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
	fprintf(stderr, "Solved in %lf seconds\n", elapsed);

	/* Anima la solución */
	stack_watch(stack);

	/* Imprime el resultado */
	FILE* f = fopen("output.txt", "w");
	fprintf(f, "%d\n", stack_count(stack));
	stack_print(stack, f);
	fclose(f);

	/* Libera memoria */
	stack_destroy(stack);
	puzzle_destroy(solution);
	puzzle_destroy(initial);

	/* Cierra la ventana */
	watcher_close();

	return 0;
}
