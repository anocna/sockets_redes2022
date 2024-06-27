# sockets_redes2022

Desarrollo de Aplicacion

Objetivo: 
Desarrollar una calculadora de números enteros positivos en C o C++, con aplicaciones comunicadas por sockets, una como servidor y otra como cliente1.
Requerimientos Funcionales: El servidor debe registrar las conexiones en un archivo server.log, aceptar operaciones matemáticas básicas y realizar validaciones de caracteres y longitud de operaciones.
Cliente: Debe poder conectarse al servidor, realizar cálculos, ver el registro de actividades y cerrar sesión, todo mediante una interfaz de usuario clara.
Entrega y Evaluación: Incluye un documento con la estrategia de resolución, código fuente, video de pruebas, archivo de log y defensa presencial del trabajo.

Escenario de Pruebas:
Se realizan en una sola máquina, con el archivo server.log vacío al inicio.
Proceso de Pruebas: Incluye verificar la conexión al servidor, ejecutar operaciones matemáticas y manejar errores de formato en las operaciones.
Resultados Esperados: Se especifican los mensajes que deben aparecer en pantalla para cada operación realizada o intento de conexión.


RESOLUCIÓN DEL TRABAJO PRÁCTICO:

Redes y Comunicaciones 
Alumno: Ancona Romano Matias Leandro. 
Docentes: Ing Diego Andrés Azcurra - Lic. Gustavo Fernández.
Para la resolución del presente, se utilizó el lenguaje de programación C, se investigó 
en libros y páginas de internet sobre la utilización de los Sockets en dicho lenguaje, ya que 
era un tema nuevo e interesante, luego utilizando el compilador CodeBlocks se creó un 
archivo .c llamado “servidor.c” donde se programara la aplicación servidor y paralelamente 
utilizando el compilador DevC++ se creó un archivo .c de nombre “cliente.c” donde se 
programara la aplicación para el cliente. Dentro de la programación, aparte de las librerías 
estándar, utilice nuevas como: #include <winsock2.h>, #include <ctype.h>. 
Comenzando el algoritmo de ambas aplicaciones se inicializa el WINSOCK, se crean los 
Sockets, en el caso del servidor se espera la conexión del cliente, y el cliente espera a que le 
ingresen manualmente la dirección de IP y el Puerto. 
Una vez conectados, mediante el menú del cliente se puede elegir las opciones sobre la 
tarea que desea realizar y se envían al servidor para ser procesadas. 
Opción 1. REALIZAR CALCULO: retorna el resultado o un mensaje de error, ya que posee una 
serie validaciones para detectar una operación incorrecta o mal formulada, posee la opción 
“Volver” dentro del cliente con la que No se envía información al servidor y se muestra el 
menú principal nuevamente. 
Opción 2. VER REGISTRO DE ACTIVIDADES: el servidor lee su archivo .log y lo envía a través 
de mensajes al cliente el cual lo muestra por consola. 
Opción 3. CERRAR SESION: el cliente se desconecta del servidor y este ultimo queda a la 
espera de una nueva conexión de cliente. 
