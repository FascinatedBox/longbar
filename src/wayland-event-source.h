/*
 * GStreamer
 * Copyright (C) 2011  Intel Corporation.
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
 * Authors:
 *  Robert Bragg <robert@linux.intel.com>
 */

/* code originally from clutter's wayland backend found here
 * http://git.gnome.org/browse/clutter/tree/clutter/wayland/clutter-event-wayland.h
 */

#ifndef __WAYLAND_EVENT_SOURCE_H__
#define __WAYLAND_EVENT_SOURCE_H__

#include <glib-object.h>

#include <wayland-client.h>

GSource *wayland_event_source_new(struct wl_display *display,
                                  struct wl_event_queue *queue);

#endif /* __WAYLAND_EVENT_SOURCE_H__ */
