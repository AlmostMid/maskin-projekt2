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
)

set(Test_Sources test/test.c src/Linkedlists.c src/view.c src/deck.c src/gameCommands.c)
set(Test_Headers )
add_executable(YukonTest ${Test_Sources} ${Headers} ${Test_Headers})
