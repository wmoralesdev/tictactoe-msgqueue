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

	// Init struct
	// Board init
	ticTacToe.messageId = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            ticTacToe.board[i][j] = '.';

	ticTacToe.confirmation = '1';

	// Confirm queue
	printf("Enviando invitacion...\n");
	msgsnd (messageQueueId, (struct msgbuf *)&ticTacToe, 
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			0);
	
	printf("Esperando confirmacion J2...\n");

	msgrcv (messageQueueId, (struct msgbuf *)&ticTacToe,
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			2, 0);

	printf("J2 ha confirmado con: %c\n\n", ticTacToe.confirmation);

	// Open game
	while(true) {
		int auxRows = 0, auxCols = 0;
		printBoard(ticTacToe.board);

		printf("Introduce fila y columna: ");
		scanf("%d", &auxRows);

		scanf("%d", &auxCols);

		ticTacToe.board[auxRows][auxCols] = 'x';

		printf("\nEnviando a J2...\n");
		msgsnd (messageQueueId, (struct msgbuf *)&ticTacToe, 
			sizeof(ticTacToe.board) + sizeof(ticTacToe.confirmation), 
			0);

		printf("Esperando a J2...\n");
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
	}
}