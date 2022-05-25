#include <iostream>

char square[9] = {'1','2','3','4','5','6','7','8','9'};

int checkWin();
void displayBoard();

int main()
{
    int choice, gameState;
	int player = 1;
    char mark;

    do {
        displayBoard();
        player = (player % 2) ? 1 : 2;

        std::cout << "Player " << player << ", enter a number:  ";
        std::cin >> choice;

        mark = (player % 2) ? 'X' : 'O';

        if (choice == 1 && square[0] == '1') {
            square[0] = mark;
        } else if (choice == 2 && square[1] == '2') {
            square[1] = mark;
        } else if (choice == 3 && square[2] == '3') {
            square[2] = mark;
        } else if (choice == 4 && square[3] == '4') {
            square[3] = mark;
        } else if (choice == 5 && square[4] == '5') {
            square[4] = mark;
        } else if (choice == 6 && square[5] == '6') {
            square[5] = mark;
        } else if (choice == 7 && square[6] == '7') {
            square[6] = mark;
        } else if (choice == 8 && square[7] == '8') {
            square[7] = mark;
        } else if (choice == 9 && square[8] == '9') {
            square[8] = mark;
        } else {
            std::cout << "Invalid move ";
            player--;
        }
        gameState = checkWin();
        player++;
    } while (gameState == -1);
    displayBoard();
    if (gameState == 1) {
        std::cout<<"==> Player "<< --player <<" wins!";
    } else {
        std::cout<<"==> Game draw ...";
    }
    return 0;
}

// 1 if win, 0 if draw, -1 if game is still going
int checkWin() { 
    if (square[0] == square[1] && square[1] == square[2]) {
        return 1;
    } else if (square[3] == square[4] && square[4] == square[5]) {
        return 1;
    } else if (square[6] == square[7] && square[7] == square[8]) {
        return 1;
    } else if (square[0] == square[3] && square[3] == square[6]) {
        return 1;
    } else if (square[1] == square[4] && square[4] == square[7]) {
        return 1;
    } else if (square[2] == square[5] && square[5] == square[7]) {
        return 1;
    } else if (square[0] == square[4] && square[4] == square[8]) {
        return 1;
    } else if (square[2] == square[4] && square[4] == square[6]) {
        return 1;
    } else if (square[0] != '1' && square[1] != '2' && square[2] != '3' && square[3] != '4' && square[4] != '5' && square[5] != '6' && square[6] != '7' && square[7] != '8' && square[8] != '9') {
        return 0;
    } else {
        return -1;
    }
}

void displayBoard() {
    system("cls");
    std::cout << "\n\n\tTic Tac Toe\n\n";
    std::cout << "Player 1 (X)  -  Player 2 (O)" << std::endl << std::endl;
    std::cout << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << square[0] << "  |  " << square[1] << "  |  " << square[2] << std::endl;
    std::cout << "_____|_____|_____" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << square[3] << "  |  " << square[4] << "  |  " << square[5] << std::endl;
    std::cout << "_____|_____|_____" << std::endl;
    std::cout << "     |     |     " << std::endl;
    std::cout << "  " << square[6] << "  |  " << square[7] << "  |  " << square[8] << std::endl;
    std::cout << "     |     |     " << std::endl << std::endl;
}
