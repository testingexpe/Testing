#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <search.h>
#include "nametbl.h"


TABLA *crear_tabla(void)
{
  TABLA *ptr;

  ptr = (TABLA *)malloc(sizeof(TABLA));
  assert(ptr != NULL);
  ptr->raiz = NULL;
  ptr->n_elementos = 0;

  return ptr;
}


int cuantos(TABLA *tabla)
{
  return tabla->n_elementos;
}

int comparar_elementos(ELEMENTO *p1,
                       ELEMENTO *p2)
{
  return(strcmp(p1->nombre, p2->nombre));
}


void imprimir_elemento(ELEMENTO *nodo)
{
  char nombre[7]="Nombre";
  char tobjeto[9]="T_Objeto";
  char trecurso[10]="T_Recurso";

  printf("%s    : %s\n", nombre, nodo->nombre);
  printf("%s  : %s\n", tobjeto, Nombre_tipo_objeto[nodo->objeto]);
  printf("%s : %s\n", trecurso, Nombre_tipo_recurso[nodo->recurso]);
  printf("-----\n");
}


void imprimir_en_ultima_visita(ELEMENTO **nodop,
                               VISIT orden,
                               int nivel)
{
  if (orden == postorder || orden == leaf)
    (void) imprimir_elemento(*nodop);
}


void insertar_elemento(TABLA *tabla,
                       char *nombre,
                       enum tipo_objeto objeto,
                       enum tipo_recurso recurso)
{
  ELEMENTO *elemento, **resultado;

  elemento = (ELEMENTO *) malloc(sizeof(ELEMENTO));
  assert(elemento != NULL);

  elemento->nombre = strdup(nombre);
  assert(elemento->nombre != NULL);
  elemento->objeto = objeto;
  elemento->recurso = recurso;
  (tabla->n_elementos)++;

  resultado = (ELEMENTO **) tsearch((char *)elemento, &(tabla->raiz), comparar_elementos); 
  assert((char *) *resultado == (char *) elemento);
}



ELEMENTO *recuperar_elemento(TABLA *tabla,
                             char *string)
{
  ELEMENTO **elemento, *resultado, clave;

  clave.nombre = string;
  elemento = (ELEMENTO **) tfind( (char *)&clave, &(tabla->raiz), comparar_elementos);
  if (elemento != NULL)
    resultado = *elemento;
  else 
    resultado = NULL;

  return resultado;
}



int mi_asignar_objeto (TABLA *tabla,
                       char *nombre,
                       enum tipo_objeto objeto)
{
  int resultado;
  ELEMENTO *elemento;

  resultado = -1;
  elemento = recuperar_elemento(tabla, nombre);
  if (elemento != NULL) {
    resultado = 0;
    elemento->objeto = objeto;
  }

  return resultado;
}


int mi_asignar_recurso (TABLA *tabla,
                        char *nombre,
                        enum tipo_recurso recurso)
{
  int resultado;
  ELEMENTO *elemento;

  resultado = -1;
  elemento = recuperar_elemento(tabla, nombre);
  if (elemento != NULL) {
    resultado = 0;
    elemento->recurso = recurso;
  }

  return resultado;
}

int insertar_simbolo(TABLA *tabla,
                     char *nombre)
{
  int resultado;
  ELEMENTO *elemento;
 
  elemento = recuperar_elemento(tabla, nombre);
  if (elemento != NULL) {
    printf("El nombre `%s' ya pertenece a la tabla\n", nombre);
    resultado = -1;
  }
  else {
    insertar_elemento(tabla, nombre, NO_INFO, R_NO_INFO);
    resultado = 0;
  }

  return resultado;
}

int asignar_objeto(TABLA *tabla,
                   char *nombre,
                   char *str_objeto)
{
  int resultado;
  enum tipo_objeto objeto;

  objeto = -1;
  if (strcmp("SISTEMA", str_objeto) == 0)
     objeto = SISTEMA;
  else if (strcmp("RECURSO", str_objeto) == 0)
       objeto = RECURSO;

  if (objeto == -1) {
    printf("Tipo de objeto `%s' no reconocido\n", objeto);
    resultado = -1;
  }
  else {
    resultado = mi_asignar_objeto(tabla, nombre, objeto);
    if (resultado)
      printf("El nombre `%s' no pertenece a la Tabla.\n", nombre);
  }

  return resultado;
}


int asignar_recurso(TABLA *tabla,
                    char *nombre,
                    char *str_recurso)
{
  int resultado;
  enum tipo_recurso recurso;

  recurso = -1;
  if (strcmp("R_SISTEMA", str_recurso) == 0)
     recurso = R_SISTEMA;
  else if (strcmp("FUNCION", str_recurso) == 0)
       recurso = FUNCION;
       else if (strcmp("DATO", str_recurso) == 0)
            recurso = DATO;

  if (recurso == -1) {
    printf("Tipo de recurso `%s' no reconocido\n", str_recurso);
    resultado = -1;
  }
  else {
    resultado = mi_asignar_recurso(tabla, nombre, recurso);
    if (resultado)
      printf("El nombre `%s' no pertenece a la Tabla.\n", nombre);
  }

  return resultado;
}


void buscar_simbolo(TABLA *tabla,
                    char *nombre)
{
  ELEMENTO *elemento;

  printf("Buscando el Nombre `%s':\n", nombre);
  elemento = recuperar_elemento(tabla, nombre);
  if (elemento != NULL)
    imprimir_elemento(elemento);
  else printf("El nombre `%s' no pertenece a la Tabla.\n", nombre);
}

void imprimir_tabla(TABLA *tabla)
{
  printf("La Tabla tiene las siguientes %d entradas:\n", cuantos(tabla));
  twalk((char *)tabla->raiz, imprimir_en_ultima_visita);
}






/*******************************************************************************
 * De aqu¡ en adelante est  el programa para leer del fichero de entrada
 * No crear casos de prueba ni construir abstracciones
 * para esta parte del c¢digo.
 *******************************************************************************/


void procesar_comandos(filep)
     FILE *filep;
{
  TABLA *nt;
  char *ptr;
  char buf[BUFSIZ], command[BUFSIZ], name[BUFSIZ], typ[BUFSIZ];

  nt = crear_tabla();
  assert (nt != NULL);

  while (fgets(buf, sizeof(buf), filep) != NULL) {
    if ( (ptr = strchr(buf, '\n')) != NULL)
      *ptr = '\0';
    printf("\nProcesando la linea `%s':\n", buf);
    *command = '\0';
    *name = '\0';
    *typ = '\0';
    sscanf(buf, "%s %s %s", command, name, typ);
    if (strcmp(command, "insertar_simbolo") == 0)
      insertar_simbolo(nt, name);
    else if (strcmp(command, "asignar_objeto") == 0) 
         asignar_objeto(nt, name, typ);
         else if (strcmp(command, "asignar_recurso") == 0)
              asignar_recurso(nt, name, typ);
              else if (strcmp(command, "buscar_simbolo") == 0)
                   buscar_simbolo(nt, name);
                   else if (strcmp(command, "imprimir_tabla") == 0)
                        imprimir_tabla(nt);
                        else printf("Comando `%s' no reconocido\n", command);
  }
}

int main(argc, argv)
     int argc; char **argv;
{
  TABLA *nt;
  FILE *filep;
  char *file;

  if (argc != 2)
    fprintf(stderr, "Uso: %s Dato\n", *argv);
  else {
    file = argv[1];
    if ((filep = fopen(file, "r")) == NULL)
      perror(file);
    else {
      printf("Procesando el fichero de entrada `%s'.\n", file);
      procesar_comandos(filep, nt);
      printf("Fin de fichero de entrada `%s'.\n", file); 
      fclose(filep);
    }
  }
  return 0;
}
