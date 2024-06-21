#ifndef LONGBAR_H
#define LONGBAR_H

typedef struct {
  GtkWindow *window;
} LongBar;

LongBar *longbar_new(GtkApplication *);
void longbar_load_modules(LongBar *);

#endif
