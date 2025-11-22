#include "Editor.h"

#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)

#define TL 218
#define TR 191
#define BL 192
#define BR 217
#define H 196
#define V 179
#define LEFT_T 195
#define RIGHT_T 180
#define BOTTOM_T 194

#define BOX 220 

#define PUSH 0
#define POP 1
#define ADD 2
#define SUB 3
#define MOV 4


// fill은 채움의 정도
void print_stack(int divide_x, int fill) {
    struct Screen screen = get_screen();

    // 스택 영역 표시
    set_console_position(divide_x + 3, 1);
    printf("Stack Memory");

    draw_horizontal_line((int)(screen.rows / 2), divide_x + 1, screen.columns, H);


    draw_vertical_line(divide_x + 6, 4, ((int)(screen.rows / 2)) - 2, V);
    draw_vertical_line(screen.columns - 5, 4, ((int)(screen.rows / 2)) - 2, V);

    draw_horizontal_line(3, divide_x + 7, screen.columns - 6, H);
    draw_horizontal_line(((int)(screen.rows / 2)) - 2, divide_x + 7, screen.columns - 6, H);

    set_console_position(divide_x + 6, 3);
    putchar(TL);

    set_console_position(screen.columns - 5, 3);
    //wprintf("┐");
    putchar(TR);

    set_console_position(divide_x + 6, ((int)(screen.rows / 2)) - 2);
    //wprintf("└");
    putchar(BL);

    set_console_position(screen.columns - 5, ((int)(screen.rows / 2)) - 2);
    //wprintf("┘");
    putchar(BR);


    for (int i = 0; i < fill; i++) {
        draw_horizontal_line(((int)(screen.rows / 2)) - (3 + i), divide_x + 7, screen.columns - 6, BOX);

        set_console_position(divide_x + 3, ((int)(screen.rows / 2)) - (3 + i));
        printf("%d", i);
    }
}

void print_stack_log(int divide_x, struct Stack *s) {
    struct Screen screen = get_screen();

    for (int i = 0; i < s->top + 1; i++) {
        if (screen.rows - (i) == ((int)(screen.rows / 2)) + 2) {
            break;
        }

        set_console_position(divide_x + 3, screen.rows - (i + 2));
        printf("%d", i);

        set_console_position(divide_x + 6, screen.rows - (i + 2));
        putchar(V);

        set_console_position(divide_x + 9, screen.rows - (i + 2));
        set_text_color(YELLOW);
        printf("%d", s->value[i]);
        set_text_color(WHITE);
    }
}

void print_register(int divide_x, struct Register* r) {
    struct Screen screen = get_screen();

    draw_horizontal_line(screen.rows - 3, 0, divide_x - 1, H);

    for (int i = 1; i <= 3; i++) {
        set_console_position((int)(divide_x / 4) * i, screen.rows - 3);
        putchar(BOTTOM_T);


        draw_vertical_line((int)(divide_x / 4) * i, screen.rows - 2, screen.rows - 1, V);
    }

    set_console_position(1, screen.rows - 2);
    printf("AX: %d", r->ax);

    set_console_position((int)(divide_x / 4) + 2, screen.rows - 2);
    printf("BX: %d", r->bx);

    set_console_position((int)(divide_x / 4) * 2 + 2, screen.rows - 2);
    printf("CX: %d", r->cx);

    set_console_position((int)(divide_x / 4) * 3 + 2, screen.rows - 2);
    printf("DX: %d", r->dx);
}


void divide_code(char* token, int code_i) {
    //int 

    //if (code_i >= 0) {
    //    if (strstr(token, "push")) {

    //    }
    //}
}


void execute(struct File* file, struct Stack* s) {
    //char* token;

    //int i = 0;
    //while (file->code[i] != '\0') {
    //    int code_i = 0;
    //    token = strtok(file->code[i], " ");

    //    while (token != NULL) {
    //        

    //        token = strtok(NULL, " ");  // 다음 토큰
    //        code_i++;
    //    }

    //    i++;
    //}
}


void editor_setup(struct File* file) {
    system("cls");

    char code[50][100] = { 0 };  // file->code와 크기 일치 (100바이트)
    struct Stack s;
    struct Register reg;

    init_stack(&s);
    push(&s, 3284);
    push(&s, 1021);
    push(&s, 23494);

    reg.ax = 1;
    reg.bx = 2;
    reg.cx = 3;
    reg.dx = 4;

    // 모든 줄을 복사 (빈 줄 포함)
    for (int i = 0; i < 50; i++) {
        strcpy_s(code[i], 100, file->code[i]);
    }

    // 마지막으로 내용이 있는 줄 찾기
    int line = 0;
    for (int i = 49; i >= 0; i--) {
        if (file->code[i][0] != '\0') {
            line = i + 1;
            break;
        }
    }

    int pos = 0;

    char cmd[20];

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
        draw_vertical_line(divider_x, 0, screen.rows - 1, V);
        
        set_console_position(divider_x, (int)(screen.rows / 2));
        putchar(LEFT_T);

        set_console_position(divider_x, screen.rows - 3);
        putchar(RIGHT_T);

        // 코드 영역 표시
        set_console_position(2, 3);
        printf("Code Editor (ESC to exit)");

        print_stack(divider_x, s.top + 1);
        print_stack_log(divider_x, &s);

        // 상태 표시
        //set_console_position(2, screen.rows - 2);
        //printf("Line: %d | Pos: %d", line + 1, pos);
        
        print_register(divider_x, &reg);

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


        else if (ch == ':') {
            set_console_position(0, screen.rows - 5);
            printf(":");

            set_console_position(1, screen.rows - 5);
            scanf_s("%s", cmd, 20);

            if (strchr(cmd, 'w')) {
                for (int i = 0; i < 50; i++) {
                    strcpy_s(file->code[i], 100, code[i]);  // 빈 줄도 포함해서 모두 복사
                }
            }
            
            if (strchr(cmd, 'x')) {
                execute(file, &s);
            }
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

            if (len < 99) {  // 버퍼 크기 (100 - 1)
                for (int i = len; i >= pos; i--) {
                    code[line][i + 1] = code[line][i];
                }

                code[line][pos] = ch;
                pos++;
            }
        }
    }
}
