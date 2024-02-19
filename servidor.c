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

#include <stdio.h>
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
        //logfile->write("[ERROR]", "No se puede abrir un socket");
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

    /* Enlaza los datos de conexión con el socket */
    if(bind(socksvr,(struct sockaddr *) &name, sizeof(name)) < 0){
        perror("Error en datos del socket\n");
        //logfile->write("[ERROR]", "No se puede enlazar los datos al socket");
        return -4; // hacer otra constante
    }

    //Pone al socket en modo de escucha
    listen(socksvr, 1); //Numero conexiones de escucha, parametro para configuar en el ini

    /* Ciclo de servicio*/
    for(;;){
        //al log
        printf("esperando conexiones...\n");

        // Aceptar la conexión y pasa al cliente a otro socket (datos)
        sockdata = accept(socksvr, (struct sockaddr *)NULL, (socklen_t *)NULL);

        if(sockdata < 0){
            //Log
            perror("Error en conexión con el cliente\n");
            return -5;
        }
        else{
            //Socket cliente válido
            /* Limpia el buffer */
            //memcpy(buffer, "", TAM_BUFFER);
            memset(buffer, '\0', TAM_BUFFER);

            //Hay que leer del socket para identificar el tipo de petición
            datos_leidos = read(sockdata, buffer, TAM_BUFFER);
            if(datos_leidos < 0){
                //log
                perror("en petición cliente\n");
            }
            else{
                printf("Conexión establecida.\n");

                //Muestra datos
                printf("[DATOS] -> %s\n", buffer);

                // Explorar los datos recibidos y verificar el formato
                //hacer una función para ver el buffer y determinar 
                //si la trama es correcta o no

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