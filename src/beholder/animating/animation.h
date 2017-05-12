#pragma once

#include <gtk/gtk.h>

/** Inicializa la rutina encargada de actualizar el tablero */
void animation_init(GtkWidget* canvas, gpointer user_data);
/** Aborta la rutina que actualiza el tablero */
void animation_abort();
