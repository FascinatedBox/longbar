#ifndef LONGBAR_CLOCK_MODULE_H
#define LONGBAR_CLOCK_MODULE_H

typedef struct {
  GtkWidget *label;
  int timer_id;
} clock_module_data;

clock_module_data *clock_module_new(GtkWidget *);

#endif
