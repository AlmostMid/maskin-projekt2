cmake_minimum_required(VERSION 3.27)
project(maskin_projekt2 C)

set(CMAKE_C_STANDARD 11)

add_executable(maskin_projekt2
        TUI/src/deck.c
        TUI/src/deck.h
        TUI/src/Linkedlists.h
        TUI/src/Linkedlists.c
        TUI/src/gameCommands.h
        TUI/src/gameCommands.c
        TUI/src/view.c
        TUI/src/controller.c
        TUI/src/main.c
)
