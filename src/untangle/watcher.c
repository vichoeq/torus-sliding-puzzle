#include "watcher.h"
#include <stdio.h>
#include <stdlib.h>

#define WATCHER "./beholder"

static FILE* watcher = NULL;

/** Initializes the watcher, the graphic interface module */
void watcher_init()
{
	if(watcher) watcher_close();

	watcher = popen(WATCHER, "w");
}

/** Imprime un archivo .pdf con el contenido actual de la ventana */
void watcher_snapshot(char* filename)
{
	/* Intenta imprimir solo si el watcher está abierto */
	if(watcher)
	{
		if(fprintf(watcher, "S %s\n",filename) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Reads the color palette and sends it to the watcher */
void watcher_read_color_palette(void* stream)
{
	int channel;

	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			fscanf(stdin, "%d", &channel) ? : abort();
			/* Intenta imprimir solo si el watcher está abierto */
			if(watcher)
			{
				if(fprintf(watcher, "%d ", channel) < 0)
				{
					watcher_close();
				}
				else
				{
					fflush(watcher);
				}
			}
		}
		/* Intenta imprimir solo si el watcher está abierto */
		if(watcher)
		{
			if(fprintf(watcher, "\n") < 0)
			{
				watcher_close();
			}
			else
			{
				fflush(watcher);
			}
		}
	}
}

/** Sets the initial state for the watcher's puzzle */
void watcher_set_initial_state(Puzzle* puz)
{
	/* Intenta imprimir solo si el watcher está abierto */
	if(watcher)
	{
		if(puzzle_print(puz, watcher) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Applies an operation on the watcher's puzzle */
void watcher_apply_operation(Operation op)
{
	/* Intenta imprimir solo si el watcher está abierto */
	if(watcher)
	{
		if(operation_print(op, watcher) < 0)
		{
			watcher_close();
		}
		else
		{
			fflush(watcher);
		}
	}
}

/** Releases the resources used by the watcher */
void watcher_close()
{
	/* Intenta imprimir solo si el watcher está abierto */
	if(watcher)
  {
    if(fprintf(watcher, "%c\n", 'X') >= 0)
    {
      fflush(watcher);
    }
		pclose(watcher);
    watcher = NULL;
  }
}
