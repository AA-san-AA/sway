rm -rf build &&
meson setup -Dwlroots:default_library=static -Dwayland:default_library=static -Dwayland:documentation=false -Dcairo:default_library=static \
-Dlibffi:default_library=static \
-Dc_std=gnu11 \
-Dc_args="-Wno-error " build \
--force-fallback-for=wayland,cairo \
-Dc_link_args=" -lfreetype -lxkbcommon -lcairo " \
--prefix=${PWD}/sway_install &&
cd build &&
meson compile && meson install &&
cd -

