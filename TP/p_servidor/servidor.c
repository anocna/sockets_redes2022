/***********************************************************/
/*********************PROGRAMA SERVIDOR*********************/
/***********************************************************/

// Estructura utilizada por el socket
/*
struct sockaddr_in{
	short int sin_family; 			// Familia de direcciones. Usaremos siempre "AF_INET"
	unsigned short int sin_port; 	// Numero de puerto
	struct in_addr sin_addr;		// Estructura que indica la direccion IP
	unsigned char sin_zero[8];		// Array de 8 bytes rellenados a cero.
}
*/


// Protocolos disponibles para el transporte de datos
/*
TCP (SOCK_STREAM) --> 	Socket de flujo que define una comunicacion bidireccional, confiables y orientada a la conexion.
						Todo lo que se mande desde el origen llegará al destino.
						Necesitan mantener una conexion abierta.

UDP (SOCK_DGRAM) --> 	Socket de datagramas que no esta orientado a la conexion y no es confiable.
						No precisan mantener una conexion abierta.
*/

#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){
	WSADATA wsa;
	SOCKET skt, skt2;
	int longitud_cliente, puerto = 55228, recv_size;
	struct sockaddr_in server, cliente;
	char mensaje[2000];

    int aux=1;// char para eleccion del menu


    /********************** Archivo LOG **********************/
    FILE * archivoEscribir;
    archivoEscribir = fopen("server.log.txt", "a" );



        /**********************INICIALIZACION WINSOCK**********************/
        printf("HORA: ==============================\n");
        fprintf(archivoEscribir,"HORA: ==============================\n");
        if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
            printf("Error al inicializar winsock\n");
            fprintf(archivoEscribir,"Error al inicializar winsock\n");
            exit(-1);
        }
        printf("HORA: ======= Inicia Servidor ======\n");
        fprintf(archivoEscribir,"HORA: ======= Inicia Servidor ======\n"); //en log


        /**********************Creacion Socket **********************/
        printf("HORA: ==============================\n");
        fprintf(archivoEscribir,"HORA: ==============================\n"); //en log
        if((skt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
            printf("Error creando socket\n");
            fprintf(archivoEscribir,"Error creando socket\n");
            exit(-1);
        }
        printf("Socket creado. Puerto de escucha: %d \n", puerto);
        fprintf(archivoEscribir, "Socket creado. Puerto de escucha: %d \n", puerto); // en log

        /********************** SOCKADDR_IN Conf **********************/
        server.sin_family = AF_INET; // Familia TCP/IP
        server.sin_port = htons(puerto); // Puerto
        server.sin_addr.s_addr = INADDR_ANY; // Cualquier cliente puede conectarse


        /********************** Bind **********************/
        if(bind(skt, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR){
            printf("Error al realizar el bind\n");
            exit(-1);
        }
        printf("Bind realizado\n");


    while(aux==1){
        listen(skt, 5);
        printf("Esperando conexiones entrantes...\n");

        longitud_cliente = sizeof(struct sockaddr_in);
        skt2 = accept(skt, (struct sockaddr*)&cliente, &longitud_cliente);
        if(skt2 == INVALID_SOCKET){
            printf("Fallo al aceptar la conexion\n");
            fprintf(archivoEscribir, "Fallo al aceptar la conexion\n");

            exit(-1);
        }

        system("cls");
        printf("Cliente %s conectado exitosamente\n", inet_ntoa(cliente.sin_addr));
        printf("HORA: Conexión Aceptada.\n");
        fprintf(archivoEscribir, "HORA: Conexión Aceptada.\n");  //en log
        aux=0;


        while(aux==0){
        printf("Esperando mensaje entrante...\n");
        if((recv_size = recv(skt2, mensaje, 2000, 0)) == SOCKET_ERROR)
            printf("Recepcion fallida\n");


        // una vez q tengo el mensaje hay q trabajar en el...


        mensaje[recv_size] = '\0';
        printf("%s\n\n", mensaje);


        printf("Haciendo cuenta...\n");

        strcpy(mensaje,"Resultado\n");

        if(send(skt2, mensaje, strlen(mensaje), 0) < 0){
            printf("Error al enviar mensaje\n");
            exit(-1);
        }

        printf("Respuesta enviada exitosamente\n");



        /******* PREGUNTA ***********/

        printf("Esperando mensaje entrante...\n");
        if((recv_size = recv(skt2, mensaje, 2000, 0)) == SOCKET_ERROR)
            printf("Recepcion fallida\n");

        mensaje[recv_size] = '\0';
        if(strcmp(mensaje,"no")==0){
            aux=1;
            printf("El Cliente se ha desconectado.\n");
            fprintf(archivoEscribir, "El Cliente se ha desconectado.\n"); // en log
        }

        printf("Conexión Cerrada por Inactividad.\n");
        fprintf(archivoEscribir, "Conexión Cerrada por Inactividad.\n");// en log

        };

    };

	fclose(archivoEscribir);
	closesocket(skt);
    WSACleanup();

	return 0;
}
