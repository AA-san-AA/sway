if wlroots_proj.found()
    protocols_client_header = wlroots_proj.get_variable('protocols_client_header')
    command = ['convert', 'cat.png', '-define', 'h:format=bgra', '-depth', '8', 'cat.h']
    run_command(command)
else
    error('Not support')
endif

executable(
    'hello-wayland',
    ['main.c', 'shm.c', protocols_client_header['xdg-shell']],
    dependencies: wlroots,
    install: true,
)