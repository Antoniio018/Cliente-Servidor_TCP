/*
    Servidor TCP
    Orientado a conexión -> Circuitos virtuales
    Monoproceso y monohilo

    cliente [IP_SERVER] [PUERTO] [mensaje]
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
#define ERROR_INCORRECT_ARGUMENTS -2
#define ERROR_NOT_FOUND_SERVER -3

/*Tamaño en bytes*/
#define TAM_BUFFER 1024

/* Tipo bool */
typedef enum{false = 0, true} bool;

int main(int argc, char **argv){
    int sock;
    struct sockaddr_in name;
    struct hostent *host;
    char buffer[TAM_BUFFER];
    int datos_leidos;

    /* Controla los argumentos pasados*/
    if(argc != 4){
        perror("Numero de argumentos erroneos");
        return ERROR_INCORRECT_ARGUMENTS;
    }
    
    /* Abre el socket del cliente */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Error al abrir el socket");
        return ERROR_NO_OPEN_SOCKET;
    }

    /* Asigna las propiedades del socket */
    name.sin_family = AF_INET;
    name.sin_port = htons(atoi(argv[2]));
    
    /* Verifica la existencia del servidor */
    host = gethostbyname(argv[1]);
    if(host == NULL){
        printf("Warning, servidor %s no existe", argv[1]);
        return ERROR_NOT_FOUND_SERVER;
    }
    
    /* Copia la dirección del servidor en el socket */
    memcpy((char *)&name.sin_addr, (char * ) host->h_addr, host->h_length);

    if(connect(sock,(struct sockaddr *) &name, sizeof(name)) < 0){
        perror("Error de conexión con servidor\n");
        return -4;
    }
    else{
        /* Envio correcto de datos */
        if(write(sock, argv[3], strlen(argv[3])) < 0){
            perror("Error de transmisión");
            //return??
        }
        else{
            printf("Petición enviada correctamente\n");
            //Cliente espera la respuesta del servidor
            //llamada a la funcion read()
            //Explora la respuesta(en función de la petición )
            
            memset(buffer, '\0', TAM_BUFFER);

            //Hay que leer del socket para identificar el tipo de petición
            datos_leidos = read(sock, buffer, TAM_BUFFER);
            if(datos_leidos < 0)
                perror("En respuesta del servidor\n");
            else
                printf("[SERVIDOR] -> %s\n", buffer);
            
        }
    }
    
    /* Cierra el socket */
    close(sock);

    return 0;
}