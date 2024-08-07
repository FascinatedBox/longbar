/*
 * GStreamer
 * Copyright (C) 2010  Intel Corporation.
 * Copyright (C) 2012 Matthew Waters <ystreet00@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors:
 *  Matthew Allum
 *  Robert Bragg
 *  Kristian Høgsberg
 */

/* code originally from clutter's wayland backend found here
 * http://git.gnome.org/browse/clutter/tree/clutter/wayland/clutter-event-wayland.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <wayland-client.h>

#include "wayland-event-source.h"

typedef struct _WaylandEventSource {
  GSource source;
  GPollFD pfd;
  uint32_t mask;
  struct wl_display *display;
  struct wl_event_queue *queue;
} WaylandEventSource;

static gboolean wayland_event_source_prepare(GSource *base, gint *timeout) {
  WaylandEventSource *source = (WaylandEventSource *)base;

  *timeout = -1;

  /* We have to add/remove the GPollFD if we want to update our
   * poll event mask dynamically.  Instead, let's just flush all
   * writes on idle */
  wl_display_flush(source->display);

  return FALSE;
}

static gboolean wayland_event_source_check(GSource *base) {
  WaylandEventSource *source = (WaylandEventSource *)base;
  gboolean retval;

  retval = source->pfd.revents;

  return retval;
}

static gboolean wayland_event_source_dispatch(GSource *base,
                                              GSourceFunc callback,
                                              gpointer data) {
  WaylandEventSource *source = (WaylandEventSource *)base;

  if (source->pfd.revents) {
    if (source->queue)
      wl_display_dispatch_queue_pending(source->display, source->queue);
    else
      wl_display_dispatch_pending(source->display);
    source->pfd.revents = 0;
  }

  if (callback)
    callback(data);

  return TRUE;
}

static GSourceFuncs wayland_event_source_funcs = {
    wayland_event_source_prepare,
    wayland_event_source_check,
    wayland_event_source_dispatch,
    NULL,
    NULL,
    NULL,
};

GSource *wayland_event_source_new(struct wl_display *display,
                                  struct wl_event_queue *queue) {
  WaylandEventSource *source;

  source = (WaylandEventSource *)g_source_new(&wayland_event_source_funcs,
                                              sizeof(WaylandEventSource));
  source->display = display;
  source->queue = queue;
  source->pfd.fd = wl_display_get_fd(display);
  source->pfd.events = G_IO_IN | G_IO_ERR;
  g_source_add_poll(&source->source, &source->pfd);

  return &source->source;
}
