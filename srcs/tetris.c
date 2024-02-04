#include "../incs/tetris.h"

char		g_table[HEIGHT][WIDTH] = {{0}};
int			g_current_score = 0;
char		g_game_end_flag = TRUE;
suseconds_t	g_span_time = INITIAL_SPAN_TIME;
int			g_decrease = INITIAL_DECREASE_TIME;

t_shape current_shape;

const t_shape	mino_array[NUM_OF_MINOS_TYPES] = {
	{(char *[]){(char []){0, 1, 1}, (char []){1, 1, 0}, (char []){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char []){1, 1, 0}, (char []){0, 1, 1}, (char []){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char []){0, 1, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char []){0, 0, 1}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char []){1, 0, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char []){1, 1}, (char []){1, 1}}, 2, 0, 0},
	{(char *[]){(char []){0, 0, 0, 0}, (char []){1, 1, 1, 1}, (char []){0, 0, 0, 0}, (char []){0, 0, 0, 0}}, 4, 0, 0}
};

t_shape	FunctionCS(t_shape shape)
{
	t_shape	new_shape = shape;
	char	**copyshape = shape.array;
	new_shape.array = (char **)malloc(new_shape.width * sizeof(char *));
	int	i;
	int	j;
	for (i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
		for (j = 0; j < new_shape.width; j++)
		{
			new_shape.array[i][j] = copyshape[i][j];
		}
	}
	return (new_shape);
}

void	FunctionDS(t_shape shape)
{
	int	i;
	for (i = 0; i < shape.width; i++)
	{
		free(shape.array[i]);
	}
	free(shape.array);
}

int	FunctionCP(t_shape shape)
{
	char	**array = shape.array;
	int	i;
	int	j;
	for (i = 0; i < shape.width; i++)
	{
		for (j = 0; j < shape.width; j++)
		{
			if ((shape.col + j < 0 || shape.col + j >= WIDTH || shape.row + i >= HEIGHT))
			{
				if (array[i][j])
					return (FALSE);

			}
			else if (g_table[shape.row + i][shape.col + j] && array[i][j])
				return (FALSE);
		}
	}
	return (TRUE);
}

void	FunctionRS(t_shape shape)
{
	t_shape	temp = FunctionCS(shape);
	int	i;
	int	j;
	int	k;
	int	width;
	width = shape.width;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			shape.array[i][j] = temp.array[k][i];
		}
	}
	FunctionDS(temp);
}

void	FunctionPT()
{
	char	Buffer[HEIGHT][WIDTH] = {{0}};
	int	i;
	int	j;
	for (i = 0; i < current_shape.width; i++)
	{
		for (j = 0; j < current_shape.width; j++)
		{
			if (current_shape.array[i][j])
				Buffer[current_shape.row + i][current_shape.col + j] = current_shape.array[i][j];
		}
	}
	clear();
	for (i = 0; i < WIDTH - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			printw("%c ", (g_table[i][j] + Buffer[i][j])? '#': '.');
		}
		printw("\n");
	}
	printw("\nScore: %d\n", g_current_score);
}

struct timeval	now;
struct timeval	before_now;

int	hasToUpdate(void)
{
	suseconds_t	now_time;
	suseconds_t	before_now_time;

	now_time = now.tv_sec * MILLION + now.tv_usec;
	before_now_time = before_now.tv_sec * MILLION + before_now.tv_usec;
	return ((now_time - before_now_time) > g_span_time);
}


void	add_score(int *current_score, int count)
{
	*current_score += (WIDTH * 100) * count;
}

int	main(void)
{
	srand(time(0));
	g_current_score = 0;
	int	c;
	initscr();
	gettimeofday(&before_now, NULL);
	timeout(1);

	t_shape	new_shape = FunctionCS(mino_array[rand() % NUM_OF_MINOS_TYPES]);
	new_shape.col = rand() % (WIDTH - new_shape.width + 1);
	new_shape.row = 0;
	FunctionDS(current_shape);
	current_shape = new_shape;
	if (!FunctionCP(current_shape))
	{
		g_game_end_flag = FALSE;
	}
	FunctionPT();
	while (g_game_end_flag)
	{
		if ((c = getch()) != ERR)
		{
			t_shape	temp = FunctionCS(current_shape);
			switch(c)
			{
				case 's':
					temp.row++;
					if (FunctionCP(temp))
						current_shape.row++;
					else {
						int	i;
						int	j;
						for (i = 0; i < current_shape.width; i++)
						{
							for (j = 0; j < current_shape.width; j++)
							{
								if (current_shape.array[i][j])
									g_table[current_shape.row + i][current_shape.col + j] = current_shape.array[i][j];
							}
						}
						int	n, m, sum, count = 0;
						for (n = 0; n < HEIGHT; n++)
						{
							sum = 0;
							for (m = 0; m < WIDTH; m++)
							{
								sum += g_table[n][m];
							}
							if (sum == WIDTH)
							{
								count++;
								int	l;
								int	k;
								for (k = n; k >= 1; k--)
									for (l = 0; l < WIDTH; l++)
										g_table[k][l] = g_table[k - 1][l];
								for (l = 0; l < WIDTH; l++)
									g_table[k][l] = 0;
								g_span_time -= g_decrease--;
							}
						}
						add_score(&g_current_score, count);
						t_shape new_shape = FunctionCS(mino_array[rand() % NUM_OF_MINOS_TYPES]);
						new_shape.col = rand() % (WIDTH - new_shape.width + 1);
						new_shape.row = 0;
						FunctionDS(current_shape);
						current_shape = new_shape;
						if (!FunctionCP(current_shape))
						{
							g_game_end_flag = FALSE;
						}
					}
					break;
				case 'd':
					temp.col++;
					if (FunctionCP(temp))
						current_shape.col++;
					break;
				case 'a':
					temp.col--;
					if (FunctionCP(temp))
						current_shape.col--;
					break;
				case 'w':
					FunctionRS(temp);
					if (FunctionCP(temp))
						FunctionRS(current_shape);
					break;
			}
			FunctionDS(temp);
			FunctionPT();
		}
		gettimeofday(&now, NULL);
		if (hasToUpdate()) {
			t_shape	temp = FunctionCS(current_shape);
			switch('s')
			{
				case 's':
					temp.row++;
					if (FunctionCP(temp))
						current_shape.row++;
					else
					{
						int	i;
						int	j;
						for (i = 0; i < current_shape.width; i++)
						{
							for (j = 0; j < current_shape.width; j++)
							{
								if (current_shape.array[i][j])
									g_table[current_shape.row + i][current_shape.col + j] = current_shape.array[i][j];
							}
						}
						int n, m, sum, count=0;
						for (n = 0; n < HEIGHT; n++)
						{
							sum = 0;
							for (m = 0; m < WIDTH; m++)
							{
								sum += g_table[n][m];
							}
							if (sum == WIDTH)
							{
								count++;
								int	l;
								int	k;
								for (k = n; k >= 1; k--)
									for (l = 0; l < WIDTH; l++)
										g_table[k][l]=g_table[k - 1][l];
								for (l = 0; l < WIDTH; l++)
									g_table[k][l] = 0;
								g_span_time -= g_decrease--;
							}
						}
						add_score(&g_current_score, count);
						t_shape	new_shape = FunctionCS(mino_array[rand() % NUM_OF_MINOS_TYPES]);
						new_shape.col = rand() % (WIDTH - new_shape.width + 1);
						new_shape.row = 0;
						FunctionDS(current_shape);
						current_shape = new_shape;
						if (!FunctionCP(current_shape))
						{
							g_game_end_flag = FALSE;
						}
					}
					break;
				case 'd':
					temp.col++;
					if (FunctionCP(temp))
						current_shape.col++;
					break;
				case 'a':
					temp.col--;
					if(FunctionCP(temp))
						current_shape.col--;
					break;
				case 'w':
					FunctionRS(temp);
					if(FunctionCP(temp))
						FunctionRS(current_shape);
					break;
			}
			FunctionDS(temp);
			FunctionPT();
			gettimeofday(&before_now, NULL);
		}
	}
	FunctionDS(current_shape);
	endwin();
	int	i;
	int	j;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			printf("%c ", g_table[i][j] ? '#' : '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", g_current_score);
	return (0);
}

__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q tetris");
}
