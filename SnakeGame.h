#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>


//Macros de los colores
#define  I_RED  0
#define  I_GRN  1
#define  I_YEL  2
#define  I_BLU  3
#define  I_MAG  4
#define  I_CYN  5
#define  I_WHT  6

#define FOREGROUND_WHITE        (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW           (FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN             (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA          (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK        0

#define FOREGROUND_INTENSE_RED      (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN    (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE     (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE    (FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW   (FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN     (FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA  (FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)


//Macros de las dimensiones
#define INITIAL_COL 20
#define FINAL_COL 80
#define INITIAL_ROW 4
#define FINAL_ROW 28
#define HORIZONTAL_WIDTH 1
#define VERTICAL_WIDTH 2

//Maximo tamanio de longitud de jugador
#define MAX_LENGTH_NAME 26

//Macros para los nombres de archivos txt a usar
#define INSTRUCTIONS_FILE "instructions.txt"
#define PLAYER_STATS_FILE "statistics.txt"
#define WELCOME_FILE "welcome.txt"
#define GOODBYE_FILE "goodbye.txt"

//Tipos de datos definidos
//Esta es un nodo, la direccion es su movimiento en X y Y, que son 0 o 1
typedef struct Direction{ 
	int dirX, dirY;
	//Contiene dos punteros, a un nodo de direccion anterior y siguiente
	struct Direction *nextDir;
	struct Direction *prevDir;
} Direction;

//Este tipo de dato es de cada una de las partes de la serpiente,
//la lista enlazada de estos es la serpiente completa
typedef struct Snake{ 
	int posX, posY; //Coordenadas de su posicion
	//Punteros a su primer y ultimo nodo de direccion
	Direction *dir; 
	Direction *lastDir;
	int idxColor; //Indice de color asignado a cada parte
	//Punteros a su siguiente y anterior bolita 
	struct Snake* nextBody;
	struct Snake* prevBody;
} Snake;

typedef struct{
	int score;
	int time;
	int level;
	char *name;
} Player;

//Prototipos de funciones para mostrar texto
void GotoXY(int, int);
void setColor(int);
void showTxt(char *);
void welcomeScreen(char *);
void instructions(char *);
void drawWalls(int *);
void printTime(int);
void saveStatsTxt(Player *, char *);

void menu(char *, int *, int *);
Snake* initSnake(int, int, int, int);
void newSnakePart(Snake **, int, int);
void printSnake(Snake *, int *);
void deleteSnake(Snake *);
void changeSnakePosition(Snake *);
void addDirectionNode(Direction **);
void writeDirections(Snake *, int, int);
void setRandomFood(Snake *, int*, int *);
bool checkCollision(Snake *);
void game(Player *, Snake *, int *, int *);

void freeSnakePartDirections(Snake *);
void freeSnake(Snake *);
#endif
