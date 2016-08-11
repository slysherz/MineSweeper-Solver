#pragma once

#include <stdio.h>
#include <stdbool.h>

#include <board_tools.h>
#include <group_list.h>

#define MAYBE_ON 0
#define MAYBE(x) if (MAYBE_ON) x

int solveMineSweeper(Board boardh, Board boardv);
