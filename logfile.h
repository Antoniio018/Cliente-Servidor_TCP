#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_HOST 25
#define LINE_SIZE 100

/* Tamaño en bytes */
#define TAM_BUFFER 1024

/* Tabla de errores*/
#define ERROR_NO_OPEN_SOCKET -1
#define ERROR_CORRECT_ARGUMENTS -2
#define ERROR_NOT_FOUND_SERVER -3
#define ERROR_NOT_LINKED_DATA_SOCKET -4
#define ERROR_NO_CONNECT_CLIENT -5
#define ERROR_NOT_FOUND_CLIENT_DATA -6

/* Tipo bool */
typedef enum{false = 0, true} bool;

/* Estructura de configuración */
typedef struct{
    int port;
    char host[LINE_HOST];

}Config;

Config readIni();
void printLog(const char *level, const char *description);
void traducirComando(const char *comando, char *buffer, size_t size);