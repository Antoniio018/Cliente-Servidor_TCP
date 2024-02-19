#include "logfile.h"

void printLog(const char *level, const char *description) {
    time_t now = time(0);
    char date_hour[20];
    struct tm *local_time = localtime(&now);
    strftime(timestamp, 20, "[%d/%m/%Y] [%H:%M:%S]", local);

    FILE *file = fopen("log.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s - [%s] - %s\n", timestamp, level, description);
        printf(file, "%s - [%s] - %s\n", timestamp, level, description);
        fclose(file);
    } else {
        printf("Error al abrir el archivo de registro");
    }
}