#ifndef TETRIS_H
# define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <stdbool.h>

#define HEIGHT 20	// R is short for row.
#define WIDTH 15	// C is short for column.

#define TRUE 1	// T is short for true.
#define FALSE 0	// F is short for false.

#define NUM_OF_MINOS_TYPES 7

#define MILLION 1000000	// constant value for gettimeofday.

#define INITIAL_SPAN_TIME 400000
#define INITIAL_DECREASE_TIME 1000

typedef struct s_shape
{
	char	**array;
	int		width;
	int		row;
	int		col;
}	t_shape;	// t_shape is a structure to store the data of tetris mino blocks.

# endif