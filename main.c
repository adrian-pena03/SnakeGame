#include "SnakeGame.h"

int main(void){
	welcomeScreen(WELCOME_FILE);
	Player player;
	char opc;
	
	do{
		int *snakeColors=(int*) malloc(2*sizeof(int));
		int *wallsColors=(int*) malloc(2*sizeof(int));
		player.name=(char *) malloc(MAX_LENGTH_NAME*sizeof(char));
		if(snakeColors==NULL){
			if(wallsColors==NULL){
				printf("\n\nERROR, MEMORIA NO OTROGADA");
				exit(0);
			}else{
				printf("\n\nERROR, MEMORIA NO OTROGADA");
				free(wallsColors);
				exit(0);
			}
		}
		if(wallsColors==NULL){
			printf("\n\nERROR, MEMORIA NO OTROGADA");
			exit(0);
		}
		
		menu(player.name, snakeColors, wallsColors);
		instructions(INSTRUCTIONS_FILE);
		drawWalls(wallsColors);
	
		Snake *head; //Inicializamos la cabeza de serpiente
		head=initSnake((int) (INITIAL_COL+FINAL_COL)/2, (INITIAL_ROW+FINAL_ROW)/2, 1, 0);
		
		srand(time(NULL));
		game(&player, head, snakeColors, wallsColors);
		saveStatsTxt(&player, PLAYER_STATS_FILE); //Guardamos los datos de la partida
		
		//Liberamos memoria
		free(player.name);
		free(wallsColors);
		free(snakeColors);
		freeSnake(head);
		
		printf("%cDeseas jugar de nuevo? Pulsa 'c'.\n", 168);
		printf("%cJugamos despues? Pulsa cualquier otra tecla.\n\n", 168);
		printf("Respuesta: ");
		opc=getche();
		opc=tolower(opc);
		system("cls");
	}while(opc=='c');
	
	setColor(6);
	showTxt(GOODBYE_FILE);
	return 0;
}
