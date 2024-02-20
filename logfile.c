#include "logfile.h"

void printLog(const char *level, const char *description) {
    time_t now = time(0);
    char date_hour[25];
    struct tm *local_time = localtime(&now);
    strftime(date_hour, 25, "[%d/%m/%Y] [%H:%M:%S]", local_time);

    FILE *file = fopen("log.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s - [%s] - %s\n", date_hour, level, description);
        printf("%s - [%s] - %s\n", date_hour, level, description);
        fclose(file);
    } else {
        printf("Error al abrir el archivo de registro");
    }
}