#include <gtk/gtk.h>

#include "dbus-autogen.h"
#include "dbus-dispatch.h"
#include "longbar.h"

static gboolean on_handle_quit(LongbarDispatch *skeleton,
                               GDBusMethodInvocation *invocation,
                               gpointer data) {
  LongBar *longbar = (LongBar *)data;

  (void)skeleton;
  (void)invocation;
  g_application_quit(G_APPLICATION(longbar->app));
  return TRUE;
}

static void on_name_acquired(GDBusConnection *connnection, const gchar *name,
                             gpointer longbar) {
  (void)name;
  LongbarDispatch *skeleton = longbar_dispatch_skeleton_new();
  g_signal_connect(skeleton, "handle-quit", G_CALLBACK(on_handle_quit),
                   longbar);
  GError *error = NULL;
  g_dbus_interface_skeleton_export(
      G_DBUS_INTERFACE_SKELETON(skeleton), connnection,
      "/com/fascinatedbox/LongbarDispatch", &error);
  if (error != NULL) {
    printf("longbar error: Unable to export skeleton: %s\n", error->message);
  }
}

void long_register_for_dbus(struct LongBar_t *longbar) {
  g_bus_own_name(G_BUS_TYPE_SESSION, "com.fascinatedbox.LongbarDispatch",
                 G_BUS_NAME_OWNER_FLAGS_NONE, NULL, on_name_acquired, NULL,
                 longbar, NULL);
}