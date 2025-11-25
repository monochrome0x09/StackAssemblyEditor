#pragma once
#include <windows.h>
#include "Screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "Stack.h"

void print_stack(int divide_x, int fill);

void print_stack_log(int divide_x, struct Stack* s);

void print_register(int divide_x, struct Register* r);

void print_error(char* msg);

void divide_code(char* asm[3], struct File* file, struct Stack* s, struct Register* reg, char* err);

void execute(struct File* file, struct Stack* s, struct Register* reg, char* err);

void editor_setup(struct File* file);
