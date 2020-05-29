#ifndef HOM1_LOG_H
#define HOM1_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define AR_SIZE 200

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

#endif
