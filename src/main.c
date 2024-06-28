#include <gtk-layer-shell.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "dbus-dispatch.h"
#include "longbar.h"

static void activate(GtkApplication *app) {
  Longbar *bar = longbar_new(app);

  long_register_for_dbus(bar);

  longbar_load_modules(bar);
}

int main(int argc, char **argv) {
  GtkApplication *app = gtk_application_new("com.fascinatedbox.longbar",
                                            G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
