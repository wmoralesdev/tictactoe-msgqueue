#ifndef TTT_UTILS
#define TTT_UTILS

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>

#define KEY 1092

void printBoard(char board[][3]) {
	printf("Tablero\n\n");
	for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            printf("%c\t", board[i][j]);
		printf("\n");
	}
}

bool checkHor(char board[][3], char equals) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] 
            && board [i][1] == board[i][2] 
            && board[i][0] == equals) {
            return true;
        }
    }

    return false;
}

bool checkVer(char board[][3], char equals) {
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] 
            && board [1][i] == board[2][i] 
            && board[0][i] == equals) {
            return true;
        }
    }

    return false;
}

bool checkDiag(char board[][3], char equals) {
    if (board[0][0] == board[1][1] 
    && board[1][1] == board[2][2]
    && board[0][0] == equals)
        return true;

    if (board[0][2] == board[1][1] 
    && board[1][1] == board[2][0]
    && board[2][0] == equals)
        return true;

    return false;
}

bool checker(char board[][3], char equals) {
	return checkVer(board, equals) || checkHor(board, equals) || checkDiag(board, equals);
}

#endif