#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"
#include <cairo/cairo.h>

#define WIDTH 400
#define HEIGHT 300

struct display {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_shm *shm;
};

struct window {
    struct display *display;
    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_buffer *buffer;
    cairo_surface_t *cairo_surface;
    cairo_t *cr;
    int width, height;
};

static int create_shm_file(off_t size) {
    char template[] = "/tmp/wayland-shm-XXXXXX";
    int fd = mkstemp(template);
    if (fd >= 0)
        unlink(template);
    ftruncate(fd, size);
    return fd;
}

static struct wl_buffer *create_buffer(struct display *d, struct window *w) {
    int stride = w->width * 4;
    int size = stride * w->height;

    int fd = create_shm_file(size);
    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct wl_shm_pool *pool = wl_shm_create_pool(d->shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
        w->width, w->height, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    w->cairo_surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32,
        w->width, w->height, stride);
    w->cr = cairo_create(w->cairo_surface);

    return buffer;
}

static void draw(struct window *w) {
    cairo_t *cr = w->cr;
    cairo_set_source_rgb(cr, 0.2, 0.4, 0.6);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 32);

    cairo_text_extents_t extents;
	cairo_text_extents(cr, "Hello Wayland!", &extents);

    cairo_move_to(cr, 50, 150);
    cairo_show_text(cr, "Hello Wayland!");
    printf("Text extents: width=%.2f, height=%d", extents.width, 32);    
    cairo_surface_flush(w->cairo_surface);
}

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    struct window *w = data;
    xdg_surface_ack_configure(xdg_surface, serial);
    draw(w);
    wl_surface_attach(w->surface, w->buffer, 0, 0);
    wl_surface_damage(w->surface, 0, 0, w->width, w->height);
    wl_surface_commit(w->surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void registry_handler(void *data, struct wl_registry *registry,
                             uint32_t id, const char *interface, uint32_t version) {
    struct display *d = data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        d->compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 4);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        d->shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        d->xdg_wm_base = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(d->xdg_wm_base, &wm_base_listener, d);
    }
}

static void registry_remove(void *data, struct wl_registry *registry, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handler,
    .global_remove = registry_remove,
};

int main() {
    struct display d = {0};
    struct window w = {0};

    d.display = wl_display_connect(NULL);
    if (!d.display) {
        fprintf(stderr, "Can't connect to display\n");
        return 1;
    }

    d.registry = wl_display_get_registry(d.display);
    wl_registry_add_listener(d.registry, &registry_listener, &d);
    wl_display_roundtrip(d.display);

    w.display = &d;
    w.width = WIDTH;
    w.height = HEIGHT;

    w.surface = wl_compositor_create_surface(d.compositor);
    w.xdg_surface = xdg_wm_base_get_xdg_surface(d.xdg_wm_base, w.surface);
    xdg_surface_add_listener(w.xdg_surface, &xdg_surface_listener, &w);
    w.xdg_toplevel = xdg_surface_get_toplevel(w.xdg_surface);
    xdg_toplevel_set_title(w.xdg_toplevel, "Wayland Cairo Demo");

    w.buffer = create_buffer(&d, &w);

    wl_surface_commit(w.surface);

    while (wl_display_dispatch(d.display) != -1) {
        // event loop
    }

    cairo_destroy(w.cr);
    cairo_surface_destroy(w.cairo_surface);
    wl_display_disconnect(d.display);
    return 0;
}
