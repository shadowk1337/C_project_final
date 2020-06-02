#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binary_tree.h"
#include "students.h"
#include "books.h"
#include "log.h"
#define SIZE 600
#define TSIZE 100

int auth(Tnode *);
char login[TSIZE];

Users *account;
struct students *students_head = NULL;
int students_struct_size;
struct books *books_head = NULL;
int books_struct_size;

int main() {
    FILE *fp;
    Tnode *p = NULL;
    char s[SIZE];
    int choice, terminate = 0; // переменные для обозначения выбора и заврешения программы
    int vip, comeback = 1; // vip - выбор меню для пользователя (1 и 2) comeback - возвращение в меню выбора для суперпользователя
    fp = open_file("users.csv", "r");
    read_students(); // считывание данных из файла students.csv в односвязный список
    read_books(); // считывание данных из файла books.csv в односвязный список
    while (fgets(s, SIZE, fp)) { // добавление информации об аккаунтах из файла users.csv в бинарное дерево
        p = addtree(p, s);
    }
    fclose(fp);
    while (auth(p) != 1); // пока не введены корректные логие и пароль
    while (comeback == 1) {
        comeback = 0, terminate = 0;
        if (account->edit_students == 1 && account->edit_books == 1) { // функция для суперпользователя
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
                } else{
                    puts("Неправильный ввод");
                    print_log(login, "WWW Неправильный ввод", "\0");
                }
            }
        }
        if ((account->edit_students == 1 && account->edit_books == 0) || vip == 1) { // функции для 1;0
            students_menu(vip);
            while (scanf("%d", &choice)) {
                getchar();
                switch (choice) {
                    case 1:
                        print_log(login, "add_student", "\0");
                        add_student();
                        break;
                    case 2:
                        print_log(login, "delete_student", "\0");
                        delete_student();
                        break;
                    case 3:
                        print_log(login, "backup", "\0");
                        backup();
                        break;
                    case 4:
                        print_log(login, "recovery", "\0");
                        recovery();
                        break;
                    case 5:
                        print_log(login, "search_student", "\0");
                        search();
                        break;
                    case 6:
                        print_log(login, "search_student_id", "\0");
                        search_id();
                        break;
                    case 7:
                        print_log(login, "terminate_student_program", "\0");
                        print_log(login, "SSS Работа программы завершена", "\0");
                        terminate = 1;
                        puts("Работа программы завершена");
                        break;
                    case 0:
                        print_log(login, "сomeback_to_choosing_menu", "\0");
                        comeback = 1;
                        terminate = 1;
                        break;
                }
                if (terminate)
                    break;
                students_menu(vip);
            }
        } else if ((account->edit_students == 0 && account->edit_books == 1) || vip == 2) { // функции для 0;1
            books_menu(vip);
            while (scanf("%d", &choice)) {
                getchar();
                switch (choice) {
                    case 1:
                        print_log(login, "add_book", "\0");
                        add_book();
                        break;
                    case 2:
                        print_log(login, "delete_book_public", "\0");
                        delete_book_public();
                        break;
                    case 3:
                        print_log(login, "search_book", "\0");
                        search_book();
                        break;
                    case 4:
                        print_log(login, "print_book", "\0");
                        print_book();
                        break;
                    case 5:
                        print_log(login, "search_book_isbn", "\0");
                        search_isbn();
                        break;
                    case 6:
                        print_log(login, "redact_book", "\0");
                        redact_book();
                        break;
                    case 7:
                        print_log(login, "issuance_book", "\0");
                        issuance_book();
                        break;
                    case 8:
                        print_log(login, "take_book", "\0");
                        take_book();
                        break;
                    case 9:
                        print_log(login, "change_amount", "\0");
                        change_amount();
                        break;
                    case 10:
                        print_log(login, "books_backup", "\0");
                        books_backup();
                        break;
                    case 11:
                        print_log(login, "books_recovery", "\0");
                        books_recovery();
                        break;
                    case 12:
                        print_log(login, "seaech_by_author", "\0");
                        search_by_author();
                        break;
                    case 13:
                        print_log(login, "termianate_book_program", "\0");
                        print_log(login, "SSS Работа программы завершена", "\0");
                        terminate = 1;
                        puts("Работа программы завершена");
                        break;
                    case 0:
                        print_log(login, "сomeback_to_choosing_menu", "\0");
                        comeback = 1;
                        terminate = 1;
                        break;
                }
                if (terminate)
                    break;
                books_menu(vip);
            }
        }
        else { // функции для 0;0
            puts("Аккаунт не имеет привилегий");
            print_log("unauthorized", "SSS Аккаунт не имеет привилегий", "\0");
        }
    }
}

Users *find_acc (Tnode *ps, char *s1, char *s2, int *status);

int auth(Tnode *ps){ // аутентификация пользователя
    int *status = (int *)malloc(sizeof(int));
    *status = 0; // переменная для обозначения был ли найден аккаунт (1 - найден, 0 - нет)
    Users *acc_info = (Users *)malloc(sizeof(Users));
    char password[TSIZE];
    puts("Введите логин:");
    scanf("%s", login);
    puts("Введите пароль:");
    scanf("%s", password);
    acc_info = find_acc(ps, login, password, status);
    if (*status) {
        puts("Вы авторизированы");
        print_log(login, "================ SSS Вы авторизованы ================", "\0");
        account = acc_info; // запись данных аккаунта в глобальную переменную
        return 1;
    }
    puts("Неправильный логин/пароль");
    print_log("unauthorized", "WWW Неправильный логин/пароль", "\0");
    return 0;
}

Users *find_acc (Tnode *ps, char *s1, char *s2, int *status){ // проверка на соответсвие логин + пароль (поиск в бинарном дереве)
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
