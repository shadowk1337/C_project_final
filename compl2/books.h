#ifndef HOM1_BOOKS_H
#define HOM1_BOOKS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define SIZE 500
//#define TSIZE 100

struct books{
    char isbn[TSIZE];
    char author[TSIZE];
    char name[TSIZE];
    int books_count;
    int students_count;
    struct books *next;
};

extern struct books *books_head;
extern int books_struct_size;

void fill_books(struct books *p, char s[SIZE]);

void add_book (){
    FILE *f;
    int i = 0, find = 0, punct_count = 0;
    struct books *iterator = books_head, *current;
    char info[SIZE], first_word[TSIZE];
    if ((f = fopen("books.csv", "r+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл books.csv\n");
        exit(EXIT_FAILURE);
    }
    fseek(f, 0L, SEEK_END);
    puts("Введине информацию о книге через точку с запятой без пробела (Пример: 9666965348;Стругацкие;Град обреченный;10;5):");
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
    if (punct_count != 4){
        puts("Должно быть 6 значений\n--Завершение функции--");
        return;
    }
    for (int j = 0; j < books_struct_size; j++){
        if (strcmp(first_word, iterator->isbn) == 0){
            puts("Такая книга уже существует\n--Завершение функции--");
            return;
        }
        iterator = iterator->next;
    }
    books_struct_size++;
    current = (struct books *)malloc(sizeof(struct books));
    fill_books(current, info);
    fprintf(f, "%s", info);
    fclose(f);
    puts("Книга добавлена");
}

void delete_book(){
    FILE *f;
    int i, j;
    struct books *iterator1 = books_head, *prev, *iterator2;
    char number[TSIZE], first_word[TSIZE];
    char info[SIZE];
    if ((f = fopen("books.csv", "w")) == NULL){
        fprintf(stderr, "Не удалось открыть файл books.csv");
        exit(EXIT_FAILURE);
    }
    if (books_struct_size != 0){
        puts("Введите номер ISBN книги:");
        if (scanf("%s", number)){
            for (i = 0; i < books_struct_size; i++){
                if (strcmp(number, iterator1->isbn) == 0){
                    if (i == 0 && books_struct_size == 1){
                        books_head = NULL;
                    }
                    else if (i == 0 && books_struct_size >= 2){
                        books_head = iterator1->next;
                    }
                    else if (i == books_struct_size - 1){
                        prev->next = NULL;
                    }
                    else
                        prev->next = iterator1->next;
                    books_struct_size--;
                    puts("Книга удалена");
                    break;
                }
                prev = iterator1;
                iterator1 = iterator1->next;
            }
            if (books_head != NULL) {
                iterator2 = books_head;
                for (j = 0; j < books_struct_size; j++){
                    fprintf(f, "%s;%s;%s;%d;%d\n", iterator2->isbn, iterator2->author, iterator2->name,
                            iterator2->books_count, iterator2->students_count);
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

void search_book (){
    int found = 0;
    char isbn[TSIZE];
    struct books *iterator = books_head;
    if (books_struct_size == 0){
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        return;
    }
    puts("Введите ISBN:");
    scanf("%s", isbn);
    for (int i = 0; i < books_struct_size; i++){
        if (strcmp(isbn, iterator->isbn) == 0) {
            printf("%s %s %s %d %d\n", iterator->isbn, iterator->author, iterator->name,
                   iterator->books_count, iterator->students_count);
            found = 1;
        }
        iterator = iterator->next;
    }
    if (found == 0)
        printf("Книги с ISBN %s не найдено\n", isbn);
}

void print_book (){
    if (books_struct_size == 0){
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        return;
    }
    struct books *iterator = books_head;
    printf("%7s %s %s %s %s\n", "ISBN", "Автор", "Название", "Кол-во книг", "У ск-ких студентов");
    for (int i = 0; i < books_struct_size; i++){
        printf("%s | %s | %s | %d | %d\n", iterator->isbn, iterator->author, iterator->name,
               iterator->books_count, iterator->students_count);
        iterator = iterator->next;
    }
}

void fill_books(struct books *p, char s[SIZE]){
    int i = 0, j = 0;
    int nametag = 0;
    char ar[SIZE];
    struct books* ps = books_head;
    char *pa;
    int func_isbn, func_books_count, func_students_count;

    if (books_head == NULL) {
        books_head = p;
    }
    else{
        for (int k = 0; k < (books_struct_size - 2); k++) {
            ps = ps->next;
        }
        ps->next = p;
    }
    p->next = NULL;
    while((ar[i] = s[j])){
        if (s[j] == ';' || s[j] == '\n'){
            ar[i] = '\0';
            if (nametag == 0)
                strcpy(p->isbn, ar);
            else if (nametag == 1)
                strcpy(p->author, ar);
            else if (nametag == 2)
                strcpy(p->name, ar);
            else if (nametag == 3)
                p->books_count = atoi(ar);
            else if (nametag == 4)
                p->students_count = atoi(ar);
            nametag++, i = 0, j++;
        }
        else {
            i++, j++;
        }
    }
    /*pa = strtok(ar, ";");
    while (pa != NULL){
        if (nametag == 0)
            strcpy(p->isbn, pa);
        else if (nametag == 1)
            strcpy(p->author, pa);
        else if (nametag == 2)
            strcpy(p->name, pa);
        else if (nametag == 3)
            p->books_count = atoi(pa);
        else if (nametag == 4)
            p->students_count = atoi(pa);
        pa = strtok(NULL, ";");
        nametag++;
    }*/
}
#endif