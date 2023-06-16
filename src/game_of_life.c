#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int draw(int matrix[25][80], int speed);
int input(int matrixNow[25][80], int *error);
int evo(int matrixNow[25][80], int matrixNext[25][80]);
int reborn(int matrixNow[25][80], int matrixNext[25][80]);
int setInput();
int resetInput();


int main() {
  struct termios first;  // Cоздаём структуры для задания новых настроек терминала и сохранения старых
  struct termios second;
  fd_set num;
  struct timeval A;
  int error = 1;  // Показатель скорости, скорость в мс и флаг ошибки
  int matrixNow[25][80] = {0};  //  Матрица текущего поколения
  input(matrixNow, &error);
    if (error != 0) {
  int speedCount = 3;
  int speed = 1000000;
  draw(matrixNow, speedCount);  // Рисуем начальное положение
  setInput(&first, &second);  // Настраиваем терминал с новыми настройками
  int matrixNext[25][80] = {0};  //  Матрица следующего поколения
  while (1) {
  evo(matrixNow, matrixNext);  // Распределение на живых и мёртвых
  reborn(matrixNow, matrixNext);  // Смена поколений
  FD_ZERO(&num);
  FD_SET(0, &num);  // Работа только с stdin
  A.tv_sec = 0;  // Сокращаем задержку ввода до нуля
  A.tv_usec = 0;
  int prom = select(2, &num, NULL, NULL, &A);  // Результат нажата клавиша или нет
  if (prom == 1) {
      switch (getchar()) {
          case ('a'):
            if (speedCount > 1) {
            speedCount = speedCount - 1;
            speed = speed / 10;
            }
          break;
          case ('d'):
            if (speedCount < 3) {
            speedCount = speedCount + 1;
            speed = speed * 10;
            }
          break;
          case ('q'):
            exit(1);
          break;
       }
  }
    draw(matrixNext, speedCount);  // Перерисовываем поле
    usleep(speed);  // Задержка для скорости
    }
    resetInput(&first);  // Возвращаем старые настройки терминала
    }
    return 0;
}

int draw(int matrix[25][80], int speed) {
    printf("\033[0d\033[2J");
    printf("Текущая скорость: %d\n", speed);
    printf(" ");
    for (int i = 0; i < 80; i++) {
        printf("-");  // Отрисовка верхней границы
    }
    printf("\n|");
for (int str = 0; str < 25; str++) {  // Проходка по строкам
    for (int stolb = 0; stolb < 80; stolb++) {  // Проходка по столбцам
         if (matrix[str][stolb] == 0) {  // Если 0 - рисуем мёртвую клетку
             printf(" ");
         } else {  // Если 1 - рисуем живую
             printf("@");
         }
    }
    printf("|\n|");  // Отрисовка правой грани, перенос на следующую строку и отрисовка левой грани
}
printf("\b \b ");
for (int i = 0; i < 80; i++) {
        printf("-");  // Отрисовка нижней границы
    }
    printf("\n\n");

    return 0;
}

  int input(int matrixNow[25][80], int *error) {
  int numb = 0;
  printf("Выберете начальную конфигурацию:\n");
  printf("1. Pole\n");
  printf("2. Cross\n");
  printf("3. Gliders\n");
  printf("4. Gosper_glider\n");
  printf("5. Long_liver\n");
  if (scanf("%d", &numb) != 1 || getchar() != '\n' || numb < 1 || numb > 5) {
  printf("Ошибка ввода");
      *error = 0;
  }
      if (*error != 0) {
  FILE* set;
  switch (numb) {  // Открываем файл с данными для чтения
  case (1): set = fopen("../initial_states/1.txt", "rt"); break;
  case (2): set = fopen("../initial_states/2.txt", "rt"); break;
  case (3): set = fopen("../initial_states/3.txt", "rt"); break;
  case (4): set = fopen("../initial_states/4.txt", "rt"); break;
  case (5): set = fopen("../initial_states/5.txt", "rt"); break;
  }
  if (set == NULL) {  // Проверочка на доступность файла
    printf("Невозможно открыть файл");
  }
  for (int str = 0; str < 25; str++) {  // Проходка по строкам
    for (int stolb = 0; stolb < 80; stolb++) {  // Проходка по столбцам
      if (fscanf(set, "%d ", &matrixNow[str][stolb]) != 1) {  // Запись из файла в массив
        printf("Ошибка ввода");
      }
     }
  }
  }
      return 0;
  }

int evo(int matrixNow[25][80], int matrixNext[25][80]) {
    int countLife = 0;  // Счётчик живых клеток в округе

    for (int str = 0; str < 25; str++) {  // Проходка по строкам
      for (int stolb = 0; stolb < 80; stolb++) {  // Проходка по столбцам
         for (int i = -1; i <= 1; i++) {
             for (int j = -1; j <= 1; j++) {
                 if ((i != 0 || j != 0) && matrixNow[(str+i+25)%25][(stolb+j+80)%80] == 1) {
                     countLife++;  // Проверка всего поля кроме границ
                 }
                 }
         }
         // Условия размножения
         if ((matrixNow[str][stolb] == 0 && countLife == 3) ||
             (matrixNow[str][stolb] == 1 && (countLife == 3 || countLife == 2))) {
             matrixNext[str][stolb] = 1;
         } else {
             matrixNext[str][stolb] = 0;
         }
         countLife = 0;
      }
      }
    return 0;
     }

int reborn(int matrixNow[25][80], int matrixNext[25][80]) {  // Обновление поколения
    for (int str = 0; str < 25; str++) {  // Проходка по строкам
    for (int stolb = 0; stolb < 80; stolb++) {  // Проходка по столбцам
         matrixNow[str][stolb] = matrixNext[str][stolb];
       }
    }
    return 0;
}

int setInput(struct termios first, struct termios second) {
  tcgetattr(0, &first);  // Считываем текущий режим работы term
  second = first;
  second.c_lflag &= (~ICANON);
  second.c_lflag &= (~ECHO);
  second.c_cc[VMIN] = 1;  // Считывание по одному символу
  second.c_cc[VTIME] = 0;  // Без ограничения времени ожидания
  tcsetattr(0, TCSANOW, &second);  // Установка нового режима работы
    return 0;
}

int resetInput(struct termios first) {
  tcsetattr(0, TCSANOW, &first);
    return 0;
}
