#include <stdlib.h>
#include <stdio.h>

#include "tictactoe.h"

T3BOARD* create_board ()
{
    T3BOARD* board = malloc (sizeof (T3BOARD));
    for (int i = 0; i < 9; i++)
        board->state[i] = EMPTY;
    return board;
}

T3BOARD* copy_board (T3BOARD* board)
{
    T3BOARD* _board = create_board ();
    for (int i = 0; i < 9; i++)
        _board->state[i] = board->state[i];
    return _board;
}

void print_board (T3BOARD* board)
{
    for (int i = 0; i < 9; i++)
    {
        if (board->state[i] == ONE)
            printf ("X");
        else if (board->state[i] == TWO)
            printf ("O");
        else
            printf (" ");

        if (i == 2 || i == 5 || i == 8)
            printf ("\n");
    }
}

bool board_full (T3BOARD* board)
{
    for (int i = 0; i < 9; i++)
        if (board->state[i] == EMPTY)
            return false;
    return true;
}

STATE _check_board (T3BOARD* b, int i, int j, int k)
{
    if (b->state[i] == b->state[j] && b->state[i] == b->state[k] && b->state[i] != EMPTY)
        return b->state[i];
    return EMPTY;
}

STATE _board_winner (T3BOARD* board)
{
    STATE winner = _check_board (board, 0, 1, 2);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 3, 4, 5);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 6, 7, 8);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 0, 3, 6);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 1, 4, 7);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 2, 5, 8);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 0, 4, 8);
    if (winner != EMPTY) return winner;

    winner = _check_board (board, 6, 4, 2);
    if (winner != EMPTY) return winner;

    return EMPTY;
}

move_list_t* _get_possible_moves (T3BOARD* board)
{
    move_list_t* list = create_move_list ();

    for (int i = 0; i < 9; i++) {
        if (board->state[i] == EMPTY) {
            add_move_to_list (list, i);
        }
    }
    return list;
}

void _destroy_move_list (move_list_t* list)
{
    if (list) {
        if (list->children)
            free (list->children);
        free (list);
    }
}

move_list_t* create_move_list ()
{
    move_list_t* list = malloc(sizeof(move_list_t));
    list->length   = 0;
    list->children = NULL;

    return list;
}

void add_move_to_list (move_list_t* list, int move)
{
    list->children = realloc (list->children, sizeof(int) * (list->length + 1));
    if (list->children != NULL) {
        list->length ++;
        list->children [list->length - 1] = move;
    }
}

void destroy_board (T3BOARD* board)
{
    if (board)
        free (board);
}

bool make_move (T3BOARD* board, STATE state, int pos)
{
    if (!_valid_move(board, pos))
        return false;
    board->state[pos] = state;
    return true;
}

bool _valid_move (T3BOARD* board, int pos)
{
    if (pos >= 0 && pos <= 9 && board->state[pos] == EMPTY)
        return true;
    return false;
}

mm_node_t* mm_create_tree (int max_depth)
{
    mm_node_t* node = malloc (sizeof (mm_node_t));
    node->max_depth = max_depth;
    node->best      = -1;

    return node;
}

void mm_destroy_tree (mm_node_t* node)
{
    free (node);
}

int mm_get_best_move (mm_node_t* node, T3BOARD* board, STATE active)
{
    _mm_get_best_move (node, board, active, 0);
    return node->best;
}

SCORE _mm_get_best_move (mm_node_t* node, T3BOARD* board, STATE active, int depth)
{
    if (depth > node->max_depth)
        return 0;

    STATE other  = active == ONE ? TWO : ONE;
    STATE winner = _board_winner (board);

    if (winner == active)
        return WIN;
    else if (winner == other)
        return LOSE;
    else if (board_full(board))
        return TIE;

    SCORE score = LOSE;
    move_list_t* moves = _get_possible_moves (board);
    move_list_t* candidates = create_move_list ();

    for (int i = 0; i < moves->length; i++) {
        T3BOARD* board2 = copy_board (board);
        make_move (board2, active, moves->children[i]);

        SCORE subscore = -_mm_get_best_move (node, board2, other, depth + 1);
        if (subscore > score)
            score = subscore;

        if (depth == 0)
            add_move_to_list (candidates, subscore);
    }

    if (depth == 0) {
        move_list_t* candidate = create_move_list ();
        for (int i = 0; i < candidates->length; i++) {
            if (candidates->children[i] == score) {
                add_move_to_list (candidate, moves->children[i]);
            }
        }
        node->best = candidate->children[rand() % candidate->length];

        _destroy_move_list (candidate);
        _destroy_move_list (moves);
        _destroy_move_list (candidates);
    }

    return score;
}
