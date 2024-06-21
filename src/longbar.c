#include <gtk-layer-shell.h>
#include <gtk/gtk.h>

#include "longbar.h"
#include "modules/clock-module.h"

LongBar *longbar_new(GtkApplication *app) {
  LongBar *result = g_new(LongBar, 1);

  GtkWindow *gtk_window = GTK_WINDOW(gtk_application_window_new(app));

  gtk_layer_init_for_window(gtk_window);

  // The bar goes below everything else.
  gtk_layer_set_layer(gtk_window, GTK_LAYER_SHELL_LAYER_BOTTOM);

  // Please don't put other windows in this area.
  gtk_layer_auto_exclusive_zone_enable(gtk_window);

  // Cover the entire bottom.
  gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_LEFT, 0);
  gtk_layer_set_margin(gtk_window, GTK_LAYER_SHELL_EDGE_RIGHT, 0);

  // Anchor this bar to the left, right, and bottom of the screen.
  // Anchoring to the bottom is optional. However, anchoring to the sides is
  // not: Failing to do so is a protocol error (kills the client).
  static const gboolean anchors[] = {TRUE, TRUE, FALSE, TRUE};
  for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
    gtk_layer_set_anchor(gtk_window, i, anchors[i]);
  }

  result->window = gtk_window;
  return result;
}

void longbar_load_modules(LongBar *bar) {
  GtkWidget *layout_box = gtk_box_new(0, 0);

  clock_module_new(layout_box);

  gtk_container_add(GTK_CONTAINER(bar->window), layout_box);
  gtk_widget_show_all(GTK_WIDGET(bar->window));
}
