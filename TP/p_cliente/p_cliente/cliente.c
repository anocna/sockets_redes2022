/***********************************************************/
/*********************PROGRAMA CLIENTE*********************/
/***********************************************************/


#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <ctype.h>

void op_uno();
void op_dos();

SOCKET skt;
WSADATA wsa;
int puerto=0, recv_size, aux=0, inicio=1;
char mensaje[]="",ip[20]="",caux;

	
int main(int argc, char **argv){
	
	
	struct sockaddr_in server;
	char str_ip;
	
	do{
	
	printf("\n*********************************************************\n"
			 "********************  BIENVENIDOS ***********************\n"
			 "*********************************************************\n\n");
	/**********************INICIALIZACION WINSOCK**********************/	
	printf("Inicializando Winsock...");
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
		printf("Error al inicializar winsock\n");
		exit(-1);
	}
	printf("Winsock inicializado\n");

	/**********************CREACION SOCKET**********************/
	printf("Creando socket...");
	if((skt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
		printf("Error creando socket\n");
		exit(-1);
	}
	printf("Socket creado\n");

	printf("Ingrese Direccion de IP: ");
	fflush(stdin);
	gets(mensaje);
	
	printf("Ingrese Puerto: ");
	scanf("%d",&puerto);
		
	/********************** CONFIGURACION SOCKADDR_IN **********************/
	server.sin_family = AF_INET; // Familia TCP/IP
	server.sin_port = htons(puerto); // Puerto de la aplicacion del servidor
	server.sin_addr.s_addr = inet_addr(mensaje); // Direccion IP del servidor "127.0.0.1"
	

	/**********************CONEXION AL SERVIDOR**********************/
	if(connect(skt, (struct sockaddr *)&server, sizeof(server)) < 0){
		printf("\n!!! Error de conexion\n");
		exit(-1);
	}
	printf("Conexion exitosa\n");

	do{
		system("cls");
		
	    printf(   "-------------------------------------"
				"\n-*********  BIENVENIDOS ************-"
				"\n-------------------------------------"
				"\n   1. REALIZAR CALCULO."
                "\n   2. VER REGISTRO DE ACTIVIDADES."
                "\n   3. CERRAR SESION."
                "\n-------------------------------------\n");

        /* Filtramos la opción elegida por el usuario */
        do
        {
            printf( "\n   Introduzca opcion (1-3): ");
            scanf( "%d", &aux );
            if(aux < 1 || aux > 3)
                printf( "\n   Opcion Invalida \n");

        } while ( aux < 1 || aux > 3 );
        /* La opción sólo puede ser 1, 2, 3 */

        switch ( aux )
        {
            case 1: op_uno();
                    break;

            case 2: op_dos();
                    break;
            
            default: break;
         }
    } while ( aux != 3 );	
			
	strcpy(mensaje,"cerrar");
	
	if(send(skt, mensaje, strlen(mensaje), 0) < 0){
		printf("	Error al enviar mensaje\n");
		exit(-1);
	}
	
	printf("Mensaje de cierre enviado exitosamente\n");

	closesocket(skt);
	WSACleanup();
	
	system("cls");
	
	}while(inicio);

	return 0;
}



void op_uno(){
    
    
    
    if(send(skt, "calculo", sizeof("calculo"), 0) < 0){
		printf("	Error al enviar mensaje\n");
		exit(-1);
	}
	
	
    
    /**************** Validacion de cantidad de caracteres *****/
    int largo=0,i=0;
    
    do{
    	system("cls");
    	
        printf( "\n-->   Ingrese la operacion a realizar.\n\n"
				  "-->   Ingrese ''volver'' para ir al menu principal\n\n"
				  "-->	  ");
        fflush(stdin);
        gets(mensaje);
        
        printf("\n------------  %s\n", mensaje);
        
        largo=strlen(mensaje);
        
        for(i;i<=largo;i++)
            mensaje[i] = tolower(mensaje[i]);
        
        if(strcmp(mensaje,"volver")==0)
            return;
        
        
        if(largo<2){
        	system("cls");
            printf( "\n   !!! ERROR, ingrese mas de dos caracteres.\n\n");
            system("pause");
    	}
        if(largo>20){
        	system("cls");
            printf( "\n   !!! ERROR, ha ingresado demasiados caracteres.\n\n");
            system("pause");
    	}
            
    }while(largo<2 || largo>20);
    
    
    /**********************ENVIAR EL MENSAJE*********************/
	if(send(skt, mensaje, largo, 0) < 0){
		printf("	Error al enviar mensaje\n");
		exit(-1);
	}
	
    printf( "\n	La operacion %s fue enviada al servidor.\n", mensaje);
    
    
    /**********************RECEPCION DE MENSAJE*********************/
	printf("	Esperando respuesta entrante...\n");
	if((recv_size = recv(skt, mensaje, 200, 0)) == SOCKET_ERROR)
		printf("	Recepcion fallida\n");

	mensaje[recv_size] = '\0';	

	/*if(!strcmp(mensaje,"inactivo")){
			
		printf("Cliente desconectado por inactividad\n");
		aux=3;
		
	}else{*/
		printf("	Resultado: %s\n\n", mensaje);
	//}
	
	system("pause");
	
}


void op_dos(){
    
    
    system("cls");
    	    
    /**********************ENVIAR UN MENSAJE*********************/
    if(send(skt, "logs", strlen(mensaje), 0) < 0){
		printf("	Error al enviar mensaje\n");
		exit(-1);
	}
	
	printf("	Mensaje de log enviado exitosamente\n");
   	
    int auxfin = 0;
		
		
    	while(!auxfin){
    		
    		/**********************RECEPCION DE MENSAJE*********************/
	    	//printf("Esperando respuesta entrante...\n");
	    	
	    	if((recv_size = recv(skt, mensaje, 200, 0)) == SOCKET_ERROR)
	    		printf("Recepcion fallida\n");
	    		
	    	/*if(!strcmp(mensaje,"inactivo")){
					
						printf("\nentro a estar inactivoooooo \n--> %s\n\n", mensaje); // verrrrrrrrrrrrrr
						aux=3;
					
			}else{*/
			
    			if(!strcmp(mensaje,"FEOF"))
					auxfin=1;
				
				printf("%s", mensaje);
			//}
		}
		printf(".\n");
		system("pause");
    		
	}

