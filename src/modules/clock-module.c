#include <gtk/gtk.h>

#include "clock-module.h"

static int _timer_update(void *_data) {
  clock_module_data *data = _data;
  time_t raw_time = time(NULL);
  struct tm *info = localtime(&raw_time);
  char buffer[32];
  sprintf(buffer, "%.2d:%.2d", info->tm_hour, info->tm_min);
  gtk_label_set_text(GTK_LABEL(data->label), buffer);
  return 1;
}

clock_module_data *clock_module_new(GtkWidget *layout_box) {
  clock_module_data *result = g_new(clock_module_data, 1);
  GtkWidget *label = gtk_label_new("");
  result->label = label;
  result->timer_id = g_timeout_add_seconds(1, _timer_update, result);
  gtk_container_add(GTK_CONTAINER(layout_box), result->label);

  // Fake an update to prevent being blank until the real one happens.
  _timer_update(result);
  return result;
}
