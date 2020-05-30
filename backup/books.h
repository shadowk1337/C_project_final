#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "log.h"
#include "students.h"
//#define SIZE 500
//#define TSIZE 100

typedef struct books{
    char isbn[TSIZE];
    char author[TSIZE];
    char name[TSIZE];
    int books_count;
    int students_count;
    struct books *next;
} Books;

extern Books *books_head;
extern int books_struct_size;

void books_fill(Books *p, char s[SIZE]);
int check_isbn (char *isbn);

void add_book (){
    FILE *f;
    int i = 0, find = 0, punct_count = 0;
    Books *iterator = books_head, *current;
    char info[SIZE], first_word[TSIZE];
    if ((f = fopen("books.csv", "r+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл books.csv\n");
        print_log(login, "EEE Не удалось открыть файл books.csv", "\0");
        exit(EXIT_FAILURE);
    }
    fseek(f, 0L, SEEK_END);
    puts("Введите информацию о книге через точку с запятой без пробела (Пример: 9666965348;Стругацкие;Град обреченный;10;5):");
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
    if (check_isbn(first_word) != 1){
        puts("В isbn должно быть 10 цифр —Завершение функции--");
        print_log(login, "WWW В isbn должно быть 10 цифр —Завершение функции--", "\0");
        return;
    }
//    printf("%d\n", first_word);
    if (punct_count != 4){
        puts("Должно быть 5 значений\n--Завершение функции--");
        return;
    }
    print_log(login, "CCC add_book", info);
    for (int j = 0; j < books_struct_size; j++){
        if (strcmp(first_word, iterator->isbn) == 0){
            puts("Такая книга уже существует\n--Завершение функции--");
            print_log(login, "WWW Такая книга уже существует —Завершение функции--", "\0");
            return;
        }
        iterator = iterator->next;
    }
    books_struct_size++;
    current = (Books *)malloc(sizeof(Books));
    books_fill(current, info);
    fprintf(f, "%s", info);
    fclose(f);
    puts("Книга добавлена");
    print_log(login, "SSS Книга добавлена", "\0");
}

void delete_book(char *number);

void delete_book_public(){
    FILE *f;
    char number[TSIZE];
    puts("Введите номер ISBN книги:");
    if (scanf("%s", number))
        delete_book(number);
    else{
        fprintf(stderr, "Неправильный ввод\n");
        print_log(login, "WWW Неправильный ввод", "\0");
        exit(EXIT_FAILURE);
    }
    print_log(login, "CCC delete_book", number);
}

void delete_book(char *number){
    FILE *f, *check;
    int i, j;
    char *isbn = (char *)malloc(11 * sizeof(char));
    Books *iterator1 = books_head, *prev, *iterator2;
    char info[SIZE];
    if ((check = fopen("student_books.csv", "r")) == NULL){
        fprintf(stderr, "Не удалось открыть файл student_books.csv");
        print_log(login, "EEE Не удалось открыть файл student_books.csv", "\0");
        exit(EXIT_FAILURE);
    }
    if (books_struct_size != 0){
        while (fgets(info, SIZE, check)) {
            isbn = strtok(info, ";");
            if (strcmp(number, isbn) == 0) {
                printf("Нельзя удалить книгу с номером %s\n", number);
                print_log(login, "WWW Нельзя удалить книгу с номером ", number);
                return;
            }
        }
        fclose(check);
        if ((f = fopen("books.csv", "w")) == NULL){
            fprintf(stderr, "Не удалось открыть файл books.csv");
            print_log(login, "EEE Не удалось открыть файл books.csv", "\0");
            exit(EXIT_FAILURE);
        }
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
                print_log(login, "SSS Книга удалена", "\0");
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
        fclose(f);
    }
    else{
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        print_log(login, "WWW В читаемом файле нету данных —Завершение функции--", "\0");
        return;
    }
}

void search_book (){
    int found = 0;
    char

            isbn[TSIZE];
    struct books *iterator = books_head;
    if (books_struct_size == 0){
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        print_log(login, "WWW В читаемом файле нету данных —Завершение функции--", "\0");
        return;
    }
    puts("Введите ISBN:");
    scanf("%s", isbn);
    print_log(login, "CCC search_book", isbn);
    for (int i = 0; i < books_struct_size; i++){
        if (strcmp(isbn, iterator->isbn) == 0) {
            printf("%s %s %s %d %d\n", iterator->isbn, iterator->author, iterator->name,
                   iterator->books_count, iterator->students_count);
            found = 1;
        }
        iterator = iterator->next;
    }
    if (found == 0) {
        char ar_to_write[AR_SIZE] = "Книги с ISBN ";
        strcat(ar_to_write, isbn);
        strcat(ar_to_write, " не найдено\n");
        printf("Книги с ISBN %s не найдено\n", isbn);
        print_log(login, ar_to_write, "\0");
    }
}


void inclusionSort(char *[], int);

void print_book (){
    char *ar_isbn[books_struct_size];
    if (books_struct_size == 0){
        printf("В читаемом файле нету данных\n--Завершение функции--\n");
        print_log(login, "WWW В читаемом файле нету данных —Завершение функции--", "\0");
        return;
    }
    Books *iterator = books_head;
    for (int i = 0; i < books_struct_size; i++){
        ar_isbn[i] = iterator->isbn;
        iterator = iterator->next;
    }
    inclusionSort(ar_isbn, books_struct_size);

    for (int i = 0; i < books_struct_size; i++){
        iterator = books_head;
        for (int j = 0; j < books_struct_size; j++){
            if (strcmp(ar_isbn[i], iterator->isbn) == 0){
                printf("%-s | %-s | %-s | %-d | %-d\n", iterator->isbn, iterator->author, iterator->name,
                       iterator->books_count, iterator->students_count);
                break;
            }
            iterator = iterator->next;
        }
    }
}

void redact_book (){
    FILE *f;
    int flag = 0;
    char isbn[TSIZE], info[SIZE];
    Books *book = books_head;
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    if ((f = fopen("books.csv", "r+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл books.csv");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < books_struct_size; i++, book = book->next){
        if (strcmp(book->isbn, isbn) == 0) {
            puts("Книга найдена");
            flag = 1;
            break;
        }
    }
    if (flag != 1){
        puts("Такой книги не нашлось");
        return;
    }
    delete_book(isbn);
    getchar();
    add_book();
}

//Поиск по ISBN
void search_isbn(){
    FILE *f, *check;
    char book_id[TSIZE], info[TSIZE];
    char info_students[TSIZE];
    char *isbn, *number, *id_in_students, *date;
    puts("Введите ISBN книги");
    if (scanf("%s", book_id)){
        print_log(login, "CCC search_book_isbn", book_id);
        if ((check = fopen("student_books.csv", "r")) == NULL){
            fprintf(stderr, "Не удалось открыть файл books.csv");
            print_log(login, "EEE Не удалось открыть файл books.csv", "\0");
            exit(EXIT_FAILURE);
        }
        if ((f = fopen("students.csv", "r")) == NULL){
            fprintf(stderr, "Не удалось открыть файл students.csv");
            print_log(login, "EEE Не удалось открыть файл students.csv", "\0");
            exit(EXIT_FAILURE);
        }
        while (fgets(info, SIZE, check)){
            isbn = strtok(info, ";");
            number = strtok(NULL, ";");
            date = strtok(NULL, ";");
            if (strcmp(book_id, isbn) == 0){
                while (fgets(info_students, SIZE, f)){
                    id_in_students = strtok(info_students, ";");
                    if (strcmp(id_in_students, number) == 0){
                        while (id_in_students != NULL){
                            printf("%s ", id_in_students);
                            id_in_students = strtok(NULL, ";");
                        }
                        printf("%s\n", date);
                    }
                }
            }
            fseek(f, 0L, SEEK_SET);
        }
    }
    else {
        puts("Неправильный ввод");
        print_log(login, "WWW Неправильный ввод", "\0");
    }
}

void fill_file_book ();

// ================================================================================================

void change_amount (){
    Books *book = books_head;
    char isbn[TSIZE];
    int new_size;
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    for (int i = 0; i < books_struct_size; i++){
        if (strcmp(book->isbn, isbn) == 0){
            puts("Введите новое количество книг");
            scanf("%d", &new_size);
            book->books_count = new_size;
            fill_file_book();
            return;
        }
        book = book->next;
    }
    printf("Книги с номером %s не найдено\n", isbn);
    return;
}

// ================================================================================================
// TASKS 7 & 8
// ================================================================================================


void issuance_book(){
    FILE *fp;
    char isbn[TSIZE];
    char number[TSIZE];
    char info[SIZE];
    char date[TSIZE];
    char *ps = (char *)malloc(sizeof(char) * TSIZE);
    int book_found = 0;
    int student_found = 0;
    Books *book = books_head;
    Students *student = students_head;
    if ((fp = fopen("student_books.csv", "r+")) == NULL){
        puts("Не удалось открыть файл student_books.csv");
        exit(EXIT_FAILURE);
    }
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    for (int i = 0; i < books_struct_size; i++){
        if (strcmp(isbn, book->isbn) == 0){
            book_found = 1;
            break;
        }
        book = book->next;
    }
    if (book_found == 0){
        puts("Книга не найдена");
        return;
    }
    puts("Введите номер студента");
    scanf("%s", number);
    for (int i = 0; i < students_struct_size; i++){
        if (strcmp(number, student->numb) == 0){
            student_found = 1;
            break;
        }
        student = student->next;
    }
    if (student_found == 0){
        puts("Студент не найден");
        return;
    }
    while (fgets(info, SIZE, fp)){
        if (strcmp(ps = strtok(info, ";"), isbn) == 0){
            if (strcmp(ps = strtok(NULL, ";"), number) == 0){
                puts("У данного студента уже есть такая книга");
                return;
            }
        }
    }
    puts("Введите дату сдачи");
    scanf("%s", date);
    if (book->students_count == 0){
        puts("Книги закончились");
        return;
    }
    fseek(fp, 0L, SEEK_END);
    fprintf(fp, "%s;%s;%s\n", isbn, number, date);
    book->students_count--;
    fill_file_book();
    fclose(fp);
}

void take_book (){
    FILE *fp;
    char isbn[TSIZE];
    char number[TSIZE];
    char info[SIZE];
    char date[TSIZE];
    char *save[100];
    char *ps = (char *)malloc(sizeof(char) * TSIZE);
    int book_found = 0;
    int student_found = 0;
    Books *book = books_head;
    Students *student = students_head;
    if ((fp = fopen("student_books.csv", "r+")) == NULL){
        puts("Не удалось открыть файл student_books.csv");
        exit(EXIT_FAILURE);
    }
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    for (int i = 0; i < books_struct_size; i++){
        if (strcmp(isbn, book->isbn) == 0){
            book_found = 1;
            break;
        }
        book = book->next;
    }
    if (book_found == 0){
        puts("Книга не найдена");
        return;
    }
    puts("Введите номер студента");
    scanf("%s", number);
    for (int i = 0; i < students_struct_size; i++){
        if (strcmp(number, student->numb) == 0){
            student_found = 1;
            break;
        }
        student = student->next;
    }
    if (student_found == 0){
        puts("Студент не найден");
        return;
    }
    if (book->students_count == book->books_count){
        puts("Достигнуто макс кол-во книг");
        return;
    }
    book->students_count++;
    int i = 0;
    while (fgets(save[i] = (char *)malloc(sizeof(char) * SIZE), SIZE, fp)){
        i++;
    }
    fclose(fp);
    fp = fopen("student_books.csv", "w");
    for (int j = 0; j < i; j++){
        strcpy(info, save[j]);
        if (strcmp(ps = strtok(info, ";"), isbn) == 0)
            if (strcmp(ps = strtok(NULL, ";"), number) == 0)
                continue;
        fprintf(fp, "%s", save[j]);
    }
    fclose(fp);
    fill_file_book();
}

void fill_file_book (){
    FILE *fp;
    Books *iterator = books_head;
    if ((fp = fopen("books.csv", "w")) == NULL){
        puts("Не удалось открыть файл books.csv");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < books_struct_size; i++){
        fprintf(fp, "%s;%s;%s;%d;%d\n", iterator->isbn, iterator->author, iterator->name,
                iterator->books_count, iterator->students_count);
        iterator = iterator->next;
    }
    fclose(fp);
}

// ================================================================================================
// TASK 9
// ================================================================================================

void books_backup () {
    int ch;
    struct tm *loc_time;
    char time_ar[TSIZE];
    const time_t ttime = time(NULL);
    loc_time = localtime(&ttime);
    strftime(time_ar, 50, "%d.%m.%Y_%H:%M:%S", loc_time);
    char filename[TSIZE] = "books_";
    strcat(filename, time_ar);
    strcat(filename, ".csv");
    FILE *in, *out;
    if ((in = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "Не удалось открыть файл %s", filename);
        print_log(login, "EEE Не удалось открыть файл ", filename);
        exit(EXIT_FAILURE);
    };
    if ((out = fopen("books.csv", "r")) == NULL) {
        fprintf(stderr, "Не удалось открыть файл books.csv");
        print_log(login, "EEE Не удалось открыть файл books.csv", "\0");
        exit(EXIT_FAILURE);
    };
    while ((ch = getc(out)) != EOF)
        putc(ch, in);
    puts("Операция прошла успешно");
    print_log(login, "SSS Операция прошла успешно", "\0");
    fclose(in), fclose(out);
}

// ================================================================================================
// TASK 10
// ================================================================================================

void books_recovery(){
    int ch;
    char filename[TSIZE];
    char str[SIZE];
    Books *current;
    FILE *in, *out;
    books_struct_size = 0;
    puts("Введите название файла бэкапа (Пример: books_15.05.2020_17:35:27.csv):");
    scanf("%s", filename);
    print_log(login, "CCC recovery", filename);
    if ((out = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Не удалось открыть файл %s", filename);
        print_log(login, "EEE Не удалось открыть файл ", filename);
        exit(EXIT_FAILURE);
    };
    if ((in = fopen("books.csv", "w+")) == NULL){
        fprintf(stderr, "Не удалось открыть файл books.csv");
        print_log(login, "EEE Не удалось открыть файл books.csv", "\0");
        exit(EXIT_FAILURE);
    };
    while ((ch = getc(out)) != EOF) {
        if (ch == '\n')
            books_struct_size++;
        putc(ch, in);
    }
    fseek(in, 0L, SEEK_SET);
    while(fgets(str, SIZE, in)){
        current = (Books *)malloc(sizeof(Books));
        books_fill(current, str);
    }
    puts("Операция прошла успешно");
    print_log(login, "SSS Операция прошла успешно", "\0");
    fclose(in), fclose(out);
}



void books_fill(Books *p, char s[SIZE]);

void read_books(){
    FILE *fp;
    char str[SIZE];
    struct books *current;
    if ((fp = fopen("books.csv", "r")) == NULL) {
        fprintf(stderr, "Не удалось открыть файл books.csv\n");
        print_log(login, "EEE Не удалось открыть файл books.csv", "\0");
        exit(EXIT_FAILURE);
    }
    while (fgets(str, SIZE, fp)) {
        books_struct_size++;
        current = (struct books *) malloc(sizeof(struct books));
        books_fill(current, str);
    }
    fclose(fp);
}

void books_fill(Books *p, char s[SIZE]){
    int i = 0, j = 0;
    int nametag = 0;
    char ar[SIZE];
    Books* ps = books_head;
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
}


void inclusionSort (char *ar[], int size){
    char value[TSIZE];
    for (int i = 1; i < size; i++){
        strcpy(value, ar[i]);
        int index = i;
// (ar[index - 1] > value)
        while ((index > 0) && strcmp(ar[index - 1], value) > 0){
// ar[index] = ar[index - 1];
            strcpy(ar[index], ar[index - 1]);
            index--;
        }
// ar[index] = value;
        strcpy(ar[index], value);
    }
}

//проверка isbn на 10 цифр
int check_isbn (char *isbn){
    if (strlen(isbn) == 10)
        return 1;
    return 0;
}
