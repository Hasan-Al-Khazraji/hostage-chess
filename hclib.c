#include "hclib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper Functions
int checkForPiece(char piece)
{
	char allPieces[] = "RNBQKPrnbqkp";
	for (int i = 0; allPieces[i] != '\0'; i++)
	{
		if (allPieces[i] == piece)
		{
			return 0; // Character is found
			break;	  // Exit the loop early as we found the character
		}
	}
	return 1; // Character is not found
}

int min(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

// Fill struct
move_t *fillMove(int from_i, int from_j, int to_i, int to_j, char promotion, char hostage)
{
	move_t *move = malloc(sizeof(move_t));
	if (!move)
	{
		return NULL;
	}
	move->from_i = from_i;
	move->from_j = from_j;
	move->to_i = to_i;
	move->to_j = to_j;
	move->promotion = promotion;
	move->hostage = hostage;
	return move;
}

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

	memset(board->bprison, 0, sizeof(board->bprison));
	memset(board->bairfield, 0, sizeof(board->bairfield));
	memset(board->wprison, 0, sizeof(board->wprison));
	memset(board->wairfield, 0, sizeof(board->wairfield));

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
	// Make sure there is a piece behind it
	if ((new_board->board[move->from_i][move->from_j] == 'P' || new_board->board[move->from_i][move->from_j] == 'p') && (abs(move->to_i - move->from_i) == 1 && abs(move->to_j - move->from_j) == 1) && checkForPiece(new_board->board[move->to_i][move->to_j]) == 1)
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

	// Drop from Air Field or Prison
	// bAirfield
	if (move->from_i == 8)
	{
		char droppedChar = '\0';
		for (int i = 0; i < 16; i++)
		{
			if (new_board->bairfield[i] == move->promotion)
			{
				droppedChar = new_board->bairfield[i];
				// https://stackoverflow.com/questions/5457608/how-to-remove-the-character-at-a-given-index-from-a-string-in-c for the memmove
				memmove(&new_board->bairfield[i], &new_board->bairfield[i + 1], strlen(new_board->bairfield) - i);
				break;
			}
			else
			{
				continue;
			}
		}
		new_board->board[move->to_i][move->to_j] = droppedChar;
	}
	// wAirfield
	else if (move->from_i == -1)
	{
		char droppedChar = '\0';
		for (int i = 0; i < 16; i++)
		{
			if (new_board->wairfield[i] == move->promotion)
			{
				droppedChar = new_board->wairfield[i];
				// https://stackoverflow.com/questions/5457608/how-to-remove-the-character-at-a-given-index-from-a-string-in-c for the memmove
				memmove(&new_board->wairfield[i], &new_board->wairfield[i + 1], strlen(new_board->wairfield) - i);
				break;
			}
			else
			{
				continue;
			}
		}
		new_board->board[move->to_i][move->to_j] = droppedChar;
	}
	// bPrison
	else if (move->from_i == 9)
	{
		char droppedChar = '\0';
		for (int i = 0; i < 16; i++)
		{
			if (new_board->bprison[i] == move->promotion)
			{
				droppedChar = new_board->bprison[i];
				memmove(&new_board->bprison[i], &new_board->bprison[i + 1], (strlen(new_board->bprison) - i));
				break;
			}
		}
		char hostageChar = '\0';
		for (int i = 0; i < 16; i++)
		{
			if (new_board->wprison[i] == move->hostage)
			{
				hostageChar = new_board->wprison[i];
				memmove(&new_board->wprison[i], &new_board->wprison[i + 1], strlen(new_board->wprison) - i);
				strncat(new_board->bairfield, &hostageChar, 1);
				break;
			}
		}
		new_board->board[move->to_i][move->to_j] = droppedChar;
	}
	// wPrison
	else if (move->from_i == -2)
	{
		char droppedChar = '\0';
		for (int i = 0; i < 16; i++)
		{
			if (new_board->wprison[i] == move->promotion)
			{
				droppedChar = new_board->wprison[i];
				memmove(&new_board->wprison[i], &new_board->wprison[i + 1], strlen(new_board->wprison) - i);
				break;
			}
		}
		char hostageChar = '\0';
		for (int i = 0; i < 16; i++)
		{
			if (new_board->bprison[i] == move->hostage)
			{
				hostageChar = new_board->bprison[i];
				memmove(&new_board->bprison[i], &new_board->bprison[i + 1], strlen(new_board->bprison) - i);
				strncat(new_board->wairfield, &hostageChar, 1);
				break;
			}
		}
		new_board->board[move->to_i][move->to_j] = droppedChar;
	}
	else // Happy path
	{
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
	}

	return new_board;
}

move_t **moves(board_t *board, int from_i, int from_j)
{

	char selectedPiece = (*board)[from_i][from_j];
	switch (selectedPiece)
	{
	case 'p':
		// pawn_moves
		return pawn_moves(board, from_i, from_j, 1);
		// return the value from this function
		break;
	case 'r':
		// rook_moves
		return rookmoves(board, from_i, from_j, 1);
		// return the value from this function
		break;
	case 'n':
		// knight_moves
		return knightmoves(board, from_i, from_j, 1);
		// return the value from this function
		break;
	case 'b':
		// bishop_moves
		return bishopmoves(board, from_i, from_j, 1);
		// return the value from this function
		break;
	case 'q':
		// queenmoves
		return queenmoves(board, from_i, from_j, 1);
		// return the value from this function
		break;
	case 'k':
		// kingmoves
		return king_moves(board, from_i, from_j, 1);
		// return the value from this function
		break;
	case 'P':
		// pawn_moves
		return pawn_moves(board, from_i, from_j, 0);
		// return the value from this function
		break;
	case 'R':
		// rook_moves
		return rookmoves(board, from_i, from_j, 0);
		// return the value from this function
		break;
	case 'N':
		// knight_moves
		return knightmoves(board, from_i, from_j, 0);
		// return the value from this function
		break;
	case 'B':
		// bishop_moves
		return bishopmoves(board, from_i, from_j, 0);
		// return the value from this function
		break;
	case 'Q':
		// queenmoves
		return queenmoves(board, from_i, from_j, 0);
		// return the value from this function
		break;
	case 'K':
		// kingmoves
		return king_moves(board, from_i, from_j, 0);
		// return the value from this function
		break;
	}
	return NULL;
}

move_t **king_moves(board_t *board, int from_i, int from_j, int colour)
{
	move_t **king_moves_list = malloc(9 * sizeof(move_t *));

	// Check if malloc failed
	if (!king_moves_list)
	{
		return NULL;
	}

	// Possible Moves
	int possible_moves[8][2] = {
		{from_i - 1, from_j - 1},
		{from_i - 1, from_j},
		{from_i - 1, from_j + 1},
		{from_i, from_j - 1},
		{from_i, from_j + 1},
		{from_i + 1, from_j - 1},
		{from_i + 1, from_j},
		{from_i + 1, from_j + 1}};

	// Check if the move is valid
	int valid_move_counter = 0;
	for (int i = 0; i < 8; i++)
	{
		if (possible_moves[i][0] >= 0 && possible_moves[i][0] < 8 && possible_moves[i][1] >= 0 && possible_moves[i][1] < 8)
		{
			char target_piece = (*board)[possible_moves[i][0]][possible_moves[i][1]];
			if (checkForPiece(target_piece) == 1 || (colour == 0 && islower(target_piece)) || (colour == 1 && isupper(target_piece)))
			{
				king_moves_list[valid_move_counter] = fillMove(from_i, from_j, possible_moves[i][0], possible_moves[i][1], ' ', ' ');
				valid_move_counter++;
			}
		}
	}
	king_moves_list[valid_move_counter] = NULL;
	king_moves_list = realloc(king_moves_list, (valid_move_counter + 1) * sizeof(move_t *));
	return king_moves_list;
}

move_t **knightmoves(board_t *board, int from_i, int from_j, int colour)
{
	move_t **knight_moves_list = malloc(9 * sizeof(move_t *));

	// Check if malloc failed
	if (!knight_moves_list)
	{
		return NULL;
	}

	// Possible Moves
	int possible_moves[8][2] = {
		{from_i + 2, from_j + 1},
		{from_i + 1, from_j + 2},
		{from_i - 2, from_j + 1},
		{from_i - 1, from_j + 2},
		{from_i + 2, from_j - 1},
		{from_i + 1, from_j - 2},
		{from_i - 2, from_j - 1},
		{from_i - 1, from_j - 2}};

	// Check if the move is valid
	int valid_move_counter = 0;
	for (int i = 0; i < 8; i++)
	{
		if (possible_moves[i][0] >= 0 && possible_moves[i][0] < 8 && possible_moves[i][1] >= 0 && possible_moves[i][1] < 8)
		{
			char target_piece = (*board)[possible_moves[i][0]][possible_moves[i][1]];
			if (checkForPiece(target_piece) == 1 || (colour == 0 && islower(target_piece)) || (colour == 1 && isupper(target_piece)))
			{
				knight_moves_list[valid_move_counter] = fillMove(from_i, from_j, possible_moves[i][0], possible_moves[i][1], ' ', ' ');
				valid_move_counter++;
			}
		}
	}
	knight_moves_list[valid_move_counter] = NULL;
	knight_moves_list = realloc(knight_moves_list, (valid_move_counter + 1) * sizeof(move_t *));
	return knight_moves_list;
}

move_t **bishopmoves(board_t *board, int from_i, int from_j, int colour)
{
	move_t **bishop_moves_list = malloc(16 * sizeof(move_t *));

	// Check if malloc failed
	if (!bishop_moves_list)
	{
		return NULL;
	}

	// Optimal approach from https://www.geeksforgeeks.org/all-possible-points-where-bishops-can-reach-in-one-move/
	int topLeft = min(from_i, from_j);
	int topRight = min(from_i, 7 - from_j);
	int bottomLeft = min(7 - from_i, from_j);
	int bottomRight = min(7 - from_i, 7 - from_j);

	int valid_move_counter = 0;

	// Top left
	for (int i = 1; i <= topLeft; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i + i][from_j - i]) == 1 && from_i + i < 8 && from_j - i >= 0)
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i + i;
			bishop_moves_list[valid_move_counter]->to_j = from_j - i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i + i][from_j - i])) || (colour == 1 && isupper((*board)[from_i + i][from_j - i])))
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i + i;
			bishop_moves_list[valid_move_counter]->to_j = from_j - i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}

	// Top right
	for (int i = 1; i <= topRight; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i + i][from_j + i]) == 1 && from_i + i < 8 && from_j + i < 8)
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i + i;
			bishop_moves_list[valid_move_counter]->to_j = from_j + i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i + i][from_j + i])) || (colour == 1 && isupper((*board)[from_i + i][from_j + i])))
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i + i;
			bishop_moves_list[valid_move_counter]->to_j = from_j + i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}

	// Bottom left
	for (int i = 1; i <= bottomLeft; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i - i][from_j - i]) == 1 && from_i - i >= 0 && from_j - i >= 0)
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i - i;
			bishop_moves_list[valid_move_counter]->to_j = from_j - i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i - i][from_j - i])) || (colour == 1 && isupper((*board)[from_i - i][from_j - i])))
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i - i;
			bishop_moves_list[valid_move_counter]->to_j = from_j - i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}

	// Bottom right
	for (int i = 1; i <= bottomRight; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i - i][from_j + i]) == 1 && from_i - i >= 0 && from_j + i < 8)
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i - i;
			bishop_moves_list[valid_move_counter]->to_j = from_j + i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i - i][from_j + i])) || (colour == 1 && isupper((*board)[from_i - i][from_j + i])))
		{
			bishop_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			bishop_moves_list[valid_move_counter]->from_i = from_i;
			bishop_moves_list[valid_move_counter]->from_j = from_j;
			bishop_moves_list[valid_move_counter]->to_i = from_i - i;
			bishop_moves_list[valid_move_counter]->to_j = from_j + i;
			bishop_moves_list[valid_move_counter]->promotion = ' ';
			bishop_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}
	bishop_moves_list[valid_move_counter] = NULL;
	bishop_moves_list = realloc(bishop_moves_list, (valid_move_counter + 1) * sizeof(move_t *));
	return bishop_moves_list;
}

move_t **rookmoves(board_t *board, int from_i, int from_j, int colour)
{
	move_t **rook_moves_list = malloc(16 * sizeof(move_t *));

	// Check if malloc failed
	if (!rook_moves_list)
	{
		return NULL;
	}

	int up = 7 - from_i;
	int right = 7 - from_j;
	int down = from_i;
	int left = from_j;

	int valid_move_counter = 0;

	// Up
	for (int i = 1; i <= up; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i + i][from_j]) == 1)
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i + i, from_j, ' ', ' ');
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i + i][from_j])) || (colour == 1 && isupper((*board)[from_i + i][from_j])))
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i + i, from_j, ' ', ' ');
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}

	// Right
	for (int i = 1; i <= right; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i][from_j + i]) == 1)
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i, from_j + i, ' ', ' ');
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i][from_j + i])) || (colour == 1 && isupper((*board)[from_i][from_j + i])))
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i, from_j + i, ' ', ' ');
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}

	// Down
	for (int i = 1; i <= down; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i - i][from_j]) == 1)
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i - i, from_j, ' ', ' ');
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i - i][from_j])) || (colour == 1 && isupper((*board)[from_i - i][from_j])))
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i - i, from_j, ' ', ' ');
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}

	// Left
	for (int i = 1; i <= left; i++)
	{
		// if it is a space
		if (checkForPiece((*board)[from_i][from_j - i]) == 1)
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i, from_j - i, ' ', ' ');
			valid_move_counter++;
		}
		// if it is a killable piece
		else if ((colour == 0 && islower((*board)[from_i][from_j - i])) || (colour == 1 && isupper((*board)[from_i][from_j - i])))
		{
			rook_moves_list[valid_move_counter] = fillMove(from_i, from_j, from_i, from_j - i, ' ', ' ');
			valid_move_counter++;
			break;
		}
		// if it is a same color piece
		else
		{
			break;
		}
	}
	rook_moves_list[valid_move_counter] = NULL;
	rook_moves_list = realloc(rook_moves_list, (valid_move_counter + 1) * sizeof(move_t *));
	return rook_moves_list;
}

move_t **queenmoves(board_t *board, int from_i, int from_j, int colour)
{
	move_t **queen_moves_list = malloc(32 * sizeof(move_t *));
	if (!queen_moves_list)
	{
		return NULL;
	}

	move_t **straight_moves = rookmoves(board, from_i, from_j, colour);
	move_t **diagonal_moves = bishopmoves(board, from_i, from_j, colour);
	int i = 0;
	for (; straight_moves[i] != NULL; i++)
	{
		queen_moves_list[i] = straight_moves[i];
	}
	for (int j = 0; diagonal_moves[j] != NULL; i++, j++)
	{
		queen_moves_list[i] = diagonal_moves[j];
	}

	queen_moves_list[i] = NULL;
	free(straight_moves);
	free(diagonal_moves);
	queen_moves_list = realloc(queen_moves_list, (i + 1) * sizeof(move_t *));
	return queen_moves_list;
}

move_t **pawn_moves(board_t *board, int from_i, int from_j, int colour)
{
	move_t **pawn_moves_list = malloc(5 * sizeof(move_t *));
	if (!pawn_moves_list)
	{
		return NULL;
	}

	// Colour 0 is white, Colour 1 is black
	// White is positive direction, Black is negative direction

	int colourValidator = colour == 0 ? 1 : -1;
	int valid_move_counter = 0;

	// Check if spot ahead is empty
	// Check also if i + 1 is within bounds OR i - 1 is within bounds
	if (checkForPiece((*board)[from_i + colourValidator][from_j]) == 1 && from_i + colourValidator < 8 && from_i + colourValidator >= 0)
	{
		pawn_moves_list[valid_move_counter] = malloc(sizeof(move_t));
		pawn_moves_list[valid_move_counter]->from_i = from_i;
		pawn_moves_list[valid_move_counter]->from_j = from_j;
		pawn_moves_list[valid_move_counter]->to_i = from_i + colourValidator;
		pawn_moves_list[valid_move_counter]->to_j = from_j;
		pawn_moves_list[valid_move_counter]->promotion = ' ';
		pawn_moves_list[valid_move_counter]->hostage = ' ';
		valid_move_counter++;
	}
	// Check if there is a piece right diagnoally
	if (checkForPiece((*board)[from_i + colourValidator][from_j + 1]) == 0 && ((colour == 0 && islower((*board)[from_i + colourValidator][from_j + 1])) || (colour == 1 && isupper((*board)[from_i + colourValidator][from_j + 1]))) && from_i + colourValidator < 8 && from_i + colourValidator >= 0 && from_j + 1 < 8 && from_j + 1 >= 0)
	{
		pawn_moves_list[valid_move_counter] = malloc(sizeof(move_t));
		pawn_moves_list[valid_move_counter]->from_i = from_i;
		pawn_moves_list[valid_move_counter]->from_j = from_j;
		pawn_moves_list[valid_move_counter]->to_i = from_i + colourValidator;
		pawn_moves_list[valid_move_counter]->to_j = from_j + 1;
		pawn_moves_list[valid_move_counter]->promotion = ' ';
		pawn_moves_list[valid_move_counter]->hostage = (*board)[from_i + colourValidator][from_j + 1];
		valid_move_counter++;
	}
	// Check if there is a piece left diagnoally
	if (checkForPiece((*board)[from_i + colourValidator][from_j - 1]) == 0 && ((colour == 0 && islower((*board)[from_i + colourValidator][from_j - 1])) || (colour == 1 && isupper((*board)[from_i + colourValidator][from_j - 1]))) && from_i + colourValidator < 8 && from_i + colourValidator >= 0 && from_j - 1 < 8 && from_j - 1 >= 0)
	{
		pawn_moves_list[valid_move_counter] = malloc(sizeof(move_t));
		pawn_moves_list[valid_move_counter]->from_i = from_i;
		pawn_moves_list[valid_move_counter]->from_j = from_j;
		pawn_moves_list[valid_move_counter]->to_i = from_i + colourValidator;
		pawn_moves_list[valid_move_counter]->to_j = from_j - 1;
		pawn_moves_list[valid_move_counter]->promotion = ' ';
		pawn_moves_list[valid_move_counter]->hostage = (*board)[from_i + colourValidator][from_j - 1];
		valid_move_counter++;
	}
	// Check if it is the first move
	if ((colour == 0 && from_i == 1) || (colour == 1 && from_i == 6))
	{
		// Check if 2 spots ahead is empty
		// Check also if i + 1 is within bounds OR i - 1 is within bounds
		if (checkForPiece((*board)[from_i + 2 * colourValidator][from_j]) == 1 && from_i + 2 * colourValidator < 8 && from_i + 2 * colourValidator >= 0)
		{
			pawn_moves_list[valid_move_counter] = malloc(sizeof(move_t));
			pawn_moves_list[valid_move_counter]->from_i = from_i;
			pawn_moves_list[valid_move_counter]->from_j = from_j;
			pawn_moves_list[valid_move_counter]->to_i = from_i + 2 * colourValidator;
			pawn_moves_list[valid_move_counter]->to_j = from_j;
			pawn_moves_list[valid_move_counter]->promotion = ' ';
			pawn_moves_list[valid_move_counter]->hostage = ' ';
			valid_move_counter++;
		}
	}

	pawn_moves_list[valid_move_counter] = NULL;
	pawn_moves_list = realloc(pawn_moves_list, (valid_move_counter + 1) * sizeof(move_t *));
	return pawn_moves_list;
}
