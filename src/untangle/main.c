#include <stdio.h>
#include <stdlib.h>
#include "watcher.h"
#include <unistd.h>
#include "search/search.h"
#include <time.h>

int main(int argc, char** argv)
{
	watcher_init();

	watcher_read_color_palette(stdin);

	Puzzle* solution = puzzle_from_stream(stdin);
	Puzzle* initial = puzzle_from_stream(stdin);

	watcher_set_initial_state(initial);

	clock_t start = clock();

	Stack* stack = search(initial, solution);

	printf("Solved in %lf seconds\n", (double)(clock() - start) / CLOCKS_PER_SEC);

	stack_watch(stack);

	stack_print(stack, stdout);

	stack_destroy(stack);

	puzzle_destroy(solution);
	puzzle_destroy(initial);

	watcher_close();

	return 0;
}
