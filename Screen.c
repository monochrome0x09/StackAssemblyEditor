#include "Screen.h"

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
    COORD pos;*/
    for (int y = top; y <= bottom; y++) {
        /*pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(hConsole, pos);*/

        set_console_position(x, y);
        putchar(ch);
    }
}