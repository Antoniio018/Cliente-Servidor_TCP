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
void obtenerHoraFecha(const char *comando, char *buffer, size_t tamano){
    time_t now = time(0);
    struct tm *local_time = localtime(&now);

    /*
        %Y para el año con cuatro dígitos
        %m para el mes en formato numérico
        %d para el día del mes
        %H para la hora en formato de 24 horas
        %I para representar la hora en formato de 12 horas 
        %M para los minutos
        %S para los segundos
        %p para indicar AM o PM en el formato de 12 horas
    */
    //size_t strftime(char *s, size_t max, const char *format, const struct tm *timeptr)
    if (strcmp(comando, "[GET_HOUR][FORMAT_12H]") == 0) {
        strftime(buffer, tamano, "%I:%M:%S %p", local_time);
    } else if (strcmp(comando, "[GET_HOUR][FORMAT_24H]") == 0) {
        strftime(buffer, tamano, "%H:%M:%S", local_time);
    } else if (strcmp(comando, "[GET_DATE][FORMAT_H]") == 0) {
        strftime(buffer, tamano, "%d/%m", local_time);
    } else if (strcmp(comando, "[GET_DATE][FORMAT_F]") == 0) {
        strftime(buffer, tamano, "%d/%m/%Y", local_time);
    }
}