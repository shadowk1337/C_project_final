#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 600
#define TSIZE 100

typedef struct users{
    char login[TSIZE];
    char password[TSIZE];
    int edit_students;
    int edit_books;
} Users;

typedef struct tnode{
    Users user;
    struct tnode *right;
    struct tnode *left;
}Tnode;

extern Users *account;
void fill_tree (Tnode *ps, char *s);

Tnode *addtree (Tnode *ps, char *s){
    int res;
    char login[SIZE];
    strcpy(login, s);
    strtok(login, " ;");
    if (ps == NULL){
        ps = (Tnode *)malloc(sizeof(Tnode));
        fill_tree(ps, s);
        ps->left = ps->right = NULL;
    }
    else if ((res = strcmp(ps->user.login, login)) == 0) {
        fprintf(stderr, "Пользователь %s встречается более одного раза", ps->user.login);
        exit(EXIT_FAILURE);
    }
    else if (res < 0) {
        ps->left = addtree(ps->left, s);
    }
    else {
        ps->right = addtree(ps->right, s);
    }
    return ps;
}

void print_tree (Tnode *ps){
    if (ps != NULL){
        print_tree(ps->left);
        printf("%s %s %d %d\n", ps->user.login, ps->user.password, ps->user.edit_students, ps->user.edit_books);
        print_tree(ps->right);
    }
}

void fill_tree (Tnode *ps, char *s){
    FILE *f;
    char *ar;
    ar = (char *)malloc(SIZE * sizeof(char));
    int i = 0, j = 0;
    int nametag = 0;
    if ((f = fopen("users.csv", "r")) == NULL){
        fprintf(stderr, "Не удалось открыть файл users.csv");
        exit(EXIT_FAILURE);
    }
    while((ar[i] = s[j])){
        if (s[j] == ';' || s[j] == '\n'){
            ar[i] = '\0';
            if (nametag == 0)
                strcpy(ps->user.login, ar);
            else if (nametag == 1)
                strcpy(ps->user.password, ar);
            else if (nametag == 2)
                ps->user.edit_students = atoi(ar);
            else if (nametag == 3)
                ps->user.edit_books = atoi(ar);
            nametag++, i = 0, j++;
        }
        else {
            i++, j++;
        }
    }
}
