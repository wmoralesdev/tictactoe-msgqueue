#include "utils.h"

typedef struct boardComm {
	long messageId;
	char board[3][3];
	char confirmation;
} boardComm;

int main(void) {
	int messageQueueId;
	boardComm ticTacToe;

	messageQueueId = msgget (KEY, 0600 | IPC_CREAT);
	if (messageQueueId == -1)
	{
		printf("Error al obtener identificador para cola mensajes\n");
		exit (-1);
	}

	// Confirm queue
	msgrcv (messageQueueId, (struct msgbuf *)&ticTacToe,
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			0, 0);

	printf("J1 ha confirmado con: %c\n\n", ticTacToe.confirmation);

	ticTacToe.messageId = 2;
	ticTacToe.confirmation = '2';

	printf("Enviando confirmacion J1...\n");
	msgsnd (messageQueueId, (struct msgbuf *)&ticTacToe, 
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			0);

	// Open game
	while(true) {
		printf("Esperando J1...\n");

		msgrcv (messageQueueId, (struct msgbuf *)&ticTacToe,
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			0, 0);

		// Check winner
		if(checker(ticTacToe.board, 'x')) {
			printf("Gana J1\n");

			return msgctl(messageQueueId, IPC_RMID, 0);
		}

		if(checker(ticTacToe.board, 'o')) {
			printf("Gana J2\n");

			return msgctl(messageQueueId, IPC_RMID, 0);
		}

		int auxRows = 0, auxCols = 0;
		printBoard(ticTacToe.board);

		printf("Introduce fila y columna: ");
		scanf("%d", &auxRows);

		scanf("%d", &auxCols);

		ticTacToe.board[auxRows][auxCols] = 'o';

		printf("Enviando a J1...\n");
		msgsnd (messageQueueId, (struct msgbuf *)&ticTacToe, 
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			0);
	}
}