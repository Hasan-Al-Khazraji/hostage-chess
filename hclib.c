#include "hclib.h"
#include <stdlib.h>
#include <string.h>

exboard_t *newboard()
{
	exboard_t *game = malloc(sizeof(exboard_t));
	if (!game)
	{
		return NULL;
	}

	for (int i = 0; i < 8; i++)
	{
		// Set up pawns
		game->board[1][i] = 'P';
		game->board[6][i] = 'p';

		game->board[2][i] = ' ';
		game->board[3][i] = ' ';
		game->board[4][i] = ' ';
		game->board[5][i] = ' ';
	}

		// Set up other pieces
		game->board[0][0] = 'R';
		game->board[7][0] = 'r';
		game->board[0][1] = 'N';
		game->board[7][1] = 'n';
		game->board[0][2] = 'B';
		game->board[7][2] = 'b';
		game->board[0][3] = 'Q';
		game->board[7][3] = 'q';
		game->board[0][4] = 'K';
		game->board[7][4] = 'k';
		game->board[0][5] = 'B';
		game->board[7][5] = 'b';
		game->board[0][6] = 'N';
		game->board[7][6] = 'n';
		game->board[0][7] = 'R';
		game->board[7][7] = 'r';

		game->bprison[0] = '\0';
		game->bairfield[0] = '\0';
		game->wprison[0] = '\0';
		game->wairfield[0] = '\0';

	return game;
}
