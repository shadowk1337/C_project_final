#ifndef HOM1_STUDENTS_H
#define HOM1_STUDENTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZE 600
#define TSIZE 100

struct students {
    char numb[TSIZE];
    char second_name[TSIZE], first_name[TSIZE], patr[TSIZE];
    char faculty[TSIZE], spec[TSIZE];
    struct students *next;
};

extern struct students *students_head;
extern int students_struct_size;

void fill (struct students * p, char s[SIZE]);

void add_student(){
    FILE *f;
    struct students *current, *iterator;
    int i = 0, j, find = 0;
    int punct_count = 0;
    char info[SIZE], first_word[TSIZE];
    if ((f = fopen("students.csv", "r+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл students.csv");
        exit(EXIT_FAILURE);
    };
    fseek(f, 0L, SEEK_END);
    puts("Введите данные студента через точку с запятой без пробела (Пример: 123456;Иванов;Иван;Иванович;ФН;\"Математика\"):");
    fgets(info, SIZE, stdin);
    for (int k = 0; k < strlen(info); k++){
        if (info[k] != ';' && find == 0){
            first_word[i++] = info[k];
        }
        else if (info[k] == ';') {
            find = 1;
            punct_count++;
        }
    }
    first_word[i] = '\0';
    if (punct_count != 5){
        puts("Должно быть 6 значений\n--Завершение функции--");
        return;
    }
    iterator = students_head;
    for (j = 0; j < students_struct_size; j++){
        if (strcmp(first_word, iterator->numb) == 0){
            puts("Такой студент уже существует\n--Завершение функции--");
            return;
        }
        iterator = iterator->next;
    }
    students_struct_size++;
    current = (struct students*)malloc(sizeof(struct students));
    fill(current, info);
    fprintf(f, "%s", info);
    fclose(f);
    puts("Студент добавлен");
}

void delete_student (){
    int i, j;
    FILE *f;
    struct students *iterator1 = students_head, *prev, *iterator2;
    char number[TSIZE], first_word[TSIZE];
    char info[SIZE];
    if ((f = fopen("students.csv", "w")) == NULL){
        fprintf(stderr, "Не удалось открыть файл students.csv");
        exit(EXIT_FAILURE);
    }
    if (students_struct_size != 0){
        puts("Введите номер зачетной книжки:");
        if (scanf("%s", number)){
            for (i = 0; i < students_struct_size; i++){
                if (strcmp(number, iterator1->numb) == 0){
                    if (i == 0 && students_struct_size == 1){
                        students_head = NULL;
                    }
                    else if (i == 0 && students_struct_size >= 2){
                        students_head = iterator1->next;
                    }
                    else if (i == students_struct_size - 1){
                        prev->next = NULL;
                    }
                    else
                        prev->next = iterator1->next;
                    students_struct_size--;
                    puts("Студент удален");
                    break;
                }
                prev = iterator1;
                iterator1 = iterator1->next;
            }
            if (students_head != NULL) {
                iterator2 = students_head;
                for (j = 0; j < students_struct_size; j++){
                    fprintf(f, "%s;%s;%s;%s;%s;%s\n", iterator2->numb, iterator2->second_name, iterator2->first_name,
                            iterator2->patr, iterator2->faculty, iterator2->spec);
                    iterator2 = iterator2->next;
                }
            }
        }
        else{
            fprintf(stderr, "Неправильный ввод\n");
            exit(EXIT_FAILURE);
        }
    }
    else{
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        return;
    }
    fclose(f);
}

void backup (){
    int ch;
    struct tm *loc_time;
    char time_ar[TSIZE];
    const time_t ttime = time(NULL);
    loc_time = localtime(&ttime);
    strftime(time_ar, 50, "%d.%m.%Y_%H:%M:%S", loc_time);
    char filename[TSIZE] = "students_";
    strcat(filename, time_ar);
    strcat(filename, ".csv");
    FILE *in, *out;
    if ((in = fopen(filename, "w")) == NULL){
        fprintf(stderr, "Не удалось открыть файл %s", filename);
        exit(EXIT_FAILURE);
    };
    if ((out = fopen("students.csv", "r")) == NULL){
        fprintf(stderr, "Не удалось открыть файл students.csv");
        exit(EXIT_FAILURE);
    };
    while ((ch = getc(out)) != EOF)
        putc(ch, in);
    puts("Операция прошла успешно");
    fclose(in), fclose(out);
}

void recovery(){
    int ch;
    char filename[TSIZE];
    char str[SIZE];
    struct students *current;
    FILE *in, *out;
    students_struct_size = 0;
    puts("Введите название файла бэкапа (Пример: students_15.05.2020_17:35:27.csv):");
    scanf("%s", filename);
    if ((in = fopen("students.csv", "w+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл students.csv");
        exit(EXIT_FAILURE);
    };
    if ((out = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Не удалось открыть файл %s", filename);
        exit(EXIT_FAILURE);
    };
    while ((ch = getc(out)) != EOF) {
        if (ch == '\n')
            students_struct_size++;
        putc(ch, in);
    }
    fseek(in, 0L, SEEK_SET);
    while(fgets(str, SIZE, in)){
        current = (struct students *)malloc(sizeof(struct students));
        fill(current, str);
    }
    puts("Операция прошла успешно");
    fclose(in), fclose(out);
}

void search (){
    int found = 0;
    char surname[TSIZE];
    struct students *iterator = students_head;
    puts("Введите фамилию:");
    scanf("%s", surname);
    for (int i = 0; i < students_struct_size; i++){
        if (strcmp(surname, iterator->second_name) == 0) {
            printf("%s %s %s %s %s %s\n", iterator->numb, iterator->second_name, iterator->first_name,
                   iterator->patr, iterator->faculty, iterator->spec);
            found = 1;
        }
        iterator = iterator->next;
    }
    if (found == 0)
        printf("Студента с фамилией %s не найдено\n", surname);
}

//функция считывания данных из файла, разделеныных точкой с запятой
void fill (struct students * p, char s[SIZE]){
    int i = 0, j = 0;
    int nametag = 0;
    char ar[SIZE];
    struct students* ps = students_head;
    char *pa;

    if (students_head == NULL) {
        students_head = p;
    }
    else{
        for (int k = 0; k < (students_struct_size - 2); k++) {
            ps = ps->next;
        }
        ps->next = p;
    }
    p->next = NULL;

    while((ar[i] = s[j])){
        if (s[j] == ';' || s[j] == '\n'){
            ar[i] = '\0';
            if (nametag == 0)
                strcpy(p->numb, ar);
            else if (nametag == 1)
                strcpy(p->second_name, ar);
            else if (nametag == 2)
                strcpy(p->first_name, ar);
            else if (nametag == 3)
                strcpy(p->patr, ar);
            else if (nametag == 4)
                strcpy(p->faculty, ar);
            else if (nametag == 5)
                strcpy(p->spec, ar);
            nametag++, i = 0, j++;
        }
        else {
            i++, j++;
        }
    }
}

#endif
