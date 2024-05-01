cmake_minimum_required(VERSION 3.27)
project(maskin_projekt2 C)

set(CMAKE_C_STANDARD 11)

add_executable(maskin_projekt2
        src/deck.c
        src/deck.h
        src/Linkedlists.h
        src/Linkedlists.c
        src/gameCommands.h
        src/gameCommands.c
        src/view.c
        src/controller.c
        src/main.c
        src/socket.c
        src/socket.h
)

target_link_libraries(maskin_projekt2 wsock32 ws2_32)
