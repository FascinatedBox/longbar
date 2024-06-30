#include <gdk/gdkx.h>
#include <gtk-layer-shell.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "dbus-dispatch.h"
#include "longbar.h"

static void ensure_wayland_display(void) {
  GdkDisplay *display = gdk_display_get_default();

  // GDK_IS_WAYLAND_DISPLAY generates warnings if given an X11 display, but the
  // reverse is not true, so use GDK_IS_X11_DISPLAY instead.
  if (GDK_IS_X11_DISPLAY(display)) {
    fputs("longbar: Cannot connect to a Wayland display. Stopping.\n", stderr);
    exit(EXIT_FAILURE);
  }
}

static void activate(GtkApplication *app) {
  ensure_wayland_display();
  Longbar *bar = longbar_new(app);
  long_register_for_dbus(bar);
  longbar_load_modules(bar);
}

int main(int argc, char **argv) {
  // If X11 is not included as a fallback, the user will see 'cannot open
  // display :0.0'. By adding it as a fallback, a more useful error message can
  // be written later.
  gdk_set_allowed_backends("wayland,x11");
  GtkApplication *app = gtk_application_new("com.fascinatedbox.longbar",
                                            G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
