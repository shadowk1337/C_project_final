#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binary_tree.h"
#include "students.h"
#include "books.h"
#define SIZE 600
#define TSIZE 100

int auth(Tnode *);

Users *account;
struct students *students_head = NULL;
int students_struct_size;
struct books *books_head = NULL;
int books_struct_size;

int main() {
    FILE *fp;
    Tnode *p = NULL;
    char s[SIZE], str[SIZE];
    int choice, terminate = 0;
    int vip, comeback = 1;
    if ((fp = fopen("users.csv", "r")) == NULL) {
        fprintf(stderr, "Не удалось открыть файл users.csv");
        exit(EXIT_FAILURE);
    }
    while (fgets(s, SIZE, fp)) {
        p = addtree(p, s);
    }
    fclose(fp);
    while (auth(p) != 1);
    while (comeback == 1) {
        comeback = 0;
        if (account->edit_students == 1 && account->edit_books == 1) {
            puts("Выберите, что редактировать:\n"
                 "1)Студенты\n"
                 "2)Книги");
            while (scanf("%d", &choice)) {
                if (choice == 1) {
                    vip = 1;
                    break;
                } else if (choice == 2) {
                    vip = 2;
                    break;
                } else
                    puts("Неправильный ввод");
            }
        }
        //1;0
        if ((account->edit_students == 1 && account->edit_books == 0) || vip == 1) {
            struct students *current;
            if ((fp = fopen("students.csv", "r")) == NULL) {
                fprintf(stderr, "Не удалось открыть файл %s", "students.csv");
                exit(EXIT_FAILURE);
            };
            while (fgets(str, SIZE, fp)) {
                students_struct_size++;
                //реализация связного списка + заполнение данными из файла
                current = (struct students *) malloc(sizeof(struct students));
                fill(current, str);
            }
            fclose(fp);
            puts("Выберите операцию:\n"
                 "1)Добавить студента\n"
                 "2)Удалить студента по номеру зачетной книжки\n"
                 "3)Бэкап\n"
                 "4)Восстановить базу из файла бэкапа\n"
                 "5)Поиск по фамилии\n"
                 "6)Завершить работу программы");
            if (vip == 1)
                puts("0)Вернуться в меню выбора");
            printf("> ");
            while (scanf("%d", &choice)) {
                getchar();
                switch (choice) {
                    case 1:
                        add_student();
                        break;
                    case 2:
                        delete_student();
                        break;
                    case 3:
                        backup();
                        break;
                    case 4:
                        recovery();
                        break;
                    case 5:
                        search();
                        break;
                    case 6:
                        terminate = 1;
                        puts("Работа программы завершена");
                        break;
                    case 0:
                        comeback = 1;
                        terminate = 1;
                        break;
                }
                if (terminate)
                    break;
                puts("\nВыберите операцию:\n"
                     "1)Добавить студента\n"
                     "2)Удалить студента по номеру зачетной книжки\n"
                     "3)Бэкап\n"
                     "4)Восстановить базу из файла бэкапа\n"
                     "5)Поиск по фамилии\n"
                     "6)Завершить работу программы");
                if (vip == 1)
                    puts("0)Вернуться в меню выбора");
                printf("> ");
            }
        } else if ((account->edit_students == 0 && account->edit_books == 1) || vip == 2) {
            struct books *current;
            if ((fp = fopen("books.csv", "r")) == NULL) {
                fprintf(stderr, "Не удалось открыть файл books.csv\n");
                exit(EXIT_FAILURE);
            }
            while (fgets(str, SIZE, fp)) {
                books_struct_size++;
                current = (struct books *) malloc(sizeof(struct books));
                fill_books(current, str);
            }
            puts("Выберите операцию:\n"
                 "1)Добавить книгу\n"
                 "2)Удалить книгу\n"
                 "3)Просмотр информации о книге\n"
                 "4)Вывести таблицу книг\n"
                 "5)Завершить программу");
            if (vip == 2)
                puts("0)Вернуться в меню выбора");
            printf("> ");
            while (scanf("%d", &choice)) {
                getchar();
                switch (choice) {
                    case 1:
                        add_book();
                        break;
                    case 2:
                        delete_book();
                        break;
                    case 3:
                        search_book();
                        break;
                    case 4:
                        print_book();
                        break;
                    case 5:
                        terminate = 1;
                        puts("Работа программы завершена");
                        break;
                    case 0:
                        comeback = 1;
                        terminate = 1;
                        break;
                }
                if (terminate)
                    break;
                puts("\nВыберите операцию:\n"
                     "1)Добавить книгу\n"
                     "2)Удалить книгу\n"
                     "3)Просмотр информации о книге\n"
                     "4)Вывести таблицу книг\n"
                     "5)Завершить программу");
                if (vip == 2)
                    puts("0)Вернуться в меню выбора");
                printf("> ");
            }
        } else
            puts("Аккаунт не имеет привилегий");
    }
}

Users *find_acc (Tnode *ps, char *s1, char *s2, int *status);

int auth(Tnode *ps){
    int *status = (int *)malloc(sizeof(int));
    Users *acc_info = (Users *)malloc(sizeof(Users));
    *status = 0;
    char login[TSIZE], password[TSIZE];
    puts("Введите логин:");
    scanf("%s", login);
    puts("Введите пароль:");
    scanf("%s", password);
    acc_info = find_acc(ps, login, password, status);
    if (*status) {
        puts("Вы авторизированы");
        account = acc_info;
        return 1;
    }
    puts("Неправильный логин/пароль");
    return 0;
}

Users *find_acc (Tnode *ps, char *s1, char *s2, int *status){
    static Users *login_info;
    if (ps != NULL){
        find_acc(ps->left, s1, s2, status);
        if ((strcmp(ps->user.login, s1) == 0) && (strcmp(ps->user.password, s2) == 0)) {
            *status = 1;
            login_info = &ps->user;
        }
        find_acc(ps->right, s1, s2, status);
    }
    return login_info;
}
