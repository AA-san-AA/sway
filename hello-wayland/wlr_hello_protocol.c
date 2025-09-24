#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"


// 定义请求
static const struct wl_message wl_hello_requesets[] = {
    { "say", "s", NULL},    // 发送问候消息，'s' 表示字符串参数
};

// 定义接口 
const struct wl_interface wl_hello_interface = {
    "wl_hello", 1,
    1, wl_hello_requesets,      // 1个请求 （从客户端到服务端）
    0, NULL                     // 没有事件（从服务端到客户端）
};


// 定义请求
static const struct wl_message wl_scheduler_requesets[] = {
    { "set_rr", "ui", NULL},    // 设置调度策略，u表示uint32，i表示int32
    { "print_scheduler_info", "", NULL }  // print_scheduler_info事件，不带参数
};
// 定义事件
static const struct wl_message scheduler_events[] = {
    { "result", "us", NULL }  // result事件，带uint32和string参数
};
// 定义接口 
const struct wl_interface wl_scheduler_interface = {
    "wl_scheduler", 1,
    2, wl_scheduler_requesets,      // 1个请求 （从客户端到服务端）
    1, scheduler_events             // 1个事件 （从服务端到客户端）
};
