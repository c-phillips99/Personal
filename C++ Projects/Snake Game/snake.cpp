#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <process.h>
#include <time.h>
#include <windows.h>

#define MAX 100  // Max size allowed
#define HEIGHT 22
#define WIDTH 78
#define INIT_SNAKE_LENGTH 4
#define DELAY 50 // Higher values make the snake go slower

// Items on the ground
#define CORNER -4
#define VERT_WALL -3
#define HORZ_WALL -2
#define SNAKE -1
#define NOTHING 0
#define FOOD 1

// Indecies for the directions array for snake
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

#define EXIT -1

// User input
int input = LEFT;
// Item that snake reached
int item = NOTHING;

// Array of directions for the snake -- UP is -1 because of using Microsoft COORD type where down is larger values (e.g. 23 is lower than 12)
static int dx[5] = {1, 0, -1, 0};
static int dy[5] = {0, -1, 0, 1};

// Array keeping track of the items on the ground
int ground[MAX][MAX];

struct Coordinate;
class snake;
void setCursorPos(int column, int row);
void clearScrean();
void initGround();
bool oppositeDirection(int direction, int input);
void firstDraw(snake* s);
void updateFood(snake* s, bool init);
void userInput(void* id);

struct Coordinate
{
	int x, y;
};

class snake
{
private:
	int length;
	int direction;
	int foodCounter;
public:
	// Position of each element of the snake's body
	Coordinate body[WIDTH * HEIGHT];
	void initSnake();
	void updateSnake();
	int getFoodCounter();
};

void snake::initSnake()
{
	length = INIT_SNAKE_LENGTH;

	// Position the head in the middle of the arena
	body[0].x = WIDTH / 2;
	body[0].y = HEIGHT / 2;

	direction = input;
	foodCounter = 0;

	int i;
	// Set the tail behind the head
	for (i = 1; i < length; i++)
	{
		body[i].x = body[i-1].x - dx[direction];
		body[i].y = body[i-1].y - dy[direction];
	}

	// Update the ground to reflect snake's position
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;
	
	return;
}

void snake::updateSnake()
{
	int i;
	Coordinate prev[WIDTH*HEIGHT];
	// Copy previous positions
	for (i = 0; i < length; i++)
	{
		prev[i].x = body[i].x;
		prev[i].y = body[i].y;
	}

	if (input != EXIT && !oppositeDirection(direction, input))
		direction = input;

	// Move head forward
	body[0].x = prev[0].x + dx[direction];
	body[0].y = prev[0].y + dy[direction];

	// See if the snake ran into an item
	if (ground[body[0].y][body[0].x] < NOTHING)
	{
		item = -1;
		return;
	}
	else if (ground[body[0].y][body[0].x] == FOOD)
	{
		item = FOOD;
		length++;
		foodCounter++;
	}
	else
	{
		ground[body[length-1].y][body[length-1].x] = NOTHING;
		item = NOTHING;

		// Erase last position on the tail beause moving forward
		setCursorPos(body[length - 1].x, body[length - 1].y);
		std::cout << " ";
	}

	// Update body positions (except for head)
	for (i = 1; i < length; i++)
	{
		body[i].x = prev[i-1].x;
		body[i].y = prev[i-1].y;
	}

	// Change the previous head to a body
	setCursorPos(body[1].x, body[1].y);
	std::cout << "+";

	// Print 'O' for new head position
	setCursorPos(body[0].x, body[0].y);
	std::cout << "O";

	// Update ground
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;

	// Speed of snake
	Sleep(DELAY);

	return;
}

int snake::getFoodCounter()
{
	return foodCounter;
}

// Sets the concole cursor position to a specific x, y coordinate for printing
void setCursorPos(int column, int row)
{
	HANDLE hStdOut;
	COORD coord;

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;

	coord.X = column;
	coord.Y = row;

	SetConsoleCursorPosition(hStdOut, coord);
	return;
}

// Clears console screen and sets cursor position to top left
void clearScreen()
{
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD home = {0, 0}; // Top left of console

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;
	
	// Get number of cells and cell attributes in current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(
		hStdOut,		// Handle to console screen buffer
		(TCHAR) ' ',	// Character to write to the buffer
		cellCount,		// Number of cells to write to
		home,		 	// Coordinates of first cell
		&count			// Variable that receives the number of characters written
		)) return;
	
	// Fill entire buffer with current colors and attributes 
	if (!FillConsoleOutputAttribute(
		hStdOut,			// Handle to console screen buffer
		csbi.wAttributes,	// Character attributes to use
		cellCount,			// Number of cells to set attribute
		home,				// Coordinate of first cell
		&count				// Variable that receives the number of characters written
		)) return;

	SetConsoleCursorPosition(hStdOut, home);
	return;
}

// Creates arena position in ground array
void initGround()
{
	for (int i = 0; i <= MAX; i++)
		for (int j = 0; j <= MAX; j++)
			ground[i][j] = NOTHING;
	
	for (int j = 0; j <= WIDTH+1; j++)
	{
		ground[0][j] = HORZ_WALL;
		ground[HEIGHT + 1][j] = HORZ_WALL;
	}

	for (int i = 0; i <= HEIGHT+1; i++)
	{
		ground[i][0] = VERT_WALL;
		ground[i][WIDTH+1] = VERT_WALL;
	}

	ground[0][0] = CORNER;
	ground[0][WIDTH+1] = CORNER;
	ground[HEIGHT+1][0] = CORNER;
	ground[HEIGHT+1][WIDTH+1] = CORNER;

	return;
}

// Checks if input is opposite to current position
bool oppositeDirection(int direction, int input)
{
	if (direction == LEFT && input == RIGHT)
		return true;
	else if (direction == RIGHT && input == LEFT)
		return true;
	else if (direction == UP && input == DOWN)
		return true;
	else if (direction == DOWN && input == UP)
		return true;
	else
		return false;
}

// Initial draw of the arena
void firstDraw(snake* s)
{
	clearScreen();
	for (int i = 0; i <= HEIGHT + 1; i++)
	{
		for (int j = 0; j <= WIDTH + 1; j++)
		{
			switch(ground[i][j])
			{
				case NOTHING:
					std::cout << " ";
					break;
				case HORZ_WALL:
					std::cout << "-";
					break;
				case VERT_WALL:
					std::cout << "|";
					break;
				case CORNER:
					std::cout << "+";
					break;
				case SNAKE:
					if (i == s->body[0].y && j == s->body[0].x) // if is head
						std::cout << "O";
					else
						std::cout << "+";
					break;
				default:
					std::cout << "x";

			}
		}
		std::cout << std::endl;
	}
	return;
}

void updateFood()
{
	int x, y;
	// Search until valid random position for food
	do
	{
		x = rand() % WIDTH + 1;
		y = rand() % HEIGHT + 1;
	} while (ground[y][x] != NOTHING);

	ground[y][x] = FOOD;

	// Print food at position
	setCursorPos(x, y);
	std::cout << "x";
	
	return;
}

void userInput(void* id)
{
	do
	{
		int ch = _getch();
		switch (ch)
		{
			case 'W': case 'w': input = UP   ; break;
			case 'S': case 's': input = DOWN ; break;
			case 'D': case 'd': input = RIGHT; break;
			case 'A': case 'a': input = LEFT ; break;
			case 27:	    	input = EXIT ; break; // 27 = ESC key
		}
	} while (input != EXIT && input >= 0);

	_endthread();
	return;
}

int main()
{
	srand(time(NULL));
	snake hungryBoi, *hbPtr;
	hbPtr = &hungryBoi;
	initGround();
	hungryBoi.initSnake();
	updateFood();
	firstDraw(hbPtr);
	_beginthread(userInput, 0, (void*)0);

	do
	{
		hungryBoi.updateSnake();
		if (item == FOOD)
			updateFood();
	} while (item >= 0 && input != EXIT);

	setCursorPos(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	std::cout << "GAME OVER";
	setCursorPos(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	std::cout << "Your score is " << hungryBoi.getFoodCounter() << "!" << std::endl;
	setCursorPos(WIDTH / 2, HEIGHT / 2);

	_getch();
	system("cls");
	return 0;
}
