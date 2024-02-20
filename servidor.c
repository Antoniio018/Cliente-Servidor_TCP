/*
    Servidor TCP
    Orientado a conexión -> Circuitos virtuales
    Monoproceso y monohilo

    cliente     servidor
    -------     --------
    write() --> read()
    read()  <-- write()
    ....
    ....
*/
/*
    gcc -c logfile.c -o logfile.o
    gcc -c servidor.c -o servidor.o
    gcc logfile.o servidor.o -o servidor
*/
#include "logfile.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Tabla de errores*/
#define ERROR_NO_OPEN_SOCKET -1
#define ERROR_CORRECT_ARGUMENTS -2
#define ERROR_NOT_FOUND_SERVER -3
#define ERROR_NOT_LINKED_DATA_SOCKET -4
#define ERROR_NO_CONNECT_CLIENT -5

/*Tamaño en bytes*/
#define TAM_BUFFER 1024

/* Tipo bool */
typedef enum{false = 0, true} bool;

int main(void){
    /* Socket de escucha*/
    int socksvr;

    /* Socket de transferecia de datos*/
    int sockdata;

    int datos_leidos;

    struct sockaddr_in name;
    char buffer[TAM_BUFFER];

    /* Abre el socket de escucha */
    socksvr = socket(PF_INET, SOCK_STREAM, 0);
    if(socksvr < 0){
        perror("En socket server...\n");
        printLog("ERROR", "No se puede abrir un socket");
        return ERROR_NO_OPEN_SOCKET;
    }

    /* Asigna los parámetros de conexión al socket */
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY iria lo que diga el ini
    name.sin_port = htons(1223);//el puerto sería tb lo que diga el ini

    /* Servidor está activo */
    printf("Servidor funcionando...\n");
    printf("Direccion IP : %s\n", inet_ntoa(name.sin_addr));
    printf("Puerto : %d\n", ntohs(name.sin_port));
    printLog("NOTIFY", "Se ha iniciado el servidor");

    /* Enlaza los datos de conexión con el socket */
    if(bind(socksvr,(struct sockaddr *) &name, sizeof(name)) < 0){
        perror("Error en datos del socket\n");
        printLog("ERROR", "No se puede enlazar los datos al socket");
        return ERROR_NOT_LINKED_DATA_SOCKET;
    }

    //Pone al socket en modo de escucha
    listen(socksvr, 1); //Numero conexiones de escucha, parametro para configuar en el ini

    /* Ciclo de servicio*/
    for(;;){
        //al log
        printLog("NOTIFY", "Servidor en espera de conexión");
        printf("esperando conexiones...\n");

        // Aceptar la conexión y pasa al cliente a otro socket (datos)
        sockdata = accept(socksvr, (struct sockaddr *)NULL, (socklen_t *)NULL);

        if(sockdata < 0){
            //Log
            printLog("ERROR", "Conexión con el cliente fallida");
            perror("Error en conexión con el cliente\n");
            return ERROR_NO_CONNECT_CLIENT;
        }
        else{
            //Socket cliente válido
            /* Limpia el buffer */
            //memcpy(buffer, "", TAM_BUFFER);
            memset(buffer, '\0', TAM_BUFFER);

            //Hay que leer del socket para identificar el tipo de petición
            datos_leidos = read(sockdata, buffer, TAM_BUFFER);
            if(datos_leidos < 0){
                printLog("ERROR", "Petición cliente fallida");
                perror("en petición cliente\n");
            }
            else{
                printf("Conexión establecida.\n");

                //¿¿Se podria escribir algun dato del cliente??
                printLog("NOTIFY", "Conexión establecida con el servidor");

                //Muestra datos
                printf("[DATOS] -> %s\n", buffer);

                //Comprueba el formato de los datos recibidos y devuelve una respuesta
                char respuesta[TAM_BUFFER];
                obtenerHoraFecha(buffer, respuesta, sizeof(respuesta));
                printf("[RESPUESTA] -> %s\n", respuesta);
                
                //Enviando respuesta al cliente
                write(sockdata, respuesta, strlen(respuesta));
            }

            //Cierre conexión cliente
            printf("Cierra conexión cliente\n");
            close(sockdata);
        }
    }

    //Cierra el socket de escucha
    close(socksvr);

    return 0;
}