//
// Created by Victor Mazanti on 24/04/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "view.h"
#include "deck.h"
#include "gameCommands.h"
#include "controller.h"

int main(void){
#if defined(_WIN32) || defined(_WIN64)
    setbuf(stdout, 0);
#endif

    runYukonGame();

    return 0;
}