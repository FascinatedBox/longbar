#include <gtk/gtk.h>

#include "dbus-dispatch.h"
#include "dbus-interface.h"
#include "longbar.h"

static gboolean on_handle_set_height(LongbarDispatch *, GDBusMethodInvocation *,
                                     guint height, gpointer data) {
  Longbar *longbar = (Longbar *)data;
  longbar_set_height(longbar, height);
  return TRUE;
}

static gboolean on_handle_quit(LongbarDispatch *, GDBusMethodInvocation *,
                               gpointer data) {
  Longbar *longbar = (Longbar *)data;
  g_application_quit(G_APPLICATION(longbar->app));
  return TRUE;
}

static void on_name_acquired(GDBusConnection *connnection, const gchar *,
                             gpointer longbar) {
  LongbarDispatch *skeleton = longbar_dispatch_skeleton_new();
  GError *error = NULL;
  g_dbus_interface_skeleton_export(
      G_DBUS_INTERFACE_SKELETON(skeleton), connnection,
      "/com/fascinatedbox/LongbarDispatch", &error);

  if (error != NULL) {
    printf("longbar error: Unable to export skeleton: %s\n", error->message);
    return;
  }

  g_signal_connect(skeleton, "handle-set-height",
                   G_CALLBACK(on_handle_set_height), longbar);
  g_signal_connect(skeleton, "handle-quit", G_CALLBACK(on_handle_quit),
                   longbar);
}

void long_register_for_dbus(struct Longbar_t *longbar) {
  g_bus_own_name(G_BUS_TYPE_SESSION, "com.fascinatedbox.LongbarDispatch",
                 G_BUS_NAME_OWNER_FLAGS_NONE, NULL, on_name_acquired, NULL,
                 longbar, NULL);
}
