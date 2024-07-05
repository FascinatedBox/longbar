#include <gtk-layer-shell.h>
#include <gtk/gtk.h>

#include "longbar.h"
#include "modules/clock-module.h"
#include "modules/taskbar-module.h"

Longbar *longbar_new(GtkApplication *app) {
  Longbar *result = g_new(Longbar, 1);
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

  result->app = app;
  result->window = gtk_window;
  return result;
}

void longbar_set_height(Longbar *longbar, int height) {
  GtkWindow *w = longbar->window;

  // GTK complains if it's zero.
  if (height == 0)
    height = 1;

  // There's no call to set just the height, so set both. The width portion
  // should be ignored, because he bar is anchored to the left and right edges.
  gtk_widget_set_size_request(GTK_WIDGET(w), height, height);
  gtk_window_resize(w, height, height);
}

void longbar_load_modules(Longbar *bar) {
  GtkWidget *layout_box = gtk_box_new(0, 0);
  clock_module_new(layout_box);
  taskbar_module_new(layout_box);
  gtk_container_add(GTK_CONTAINER(bar->window), layout_box);
  gtk_widget_show_all(GTK_WIDGET(bar->window));
}
