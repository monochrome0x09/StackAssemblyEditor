#include "Menu.h"
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define H 196

void createfile_setup(struct File files[])
{
    system("cls");

    char* txt = "Input the name of file you create";
    struct Screen screen = get_screen();

    set_console_position(screen.columns / 2 - (strlen(txt)) / 2, screen.rows / 2 - 2);
    printf("%s", txt);

    //set_console_position(screen.columns / 2 - 10, screen.rows / 2 + 2);
    //printf("----------------------");

    draw_horizontal_line(screen.rows / 2 + 2, screen.columns / 2 - 10, screen.columns / 2 + 10, H);

    set_console_position(screen.columns / 2 - 10, screen.rows / 2 + 1);

    int empty_index = -1;
    for (int i = 0; i < 10; i++) {
        if (strstr(files[i].name, "undefined") != NULL) {
            empty_index = i;
            break;
        }
    }

    if (empty_index == -1) {
        printf("No empty slots!");
        _getch();
        return;
    }

    scanf_s("%s", files[empty_index].name, (unsigned)sizeof(files[empty_index].name));

    // 코드 초기화
    for (int i = 0; i < 50; i++) {
        files[empty_index].code[i][0] = '\0';
    }
}

void deletefile_setup(struct File files[]) {
    int selected = 0;

    while (1) {
        system("cls");  // 루프마다 화면 새로고침

        int max_size = 0;
        int file_indices[10];  // 실제 인덱스 매핑
        struct Screen screen = get_screen();

        char* txt = "Select the file you want to delete";
        set_console_position(screen.columns / 2 - (strlen(txt) / 2), screen.rows / 2 - 2);
        printf("%s", txt);

        int item_y = 0;
        for (int i = 0; i < 10; i++) {
            if (strstr(files[i].name, "undefined") != NULL) {
                continue;
            }

            file_indices[item_y] = i;  // 매핑 저장

            set_console_position(screen.columns / 2 - (strlen(txt) / 4) + 4, screen.rows / 2 + item_y);

            if (selected == item_y) set_text_color(YELLOW);  // item_y와 비교
            else set_text_color(WHITE);

            printf("%d. %s.asm", item_y + 1, files[i].name);

            item_y += 1;
        }

        max_size = item_y;

        if (max_size == 0) {
            set_console_position(screen.columns / 2 - 10, screen.rows / 2);
            printf("No files to delete!");
            _getch();
            break;
        }

        int ch = _getch();

        if (ch == 224) {
            ch = _getch();
            if (ch == 72 && selected > 0) selected--;  // UP (72)
            else if (ch == 80 && selected < max_size - 1) selected++;  // DOWN (80)
        }
        else if (ch == 13) {  // Enter
            int actual_index = file_indices[selected];  // 실제 인덱스

            strcpy_s(files[actual_index].name, 20, "undefined");

            // 모든 코드 라인 초기화
            for (int i = 0; i < 50; i++) {
                files[actual_index].code[i][0] = '\0';
            }

            break;
        }
        else if (ch == 27) {  // ESC
            break;
        }
    }
}

void selectfile_setup(struct File files[]) {
    int selected = 0;

    while (1) {
        system("cls");  // 루프마다 화면 새로고침

        int max_size = 0;
        int file_indices[10];  // 실제 인덱스 매핑
        struct Screen screen = get_screen();

        char* txt = "List of file";
        set_console_position(screen.columns / 2 - (strlen(txt) / 2), screen.rows / 2 - 2);
        set_text_color(CYAN);
        printf("%s", txt);
        set_text_color(WHITE);

        int item_y = 0;
        for (int i = 0; i < 10; i++) {
            if (strstr(files[i].name, "undefined") != NULL) {
                continue;
            }

            file_indices[item_y] = i;  // 매핑 저장

            set_console_position(screen.columns / 2 - (strlen(txt) / 2), screen.rows / 2 + item_y);

            if (selected == item_y) set_text_color(YELLOW);  // item_y와 비교
            else set_text_color(WHITE);

            printf("%d. %s.asm", item_y + 1, files[i].name);

            item_y += 1;
        }

        max_size = item_y;

        if (max_size == 0) {
            set_console_position(screen.columns / 2 - 9, screen.rows / 2);
            printf("No files available");
            _getch();
            break;
        }

        int ch = _getch();

        if (ch == 224) {
            ch = _getch();
            if (ch == 72 && selected > 0) selected--;  // UP (72)
            else if (ch == 80 && selected < max_size - 1) selected++;  // DOWN (80)
        }
        else if (ch == 13) {  // Enter
            int actual_index = file_indices[selected];  // 실제 인덱스
            editor_setup(&files[actual_index]);
            break;
        }
        else if (ch == 27) {  // ESC
            break;
        }
    }
}
