rm -rf build &&
meson setup -Dwlroots:default_library=static -Dwayland:default_library=static -Dwayland:documentation=false \
-Dlibffi:default_library=static \
-Dc_std=gnu11 \
--force-fallback-for=wayland,libffi \
-Dc_args="-Wno-error=deprecated-declarations -Wno-error=implicit-function-declaration" build \
--prefix=${PWD}/sway_install &&
cd build &&
meson compile && meson install &&
cd -

