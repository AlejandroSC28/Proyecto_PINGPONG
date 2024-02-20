
#include <iostream>
#include <raylib.h>
#include <conio.h>
#include <fstream>
using namespace std;

Color Green = Color{ 38,185,154,255 };
Color Dark_Green = Color{ 20,160,133,255 };
Color Light_Green = Color{ 129,204,184,255 };
Color Yellow = Color{243,213,91,255 };
int player_score = 0;
int player_2_score = 0;
bool gameOver = false;


enum MenuOption {
	PLAY,
	SCORES,
	CREDITS,
	EXIT,
};
//Variables para almacenar las victorias de cada jugador 
int playerwins = 0;
int player_2wins = 0;
//Funcion para guardar los puntajes 
void SaveScores() {
	ofstream scoresFile("scores.txt");
	if (scoresFile.is_open()) {
		scoresFile << playerwins << endl;
		scoresFile << player_2wins << endl;
	}
}
//Funcion para cargar los puntajes desde un archivo de texto
void LoadScores() {
	ifstream scoresFile("scores.text");
		if(scoresFile.is_open()) {
			scoresFile >> playerwins;
			scoresFile >> player_2wins;

			scoresFile.close();
		}
}
//funcion para mostrar los puntajes de los jugadores
void ShowScores() {
	LoadScores();
	ClearBackground(RAYWHITE);
	DrawText("Puntajes", 150,420, 30, RED);
	//mostrar las veces que ha ganado cada jugador
	DrawText(TextFormat("Jugador 1: % d", playerwins), 150, 470, 20, WHITE);
	DrawText(TextFormat("Jugador 2: %d ", player_2wins), 150,500, 20,WHITE);
}

class Ball {
public:
	float x, y;
	int speed_x, speed_y;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius,YELLOW);

	}
	void Update() {
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight()|| y - radius <= 0){
			speed_y *= -1;
		}
		if (x + radius >= GetScreenWidth()) {
			player_2_score++;
			ResetBall();
		}
		if (x - radius <= 0) {
			player_score++;
			ResetBall();
		}
	}
 	void ResetBall() {
			x = GetScreenWidth() / 2;
			y = GetScreenHeight() / 2;
			int speed_choices[2] = { -1,1 };
			speed_x *= speed_choices[GetRandomValue(0, 1)];
			speed_y *= speed_choices[GetRandomValue(0, 1)];
	}
};
class  Paddle {
public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangleRounded(Rectangle{ x,y,width,height }, 0.8, 0, WHITE);

	}
	void Update() {
		if (IsKeyDown(KEY_UP)) {
			y = y - speed;
		}
		if (IsKeyDown(KEY_DOWN)) {
			y = y + speed;
		}
		if (y <= 0) {
			y = 0;
		}
		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}

};
class Paddle_2 {
public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangleRounded(Rectangle{ x,y,width,height }, 0.8, 0, WHITE);

	}
	void Update() {
		if (IsKeyDown(KEY_W)) {
			y = y - speed;
		}
		if (IsKeyDown(KEY_S)) {
			y = y + speed;
		}
		if (y <= 0) {
			y = 0;
		}
		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}
};

Ball ball;
Paddle player;
Paddle_2 player_2;
int main()
{
	cout << "Starting the game" << endl;
	const int screen_width = 1280;
	const int screen_height = 800;
	bool inGame = false;
	//variable para almacenar la opcion seleccionada del menu
	MenuOption selectedOption = PLAY;
	InitWindow(screen_width, screen_height, "My pong Game");
	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speed_x = 12;
	ball.speed_y = 12;

	player.width = 25;
	player.height = 120;
	player.x = screen_width - player.width - 10;
	player.y = screen_height / 2 - player.height / 2;
	player.speed = 10;

	player_2.width = 25;
	player_2.height = 120;
	player_2.x = 10;
	player_2.y = screen_height / 2 - player_2.height / 2;
	player_2.speed =10;


	const char* winnerText = nullptr;
	while (WindowShouldClose() == false) {
		//verificar si estamos en el juego o en el menu 
		if (!inGame) {
			//dibujar el menu
			BeginDrawing();
			ClearBackground(DARKBLUE);
			DrawText("PING-PONG", screen_width / 2 - MeasureText("PING-PONG",150) / 2, 100,150, WHITE);
			DrawText("1. Jugar", screen_width / 2 - MeasureText("1.Jugar", 60) / 2, 330, 60, YELLOW);
			DrawText("2. Puntajes", screen_width / 2 - MeasureText("2.Puntajes", 60) / 2, 420, 60, YELLOW);
			DrawText("3. Creditos", screen_width / 2 - MeasureText("3. Creditos", 60) / 2, 510, 60, YELLOW);
			DrawText("4. Salir", screen_width / 2 - MeasureText("4. Salir", 60) / 2, 600, 60, YELLOW);
			EndDrawing();

			//Verificar si se ha presionado alguna tecla 
			if (IsKeyPressed(KEY_ONE)) {
				selectedOption = PLAY;
				inGame = true;
			}
			else if (IsKeyPressed(KEY_TWO)) {
				selectedOption = SCORES;
			}
			else if (IsKeyPressed(KEY_THREE)) {
				selectedOption = CREDITS;
			}
			else if (IsKeyPressed(KEY_FOUR)) {
				selectedOption = EXIT;
				break;
			}
		}
		else {
			//si estamos en el juego 
			//verificar si se ha presionado la barra espaciadora
			if (IsKeyPressed(KEY_SPACE)) {
				inGame = false;
				selectedOption = PLAY; 
			}
			//LOGICA DEL JUEGO
			BeginDrawing();
			//Colisiones 
			if (CheckCollisionCircleRec(Vector2{ ball.x , ball.y }, ball.radius, Rectangle{ player.x,player.y,player.width,player.height })) {
				ball.speed_x *= -1;
			}
			if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player_2.x,player_2.y,player_2.width,player_2.height })) {
				ball.speed_x *= -1;
			}
			//Updating
			ball.Update();
			player.Update();
			player_2.Update();
			//Draws
			if (player_2_score > 10 || player_score >= 10) {
				ClearBackground(Dark_Green);
				if (player_score >= 10) {
					playerwins++;
					DrawText("!Jugador 1 es el ganador!", 240, 340, 60, RED);

				}else if (player_2_score >= 10){
					player_2wins++;
					DrawText("!Jugador 2 es el ganador! ", 240, 340, 60, RED);
				}
			}
			else {
				ClearBackground(Dark_Green);
      			DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
     		   	DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
	     		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
     		  	ball.Draw();
	    		player_2.Draw();
    			player.Draw();
	    		DrawText(TextFormat("%i", player_2_score), screen_width / 4 - 20, 20, 80, WHITE);
	    		DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
			}
	    	EndDrawing();
		}
		//Guardar los puntajes antes de salir del juego
		SaveScores();
		if (selectedOption == SCORES) {
			ShowScores();
		}
		if (selectedOption == CREDITS) {
			DrawText("Hecho por [ALEJANDRO SC]", 10, 10, 20, PURPLE);
		}
		
	}
	CloseWindow();
	return 0;
}
