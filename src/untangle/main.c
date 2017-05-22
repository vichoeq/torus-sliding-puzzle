#include <stdio.h>
#include <stdlib.h>
#include "watcher.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	watcher_init();

	watcher_read_color_palette(stdin);

	Puzzle* solution = puzzle_from_stream(stdin);
	Puzzle* initial = puzzle_from_stream(stdin);

	watcher_set_initial_state(initial);

	Operation* ops = operation_generate_all(initial);

	for(int i = 0; i < 2 * initial -> height + 2* initial -> width; i++)
	{
		watcher_apply_operation(ops[i]);
	}

	free(ops);

	puzzle_destroy(solution);
	puzzle_destroy(initial);

	watcher_close();

	return 0;
}
