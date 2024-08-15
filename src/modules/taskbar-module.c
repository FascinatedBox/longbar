#include <gdk/gdkwayland.h>
#include <gtk/gtk.h>

#include "longbar.h"
#include "taskbar-module.h"
#include "wayland-event-source.h"
#include "box-foreign-toplevel-management-unstable-v1-protocol.h"

#define BOX_FOREIGN_TOPLEVEL_MANAGEMENT_VERSION 10

typedef struct {
  struct zbox_foreign_toplevel_handle_v1 *zbox_handle;
  taskbar_module_data *module_data;
  GtkWidget *button;
  GtkWidget *menu;
  int visible;
  int active;
  int pending_sync;
} toplevel_data;

static void nop_app_id(void *, struct zbox_foreign_toplevel_handle_v1 *,
                       const char *) {}

static void nop_output(void *, struct zbox_foreign_toplevel_handle_v1 *,
                       struct wl_output *) {}

static void nop_parent(void *, struct zbox_foreign_toplevel_handle_v1 *,
                       struct zbox_foreign_toplevel_handle_v1 *) {}

static void toplevel_handle_title(void *data,
                                  struct zbox_foreign_toplevel_handle_v1 *,
                                  const char *title) {
  toplevel_data *toplevel = data;
  gtk_button_set_label(GTK_BUTTON(toplevel->button), title);
}

static void toplevel_handle_state(void *data,
                                  struct zbox_foreign_toplevel_handle_v1 *,
                                  struct wl_array *array) {
  toplevel_data *toplevel = data;
  int active = FALSE;
  uint32_t *entry;

  wl_array_for_each(entry, array) {
    // This is the only state that needs to be tracked.
    if (*entry == ZBOX_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_ACTIVATED)
      active = TRUE;
  }

  toplevel->active = active;
  toplevel->pending_sync = TRUE;
}

static void toplevel_handle_done(void *data,
                                 struct zbox_foreign_toplevel_handle_v1 *) {
  toplevel_data *toplevel = data;
  taskbar_module_data *tmd = toplevel->module_data;
  GtkWidget *grid = tmd->grid;

  if (toplevel->visible == FALSE) {
    gtk_container_add(GTK_CONTAINER(grid), toplevel->button);
    gtk_widget_show(toplevel->button);
    toplevel->visible = TRUE;
  }

  if (toplevel->pending_sync) {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toplevel->button),
                                 toplevel->active);
    toplevel->pending_sync = FALSE;
  }
}

static void
toplevel_handle_closed(void *data,
                       struct zbox_foreign_toplevel_handle_v1 *zbox_handle) {
  toplevel_data *toplevel = data;

  if (toplevel->visible) {
    taskbar_module_data *tmd = toplevel->module_data;
    GtkWidget *grid = tmd->grid;
    gtk_container_remove(GTK_CONTAINER(grid), toplevel->button);
  }

  zbox_foreign_toplevel_handle_v1_destroy(zbox_handle);
  free(toplevel);
}

static void on_menu_close(toplevel_data *toplevel) {
  zbox_foreign_toplevel_handle_v1_close(toplevel->zbox_handle);
}

static gint on_toplevel_button_press(toplevel_data *toplevel, GdkEvent *event) {
  if (event->type != GDK_BUTTON_PRESS)
    return TRUE;

  GdkEventButton *event_button = (GdkEventButton *)event;

  if (event_button->button == GDK_BUTTON_PRIMARY) {
    if (toplevel->active)
      zbox_foreign_toplevel_handle_v1_set_minimized(toplevel->zbox_handle);
    else {
      struct wl_seat *seat = toplevel->module_data->seat;

      // Make sure the window isn't minimized before trying to activate it.
      zbox_foreign_toplevel_handle_v1_unset_minimized(toplevel->zbox_handle);
      zbox_foreign_toplevel_handle_v1_activate(toplevel->zbox_handle, seat);
    }
  } else if (event_button->button == GDK_BUTTON_SECONDARY) {
    GtkMenu *menu = GTK_MENU(toplevel->menu);
    gtk_menu_popup_at_pointer(menu, event);
  }

  return TRUE;
}

static void create_menu_for_toplevel(toplevel_data *toplevel) {
  GtkButton *button = GTK_BUTTON(toplevel->button);
  GtkWidget *menu = gtk_menu_new();
  GtkWidget *item = gtk_menu_item_new_with_label("Close");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
  gtk_widget_show(item);
  g_signal_connect_swapped(item, "activate", G_CALLBACK(on_menu_close),
                           toplevel);
  g_signal_connect_swapped(button, "button-press-event",
                           G_CALLBACK(on_toplevel_button_press), toplevel);
  toplevel->menu = menu;
}

static const struct zbox_foreign_toplevel_handle_v1_listener toplevel_impl = {
    .app_id = nop_app_id,
    .closed = toplevel_handle_closed,
    .done = toplevel_handle_done,
    .output_enter = nop_output,
    .output_leave = nop_output,
    .parent = nop_parent,
    .state = toplevel_handle_state,
    .title = toplevel_handle_title};

static void toplevel_manager_handle_toplevel(
    void *data_, struct zbox_foreign_toplevel_manager_v1 *,
    struct zbox_foreign_toplevel_handle_v1 *zbox_handle) {
  taskbar_module_data *data = data_;
  toplevel_data *toplevel = g_new0(toplevel_data, 1);
  toplevel->zbox_handle = zbox_handle;
  toplevel->module_data = data;
  toplevel->button = gtk_toggle_button_new();
  toplevel->visible = FALSE;
  gtk_button_set_relief(GTK_BUTTON(toplevel->button), GTK_RELIEF_NONE);
  create_menu_for_toplevel(toplevel);
  zbox_foreign_toplevel_handle_v1_add_listener(zbox_handle, &toplevel_impl,
                                               toplevel);
}

static void toplevel_manager_handle_finished(
    void *, struct zbox_foreign_toplevel_manager_v1 *toplevel_manager) {
  zbox_foreign_toplevel_manager_v1_destroy(toplevel_manager);
}

static const struct zbox_foreign_toplevel_manager_v1_listener
    toplevel_manager_impl = {
        .finished = toplevel_manager_handle_finished,
        .toplevel = toplevel_manager_handle_toplevel,
};

static void handle_global(void *data_, struct wl_registry *registry,
                          uint32_t name, const char *interface,
                          uint32_t version) {
  taskbar_module_data *data = data_;
  if (strcmp(interface, zbox_foreign_toplevel_manager_v1_interface.name) == 0) {
    data->manager = wl_registry_bind(
        registry, name, &zbox_foreign_toplevel_manager_v1_interface,
        BOX_FOREIGN_TOPLEVEL_MANAGEMENT_VERSION);
    zbox_foreign_toplevel_manager_v1_add_listener(data->manager,
                                                  &toplevel_manager_impl, data);
  } else if (strcmp(interface, wl_seat_interface.name) == 0 &&
             data->seat == NULL) {
    data->seat = wl_registry_bind(registry, name, &wl_seat_interface, version);
  }
}

static void nop_remove(void *, struct wl_registry *, uint32_t) {}

static const struct wl_registry_listener registry_listener = {
    .global = handle_global,
    .global_remove = nop_remove,
};

static void attach_to_global(taskbar_module_data *data) {
  // Init makes sure this is a Wayland display, no need to check again.
  GdkDisplay *gdk_display = gdk_display_get_default();
  struct wl_display *display = gdk_wayland_display_get_wl_display(gdk_display);
  struct wl_registry *registry = wl_display_get_registry(display);
  struct wl_event_queue *eq = wl_display_create_queue(display);
  GSource *src = wayland_event_source_new(display, eq);
  (void)src;
  wl_registry_add_listener(registry, &registry_listener, data);
}

taskbar_module_data *taskbar_module_new(GtkWidget *layout_box) {
  taskbar_module_data *result = g_new0(taskbar_module_data, 1);
  result->manager = NULL;
  result->grid = gtk_grid_new();
  result->seat = NULL;
  gtk_container_add(GTK_CONTAINER(layout_box), result->grid);
  attach_to_global(result);
  return result;
}
