// Copyright 2022 Yakov Vainshteyn
#include <stdio.h>
#include <stdlib.h>
#define Width 80
#define Height 25
// Прототипы функций
// проверка на статичное поле
int staticCheck(int matr[Height][Width], int backup[Height][Width]);
// приветсвие
void startGame();
// окончание игры
void endGame();
// ввод из файла
void PatternInput(int matr[Height][Width]);
// отрисовка
void draw(int matr[Height][Width]);
// вызов логики
void Logic(int matr[Height][Width],  int backup[Height][Width]);
// вызов логики внутренних клеток
void InnerLogic(int matr[Height][Width], int backup[Height][Width]);
// вызов логики внешних клеток
void OuterLogic(int matr[Height][Width],  int backup[Height][Width]);
// подсчет соседей внутренних клеток
int InnerNeighbors(int matr[Height][Width], int x, int y);
// подсчет соседей внешних клеток
int OuterNeighbors(int matr[Height][Width], int x, int y);
// приравнивание матрицы
void MatEq(int origin[Height][Width], int copy[Height][Width]);
// обработчик ошибок
void errorMsg();
int main() {
    // создание основной матрицы
    int matrix[Height][Width];
    // создание доп матрицы для прошлого состояния
    int backup[Height][Width];
    // создание переменной типа чар для обработки команд игрока
    char c = ' ';
    // ввод данных из файла
    PatternInput(matrix);
    // перезапуск потока ввода на клавиатуру
    if (freopen("/dev/tty", "rw", stdin) == NULL)
        errorMsg();
    // вывод приветсвия
    startGame();
    // отрисовка поля в 1-й раз
    draw(matrix);
    // создание дополнительного чара
    char end;
    // пока пользователь не введет команду выхода
    while (c != 'q') {
        // считываем команду
        c = getchar();
        // если пользователь ввел продолженние игры
        if (c == ' ') {
            // проверка что ввод корректный
            end = getchar();
            // очистка экрана
                system("clear");
            // если команда корректная
            if (end == '\n') {
            // если состояние не изменилось уведомить пользователя
            if (!staticCheck(matrix, backup)) {
                printf("Game reached static state, press q to quit\n");
            }
                // приравнять бекапу текущее состояние
                MatEq(matrix, backup);
                // вызвать обработку логики
                Logic(matrix, backup);
            }
            // вызвать отрисовку
             draw(matrix);
        } else {
            // пользователь ввел не пробел
            // если пользователь ввел cntrl + d
        if (c == (int)(EOF)) {
            // очистить экран
            system("clear");
            // уведомить пользователя так не делать
            printf("Please don't use that\n");
            // перезапустить поток ввода
            if (freopen("/dev/tty", "rw", stdin) == NULL)
                errorMsg();
            // вызвать отрисовку
            draw(matrix);
        } else {
            // пользователь ввел случайный символ
            // очистить экран
            system("clear");
            // вызвать отрисовку
            draw(matrix);
        }
        }
    }
    // если цикл завершился значит игра закончилась
    // вызвать прощание с игроком
    endGame();
    return 0;
}


void errorMsg() {
    printf("Input stream error\n");
    exit(0);
}
// проверка на статичсное состояние
int staticCheck(int matr[Height][Width], int backup[Height][Width]) {
    // изначально 0 изменений
    int res = 0;
    for (int i = 0; i < Height; i++)  // для каждой строки
        for (int j = 0; j < Width; j++)  // для каждого столбца в строке
            if (matr[i][j] != backup[i][j])  // если состояние изменилось изменить счетчик
                res += 1;
    // если счетчик 0 ноль изменений тогда if(static check) = false и тогда статичное состояние
    return res;
}
// ввод из файла
void PatternInput(int matr[Height][Width]) {
    // для каждого элемента просканировать 1 цифру
    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
            scanf("%1d", &matr[i][j]);
}

// приравние матрицы
void MatEq(int origin[Height][Width], int copy[Height][Width]) {
    // для каждого элемента копии присвоить значение оригинала
    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
            copy[i][j] = origin[i][j];
}
// начать игру
void startGame() {
    char c;
    // очистить экран
    system("clear");
    // расписать правила
    printf("Welcome to Game of Life\n");
    printf("In the game:\n \t1. Press space to iterate\n \t2.Press q to quit\n");
    printf("Now press any key to start the game\n");
    // ждать ввода любой кнопки
    c = getchar();
    if (c == 'q') {
        endGame();
        exit(0);
    }
}
// закончить игру
void endGame() {
    // очистить экран
    system("clear");
    // написать что произошло
    printf("Game stopped by player.\nThanks for playing\n Goodbye\n");
}
// отрисовка
void draw(int matr[Height][Width]) {
    // отрисовать верхнюю границу
    for (int i = 0; i < Width + 2; i++)
        printf("=");
    printf("\n");
    // основная отрисовка
    // если горизонтальная координата гранична то отрисовать символ границы
    // иначе отрисовать пробел для мертвой клетки
    // и звездочку для живой
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width + 2; j++) {
            if (j == 0 || j == Width + 1) {
                printf("|");
            } else {
                if (matr[i][j-1]) {  // в си Любое значение отличное от нуля = true
                    printf("*");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    // отрисовать нижнюю границу
    for (int i = 0; i < Width + 2; i++)
        printf("=");
    printf("\n");
}
// вызов отдельной логики для внутр и внеш клеток
void Logic(int matr[Height][Width], int backup[Height][Width]) {
    InnerLogic(matr, backup);
    OuterLogic(matr, backup);
}
// логика внешних клеток
void OuterLogic(int matr[Height][Width],  int backup[Height][Width]) {
    int n;
    // для каждой клетки
    for (int i = 0; i < Height ; i++) {
        for (int j = 0; j < Width ; j++) {
            // есди одна координата граничная то
            // подсчитать кол-во соседей
            if (i == 0 || j == 0 || i == Height - 1 || j == Width - 1) {
               n = OuterNeighbors(backup, j, i);
                if (backup[i][j]) {
                    // если клетка жива
                    // она умирает от перенаселения(>3 соседей)
                    // или недонаселения (< 2 соседей)
                    // если не умерает то живет
                    // а значит д-й не требуется
                    if (n < 2 || n > 3)
                        matr[i][j] = 0;
                } else {
                    // если клетка мертва
                    // оживает если 3 соседа
                    // или дальше мертва
                    // то есть д-й не требуется
                    if (n == 3) {
                        matr[i][j] = 1;
                    }
                }
            }
        }
    }
}
// логика внутренних клеток
void InnerLogic(int matr[Height][Width],  int backup[Height][Width]) {
    int n;
    // для каждой клетки без граничных координат
    // логика аналогична внешней
    for (int i = 1; i < Height - 1; i++)
        for (int j = 1; j < Width - 1 ; j++) {
            n = InnerNeighbors(backup, j, i);
            if (backup[i][j]) {
                if (n < 2 || n > 3) {
                    matr[i][j] = 0;
                }
            } else {
                if (n == 3) {
                    matr[i][j] = 1;
                }
            }
        }
}
// подсчет соседей внутренней клетки
int InnerNeighbors(int matr[Height][Width], int x, int y) {
    // для каждой клетке в квадрате 3х3
    // если клетка жива и координаты не совпадают с исходной
    // увеличить счетчик
    int res = 0;
    for (int i = y - 1; i < y + 2; i++) {
        for (int j = x - 1; j < x + 2; j++) {
            if  ((i != y || j != x) && (matr[i][j])) {
                res++;
            }
        }
    }
    return res;
}
// подсчет соседей внешней клетки
int OuterNeighbors(int matr[Height][Width], int x, int y) {
    int res = 0;
    int flag = 1;
    // левый верхний угол
    if (y == 0 && x == 0) {
        flag = 0;
        res += matr[1][0];
        res += matr[0][1];
        res += matr[1][1];
        res += matr[Height - 1][0];
        res += matr[Height - 1][Width - 1];
        res += matr[Height - 1][1];
        res += matr[0][Width - 1];
        res += matr[1][Width - 1];
    }
    // правый верхний угол
    if (y == 0 && x == Width - 1 && flag) {
        flag = 0;
        res += matr[Height - 1][0];
        res += matr[Height - 1][Width - 1];
        res += matr[0][0];
        res += matr[0][x - 1];
        res += matr[1][x];
        res += matr[1][x - 1];
        res += matr[Height - 1][x - 1];
        res += matr[1][0];
    }
    // левый нижний угол
    if (y == Height - 1 && x == 0 && flag) {
        flag = 0;
        res += matr[y][1];
        res += matr[y - 1][0];
        res += matr[y - 1][1];
        res += matr[Height - 1][Width - 1];
        res += matr[0][0];
        res += matr[0][Width - 1];
        res += matr[0][1];
        res += matr[y - 1][Width - 1];
    }
    // правый нижний угол
    if (y == Height - 1  && x == Width - 1 && flag) {
        flag = 0;
        res += matr[0][0];
        res += matr[Height - 1][0];
        res += matr[0][Width - 1];
        res += matr[y - 1][x];
        res += matr[y - 1][x - 1];
        res += matr[y][x - 1];
        res += matr[Height - 2][0];
        res += matr[0][Width - 1];
    }
    // левая граница без углов
    if ((x == 0) && (y >  0) && (y < Height - 1) && flag) {
        res += matr[y - 1][0];
        res += matr[y + 1][0];
        res += matr[y - 1][1];
        res += matr[y][1];
        res += matr[y + 1][1];
        res += matr[y - 1][Width - 1];
        res += matr[y][Width - 1];
        res += matr[y+1][Width - 1];
        flag = 0;
    }
    // правая граница без углов
    if ((x == Width - 1)  && (y > 0) && (y < Height - 1) && flag) {
        res += matr[y - 1][x];
        res += matr[y + 1][x];
        res += matr[y - 1][x - 1];
        res += matr[y][x - 1];
        res += matr[y + 1][x - 1];
        res += matr[y - 1][0];
        res += matr[y][0];
        res += matr[y + 1][0];
        flag = 0;
    }
    // верхняя граница без углов
    if (y == 0 && x != 0 && x != Width - 1 && flag) {
        res += matr[0][x - 1];
        res += matr[0][x + 1];
        res += matr[1][x - 1];
        res += matr[1][x];
        res += matr[1][x + 1];
        res += matr[Height - 1][x - 1];
        res += matr[Height - 1][x];
        res += matr[Height - 1][x + 1];
        flag = 0;
    }
    // нижняя граница без углов
    if (y == Height - 1 && x!= 0 && x != Width - 1 && flag) {
        res += matr[y][x - 1];
        res += matr[y][x + 1];
        res += matr[y - 1][x - 1];
        res += matr[y - 1][x];
        res += matr[y - 1][x + 1];
        res += matr[0][x - 1];
        res += matr[0][x];
        res += matr[0][x + 1];
    }
    return res;
}
