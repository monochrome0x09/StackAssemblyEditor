#include "Editor.h"

#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define RED (FOREGROUND_RED | FOREGROUND_INTENSITY)

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

void print_error(char* msg) {
    struct Screen screen = get_screen();

    set_console_position(0, screen.rows - 6);
    set_text_color(RED);

    if (msg != NULL) printf("%s", msg);

    set_text_color(WHITE);
}

void divide_code(char* asm[3], struct File* file, struct Stack* s, struct Register* reg, char* err) {
    size_t err_size = 100;

    // NULL 체크
    if (asm[0] == NULL) {
        strcpy_s(err, err_size, "Empty command");
        return;
    }

    int opcode = -1;

    if (strcmp(asm[0], "push") == 0) opcode = PUSH;
    else if (strcmp(asm[0], "pop") == 0) opcode = POP;
    else if (strcmp(asm[0], "add") == 0) opcode = ADD;
    else if (strcmp(asm[0], "sub") == 0) opcode = SUB;
    else if (strcmp(asm[0], "mov") == 0) opcode = MOV;
    else {
        strcpy_s(err, err_size, "There is no opcode that you typed");
        return;  // 오류 발생 시 즉시 종료
    }

    switch (opcode) {
    case PUSH:
        if (asm[1] == NULL) {
            strcpy_s(err, err_size, "PUSH requires an argument");
            return;
        }
        push(s, atoi(asm[1]));
        break;

    case POP:
        if (asm[1] == NULL) {
            strcpy_s(err, err_size, "POP requires a register");
            return;
        }

        if (strcmp(asm[1], "ax") == 0) pop(s, &(reg->ax));
        else if (strcmp(asm[1], "bx") == 0) pop(s, &(reg->bx));
        else if (strcmp(asm[1], "cx") == 0) pop(s, &(reg->cx));
        else if (strcmp(asm[1], "dx") == 0) pop(s, &(reg->dx));
        else {
            strcpy_s(err, err_size, "Invalid register for POP");
            return;
        }
        break;

    case ADD:
        if (asm[1] == NULL || asm[2] == NULL) {
            strcpy_s(err, err_size, "ADD requires two arguments");
            return;
        }

        int sec_oprand = 0;

        // 두 번째 오퍼랜드 레지스터 확인
        if (strcmp(asm[2], "ax") == 0) sec_oprand = reg->ax;
        else if (strcmp(asm[2], "bx") == 0) sec_oprand = reg->bx;
        else if (strcmp(asm[2], "cx") == 0) sec_oprand = reg->cx;
        else if (strcmp(asm[2], "dx") == 0) sec_oprand = reg->dx;
        else {
            sec_oprand = atoi(asm[2]);
        }

        if (strcmp(asm[1], "ax") == 0) {
            //reg->ax += atoi(asm[2]);
            reg->ax += sec_oprand;
        }
        else if (strcmp(asm[1], "bx") == 0) {
            //reg->bx += atoi(asm[2]);
            reg->bx += sec_oprand;
        }
        else if (strcmp(asm[1], "cx") == 0) {
            //reg->cx += atoi(asm[2]);
            reg->cx += sec_oprand;
        }
        else if (strcmp(asm[1], "dx") == 0) {
            //reg->dx += atoi(asm[2]);
            reg->dx += sec_oprand;
        }
        else {
            sec_oprand = atoi(asm[2]);
            strcpy_s(err, err_size, "Invalid register for ADD");
            return;
        }

        break;

    case SUB:
        if (asm[1] == NULL || asm[2] == NULL) {
            strcpy_s(err, err_size, "SUB requires two arguments");
            return;
        }

        int sec_oprand_sub = 0;

        // 두 번째 오퍼랜드 레지스터 확인
        if (strcmp(asm[2], "ax") == 0) sec_oprand_sub = reg->ax;
        else if (strcmp(asm[2], "bx") == 0) sec_oprand_sub = reg->bx;
        else if (strcmp(asm[2], "cx") == 0) sec_oprand_sub = reg->cx;
        else if (strcmp(asm[2], "dx") == 0) sec_oprand_sub = reg->dx;
        else {
            sec_oprand_sub = atoi(asm[2]);
        }

        if (strcmp(asm[1], "ax") == 0) {
            reg->ax -= sec_oprand_sub;
        }
        else if (strcmp(asm[1], "bx") == 0) {
            reg->bx -= sec_oprand_sub;
        }
        else if (strcmp(asm[1], "cx") == 0) {
            reg->cx -= sec_oprand_sub;
        }
        else if (strcmp(asm[1], "dx") == 0) {
            reg->dx -= sec_oprand_sub;
        }
        else {
            sec_oprand_sub
                = atoi(asm[2]);
            strcpy_s(err, err_size, "Invalid register for SUB");
            return;
        }

        break;

    case MOV:
        if (asm[1] == NULL || asm[2] == NULL) {
            strcpy_s(err, err_size, "MOV requires two arguments");
            return;
        }

        int value = 0;
        if (strcmp(asm[2], "ax") == 0) value = reg->ax;
        else if (strcmp(asm[2], "bx") == 0) value = reg->bx;
        else if (strcmp(asm[2], "cx") == 0) value = reg->cx;
        else if (strcmp(asm[2], "dx") == 0) value = reg->dx;
        else {
            value = atoi(asm[2]);
        }

        if (strcmp(asm[1], "ax") == 0) reg->ax = value;
        else if (strcmp(asm[1], "bx") == 0) reg->bx = value;
        else if (strcmp(asm[1], "cx") == 0) reg->cx = value;
        else if (strcmp(asm[1], "dx") == 0) reg->dx = value;
        else {
            strcpy_s(err, err_size, "Invalid register for MOV");
            return;
        }
        break;
    }

    // 성공 시 에러 메시지 초기화
    err[0] = '\0';
}


void execute(struct File* file, struct Stack* s, struct Register* reg, char* err) {
    int i = 0;

    init_stack(s);

    reg->ax = 0;
    reg->bx = 0;
    reg->cx = 0;
    reg->dx = 0;

    // 마지막으로 내용이 있는 줄 찾기
    int line = 0;
    for (int j = 49; j >= 0; j--) {
        if (file->code[j][0] != '\0') {
            line = j + 1;
            break;
        }
    }

    while (i <= line) {
        system("cls");

        char temp[100];
        strcpy_s(temp, sizeof(temp), file->code[i]);

        char* asm_parts[3] = { NULL, NULL, NULL };  // 명시적으로 NULL 초기화
        int code_i = 0;

        char* token;
        char* next_token = NULL;


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

        print_stack(divider_x, s->top + 1);
        print_stack_log(divider_x, s);

        print_register(divider_x, reg);

        for (int j = 0; j <= line && j < 50; j++) {
            set_console_position(2, 5 + j);

            if (i == j) {
                set_text_color(YELLOW);
            }

            printf("%s", file->code[j]);

            set_text_color(WHITE);
        }



        // 첫 번째 토큰
        token = strtok_s(temp, " \t", &next_token);

        // 모든 토큰 추출
        while (token != NULL && code_i < 3) {
            asm_parts[code_i] = token;
            code_i++;

            token = strtok_s(NULL, " \t", &next_token);
        }

        // 빈 줄 무시
        if (asm_parts[0] != NULL) {
            divide_code(asm_parts, file, s, reg, err);

            // 에러 발생 시 실행 중단
            if (err[0] != '\0') {
                printf("Error at line %d: %s\n", i + 1, err);
                break;  // 또는 return으로 완전 종료
            }
        }

        Sleep(2000);

        i++;
    }
}


void editor_setup(struct File* file) {
    system("cls");

    char code[50][100] = { 0 };  // file->code와 크기 일치 (100바이트)
    struct Stack s;
    struct Register reg;

    init_stack(&s);

    reg.ax = 0;
    reg.bx = 0;
    reg.cx = 0;
    reg.dx = 0;

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
    char err[100] = { 0, };

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

        print_error(err);

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
                execute(file, &s, &reg, err);
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
