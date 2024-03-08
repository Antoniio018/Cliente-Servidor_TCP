#include "logfile.h"


Config readIni(){
    Config config;
    char *token;
    FILE *file = fopen("config.ini", "r");
    if (file != NULL) {
        char line[LINE_SIZE];
        while(fgets(line, LINE_SIZE, file)!= NULL) {
            char *ptr;
            printf("%s", line);
            if ((ptr = strstr(line, "host =")) != NULL) {
                char *token = strtok(ptr + strlen("host ="), "\"\n");
                if (token != NULL) {
                    if(strcmp("localhost","token") == 0)
                        strcpy(config.host,"127.0.0.1");
                    else
                        strcpy(config.host, token);
                }
            }
            else if((ptr = strstr(line, "port =")) != NULL) {
                char *port = strtok(ptr + strlen("port ="), "\"\n");
                if (port != NULL) {
                    config.port = atoi(port);
                }
            }
        }
    }
    fclose(file);
    return config;
}

void printLog(const char *level, const char *msg) {
    time_t now = time(0);
    char date_hour[25];
    struct tm *local_time = localtime(&now);
    strftime(date_hour, 25, "[%d/%m/%Y] [%H:%M:%S]", local_time);

    FILE *file = fopen("log.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s - [%s] - %s\n", date_hour, level, msg);
        printf("%s - [%s] - %s\n", date_hour, level, msg);
        fclose(file);
    } else {
        printf("Error al abrir el archivo de registro");
    }
}
void traducirComando(const char *comando, char *buffer, size_t size){
    time_t now = time(0);
    struct tm *local_time = localtime(&now);
    struct sysinfo info;
    sysinfo(&info);
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
        strftime(buffer, size, "%I:%M:%S %p", local_time);
    } else if (strcmp(comando, "[GET_HOUR][FORMAT_24H]") == 0) {
        strftime(buffer, size, "%H:%M:%S", local_time);
    } else if (strcmp(comando, "[GET_DATE][FORMAT_H]") == 0) {
        strftime(buffer, size, "%d/%m", local_time);
    } else if (strcmp(comando, "[GET_DATE][FORMAT_F]") == 0) {
        strftime(buffer, size, "%d/%m/%Y", local_time);
    } else if (strcmp(comando, "[GET_NPROC]") == 0) {
        snprintf(buffer, size, "Número de procesos activos: %d", info.procs);
    } else if (strcmp(comando, "[GET_MEMLOAD]") == 0) {
        long totalram = info.totalram;
        long freeram = info.freeram;
        long ram = (totalram - freeram) * 100 / totalram;
        snprintf(buffer, size, "Memoria ram usada (En porcentaje): %ld", ram);
    } else if (strcmp(comando, "[GET_UPTIME]") == 0) {
        snprintf(buffer, size, "Tiempo en segundos: %ld", info.uptime); 
    //10 para comparar el principio del comando y saber que se refiere printf("Memoria ram usada en porcentaje:%ld\n", (totalram - freeram) * 100 / totalram);
    //a un formato de hora o fecha pero se ha equivocado con el comando completo
    } else if (strncmp(comando, "[GET_HOUR]", 10) == 0) {
        //Formato de hora no reconocido
        snprintf(buffer, size, "[ERROR][HOUR]");
    } else if (strncmp(comando, "[GET_DATE]", 10) == 0) {
        //Formato de fecha no reconocido
        snprintf(buffer, size, "[ERROR][DATE]");
    } else {
        //Comando no reconocido
        snprintf(buffer, size, "[ERROR][NO_SUPPORT]");
    }
}
/* https://manpages.ubuntu.com/manpages/trusty/es/man2/sysinfo.2.html 
    struct sysinfo {
                    long uptime;              // Segundos desde el arranque 
                    unsigned long loads[3];   // cargas medias en 1, 5, y 15 min 
                    unsigned long totalram;   // Mem. pral. total útil 
                    unsigned long freeram;    // Tamaño de memoria disponible 
                    unsigned long sharedram;  // Cantidad de memoria compartida 
                    unsigned long bufferram;  // Memoria empleaada por búferes 
                    unsigned long totalswap;  // Tamaño del espacio total de swap 
                    unsigned long freeswap;   // Espacio de swap aún disponible 
                    unsigned short procs;     // Nº de procesos actualmente 
                    unsigned long totalhigh;  // Tamaño total de memoria alta 
                    unsigned long freehigh;   // Tamaño disponible de memoria alta 
                    unsigned int mem_unit;    // Tamaño de la unidad de memoria en bytes 
                    char _f[20-2*sizeof(long)-sizeof(int)]; // Relleno para libc5 
    }
*/