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

exboard_t *copyboard(exboard_t *board)
{
	exboard_t *copied_board = malloc(sizeof(exboard_t));
	if (!copied_board)
	{
		return NULL;
	}

	memcpy(copied_board, board, sizeof(exboard_t));
	return copied_board;
}

exboard_t *stringboard(exboard_t *board)
{
	// Malloc space for the stringified board
	char *stringfied_board = malloc(sizeof(char) * 162);
	if (!stringfied_board)
	{
		return NULL;
	}

	// Every 9th character is a newline
	for (int i = 8; i < 162; i += 9)
	{
		stringfied_board[i] = '\n';
	}

	// Fill string with bprison for the first 8 indicies
	int i = 0;
	for (; i < 8 && board->bprison[i] != '\0'; i++)
	{
		stringfied_board[i] = board->bprison[i];
	}
	for (; i < 8; i++)
	{
		stringfied_board[i] = ' ';
	}

	// Fill string with bprison for the next 8 indicies after space
	i = 9;

	for (; i < 17 && board->bprison[i - 1] != '\0'; i++)
	{
		stringfied_board[i] = board->bprison[i - 1];
	}
	for (; i < 17; i++)
	{
		stringfied_board[i] = ' ';
	}

	// Fill string with wprison for the first 8 indicies
	int i = 0;
	for (; i < 8 && board->wprison[i] != '\0'; i++)
	{
		stringfied_board[i + 144] = board->wprison[i];
	}
	for (; i < 8; i++)
	{
		stringfied_board[i + 144] = ' ';
	}

	// Fill string with wprison for the next 8 indicies after space
	i = 9;

	for (; i < 17 && board->wprison[i - 1] != '\0'; i++)
	{
		stringfied_board[i + 144] = board->wprison[i - 1];
	}
	for (; i < 17; i++)
	{
		stringfied_board[i + 144] = ' ';
	}

	// Fill string indicies 36-43 with '-' and 117-124 with '-'
	for (int i = 36; i < 44; i++)
	{
		stringfied_board[i] = '-';
		stringfied_board[i + 81] = '-';
	}

	// Set the null terminator at the end of the string
	stringfied_board[161] = '\0';
}