#include "Editor.h"

#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

void editor_setup(struct File* file) {
    system("cls");

    char code[50][100] = { 0 };  // ← file->code와 크기 일치 (100바이트)

    // file 구조체에 이미 작성한 코드를 임시 변수로 복사
    int existed = 0;
    while (existed < 50 && file->code[existed][0] != '\0') {
        strcpy_s(code[existed], 100, file->code[existed]);  // ← 100으로 수정
        existed++;
    }

    int line = 0;
    int pos = 0;

    while (1) {
        system("cls");

        struct Screen screen = get_screen();

        // 파일 이름 표시
        set_console_position(2, 1);
        set_text_color(CYAN);
        printf("File: %s.asm", file->name);
        set_text_color(WHITE);

        // 세로선 그리기
        int divider_x = (int)(screen.columns / 1.5);
        draw_vertical_line(divider_x, 0, screen.rows - 1, '|');

        // 코드 영역 표시
        set_console_position(2, 3);
        printf("Code Editor (ESC to exit)");

        // 스택 영역 표시
        set_console_position(divider_x + 3, 1);
        printf("Stack Memory");

        // 상태 표시
        set_console_position(2, screen.rows - 2);
        printf("Line: %d | Pos: %d", line + 1, pos);

        // 모든 코드 라인 출력
        for (int i = 0; i <= line && i < 50; i++) {
            set_console_position(2, 5 + i);
            printf("%s", code[i]);
        }

        // 커서 위치 표시
        set_console_position(pos + 2, line + 5);
        set_text_color(YELLOW);
        printf("_");
        set_text_color(WHITE);

        char ch = _getch();

        if (ch == 27) {  // ESC
            break;
        }
        else if (ch == 8) {  // Backspace
            if (pos > 0) {
                int len = strlen(code[line]);
                for (int i = pos - 1; i < len; i++) {
                    code[line][i] = code[line][i + 1];
                }
                pos--;
            }
            else if (line > 0) {
                line--;
                pos = strlen(code[line]);
            }
        }
        else if (ch == 13) {  // Enter
            if (line < 49) {
                line++;
                pos = 0;
            }
        }
        else if (ch == 224 || ch == 0) {  // 방향키
            ch = _getch();
            if (ch == 75 && pos > 0) pos--;  // 왼쪽
            else if (ch == 77 && pos < strlen(code[line])) pos++;  // 오른쪽
            else if (ch == 72 && line > 0) {  // 위
                line--;
                if (pos > strlen(code[line])) pos = strlen(code[line]);
            }
            else if (ch == 80 && line < 49) {  // 아래
                line++;
                if (pos > strlen(code[line])) pos = strlen(code[line]);
            }
        }
        else if (ch >= 32 && ch <= 126) {  // 출력 가능한 문자
            int len = strlen(code[line]);

            if (len < 99) {  // ← 버퍼 크기 (100 - 1)
                for (int i = len; i >= pos; i--) {
                    code[line][i + 1] = code[line][i];
                }

                code[line][pos] = ch;
                pos++;
            }
        }
    }
}
