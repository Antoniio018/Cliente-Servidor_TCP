#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/sysinfo.h>

#define LINE_SIZE 100
typedef struct{
    //Añadir los atributos q se van a configurar en el servidor
    int port;
    char host[100];

}Config;

Config readIni();
void printLog(const char *level, const char *description);
void traducirComando(const char *comando, char *buffer, size_t size);