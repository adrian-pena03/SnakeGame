#include "SnakeGame.h"

void GotoXY(int x, int y){
 	COORD coord;
 	coord.X = x;
 	coord.Y = y;
 	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int numColor){
	switch(numColor){
    	case I_RED:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            break;
        case I_GRN:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
            break;
        case I_YEL:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_YELLOW);
            break;
        case I_BLU:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
            break;  
        case I_MAG:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_MAGENTA);
            break; 
		case I_CYN:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_CYAN);
            break;
		case I_WHT:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE);
            break; 
    }
}

//Funcion para leer un archivo .txt y mostrar su contenido en pantalla
void showTxt(char *name){
	FILE *fp;
	fp=fopen(name, "r");
	if(fp==NULL){
		printf("ERROR, NO SE PUEDO ABRIR EL ARCHIVO");
		exit(1);
	}
	char hold=getc(fp);
	while(hold!=EOF){
		printf("%c", hold);
		hold=getc(fp);
	}	
	fclose(fp);
	return;
}

//Funcion que lanza la portada del juego, la portada esta en un .txt
void welcomeScreen(char *name){
	setColor(1);
	showTxt(name);
	
	GotoXY(20, 17); //Mostramos un mensaje
	setColor(2);
	printf("Presiona cualquier tecla para continuar");
	GotoXY(0, 19);
	setColor(6);
	printf("Por Adrian Pe%ca.\n\n", 164);
	printf("Amplia tu pantalla para mejor experiencia.");
	getch();
	system("cls");
	return;
}

void instructions(char *name){ //Muestra las instrucciones
	setColor(6);
	showTxt(name);
	getch();
	system("cls");
	return;
}

//Funcion que dibuja los bordes del terreno de juego
void drawWalls(int *wallsColors){ //wallsColors es el puntero al arreglo de dos colores que el usuario quiere
	int i, j;
	for(i=INITIAL_COL; i<=FINAL_COL; i++){ //Horizontal superior
		for(j=INITIAL_ROW; j<INITIAL_ROW+HORIZONTAL_WIDTH; j++){
			GotoXY(i, j);
			//If para que los colores sean contiguos y se forme un patron (ya que las celdas no son cuadradas)
			if(i%4==0 || i%4==1){
				setColor(wallsColors[0]);
			}else{
				setColor(wallsColors[1]);
			}
			printf("%c", 178);
		}
	}
	for(i=INITIAL_COL; i<=FINAL_COL; i++){ //Horizontal inferior
		for(j=FINAL_ROW-HORIZONTAL_WIDTH+1; j<=FINAL_ROW; j++){
			GotoXY(i, j);
			if(i%4==0 || i%4==1){
				setColor(wallsColors[0]);
			}else{
				setColor(wallsColors[1]);
			}
			printf("%c", 178);
		}
	}
	for(i=INITIAL_ROW; i<=FINAL_ROW; i++){ //Vertical izquierdo
		for(j=INITIAL_COL; j<INITIAL_COL+VERTICAL_WIDTH; j++){
			GotoXY(j, i);
			setColor(wallsColors[i%2]);
			printf("%c", 178);
		}
	}
	for(i=INITIAL_ROW; i<=FINAL_ROW; i++){ //Vertical derecho
		for(j=FINAL_COL-VERTICAL_WIDTH+1; j<=FINAL_COL; j++){
			GotoXY(j, i);
			setColor(wallsColors[i%2]);
			printf("%c", 178);
		}
	}
	return;
}

void printTime(int seconds){ //Recibe segundos, e imprime en formato hh:mm:ss arriba del tablero
	int min=(int) seconds/60;
	seconds%=60;
	int hrs=(int) min/60;
	min%=60;
	GotoXY(FINAL_COL-12, INITIAL_ROW-1);
	setColor(6);
	printf("Time %.2d:%.2d:%.2d", hrs, min, seconds);
	return;
}

//Guarda y muestra el historial de la partida en un .txt
void saveStatsTxt(Player *player, char *name){
	FILE *fp;
	fp=fopen(name, "a+");
	if(fp==NULL){
		printf("ERROR, NO SE PUDO ABRIR EL REGISTRO");
	}
	int sec=player->time;
	int min=(int) sec/60;
	sec%=60;
	int hrs= (int) min/60;
	min%=60;
	
	fprintf(fp, "Nombre del jugador: %s", player->name);
	fprintf(fp, "Nivel alcanzado: %d.\tPuntaje: %d.\t Tiempo: %.2d:%.2d:%.2d\n", player->level, player->score, hrs, min, sec);
	fprintf(fp, "_______________________________________\n\n");
	fclose(fp);
	showTxt(PLAYER_STATS_FILE);
	setColor(6);
	printf("Pulsa cualquier tecla para continuar");
	getch();
	system("cls");
}

//Funcion que muestra el menu, y llena los parametros del jugador y su partida
void menu(char *name, int *vecColor, int *wallsColors){
	system("cls");
	printf("\t\tMENU PRINCIPAL\n\n");
	printf("Escribe tu nombre (maximo 25 caracteres): "); fflush(stdin);
	fgets(name, MAX_LENGTH_NAME, stdin);
	
	printf("Escoge dos colores para tu serpiente y dos colores para las paredes.\n\n");
	printf("\t\t%c Colores disponibles %c\n", 204, 185);
	//Mostramos paleta de olores
	setColor(0); printf("\t[Rojo] : 0.\n");
	setColor(1); printf("\t[Verde] : 1.\n");
	setColor(2); printf("\t[Amarillo] : 2.\n");
	setColor(3); printf("\t[Azul] : 3.\n");
	setColor(4); printf("\t[Magenta] : 4.\n");
	setColor(5); printf("\t[Cyan] : 5.\n");
	setColor(6); printf("\t[Blanco] : 6.\n\n");
	
	int menuX=8, menuY=14; //Coords auxiliares para escribir las siguientes oraciones
	
	//Nos aseguramos que sea un color valido con el do-while
	do{
		GotoXY(menuX, menuY);
		printf("%cColor 1 para serpiente: ", 175);
		scanf("%d", &vecColor[0]);
		if(vecColor[0]<0 || vecColor[0]>6){
			GotoXY(menuX, menuY+1);
			printf("Color no valido, ingrese nuevamente.");
			GotoXY(menuX+25, menuY);
			printf("            "); //Borramos el numero ingresado erroneo
		}
	}while(vecColor[0]<0 || vecColor[0]>6);
	GotoXY(menuX, menuY+1);              
	printf("                                   "); //Borramos el mensaje de color no valido
	
	//Los siguientes son analogos al anterior
	do{
		GotoXY(menuX, menuY+1);
		printf("%cColor 2 para serpiente: ", 175);
		scanf("%d", &vecColor[1]);
		if(vecColor[1]<0 || vecColor[1]>6){
			GotoXY(menuX, menuY+2);
			printf("Color no valido, ingrese nuevamente.");
			GotoXY(menuX+25, menuY+1);
			printf("        ");
		}
	}while(vecColor[1]<0 || vecColor[1]>6);
	GotoXY(menuX, menuY+2);              
	printf("                                   ");
	
	do{
		GotoXY(menuX, menuY+3);
		printf("%cColor 1 para pared: ", 175);
		scanf("%d", &wallsColors[0]);
		if(wallsColors[0]<0 || wallsColors[0]>6){
			GotoXY(menuX, menuY+4);
			printf("Color no valido, ingrese nuevamente.");
			GotoXY(menuX+21, menuY+3);
			printf("        ");
		}
	}while(wallsColors[0]<0 || wallsColors[0]>6);
	GotoXY(menuX, menuY+4);              
	printf("                                   ");
	
	do{
		GotoXY(menuX, menuY+4);
		printf("%cColor 2 para pared: ", 175);
		scanf("%d", &wallsColors[1]);
		if(wallsColors[1]<0 || wallsColors[1]>6){
			GotoXY(menuX, menuY+5);
			printf("Color no valido, ingrese nuevamente.");
			GotoXY(menuX+21, menuY+4);
			printf("        ");
		}
	}while(wallsColors[1]<0 || wallsColors[1]>6);
	GotoXY(menuX, menuY+5);     
	printf("                                   ");
	
	getchar();
	system("cls");
}


//Esta funcion inicializa la cabeza de la serpiente, dandole su coordenada y direccion
Snake* initSnake(int x, int y, int _dirx, int _diry){
	Snake *head;
	head=(Snake*) malloc(sizeof(Snake)); //Pedimos memoria para la cabeza
	if(head==NULL){
		printf("\n\nERROR, MEMORIA NO OTROGADA");
		exit(0);
	}
	
	head->posX=x;
	head->posY=y;
	head->idxColor=0;
	
	//Creamos su primer nodo de direccion, y lo llenamos con los parametros de la funcion
	head->dir=(Direction *) malloc(sizeof(Direction));
	if(head->dir==NULL){
		printf("\n\nERROR, MEMORIA NO OTROGADA");
		exit(0);
	}
	head->dir->dirX=_dirx;
	head->dir->dirY=_diry;
	
	//Su siguiente nodo de direccion no existe aun, entonces es Nulo, y el nodo anterior es el mismo nodo
	head->dir->nextDir=NULL;
	head->dir->prevDir=head->dir;
	
	head->lastDir=head->dir;
	
	//Como es la cabeza, no tiene parte siguiente y la parte anterior, es si misma
	head->prevBody=head;
	head->nextBody=NULL;
	
	return head;
}

//Funcion que crea una parte de serpiente en la cola cuando el jugador come una fruta
//Recibe el puntero de la cola, y las coordenadas que tendra la nueva parte
void newSnakePart(Snake **end, int x, int y){
	Snake *newPart;
	newPart=(Snake *) malloc(sizeof(Snake));
	if(newPart==NULL){
		printf("\n\nERROR, MEMORIA NO OTROGADA");
		exit(0);
	}
	newPart->posX=x;
	newPart->posY=y;
	
	//Para que el movimiento sea como el de la serpiente, hereda la ultima direccion de la parte previa
	newPart->dir=(Direction *) malloc(sizeof(Direction));
	if(newPart->dir==NULL){
		printf("\n\nERROR, MEMORIA NO OTROGADA");
		exit(0);
	}
	newPart->dir->dirX = (*end)->lastDir->dirX;
	newPart->dir->dirY = (*end)->lastDir->dirY;
	//Su sig direccion es Nula porque solo tiene 1 direccion
	newPart->dir->nextDir=NULL;
	newPart->dir->prevDir=newPart->dir;
	newPart->lastDir=newPart->dir;
	
	newPart->nextBody=NULL;
	newPart->prevBody=*end;
	
	//Su indice de color es 0 o 1 si el de la parte previa es 1 o 0 respec,
	//esto, para que sean colores alternados
	newPart->idxColor=(newPart->prevBody->idxColor+1)%2;
	
	//Acomodamos el puntero del final a la nueva cola y lo enlazamos (por eso es doble puntero)
	(*end)->nextBody=newPart;
	*end=newPart;
	
	return;
}

//Imprime serpiente, recibe el puntero cabeza y el vector de colores
void printSnake(Snake *s, int *snakeColors){
	Snake *aux;
	aux=s;
	int i=0;
	while(aux!=NULL){
		GotoXY(aux->posX, aux->posY);
		setColor(snakeColors[aux->idxColor]);
		if(i!=0){
			printf("o");
		}else{ //Si i=0, se esta imprimiendo la cabeza, que tendra un distinto simbolo para que se distinga
			printf("%c", 233);
		}
		i++;
		aux=aux->nextBody;
	}
	return;
}

void deleteSnake(Snake *s){
	Snake *aux;
	aux=s;
	while(aux!=NULL){
		GotoXY(aux->posX, aux->posY);
		printf(" ");
		aux=aux->nextBody;
	}
	return;
}

//Funcion que modifica las direcciones de toda
//la serpiente cuando la serpiente da vuelta
void changeSnakePosition(Snake *s){
	Snake *aux=s;
	while(aux!=NULL){
		//Movemos sus coordenadas basandose en la direccion que lleva
		aux->posX+=aux->lastDir->dirX;
		aux->posY+=aux->lastDir->dirY;
		
		//Cuando se llega aqui, ya se movio, y falta asignar la nueva direccion
		addDirectionNode(&(aux->lastDir));
		if(aux->prevBody==aux){ //Caso en que el puntero es la cabeza, hereda la direccion anterior
			aux->lastDir->dirX=aux->lastDir->prevDir->dirX;
			aux->lastDir->dirY=aux->lastDir->prevDir->dirY;
		}else{ //Si no es el puntero cabeza, hereda la penultima direccion de la parte de serpiente previa
		//El algoritmo implica que cada parte se mueve dependiendo de la direccion de su parte anterior y 
		//no directamente de la direccion que el usuario pulsa
			aux->lastDir->dirX=aux->prevBody->lastDir->prevDir->dirX;
			aux->lastDir->dirY=aux->prevBody->lastDir->prevDir->dirY;
		}
			
		aux=aux->nextBody;
	}
	return;
} 

//Funcion que crea un nuevo nodo de direccion (solo pide la memoria), y enlaza a la lista previa
void addDirectionNode(Direction **finalDir){
	Direction *newDir=(Direction *) malloc(sizeof(Direction));
	if(newDir==NULL){
		printf("\n\nERROR, MEMORIA NO OTROGADA");
		exit(0);
	}
	newDir->prevDir=*finalDir;
	newDir->nextDir=NULL;
	(*finalDir)->nextDir=newDir;
	*finalDir=newDir;
	return;
}

//Funcion que agrega un nodo de direccion y lo llena (solo se usa con el puntero cabeza) y
//cuando el usuario pulsa una tecla de movimiento
void writeDirections(Snake *head, int dx, int dy){
	Snake *aux=head;
	addDirectionNode(&(head->lastDir));
	head->lastDir->dirX=dx;
	head->lastDir->dirY=dy;
}

//Funcion que crea e imprime la comida, recibe el puntero cabeza y direcion donde almacenara
//las coordenadas de la fruta
void setRandomFood(Snake *head, int *x, int *y){
	//Llenamos las coordenadas al azar (y que esten dentro del tablero)
	*x=rand()%(FINAL_COL-2*VERTICAL_WIDTH-INITIAL_COL)+INITIAL_COL+VERTICAL_WIDTH;
	*y=rand()%(FINAL_ROW-2*HORIZONTAL_WIDTH-INITIAL_ROW)+INITIAL_ROW+HORIZONTAL_WIDTH;
	
	//Checamos que la comida no caiga sobre el cuerpo de la serpiente,
	//si lo hace, llamamos a la funcion de nuevo
	Snake *aux=head;
	while(aux!=NULL){
		if((*x)==aux->posX && (*y)==aux->posY){
			setRandomFood(head, x, y);
			return;
		}
		aux=aux->nextBody;
	}
	
	GotoXY(*x, *y); //Imprimimos la comida
	setColor(0);
	printf("%c", 238);
	return;
}

bool checkCollision(Snake *head){
	//Checamos que la serpiente no haya chocado con las paredes
	if(head->posX<=INITIAL_COL+VERTICAL_WIDTH-1 || head->posX>=FINAL_COL-VERTICAL_WIDTH+1){
		GotoXY(INITIAL_COL, FINAL_ROW+2);
		setColor(0);
		printf("CHOCASTE CON UNA PARED! PERDISTE :(");
		Beep(1200, 20); //Sonido de que perdiste
		Beep(1200, 20);
		Beep(1200, 20);
		Beep(1200, 20);
		return true;
	}
	if(head->posY<=INITIAL_ROW+HORIZONTAL_WIDTH-1 || head->posY>=FINAL_ROW-HORIZONTAL_WIDTH+1){
		GotoXY(INITIAL_COL, FINAL_ROW+2);
		setColor(0);
		printf("CHOCASTE CON UNA PARED! PERDISTE :(");
		Beep(1200, 20);
		Beep(1200, 20);
		Beep(1200, 20);
		Beep(1200, 20);
		return true;
	}
	
	//Checamos que la serpiente no haya chocado consigo misa
	Snake *aux1=head;
	while(aux1!=NULL){
		Snake *aux2=aux1->nextBody;
		while(aux2!=NULL){
			if(aux1->posX==aux2->posX && aux1->posY==aux2->posY){
				GotoXY(INITIAL_COL, FINAL_ROW+2);
				setColor(0);
				printf("CHOCASTE CONTIGO MISMO! PERDISTE :(");
				Beep(1200, 20);
				Beep(1200, 20);
				Beep(1200, 20);
				Beep(1200, 20);
				return true;
			}
			aux2=aux2->nextBody;
		}
		aux1=aux1->nextBody;
	}
	return false;
}

//Funcion que corre una partida
void game(Player *player, Snake *head, int *snakeColors, int *wallsColors){
	GotoXY(INITIAL_COL, INITIAL_ROW-2); //Mensaje antes de comenzar
	setColor(6);
	printf("%cEstas listo para jugar? Pulsa cualquier tecla para empezar.", 168);
	getch();
	
	int i;
	for(i=INITIAL_COL; i<INITIAL_COL+60; i++){ //Borramos el mensaje de comenzar
		GotoXY(i, INITIAL_ROW-2);
		printf(" ");
	}
	
	Snake *end=head;
	char charDir='d'; //Caracter que guarda la tecla de direccion, en este caso, la serpiente va hacia la derecha
	
	player->score=0;
	player->level=0;
	
	GotoXY(INITIAL_COL, INITIAL_ROW-2);
	setColor(6);
	printf("Player: %s", player->name);
	GotoXY(INITIAL_COL, INITIAL_ROW-1);
	printf("Level: %.2d", player->level);
	GotoXY(INITIAL_COL+15, INITIAL_ROW-1);
	printf("Score: %.2d", player->score);
	
	int speed=250; //Rapidez del juego
	bool endgame=false;
	
	int foodX, foodY;
	setRandomFood(head, &foodX, &foodY);
	
	clock_t startTime=clock(); //Iniciamos conteo de tiempo
	while(endgame==false){
		printSnake(head, snakeColors);
		GotoXY(0,0);
		
		if(head->nextBody==NULL){ //Caso en que solo se tiene la cabeza
			//Este caso solo es porque cuando inicias, puedes cambiar a direccion contraria
			if(kbhit()){
				char hold=getch();
				hold=tolower(hold);
					//Se determina que tecla se presiono, y se asigna la nueva direccion a la cabeza 
					switch(hold){
						case 'w': writeDirections(head, 0, -1); break; //Arriba
						case 's': writeDirections(head, 0, 1); break; //Abajo
						case 'd': writeDirections(head, 1, 0); break; //Derecha
						case 'a': writeDirections(head, -1, 0 ); //Izquierda
					}
				charDir=hold;
			}
		}else if(kbhit()){ //Si no es la cabeza, tienes al menos dos partes de serpiente, 
			//y no puedes cambiar a direccion contraria (ej. der a izq)
			char hold=getch();
			hold=tolower(hold);
			
			//La condicion es que no vayas a direccion contraria
			if((charDir=='w' && hold!='s') || (charDir=='s' && hold!='w') || (charDir=='a' && hold!='d') || (charDir=='d' && hold!='a')){
				switch(hold){
					case 'w': writeDirections(head, 0, -1); break;
					case 's': writeDirections(head, 0, 1); break;
					case 'd': writeDirections(head, 1, 0); break;
					case 'a': writeDirections(head, -1, 0 );
				}
				charDir=hold;
			}
		}
		
		//Corroboramos si el jugador comio la fruta
		if(head->posX==foodX && head->posY==foodY){
			Beep(900, 50);
			(player->score)++;
			player->level=(int) (player->score)/5; //Asi defini la formula del nivel del jugador
			GotoXY(INITIAL_COL, INITIAL_ROW-1);
			setColor(6);
			printf("Level: %.2d", player->level);
			GotoXY(INITIAL_COL+15, INITIAL_ROW-1);
			printf("Score: %.2d", player->score);
			
			//Creamos la nueva parte de la serpiente, basandonos en la direccion de la cola
			if(end->lastDir->dirX==0 && end->lastDir->dirY==-1){
				newSnakePart(&end, end->posX, end->posY+1);
			}else if(end->lastDir->dirX==0 && end->lastDir->dirY==1){
				newSnakePart(&end, end->posX, end->posY-1);
			}else if(end->lastDir->dirX==1 && end->lastDir->dirY==0){
				newSnakePart(&end, end->posX-1, end->posY);
			}else if(end->lastDir->dirX==-1 && end->lastDir->dirY==0){
				newSnakePart(&end, end->posX+1, end->posY);
			}
			setRandomFood(head, &foodX, &foodY); //Ponemos la nueva comida
			
			switch(player->level){ //Aumentamos dificultad
				case 0: break;
				case 1: speed=200; break;
				case 2: speed=150; break;
				case 3: speed=100; break;
				case 4: speed=75; break;
				case 5: speed=65; break;
				case 6: speed=60; break;
				case 7: speed=55;
			}
		}
	
		Sleep(speed); //Una pausa para que se pueda jugar
		deleteSnake(head);
		changeSnakePosition(head); //Cambiamos las nuevas coordenadas de la serpiente
		endgame=checkCollision(head);
		
		//Imprimimos el tiempo hasta el momento
		printTime((int) (clock()-startTime)/CLOCKS_PER_SEC);	
	}
	printSnake(head, snakeColors);
	
	player->time=(int) (clock()-startTime)/CLOCKS_PER_SEC; //Tiempo total de la partida (ya que acabo')
	GotoXY(0,0);
	setColor(5);
	printf("Presiona cualquier tecla para continuar.");
	getch();
	getch();
	system("cls");
}

//Libera memoria de los nodos de direccion de cada parte de la serpiente
void freeSnakePartDirections(Snake *snakePart){ 
	Direction *aux1;
	aux1=snakePart->dir;
	while(aux1!=NULL){
		Direction *aux2=aux1;
		aux1=aux1->nextDir;
		free(aux2);
	}
	snakePart=NULL;
}

void freeSnake(Snake *s){ //Libera memoria de toda la serpiente
	Snake *aux1=s;
	while(aux1!=NULL){
		Snake *aux2=aux1;
		freeSnakePartDirections(aux2);
		free(aux2);
		aux2=NULL;
		aux1=aux1->nextBody;
	}
	aux1=NULL;
	s=NULL;
	return;
}
