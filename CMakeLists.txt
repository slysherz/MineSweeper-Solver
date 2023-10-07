cmake_minimum_required(VERSION 3.10)

project(
    MinesweeperSolver
    VERSION 1.0
    LANGUAGES C
)

add_executable(ms-solver 
    board_tools.c
    board_tools.h
    board.c
    board.h
    group_list.c
    group_list.h
    group.c
    group.h
    main.c
    solver.c
    solver.h
    ui.c
    ui.h
)
