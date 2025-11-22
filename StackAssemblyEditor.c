#include <stdio.h>
#include <windows.h>
#include "Screen.h"
#include "Menu.h"

#define MENU_COUNT 4

#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

int menu_setup() {
    system("cls");

    char* items[MENU_COUNT] = { "1. Create File", "2. Delete File", "3. Select Files", "4. Exit"};

    int selected = -1;

    while (1) {
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
        else if (ch == 9) {
            system("cls");
        }
    }

    set_text_color(WHITE);

    return selected;
}

int main() {
    struct File files[10];
    //editor_setup();

    for (int i = 0; i < 10; i++) {
        strcpy_s(files[i].name, 20, "undefined");
    }


    while (1) {
        int selection = menu_setup();

        switch (selection) {
        case 0:
            createfile_setup(files);
            break;

        case 1:
            deletefile_setup(files);
            break;

        case 2:
            selectfile_setup(files);
            break;

        case 3:
            exit(1);
            break;

        default:
            break;
        }
    }


    return 0;
}