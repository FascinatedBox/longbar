#ifndef LONGBAR_TASKBAR_MODULE_H
#define LONGBAR_TASKBAR_MODULE_H

struct zwlr_foreign_toplevel_manager_v1;

typedef struct {
  GtkWidget *grid;
  struct zbox_foreign_toplevel_manager_v1 *manager;
  struct wl_seat *seat;
} taskbar_module_data;

taskbar_module_data *taskbar_module_new(GtkWidget *);

#endif
