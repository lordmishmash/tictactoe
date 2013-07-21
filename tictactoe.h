#ifndef _TICTACTOE_HPP_
#define _TICTACTOE_HPP_

#include <stdbool.h>

/**
* Tic tac toe related stuff
*/
typedef enum
{
    EMPTY = 0,
    ONE   = 1,
    TWO   = 2
} STATE;

typedef struct
{
    int length;
    int *children;
} move_list_t;

typedef struct
{
    STATE state[9];
} T3BOARD;

T3BOARD* create_board  ();
T3BOARD* copy_board    (T3BOARD* board);
void print_board      (T3BOARD* board);
bool     board_full    (T3BOARD* board);
void     destroy_board (T3BOARD* board);
bool     make_move     (T3BOARD* board, STATE state, int move);
bool     _valid_move   (T3BOARD* board, int move);
STATE    _check_board  (T3BOARD* board, int a, int b, int c);
STATE    _board_winner (T3BOARD* board);
move_list_t* get_possible_moves (T3BOARD* board);
move_list_t* create_move_list ();
void add_move_to_list (move_list_t* list, int move);
void _destroy_move_list (move_list_t* list);

/**
* Minimax related stuff
*/
typedef enum
{
    WIN  = 1,
    LOSE = -1,
    TIE  = 0
} SCORE;

typedef struct
{
    int          max_depth;
    int          best;
} mm_node_t, mm_tree_t;

mm_node_t* mm_create_tree   (int max_depth);
int   mm_get_best_move      (mm_node_t* node, T3BOARD* board, STATE active);
SCORE _mm_get_best_move     (mm_node_t* node, T3BOARD* board, STATE active, int depth);

#endif // _TICTACTOE_HPP_
