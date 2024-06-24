#ifndef LONGBAR_H
#define LONGBAR_H

typedef struct LongBar_t {
  GtkWindow *window;
  GtkApplication *app;
} LongBar;

LongBar *longbar_new(GtkApplication *);
void longbar_load_modules(LongBar *);

#endif
