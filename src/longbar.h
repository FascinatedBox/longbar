#ifndef LONGBAR_H
#define LONGBAR_H

typedef struct Longbar_t {
  GtkWindow *window;
  GtkApplication *app;
} Longbar;

Longbar *longbar_new(GtkApplication *);
void longbar_load_modules(Longbar *);
void longbar_set_height(Longbar *, int);

#endif
