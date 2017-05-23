#pragma once

#include "../puzzle/operation.h"

/** Initializes the watcher, the graphic interface module */
void watcher_init               ();
/** Imprime un archivo .pdf con el contenido actual de la ventana */
void watcher_snapshot           (char* filename);
/** Reads the color palette and sends it to the watcher */
void watcher_read_color_palette (void* stream);
/** Sets the initial state for the watcher's puzzle */
void watcher_set_initial_state  (Puzzle* puz);
/** Applies an operation on the watcher's puzzle */
void watcher_apply_operation    (Operation op);
/** Releases the resources used by the watcher */
void watcher_close              ();
