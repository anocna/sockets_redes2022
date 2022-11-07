/***********************************************************/
/*********************PROGRAMA SERVIDOR*********************/
/***********************************************************/

#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>


int calculo(int a,int b, char c);
int valCaracter(const char* str);
int buscarSigno(const char * str);
int buscarOp1(char str[20]);
int buscarOp2(char str[20]);
int valOperacion(const char* str);

char * tiempo();
time_t current_time = 0;
char c, posError[3]="";



int main(int argc, char **argv)
{
    WSADATA wsa;
    SOCKET skt, skt2;
    int longitud_cliente, puerto = 5000, recv_size;
    struct sockaddr_in server, cliente;
    char mensaje[]="", t[20], leer[200];;
    int aux=1, op1, op2, inactivo=0;


    /********************** Archivo LOG **********************/
    FILE * archivoEscribir;
    FILE * archivoLeer;
    archivoEscribir = fopen("server.log", "a" );


    /**********************INICIALIZACION WINSOCK**********************/
    printf("%s: ==============================\n",tiempo(t));
    fprintf(archivoEscribir,"%s: ==============================\n",tiempo(t));
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Error al inicializar winsock\n");
        fprintf(archivoEscribir,"Error al inicializar winsock\n");
        exit(-1);
    }
    printf("%s: ======= Inicia Servidor ======\n",tiempo(t));
    fprintf(archivoEscribir,"%s: ======= Inicia Servidor ======\n",tiempo(t)); //en log




    /**********************Creacion Socket **********************/
    printf("%s: ==============================\n",tiempo(t));
    fprintf(archivoEscribir,"%s: ==============================\n",tiempo(t)); //en log
    if((skt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
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


    /********************** Inactividad **********************
    DWORD timeout = 10 * 1000;
    setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
*/

    /********************** Bind **********************/
    if(bind(skt, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Error al realizar el bind\n");
        exit(-1);
    }
    printf("Bind realizado\n");


    while(aux==1){

        listen(skt, 5);
        printf("Esperando conexiones entrantes...\n");

        longitud_cliente = sizeof(struct sockaddr_in);
        skt2 = accept(skt, (struct sockaddr*)&cliente, &longitud_cliente);
        if(skt2 == INVALID_SOCKET)
        {
            printf("Fallo al aceptar la conexion\n");
            fprintf(archivoEscribir, "Fallo al aceptar la conexion\n");
            exit(-1);
        }


        system("cls");
        printf("Cliente %s conectado exitosamente\n", inet_ntoa(cliente.sin_addr));
        printf("%s: Conexion Aceptada.\n",tiempo(t));
        fprintf(archivoEscribir, "%s: Conexion Aceptada.\n",tiempo(t));  //en log
        fprintf(archivoEscribir,"Cliente %s conectado exitosamente\n", inet_ntoa(cliente.sin_addr));
        aux=0;
        inactivo=0;


        /********************** Esperando mensaje **********************/

        while(aux==0)
        {


            printf("Esperando mensaje entrante...\n");
            if((recv_size = recv(skt2, mensaje, 20, 0)) == SOCKET_ERROR){
                printf("Recepcion fallida\n");

                 //if(WSAGetLastError() == WSAETIMEDOUT)
                   //             inactivo = 1;
            }



            /********************** OPCION 3 - CERRAR **********************/

            mensaje[recv_size] = '\0';

            if(!strcmp(mensaje,"cerrar"))
            {
                aux=1;
                printf("El Cliente se ha desconectado.\n");
                fprintf(archivoEscribir, "El Cliente se ha desconectado.\n"); // en log

            }


            /********************** OPCION 2 - MOSTRAR LOGS **********************/

            if(!strcmp(mensaje,"logs")){

                printf("\n------- entro al logsss  --------\n");


                printf("\n------- entro al else de leer archivo--------\n");

                archivoLeer = fopen("server.log", "r");

                while(!feof(archivoLeer)){

                    fgets(leer, 100, archivoLeer);
                    if(send(skt2, leer, sizeof(leer), 0) < 0)
                    {
                        printf("Error al enviar mensaje\n");
                        exit(-1);
                    }


                }

                strcpy(leer,"FEOF");
                if(send(skt2, leer, sizeof(leer), 0) < 0)
                {
                    printf("Error al enviar mensaje\n");
                    exit(-1);
                }

                printf("\nLog enviado.\n");

                fclose(archivoLeer);

            }

            /********************** OPCION 1 - CALCULO ***************************/

            /********************** Validacion de operacion **********************/

            mensaje[recv_size] = '\0';

            if(!strcmp(mensaje,"calculo"))
            {

                printf("calcular act. mensaje: %s\n",mensaje);

                printf("Esperando mensaje entrante...\n");

                if((recv_size = recv(skt2, mensaje, 20, 0)) == SOCKET_ERROR)
                    printf("Recepcion fallida\n");

                mensaje[recv_size] = '\0';


                /*if(inactivo){

                    strcpy(mensaje,"inactivo");

                    if(send(skt2, mensaje, strlen(mensaje), 0) < 0)
                    {
                        printf("Error al enviar mensaje\n");
                        exit(-1);
                    }

                    printf("\ninactividad enviada exitosamente\n");

                    aux=1;

                    //break;
                }*/


                //Validación de caracteres de la operación
                if(valCaracter(mensaje))
                {
                    strcpy(mensaje,"No se pudo realizar la operacion, se encontro un caracter no contemplado:  ");
                    mensaje[strlen(mensaje+1)] = c;

                    if(send(skt2, mensaje, strlen(mensaje), 0) < 0)
                    {
                        printf("Error al enviar mensaje\n");
                        exit(-1);
                    }

                    printf("\nRespuesta fallo caracter enviada exitosamente\n");

                }
                else
                {


                    //Validación de operación mal formada
                    if(valOperacion(mensaje))
                    {

                        strcpy(mensaje,"No se pudo realizar la operacion, la operacion esta mal formada: ");

                        strcat(mensaje,posError);

                        if(send(skt2, mensaje, strlen(mensaje), 0) < 0)
                        {
                            printf("Error al enviar mensaje\n");
                            exit(-1);
                        }


                    }
                    else
                    {

                        /********************** Calculo **********************/

                        printf("\n------- entro al realizar la operacion --------\n");

                        op1 = buscarOp1(mensaje);

                        op2 = buscarOp2(mensaje);

                        itoa(calculo(op1,op2,mensaje[buscarSigno(mensaje)]),mensaje,10);

                        if(send(skt2, mensaje, strlen(mensaje), 0) < 0)
                        {
                            printf("Error al enviar mensaje\n");
                            exit(-1);
                        }

                        printf("Respuesta de calculo enviada exitosamente\n");

                    }
                }

            }

        }
        // inactividad aux=0 y se cierra el servidor
        //printf("Conexion Cerrada por Inactividad.\n");
        //fprintf(archivoEscribir, "Conexion Cerrada por Inactividad.\n");// en log
    }



    fclose(archivoEscribir);
    closesocket(skt);
    WSACleanup();

    return 0;
};


int buscarOp1(char str[20])
{

    char aux[20]="";
    int p = buscarSigno(str)-1, i=0, auxi=0;

    for(i; i<=p; i++)
    {
        aux[i]=str[i];
    }

    auxi=atoi(aux);

    return auxi;

}

int buscarOp2(char str[20])
{

    char aux[20]="";
    int p = buscarSigno(str), i, auxi;

    p+=1;

    for(i=p; i<strlen(str); i++)
    {
        aux[i-p]=str[i];
    }

    auxi=atoi(aux);

    return auxi;

}


int calculo(int a,int b, char c)
{
    int resultado=1,i;

    switch (c)
    {
    case '+':
        resultado = a + b;
        break;
    case '-':
        resultado = a - b;
        break;
    case '*':
        resultado = a * b;
        break;
    case '/':
        resultado = a / b;
        break;
    case '^':
        for (i = 0; i < b; ++i)
        {
            resultado *= a;
        }
        break;
    case '!':
        for (i = 1; i <= a; i++)
        {
            resultado *= i;
        }
        break;
    default:
        break;
    }

    return resultado;
}

int buscarSigno(const char * str)
{
    int i=0;

    for(i; i<=strlen(str); i++)
    {
        if(!isalnum(str[i]))
        {
            break;
        }
    }

    return i;
}

// retorna 0 si no hay letras,
int valCaracter(const char* str)
{
    int i, auxi = 0;

    for(i=0; i<=strlen(str); i++)
    {
        if(isalpha(str[i]))
        {
            auxi=1;
            c=str[i];
            printf("Se encontro el caracter '%c' ",c);
        }
    }

    return auxi;
}


int valOperacion(const char* str)
{

    int i, auxi=0, p=buscarSigno(str);

    char sop1[20]="",sop2[20]="";

    if(p==(strlen(str)-1))
    {
        auxi=1;
        posError[0]=str[strlen(str)-2];
        posError[1]=str[strlen(str)-1];
        posError[2]='.';

    }

    if(p==0)
    {
        auxi=1;
        posError[0]=str[0];
        posError[1]=str[1];
        posError[2]='.';
    }


    for(i=0; i<=p-1; i++)
    {
        sop1[i]=str[i];
    }

    p+=1;

    for(i=p; i<strlen(str); i++)
    {
        sop2[i-p]=str[i];
    }


    if(buscarSigno(sop1)!=strlen(sop1))
        auxi=1;

    if(buscarSigno(sop2)!=strlen(sop2))
    {
        auxi=1;
        posError[0]=str[buscarSigno(sop2)+p-1];
        posError[1]=str[buscarSigno(sop2)+p];
        posError[2]=str[buscarSigno(sop2)+p+1];
    }

    return auxi;
}



/******************* TIEMPO ********************/

char * tiempo(char * t)
{

    time_t tiempo = time(NULL);
    struct tm *tlocal = localtime(&tiempo);
    strftime(t,20,"20%y-%m-%d_%H:%M",tlocal);

    return t;
}
