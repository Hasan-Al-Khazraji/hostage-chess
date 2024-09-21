#include "hclib.h"
#include <stdlib.h>
#include <string.h>

exboard_t *newboard()
{
	exboard_t *board = malloc(sizeof(exboard_t));
	if (!board)
	{
		return NULL;
	}

	for (int i = 0; i < 8; i++)
	{
		// Set up pawns
		board->board[1][i] = 'P';
		board->board[6][i] = 'p';

		board->board[2][i] = ' ';
		board->board[3][i] = ' ';
		board->board[4][i] = ' ';
		board->board[5][i] = ' ';
	}

		// Set up other pieces
		board->board[0][0] = 'R';
		board->board[7][0] = 'r';
		board->board[0][1] = 'N';
		board->board[7][1] = 'n';
		board->board[0][2] = 'B';
		board->board[7][2] = 'b';
		board->board[0][3] = 'Q';
		board->board[7][3] = 'q';
		board->board[0][4] = 'K';
		board->board[7][4] = 'k';
		board->board[0][5] = 'B';
		board->board[7][5] = 'b';
		board->board[0][6] = 'N';
		board->board[7][6] = 'n';
		board->board[0][7] = 'R';
		board->board[7][7] = 'r';

		board->bprison[0] = '\0';
		board->bairfield[0] = '\0';
		board->wprison[0] = '\0';
		board->wairfield[0] = '\0';

	return board;
}

exboard_t *copyboard(exboard_t *board){
	exboard_t *copied_board = malloc(sizeof(exboard_t));
	if (!copied_board)
	{
		return NULL;
	}

	memcpy(copied_board, board, sizeof(exboard_t));
	return copied_board;
}