cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")

# Enable CMAKE_EXPORT_COMPILE_COMMANDS
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tcp_syn_monitor)

# Add the include directory
include_directories(include)

# Update source files with the new path
set(SOURCE_FILES src/main.c src/tcp_syn_monitor.c src/circular_buffer.c src/packet_handler.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} pcap)

add_custom_target(clean-all
    COMMAND rm -f *.o *.s ${PROJECT_NAME}
)
