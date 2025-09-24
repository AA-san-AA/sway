CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Wno-unused-parameter -g
PKG_CONFIG ?= pkg-config

# Host deps
WAYLAND_FLAGS = $(shell $(PKG_CONFIG) wayland-client --cflags --libs)
WAYLAND_PROTOCOLS_DIR = $(shell $(PKG_CONFIG) wayland-protocols --variable=pkgdatadir)

# Build deps
WAYLAND_SCANNER = $(shell pkg-config --variable=wayland_scanner wayland-scanner)

XDG_SHELL_PROTOCOL = $(WAYLAND_PROTOCOLS_DIR)/stable/xdg-shell/xdg-shell.xml

HEADERS_WAYLAND=cat.h xdg-shell-client-protocol.h shm.h wlr_hello_protocol.h
SOURCES_WAYLAND=main.c xdg-shell-protocol.c shm.c wlr_hello_protocol.c

HEADERS_FONT=xdg-shell-client-protocol.h shm.h wlr_hello_protocol.h
SOURCES_FONT=simple-font.c xdg-shell-protocol.c shm.c wlr_hello_protocol.c

CFLAGS_FONT += $(shell pkg-config --cflags cairo)
LDFLAGS_FONT += $(shell pkg-config --libs cairo)

all: hello-wayland

hello-wayland: $(HEADERS_WAYLAND) $(SOURCES_WAYLAND)
	$(CC) $(CFLAGS) -o $@ $(SOURCES_WAYLAND) -lrt $(WAYLAND_FLAGS)

simple-font: $(HEADERS_FONT) $(SOURCES_FONT)
	$(CC) $(CFLAGS_FONT) -o $@ $(SOURCES_FONT) -lrt $(WAYLAND_FLAGS) $(LDFLAGS_FONT)

xdg-shell-client-protocol.h:
	$(WAYLAND_SCANNER) client-header $(XDG_SHELL_PROTOCOL) xdg-shell-client-protocol.h

xdg-shell-protocol.c:
	$(WAYLAND_SCANNER) private-code $(XDG_SHELL_PROTOCOL) xdg-shell-protocol.c

cat.h: cat.png
	convert cat.png -define h:format=bgra -depth 8 cat.h

.PHONY: clean
clean:
	$(RM) hello-wayland cat.h xdg-shell-protocol.c xdg-shell-client-protocol.h
