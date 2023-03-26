#include <math.h>
#include <stdio.h>
#include <stdbool.h>

int abs(int x) {
    return x < 0 ? -x : x;
}

float abs(float x) {
    return x < 0 ? -x : x;
}

int sqr(int x) {
    return x * x;
}

float sqr(float x) {
    return x * x;
}

bool odd(int x) {
    return x % 2 == 1;
}

char chr(int x) {
    return (char)x;
}

int ord(char x) {
    return (int)x;
}

int succ(int x) {
    return x + 1;
}

char succ(char x) {
    return x + 1;
}

int pred(int x) {
    return x - 1;
}

char pred(char x) {
    return x - 1;
}

int round(float x) {
    return (int)(x + 0.5);
}

int trunc(float x) {
    return (int)x;
}

float sin(float x) {
    return (float)sin(x);
}

float sin(int x) {
    return (float)sin(x);
}

float cos(float x) {
    return (float)cos(x);
}

float cos(int x) {
    return (float)cos(x);
}

float exp(float x) {
    return (float)exp(x);
}

float exp(int x) {
    return (float)exp(x);
}

float ln(float x) {
    return (float)log(x);
}

float ln(int x) {
    return (float)log(x);
}

float sqrt(float x) {
    return (float)sqrt(x);
}

float sqrt(int x) {
    return (float)sqrt(x);
}

float arctan(float x) {
    return (float)atan(x);
}

float arctan(int x) {
    return (float)atan(x);
}

bool eof() {
    return feof(stdin);
}

bool eoln() {
    int c = getchar();
    ungetc(c, stdin);
    return c == '\n' || c == EOF;
}