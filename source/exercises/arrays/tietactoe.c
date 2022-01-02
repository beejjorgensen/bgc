#include <stdio.h>

// Note: If there were a real tic-tac-toe game, you
// wouldn't test every board position. You'd only test
// for whichever player made the last move, and then
// you'd only test the directions that needed testing
// from that latest move.

int check(int b[3][3])
{
    int candidate;

    // Test rows
    for (int row = 0; row < 3; row++) {
        candidate = b[row][0];

        if (candidate == 0) continue;

        for (int col = 1; col < 3; col++) {
            if (b[row][col] != candidate) break;
            if (col == 2) return candidate;
        }
    }

    // Test columns
    for (int col = 0; col < 3; col++) {
        candidate = b[0][col];

        if (candidate == 0) continue;

        for (int row = 1; row < 3; row++) {
            if (b[row][col] != candidate) break;
            if (row == 2) return candidate;
        }
    }

    // Test diagonal
    candidate = b[0][0];

    if (candidate != 0) {
        for (int i = 1; i < 3; i++) {
            if (b[i][i] != candidate) break;
            if (i == 2) return candidate;
        }
    }

    // Test back diagonal
    candidate = b[0][2];

    if (candidate != 0) {
        for (int i = 1; i < 3; i++) {
            if (b[i][2-i] != candidate) break;
            if (i == 2) return candidate;
        }
    }

    return 0;
}

char *winner(int w)
{
    if (w == 1) return "X";
    if (w == 2) return "O";
    return "None";
}

int main(void)
{
    int board[][3][3] = {
        {
            {1, 1, 2},
            {2, 1, 1},
            {1, 2, 2},
        },
        {
            {0, 2, 1},
            {2, 1, 1},
            {1, 0, 2},
        },
        {
            {2, 2, 2},
            {1, 1, 0},
            {0, 1, 0},
        },
        {
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
        },
        {
            {0, 0, 2},
            {0, 2, 0},
            {2, 0, 0},
        },
        {
            {1, 2, 2},
            {2, 1, 2},
            {2, 2, 1},
        },
    };

    for (int i = 0; i < 6; i++) {
        printf("Winner of game 1: %s\n", winner(check(board[i])));
    }
}
