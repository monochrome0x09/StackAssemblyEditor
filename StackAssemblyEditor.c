#include <stdio.h>
#include <windows.h>
#include "Screen.h"

#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define MENU_COUNT 5

/*
struct Screen {
    int columns;
    int rows;
};

struct Screen get_screen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    struct Screen screen = { columns, rows };

    return screen;
}

void set_console_position(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void set_text_color(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void draw_vertical_line(int x, int top, int bottom, char ch) {
    /*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    for (int y = top; y <= bottom; y++) {

        set_console_position(x, y);
        putchar(ch);
    }
}*/

void editor_setup() {
    struct Screen screen = get_screen();
    int x = screen.columns / 1.5; // 콘솔의 오른쪽에 vertical line
    draw_vertical_line(x, 0, screen.rows - 1, '|'); 

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(hConsole, pos);
}

void menu_setup() {
    char* items[MENU_COUNT] = { "1. Create File", "2. Delete File", "3. View Files", "4. Select File", "5. Exit"};
    
    int selected = -1;

    while (1) {
        system("cls");

        struct Screen screen = get_screen();

        set_text_color(CYAN);
        char* title = "--- Stack Assembly Editor ---";
        set_console_position(screen.columns / 2 - (strlen(title) / 2), screen.rows / 2 - 3);
        printf("%-20s", title);
        
        for (int i = 0; i < MENU_COUNT; i++) {
            set_console_position(screen.columns / 2 - (strlen(items[0]) / 2), screen.rows / 2 + (i * 2));

            if (i == selected) set_text_color(YELLOW);
            else set_text_color(WHITE);

            printf("%-20s", items[i]);
        }

        int ch = _getch();
        if (ch == 224) { // 특수키 (방향키 등)
            ch = _getch();

            if (ch == 'H' && selected > 0) selected--; // UP

            else if (ch == 'P' && selected < MENU_COUNT - 1) selected++; // DOWN
        }
        else if (ch == 13) { // Enter
            break;
        }
    }

    set_text_color(WHITE);
}

int main() {
    //editor_setup();
    menu_setup();


    return 0;
}