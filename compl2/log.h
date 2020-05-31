#ifndef HOM1_LOG_H
#define HOM1_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define AR_SIZE 200

extern char login[TSIZE];

void print_log(char *login, char *func_name, char *param){
    FILE *f;
    char parameter[TSIZE];
    strcpy(parameter, param);
    char str_to_print[AR_SIZE];
    struct tm *loc_time;
    char time_ar[AR_SIZE];
    const time_t ttime = time(NULL);
    loc_time = localtime(&ttime);
    strftime(time_ar, 50, "%d.%m.%Y %H:%M:%S", loc_time);
    for (int i = 0; *(parameter + i); i++ ){
        if (*(parameter + i) == '\n') {
            *(parameter + i) = '\0';
            break;
        }
    }
    if ((f = fopen("library.log", "r+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл library.log");
        exit(EXIT_FAILURE);
    }
    fseek(f, 0L, SEEK_END);
    if (strcmp(parameter, "\0")){
        fprintf(f, "\"%s\";\"%s\";\"%s\";\"%s\"\n", time_ar, login, func_name, parameter);
    }
    else
        fprintf(f, "\"%s\";\"%s\";\"%s\"\n", time_ar, login, func_name);
    fclose(f);
}

FILE *open_file(char *filename, char *func){
    FILE *f;
    if ((f = fopen(filename, func)) == NULL){
        fprintf(stderr, "Не удалось открыть файл %s", filename);
        print_log(login, "EEE Не удалось открыть файл", "\0");
        exit(EXIT_FAILURE);
    };
    return f;
}

void students_menu(int vip){
    puts("\n**** STUDENTS MENU ****\n"
         "Выберите операцию:\n"
         " 1) Добавить студента\n"
         " 2) Удалить студента по номеру зачетной книжки\n"
         " 3) Бэкап\n"
         " 4) Восстановить базу из файла бэкапа\n"
         " 5) Поиск по фамилии\n"
         " 6) Поиск по номеру зачетки\n"
         " 7) Завершить работу программы");
    if (vip == 1)
        puts("0)Вернуться в меню выбора");
    printf("> ");
}

void books_menu(int vip){
    puts("\n**** BOOKS MENU ****\n"
         "Выберите операцию:\n"
         " 1) Добавить книгу\n"
         " 2) Удалить книгу\n"
         " 3) Просмотр информации о книге\n"
         " 4) Вывести таблицу книг\n"
         " 5) Поиск по ISBN\n"
         " 6) Редакитровать книгу\n"
         " 7) Выдача книги\n"
         " 8) Сдача книги\n"
         " 9) Изменить количество книг\n"
         "10) Бэкап\n"
         "11) Восстановление\n"
         "12) Поиск по автору\n"
         "13) Завершить программу");
    if (vip == 2)
        puts("0)Вернуться в меню выбора");
    printf("> ");
}

int no_data (int size){
    if (size == 0){
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        print_log(login, "WWW В читаемом файле нету данных —Завершение функции--", "\0");
        return 1;
    }
    return 0;
}
#endif
