cmake_minimum_required(VERSION 3.27)
project(maskin_projekt2 C)

set(CMAKE_C_STANDARD 11)

add_executable(maskin_projekt2
        main.c
        src/deck.c
        src/deck.h
        src/Linkedlists.h
        src/Linkedlists.c
        src/gameCommands.h
        src/gameCommands.c
        src/view.c
        src/controller.c)
