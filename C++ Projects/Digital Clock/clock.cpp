#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

char* removeWhiteSpace(char* input)
{
    int i, j;
    char* output= input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
        if (input[i]!=' ')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}

int main() {
    int h, m, c, i;
    int s = 0;
    char amPm[3];
    char am[3] = "AM";
    char input[5];

    std::cout << "Enter time (HH:MM): ";
    std::cin >> input;
    char* input_stripped = removeWhiteSpace(input);

    char *ptr;
    ptr = strtok(input_stripped, ":");

    h = std::stoi(ptr);
    ptr = strtok (NULL, ":");
    m = std::stoi(ptr);
    ptr = NULL;
   
    std::cout << "Enter AM or PM: ";
    std::cin >> amPm;

    char* amPm_stripped = removeWhiteSpace(amPm);
    for (int i = 0; i < sizeof(amPm_stripped)/sizeof(amPm_stripped[0])-1; i++) {
        amPm_stripped[i] &= ~' ';
    }

    if (strcmp(amPm_stripped, am)) {
        c = 0;
    }
    else {
        c = 1;
    }

    while (true) {
        s++;
        if (s > 59) {
            m++;
            s = 0;
        }
        if (m > 59) {
            h++;
            m = 0;
        }
        if (h > 12) {
            h = 1;
        }
        if (h > 11 && m ==0 && s==0 ) {
            c = (c + 1) % 2;
        }

        Sleep(1000);
        system("cls");
        if (s < 10 && m < 10) {
            std::cout << h << ":0" << m << ":0" << s;
        }
        else if (s < 10 && m >= 10){
            std::cout << h << ":" << m << ":0" << s;
        }
        else if (s >= 10 && m < 10) {
            std::cout << h << ":0" << m << ":" << s;
        }
        else {
            std::cout << h << ":" << m << ":" << s;
        }
        if (c == 0) {
            std::cout << "   AM";
        }
        else {
            std::cout << "   PM";
        }
    }
    return 0;
}