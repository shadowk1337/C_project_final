#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "log.h"
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

void add_book (){ // добавление книги
    FILE *f;
    int i = 0, find = 0, punct_count = 0; // i - итератор для поиска первого знака точки с запятой find - обозначение найденного знака точки с запятой punct_count - подсчет количества знаков точки с запятой
    Books *iterator = books_head, *current;
    char info[SIZE], first_word[TSIZE];
    f = open_file("books.csv", "r+");
    fseek(f, 0L, SEEK_END); // переход в конец файла
    puts("Введите информацию о книге через точку с запятой без пробела (Пример: 9666965348;Стругацкие;Град обреченный;10;5):");
    fgets(info, SIZE, stdin);
    for (int k = 0; k < strlen(info); k++){ // подсчет количества точек с запятыми, нахождение первого слова (isbn книги)
        if (info[k] != ';' && find == 0){
            first_word[i++] = info[k];
        }
        else if (info[k] == ';') {
            find = 1;
            punct_count++;
        }
    }
    first_word[i] = '\0'; // isbn
    if (check_isbn(first_word) != 1){
        puts("В isbn должно быть 10 цифр —Завершение функции--");
        print_log(login, "WWW В isbn должно быть 10 цифр —Завершение функции--", "\0");
        return;
    }
//    printf("%d\n", first_word);
    if (punct_count != 4){
        puts("Должно быть 5 значений\n--Завершение функции--");
        print_log(login, "WWW Должно быть 5 значений --Завершение функции--", "\0");
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
    books_struct_size++; // увеличение числа книг
    current = (Books *)malloc(sizeof(Books));
    books_fill(current, info); // новая нода в списке
    fprintf(f, "%s", info); // печать в файл
    fclose(f);
    puts("Книга добавлена");
    print_log(login, "SSS Книга добавлена", "\0");
}

int delete_book(char *number, int access);

void delete_book_public(){ // интерфейс функции удаления пользователя
    FILE *f;
    char number[TSIZE];
    puts("Введите номер ISBN книги:");
    if (scanf("%s", number)){
        if (delete_book(number, 1) == 0)
            return;
    }
    else{
        fprintf(stderr, "Неправильный ввод\n");
        print_log(login, "WWW Неправильный ввод", "\0");
        exit(EXIT_FAILURE);
    }
    puts("Книга удалена");
    print_log(login, "SSS Книга удалена", "\0");
    print_log(login, "CCC delete_book", number);
}

int delete_book(char *number, int access){ // удаление книги
    FILE *f, *check;
    int i, j;
    char *isbn = (char *)malloc(11 * sizeof(char));
    int found = 0;
    Books *iterator1 = books_head, *prev, *iterator2;
    char info[SIZE];
    check = open_file("student_books.csv", "r");
    if (books_struct_size != 0) { // список не пуст
        if (access == 1) {
            while (fgets(info, SIZE, check)) {
                isbn = strtok(info, ";");
                if (strcmp(number, isbn) == 0) {
                    printf("Нельзя удалить книгу с номером %s\n", number);
                    print_log(login, "WWW Нельзя удалить книгу с номером ", number);
                    return 0;
                }
            }
        }
        fclose(check);
        f = open_file("books.csv", "w");
        for (i = 0; i < books_struct_size; i++){ // удаление книги
            if (strcmp(number, iterator1->isbn) == 0){
                found = 1;
                if (i == 0 && books_struct_size == 1){ // в файле одна книга
                    books_head = NULL;
                }
                else if (i == 0 && books_struct_size >= 2){ // книга находится в начале списка
                    books_head = iterator1->next;
                }
                else if (i == books_struct_size - 1){ // книга находится в конце списка
                    prev->next = NULL;
                    free(iterator1);
                }
                else {
                    prev->next = iterator1->next;
                    free(iterator1);
                }
                books_struct_size--;
                break;
            }
            prev = iterator1;
            iterator1 = iterator1->next;
        }
        if (found == 0){
            puts("Книга не найдена");
            print_log(login, "WWW Книга не найдена", "\0");
            return 0;
        }
        if (books_head != NULL) { // печать в файл
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
        return 0;
    }
    return 1;
}

void search_book (){ // поиск книги
    int found = 0; // найдена ли книга
    char isbn[TSIZE];
    struct books *iterator = books_head;
    if (no_data(books_struct_size))
        return;
    puts("Введите ISBN:");
    scanf("%s", isbn);
    print_log(login, "CCC search_book", isbn);
    for (int i = 0; i < books_struct_size; i++){ // поиск книги
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

void print_book (){  // вывод таблицы книг
    char *ar_isbn[books_struct_size];
    if (no_data(books_struct_size))
        return;
    Books *iterator = books_head;
    for (int i = 0; i < books_struct_size; i++){
        ar_isbn[i] = iterator->isbn;
        iterator = iterator->next;
    }
    inclusionSort(ar_isbn, books_struct_size); // сортировка

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

void get_read_book (char *to);

void redact_book (){ // редактирование информации о книге
    FILE *f;
    int flag = 0; // найдена ли книга
    char isbn[TSIZE], info[SIZE];
    char str[TSIZE]; // массив для считывания
    Books *book = books_head;
    if (no_data(books_struct_size))
        return;
    open_file("books.csv", "r+");
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    print_log(login, "CCC redact_book", isbn);
    f = open_file("books.csv", "r+");
    for (int i = 0; i < books_struct_size; i++, book = book->next){ // поиск книги в списке
        if (strcmp(book->isbn, isbn) == 0) {
            flag = 1;
            break;
        }
    }
    if (flag != 1){
        puts("Такой книги не нашлось");
        print_log(login, "WWW Такой книги не нашлось", "\0");
        return;
    }
    delete_book(isbn, 0);
    strcpy(info, isbn);
    strcat(info, ";");
    puts("Введите авторов книги");
    get_read_book(info);
    print_log(login, "CCC redact_book", info);
    puts("Введите название книги");
    get_read_book(info);
    print_log(login, "CCC redact_book", info);
    puts("Введите общее количество книг");
    get_read_book(info);
    print_log(login, "CCC redact_book", info);
    sprintf(str, "%d", book->students_count);
    strcat(info, str);
    strcat(info, "\n");
    books_fill(book, info);
    puts(info);
    fseek(f, 0L, SEEK_END);
    fprintf(f, "%s", info);
    fclose(f);
}

void get_read_book (char *to){ // получение следующей части строки
    char str[TSIZE];
    scanf("%s", str);
    strcat(to, str);
    strcat(to, ";");
}

void search_isbn(){ // поиск по ISBN
    FILE *f, *check;
    char book_id[TSIZE], info[TSIZE];
    char info_students[TSIZE];
    char *isbn, *number, *id_in_students, *date;
    puts("Введите ISBN книги");
    if (scanf("%s", book_id)){
        print_log(login, "CCC search_book_isbn", book_id);
        check = open_file("student_books.csv", "r");
        f = open_file("students.csv", "r");
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
                            id_in_students = strtok(NULL, ";\n");
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

void change_amount (){ // поменять общее количество какой-то книги
    Books *book = books_head;
    char isbn[TSIZE];
    int new_size;
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    print_log(login, "CCC change_amount", isbn);
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
    print_log(login, "WWW Книги не найдено", isbn);
    return;
}

char *search_closest_date(); // функция поиска наименьшей даты

void issuance_book(){ // выдача книги
    FILE *fp;
    char isbn[TSIZE];
    char number[TSIZE];
    char info[SIZE];
    char date[TSIZE];
    char *ps = (char *)malloc(sizeof(char) * TSIZE);
    int book_found = 0; // найдена ли книга
    int student_found = 0; // найден ли студент
    Books *book = books_head;
    Students *student = students_head;
    fp = open_file("student_books.csv", "r+");
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    print_log(login, "CCC issuance_book", isbn);
    for (int i = 0; i < books_struct_size; i++){ // поиск книги по isbn
        if (strcmp(isbn, book->isbn) == 0){
            book_found = 1;
            break;
        }
        book = book->next;
    }
    if (book_found == 0){
        puts("Книга не найдена");
        print_log(login, "WWW книга не найдена", "\0");
        return;
    }
    if (book->students_count == 0){ // если число оставшихся книг == 0
        puts("Книги закончились");
        printf("Следующую книгу сдадут %s числа\n", search_closest_date());
        print_log(login, "WWW Книги закончились\nСледующую книгу сдадут", search_closest_date());
        return;
    }
    puts("Введите номер студента");
    scanf("%s", number);
    print_log(login, "CCC issuance_book", number);
    for (int i = 0; i < students_struct_size; i++){ // поиск студента по номеру
        if (strcmp(number, student->numb) == 0){
            student_found = 1;
            break;
        }
        student = student->next;
    }
    if (student_found == 0){
        puts("Студент не найден");
        print_log(login, "WWW Студет не найден", "\0");
        return;
    }
    while (fgets(info, SIZE, fp)){ // имеет ли студент уже эту книгу
        if (strcmp(ps = strtok(info, ";"), isbn) == 0){
            if (strcmp(ps = strtok(NULL, ";"), number) == 0){
                puts("У данного студента уже есть такая книга");
                print_log(login, "WWW У данного студента уже есть такая книга", "\0");
                return;
            }
        }
    }
    puts("Введите дату сдачи");
    scanf("%s", date);
    print_log(login, "CCC issuance_book", date);
    fseek(fp, 0L, SEEK_END);
    fprintf(fp, "%s;%s;%s\n", isbn, number, date); // печать в файл
    book->students_count--;
    fill_file_book();
    fclose(fp);
}

void take_book (){ // выдача книги
    FILE *fp;
    char isbn[TSIZE];
    char number[TSIZE];
    char info[SIZE];
    char date[TSIZE];
    char *save[100]; //
    char *ps = (char *)malloc(sizeof(char) * TSIZE);
    int book_found = 0; // найдена ли книга
    int student_found = 0; // найден ли студент
    Books *book = books_head;
    Students *student = students_head;
    fp = open_file("student_books.csv", "r+");
    puts("Введите ISBN книги");
    scanf("%s", isbn);
    print_log(login, "CCC take_book", isbn);
    for (int i = 0; i < books_struct_size; i++){ // найдена ли книга
        if (strcmp(isbn, book->isbn) == 0){
            book_found = 1;
            break;
        }
        book = book->next;
    }
    if (book_found == 0){
        puts("Книга не найдена");
        print_log(login, "WWW Книга не найдена", "\0");
        return;
    }
    puts("Введите номер студента");
    scanf("%s", number);
    print_log(login, "CCC take_book", number);
    for (int i = 0; i < students_struct_size; i++){ // найден ли студент
        if (strcmp(number, student->numb) == 0){
            student_found = 1;
            break;
        }
        student = student->next;
    }
    if (student_found == 0){
        puts("Студент не найден");
        print_log(login, "WWW Студент не найден", "\0");
        return;
    }
    if (book->students_count == book->books_count){
        puts("Достигнуто макс кол-во книг");
        print_log(login, "WWW Достигнуто макс кол-во книг", "\0");
        return;
    }
    book->students_count++;
    int i = 0;
    while (fgets(save[i] = (char *)malloc(sizeof(char) * SIZE), SIZE, fp)){
        i++;
    }
    fclose(fp);
    fp = open_file("student_books.csv", "w");
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

void fill_file_book (){ // заполнение файла books.csv книгами из списка
    FILE *fp;
    Books *iterator = books_head;
    fp = open_file("books.csv", "w");
    for (int i = 0; i < books_struct_size; i++){
        fprintf(fp, "%s;%s;%s;%d;%d\n", iterator->isbn, iterator->author, iterator->name,
                iterator->books_count, iterator->students_count);
        iterator = iterator->next;
    }
    fclose(fp);
}

void books_backup () { // бэкап
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
    in = open_file(filename, "w");
    out = open_file("books.csv", "r");
    while ((ch = getc(out)) != EOF)
        putc(ch, in);
    puts("Операция прошла успешно");
    print_log(login, "SSS Операция прошла успешно", "\0");
    fclose(in), fclose(out);
}

void books_recovery(){ // восстановление файла бэкапа
    int ch;
    char filename[TSIZE];
    char str[SIZE];
    Books *current;
    FILE *in, *out;
    books_struct_size = 0;
    puts("Введите название файла бэкапа (Пример: books_15.05.2020_17:35:27.csv):");
    scanf("%s", filename);
    print_log(login, "CCC recovery", filename);
    out = open_file(filename, "r");
    in = open_file("books.csv", "w+");
    while ((ch = getc(out)) != EOF) {
        if (ch == '\n')
            books_struct_size++;
        putc(ch, in);
    }
    fseek(in, 0L, SEEK_SET);
    while(fgets(str, SIZE, in)){ // заполнение списка данными из файла
        current = (Books *)malloc(sizeof(Books));
        books_fill(current, str);
    }
    puts("Операция прошла успешно");
    print_log(login, "SSS Операция прошла успешно", "\0");
    fclose(in), fclose(out);
}

void search_by_author(){ // поиск книги по именам авторов
    Books *book = books_head;
    char author[TSIZE];
    puts("Введите фамилию автора");
    scanf("%s", author);
    if (no_data(books_struct_size))
        return;
    for (int i = 0; i < books_struct_size; i++){
        if (strcmp(author, book->author) == 0){
            printf("%s %s %s %d %d\n", book->isbn, book->author, book->name, book->books_count, book->students_count);
        }
        book = book->next;
    }
}

void books_fill(Books *p, char s[SIZE]);

void read_books(){ // заполнение списка данными из файла books.csv
    FILE *fp;
    char str[SIZE];
    struct books *current;
    fp = open_file("books.csv", "r");
    while (fgets(str, SIZE, fp)) {
        books_struct_size++;
        current = (struct books *) malloc(sizeof(struct books));
        books_fill(current, str);
    }
    fclose(fp);
}

void books_fill(Books *p, char s[SIZE]){ // заполнение списка
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

int date_to_int (char *str);

char *search_closest_date(){ // поиск наименьшей даты
    FILE *f;
    char get_from_file[SIZE];
    char *ps, *ps_dot1, *ps_dot2;
    int first = 0;
    char *min_date = (char *)malloc(sizeof(char) * TSIZE);
    int date1, date2;
    f = open_file("student_books.csv", "r");
    while (fgets(get_from_file, SIZE, f)){
        ps = strtok(get_from_file, ";");
        ps = strtok(NULL, ";");
        ps = strtok(NULL, "\n");
        if (first == 0){
            strcpy(min_date, ps);
            date2 = date_to_int(ps);
            first++;
            continue;
        }
        date1 = date_to_int(ps);
        if (date1 < date2){
            strcpy(min_date, ps);
            date2 = date1;
        }
    }
    return min_date;
}

int date_to_int (char *str){ // перевод даты в значение тип int и удаление точек между числами
    int d, m, y;
    int date;
    if (sscanf(str,"%d.%d.%d",&d,&m,&y) == 3)
        date = y * 10000 + m * 100 + d;
    return date;
}

void inclusionSort (char *ar[], int size){ // сортировка вставками
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

int check_isbn (char *isbn){ // проверка isbn на 10 цифр
    if (strlen(isbn) == 10)
        return 1;
    return 0;
}
