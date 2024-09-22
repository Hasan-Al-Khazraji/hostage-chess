#include "hclib.h"
#include <ctype.h>
#include <stdio.h>
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
	char pieces[8] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
	for (int i = 0; i < 8; i++)
	{
		board->board[0][i] = pieces[i];
		board->board[7][i] = tolower(pieces[i]);
	}

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

char *stringboard(exboard_t *board)
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
	i = 0;
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

	// Fill string with bairfield for the first 8 indicies
	i = 0;
	for (; i < 8 && board->bairfield[i] != '\0'; i++)
	{
		stringfied_board[i + 18] = board->bairfield[i];
	}
	for (; i < 8; i++)
	{
		stringfied_board[i + 18] = ' ';
	}

	// Fill string with bairfield for the next 8 indicies after space
	i = 9;

	for (; i < 17 && board->bairfield[i - 1] != '\0'; i++)
	{
		stringfied_board[i + 18] = board->bairfield[i - 1];
	}
	for (; i < 17; i++)
	{
		stringfied_board[i + 18] = ' ';
	}

	// Fill string with wairfield for the first 8 indicies
	i = 0;
	for (; i < 8 && board->wairfield[i] != '\0'; i++)
	{
		stringfied_board[i + 126] = board->wairfield[i];
	}
	for (; i < 8; i++)
	{
		stringfied_board[i + 126] = ' ';
	}

	// Fill string with wairfield for the next 8 indicies after space
	i = 9;

	for (; i < 17 && board->wairfield[i - 1] != '\0'; i++)
	{
		stringfied_board[i + 126] = board->wairfield[i - 1];
	}
	for (; i < 17; i++)
	{
		stringfied_board[i + 126] = ' ';
	}

	// Fill string indicies 36-43 with '-' and 117-124 with '-'
	for (int i = 36; i < 44; i++)
	{
		stringfied_board[i] = '-';
		stringfied_board[i + 81] = '-';
	}

	for (int i = 0; i < 8; i++)
	{
		stringfied_board[i + 45] = board->board[7][i];
		stringfied_board[i + 54] = board->board[6][i];
		stringfied_board[i + 63] = board->board[5][i];
		stringfied_board[i + 72] = board->board[4][i];
		stringfied_board[i + 81] = board->board[3][i];
		stringfied_board[i + 90] = board->board[2][i];
		stringfied_board[i + 99] = board->board[1][i];
		stringfied_board[i + 108] = board->board[0][i];
	}

	// Set the null terminator at the end of the string
	stringfied_board[161] = '\0';

	return stringfied_board;
}

exboard_t *apply_move(exboard_t *board, move_t *move)
{
	exboard_t *new_board = copyboard(board);
	if (!new_board)
	{
		return NULL;
	}

	// USED https://www.geeksforgeeks.org/how-to-append-a-character-to-a-string-in-c/ for strncat
	// Send piece to appropriate prision
	if (new_board->board[move->to_i][move->to_j] != ' ')
	{
		// Send white piece to bprision
		if (isupper(new_board->board[move->to_i][move->to_j]))
		{
			strncat(new_board->bprison, &new_board->board[move->to_i][move->to_j], 1);
		}
		// send black piece to wprision
		else
		{
			strncat(new_board->wprison, &new_board->board[move->to_i][move->to_j], 1);
		}
	}

	// CASTLING
	if ((new_board->board[move->from_i][move->from_j] == 'K' || new_board->board[move->from_i][move->from_j] == 'k') && abs(move->to_j - move->from_j) > 1)
	{
		// Castle to the right
		if (move->to_j - move->from_j > 1)
		{
			int j = move->to_j + 1;
			for (; j < 8; j++)
			{
				if (new_board->board[move->to_i][j] == ' ')
				{
					continue;
				}
				else
				{
					break;
				}
			}
			new_board->board[move->to_i][move->to_j - 1] = new_board->board[move->to_i][j];
			new_board->board[move->to_i][j] = ' ';
		}
		// Castle to the left
		else if (move->from_j - move->to_j > 1)
		{
			int j = move->to_j - 1;
			for (; j > 0; j--)
			{
				if (new_board->board[move->to_i][j] == ' ')
				{
					continue;
				}
				else
				{
					break;
				}
			}
			new_board->board[move->to_i][move->to_j + 1] = new_board->board[move->to_i][j];
			new_board->board[move->to_i][j] = ' ';
		}
	}

	// En Passant
	if ((new_board->board[move->from_i][move->from_j] == 'P' || new_board->board[move->from_i][move->from_j] == 'p') && (abs(move->to_i - move->from_i) == 1 && abs(move->to_j - move->from_j) == 1))
	{
		// Send white piece to bprision
		if (isupper(new_board->board[move->from_i][move->to_j]))
		{
			strncat(new_board->bprison, &new_board->board[move->from_i][move->to_j], 1);
		}
		// send black piece to wprision
		else
		{
			strncat(new_board->wprison, &new_board->board[move->from_i][move->to_j], 1);
		}
		new_board->board[move->from_i][move->to_j] = ' ';
	}

	// Move piece (regardless of other pieces or legality)
	// if promotion, make old from blank, make to a promoted piece (regardless if in prison or not)
	if (move->promotion == ' ')
	{
		new_board->board[move->to_i][move->to_j] = new_board->board[move->from_i][move->from_j];
		new_board->board[move->from_i][move->from_j] = ' ';
	}
	else
	{
		new_board->board[move->to_i][move->to_j] = move->promotion;
		new_board->board[move->from_i][move->from_j] = ' ';
	}

	return new_board;
}
