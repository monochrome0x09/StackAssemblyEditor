#pragma once
#include <windows.h>
#include <stdio.h>

struct Screen {
    int columns;
    int rows;
};

struct File {
    char name[20];
    char code[50][100];
};

struct Screen get_screen();

void set_console_position(int x, int y);

void set_text_color(WORD color);

void draw_vertical_line(int x, int top, int bottom, char ch);
