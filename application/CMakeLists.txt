cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(DNS-based-DDoS-mitigation)

include_directories(include)

function(build_app APP_NAME)
    add_executable(${APP_NAME} ${ARGS})
    target_link_libraries(${APP_NAME} pcap ssl crypto)
endfunction(build_app)

build_app(host src/host.c)
build_app(auth src/auth.c)
build_app(local src/local.c)
build_app(client src/client.c)
build_app(bot src/bot.c)
build_app(test src/test.c)
build_app(monitor
    src/monitor/main.c
    src/monitor/circular_buffer.c
    src/monitor/packet_handler.c
    src/monitor/tcp_syn_monitor.c
    src/monitor/monitor_log.c
    src/monitor/detect_ddos.c)