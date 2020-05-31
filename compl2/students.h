#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "log.h"
#define SIZE 600
#define TSIZE 100

typedef struct students {
    char numb[TSIZE];
    char second_name[TSIZE], first_name[TSIZE], patr[TSIZE];
    char faculty[TSIZE], spec[TSIZE];
    struct students *next;
}Students;

extern Students *students_head;
extern int students_struct_size;
extern char login[TSIZE];

void fill (Students * p, char s[SIZE]);

//Добавление
void add_student(){
    FILE *f;
    Students *current, *iterator;
    int i = 0, j, find = 0;
    int punct_count = 0;
    char info[SIZE], first_word[TSIZE];
    f = open_file("students.csv", "r+");
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
        print_log(login, "WWW Должно быть 6 значений --Завершение функции--", "\0");
        return;
    }
    print_log(login, "CCC add_student", info);
    iterator = students_head;
    for (j = 0; j < students_struct_size; j++){
        if (strcmp(first_word, iterator->numb) == 0){
            puts("Такой студент уже существует\n--Завершение функции--");
            print_log(login, "WWW Такой студент уже существует --Завершение функции--", "\0");
            return;
        }
        iterator = iterator->next;
    }
    students_struct_size++;
    current = (Students*)malloc(sizeof(Students));
    fill(current, info);
    fprintf(f, "%s", info);
    fclose(f);
    puts("Студент добавлен");
    print_log(login, "SSS Студент добавлен", "\0");
}

//Удаление
void delete_student (){
    int i, j;
    FILE *f, *check;
    char *id = (char *)malloc(7 * sizeof(char));

    Students *iterator1 = students_head, *prev, *iterator2;
    char number[TSIZE], first_word[TSIZE];
    char info[SIZE];
    check = open_file("student_books.csv", "r");
    if (students_struct_size != 0){
        //проверка номера студента в файле students_books
        puts("Введите номер зачетной книжки:");
        if (scanf("%s", number)){
            print_log(login, "CCC delete_student", number);
            while (fgets(info, SIZE, check)) {
                id = strtok(info, ";");
                id = strtok(NULL, ";");
                if (strcmp(number, id) == 0) {
                    printf("Нельзя удалить студента с номером %s\n", number);
                    print_log(login, "WWW Нельзя удалить студента с номером ", number);
                    return;
                }
            }
            fclose(check);
            f = open_file("students.csv", "w");
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
                    print_log(login, "SSS Студент удален", "\0");
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
            fclose(f);
        }
        else{
            fprintf(stderr, "Неправильный ввод\n");
            print_log(login, "WWW Неправильный ввод", "\0");
            exit(EXIT_FAILURE);
        }
    }
    else{
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        print_log(login, "WWW В читаемом файле нету данных --Завершение функции--\n", "\0");
        return;
    }
}

//Бэкап
void backup (){
    int ch;
    struct tm *loc_time;
    char time_ar[TSIZE];
    const time_t ttime = time(NULL);
    if (no_data(students_struct_size))
        return;
    loc_time = localtime(&ttime);
    strftime(time_ar, 50, "%d.%m.%Y_%H:%M:%S", loc_time);
    char filename[TSIZE] = "students_";
    strcat(filename, time_ar);
    strcat(filename, ".csv");
    FILE *in, *out;
    in = open_file(filename, "w");
    out = open_file("students.csv", "r");
    while ((ch = getc(out)) != EOF)
        putc(ch, in);
    puts("Операция прошла успешно");
    print_log(login, "SSS Операция прошла успешно", "\0");
    fclose(in), fclose(out);
}

//Восстановление содержимого
void recovery(){
    int ch;
    char filename[TSIZE];
    char str[SIZE];
    Students *current;
    FILE *in, *out;
    students_struct_size = 0;
    puts("Введите название файла бэкапа (Пример: students_15.05.2020_17:35:27.csv):");
    scanf("%s", filename);
    print_log(login, "CCC recovery", filename);
    out = open_file(filename, "r");
    in = open_file("students.csv", "w+");
    while ((ch = getc(out)) != EOF) {
        if (ch == '\n')
            students_struct_size++;
        putc(ch, in);
    }
    fseek(in, 0L, SEEK_SET);
    while(fgets(str, SIZE, in)){
        current = (Students *)malloc(sizeof(Students));
        fill(current, str);
    }
    puts("Операция прошла успешно");
    print_log(login, "SSS Операция прошла успешно", "\0");
    fclose(in), fclose(out);
}

//Поиск по фамилии
void search (){
    int found = 0;
    char surname[TSIZE];
    Students *iterator = students_head;
    if (no_data(students_struct_size))
        return;
    puts("Введите фамилию:");
    scanf("%s", surname);
    print_log(login, "CCC search_student", surname);
    for (int i = 0; i < students_struct_size; i++){
        if (strcmp(surname, iterator->second_name) == 0) {
            printf("%s %s %s %s %s %s\n", iterator->numb, iterator->second_name, iterator->first_name,
                   iterator->patr, iterator->faculty, iterator->spec);
            found = 1;
        }
        iterator = iterator->next;
    }
    if (found == 0) {
        char ar_to_write[AR_SIZE] = "Студента с фамилией ";
        strcat(ar_to_write, surname);
        strcat(ar_to_write, " не найдено\n");
        printf("WWW Студента с фамилией %s не найдено ", surname);
        print_log(login, ar_to_write, "\0");
    }
}

//Поиск по номеру зачетки
void search_id (){
    FILE *f, *check;
    char id[TSIZE], info[SIZE];
    char info_books[SIZE];
    char *isbn, *number, *isbn_in_books, *date;
    puts("Введите номер зачетки");
    if (scanf("%s", id)){
        print_log(login, "CCC search_student_id", id);
        check = open_file("student_books.csv", "r");
        f = open_file("books.csv", "r");
        while (fgets(info, SIZE, check)){
            isbn = strtok(info, ";");
            number = strtok(NULL, ";");
            date = strtok(NULL, ";");
            if (strcmp(number, id) == 0){
                while (fgets(info_books, SIZE, f)){
                    isbn_in_books = strtok(info_books, ";");
                    if (strcmp(isbn, isbn_in_books) == 0){
                        while (isbn_in_books != NULL){
                            printf("%s ", isbn_in_books);
                            isbn_in_books = strtok(NULL, ";\n");
                        }
                        printf("%s\n", date);
                    }
                }
            }
            fseek(f, 0L, SEEK_SET);
        }
        fclose(f);
        fclose(check);
    }
    else {
        puts("Неправильный ввод");
        print_log(login, "WWW Неправильный ввод", "\0");
    }
}

void fill (Students * p, char s[SIZE]);

void read_students (){
    FILE *fp;
    char  str[SIZE];
    Students *current;
    fp = open_file("students.csv", "r");
    while (fgets(str, SIZE, fp)) {
        students_struct_size++;
        //реализация связного списка + заполнение данными из файла
        current = (Students *) malloc(sizeof(Students));
        fill(current, str);
    }
    fclose(fp);
}

//функция считывания данных из файла, разделеныных точкой с запятой
void fill (Students * p, char s[SIZE]){
    int i = 0, j = 0;
    int nametag = 0;
    char ar[SIZE];
    Students* ps = students_head;
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
