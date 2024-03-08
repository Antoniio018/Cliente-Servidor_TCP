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

int main(void){
    /* Socket de escucha*/
    int socksvr;

    /* Socket de transferecia de datos*/
    int sockdata;

    int datos_leidos;
    bool ok = true;
    struct sockaddr_in name;
    char buffer[TAM_BUFFER];
    char respuesta[TAM_BUFFER];

    /* Abre el socket de escucha */
    socksvr = socket(PF_INET, SOCK_STREAM, 0);
    if(socksvr < 0){
        perror("En socket server...\n");
        printLog("ERROR", "No se puede abrir un socket");
        return ERROR_NO_OPEN_SOCKET;
    }

    /* Lee los datos del fichero .ini */
    Config config = readIni();

    /* Asigna los parámetros de conexión al socket */
    /* 
        Si en el fichero .ini pone local host selecciona una IP Local,
        en caso contrario, selecciona la IP propuesta en el .ini
    */
    if(strcmp(config.host, "localhost") == 0){
        name.sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
        struct in_addr addr;
        inet_aton(config.host, &addr);
        name.sin_addr = addr;
    }
    name.sin_family = AF_INET;
    name.sin_port = htons(config.port);

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
    while(ok){
        //al log
        printLog("NOTIFY", "Servidor en espera de conexión");
        printf("esperando conexiones...\n");

        // Aceptar la conexión y pasa al cliente a otro socket (datos)
        sockdata = accept(socksvr, (struct sockaddr *)NULL, (socklen_t *)NULL);

        if(sockdata < 0){
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

                //int getpeername (int s, struct sockaddr *name , int namelen )
                //int desc = getpeername(sockdata, (struct sockaddr *)&name, &name_len);
                //printf("Desc: %d\n", desc);

                socklen_t name_len = sizeof(name);
                if(getpeername(sockdata, (struct sockaddr *)&name, &name_len) < 0){

                    perror("Error al obtener direccion del cliente\n");
                    printLog("ERROR", "Error al obtener direccion del cliente");
                    return ERROR_NOT_FOUND_CLIENT_DATA;
                }
                else{
                    //Imprime la IP en el log
                    char log[100];
                    strcpy(log, "Conexión establecida con el servidor IP Cliente: ");
                    strcat(log, inet_ntoa(name.sin_addr));
                    printLog("NOTIFY", log);
                }
                //Muestra datos enviados
                printf("[DATOS] -> %s\n", buffer);

                //Comprueba si el comando recibido es para apagar el servidor   
                if (strcmp(buffer, "[quit]") == 0) {
                    ok = false;
                    printLog("NOTIFY", "Servidor apagado");
                    snprintf(respuesta, sizeof(respuesta), "Servidor apagado");
                }else{
                    //Comprueba el formato de los datos recibidos y devuelve una respuesta
                    traducirComando(buffer, respuesta, sizeof(respuesta));
                    printf("[RESPUESTA] -> %s\n", respuesta);
                }                
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