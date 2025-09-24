#ifndef WLR_HELLO_PROTOCOL_H
#define WLR_HELLO_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include "wayland-server.h"
#define WL_HELLO_VERSION 1
#define WL_SCHEDULER_RESULT 0
#define WL_SCHEDULER_SET_RR 0

extern const struct wl_interface wl_hello_interface;
extern const struct wl_interface wl_scheduler_interface;
#endif