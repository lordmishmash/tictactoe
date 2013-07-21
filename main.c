#include <stdio.h>
#include "tictactoe.h"

void draw_board (T3BOARD* board)
{
}

int main (int argc, char *argv[])
{
    T3BOARD* board = create_board ();

    int move  = -1;
    STATE who = TWO;
    mm_tree_t* tree = mm_create_tree (9999);

    while (true)
    {
        if (who == ONE)
        {

            printf ("enter move: ");
            scanf ("%d", &move);
            make_move (board, ONE, move - 1);
        }
        else if (who == TWO)
        {
            printf ("thinking...\n");
            int best = mm_get_best_move(tree, board, TWO);
            printf ("best: %d\n", best);
            make_move (board, TWO, best);
        }



        who = who == ONE ? TWO : ONE;
        print_board (board);

        if (_board_winner(board) || board_full(board))
            break;

    }
    printf ("win\n");

    return 0;
}
