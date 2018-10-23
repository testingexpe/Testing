#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "ntree.h"

static NODO *inicializar_nodo (char *clave, 
                               char *dato,
                               NODO *padre)
{
  NODO *nodo;

  assert(clave != NULL);
  assert(dato != NULL);
  nodo = (NODO *)malloc(sizeof(NODO));
  assert(nodo != NULL);
  nodo->clave = clave;
  nodo->dato = dato;
  nodo->n_hijos = 0;
  nodo->padre = padre;
  nodo->max_hijos = CAPACIDAD_INICIAL;
  nodo->hijos = (NODO **)malloc(sizeof(NODO *) * CAPACIDAD_INICIAL);
  assert(nodo->hijos != NULL);

  return nodo;
}

ARBOL *crear_arbol(char *clave,
	           char *dato)
{
  ARBOL *arbol;

  assert(clave != NULL);
  assert(dato != NULL);
  arbol = (ARBOL *)malloc(sizeof(ARBOL));
  assert(arbol != NULL);
  arbol->raiz = inicializar_nodo(clave, dato, NULL);
  assert(arbol->raiz != NULL);

  return arbol;
}


static NODO *encontrar_nodo(NODO *nodo,
                            char *clave)
{
  int i;
  NODO *encontrado, *resultado;

  assert(nodo != NULL);
  assert(clave != NULL);
  encontrado = NULL;
  resultado = NULL;
  if (strcmp(clave, nodo->clave) == 0) {
    resultado = nodo;
  }
  else {
    for (i = 0; i < nodo->n_hijos; ++i) {
      encontrado = encontrar_nodo(nodo->hijos[i], clave);
     if (encontrado)
        resultado = encontrado; 
    }
  }
  return resultado;
}


int aniadir_hijo_arbol(ARBOL *arbol,
                       char *clave_padre, 
                       char *clave_hijo,
                       char *dato_hijo)
{
  NODO *padre, *hijo;
  NODO **espacio;
  int resultado;

  assert(arbol != NULL);
  assert(clave_padre != NULL);
  assert(clave_hijo != NULL);
  padre = encontrar_nodo(arbol->raiz, clave_padre);
  if (padre == NULL) {
    printf("No se encuentra nodo padre con clave %s\n", clave_padre);
    resultado = -1;
  }
  else {
    if (padre->n_hijos == padre->max_hijos) {
      bcopy (padre->hijos, espacio, sizeof(NODO *) * padre->max_hijos);
      free(padre->hijos);
      padre->hijos = espacio;
      (padre->max_hijos) *= 2;
    }
    hijo = inicializar_nodo(clave_hijo, dato_hijo, padre);
    assert(hijo != NULL);
    padre->hijos[padre->n_hijos] = hijo;
    ++(padre->n_hijos);
    resultado = 0;
  }

  return resultado;                                    
}


int buscar_arbol(ARBOL *arbol,
                 char *clave)
{
  int resultado;
  NODO *nodo;

  resultado = -1;
  assert(arbol != NULL);
  assert(clave != NULL);
  nodo = encontrar_nodo(arbol->raiz, clave);
  if (nodo != NULL) {
    printf("El contenido es: %s\n", nodo->dato);
    resultado = 0;
  } else printf("No se encuentra ningun nodo con clave %s\n", clave);

  return resultado;
}


int son_hermanos_arbol(ARBOL *arbol,
                       char *clave1, 
                       char *clave2)
{
  NODO *nodo1, *nodo2;
  int resultado;

  assert(arbol != NULL);
  assert(clave1 != NULL);
  assert(clave2 != NULL);
  resultado = 0; 
  nodo1 = encontrar_nodo(arbol->raiz, clave1);
  nodo2 = encontrar_nodo(arbol->raiz, clave2);
  if ((nodo1 == NULL) && (nodo2 == NULL))
    printf("Los nodos %s y %s no son hermanos\n", clave1, clave2);
    else if (nodo1 == NULL) {
      printf("No se encuentra ningun nodo con clave %s\n", clave1);
      resultado = -1;
    } else {
    printf("Los nodos %s y %s %sson hermanos\n", clave1, clave2,
             (((nodo1->padre == nodo2->padre) && (nodo1!=nodo2)) ? "" : "no "));
  }
  return resultado;
}


static void imprimir_nodos_arbol(NODO *nodo,
                                 int nivel)
{
  int i;

  assert(nodo != (NODO *)0);
  for (i=0; i<nivel; i++)
      printf("    ");
  printf("Nodo (Nivel %d): Clave '%s', Contenido '%s'\n",
	 nivel, nodo->clave, nodo->dato);
  for (i = 0; i < nodo->n_hijos; ++i) {
    imprimir_nodos_arbol(nodo->hijos[i], nivel+1);
  }
}


int imprimir_arbol(ARBOL *arbol)
{
  int resultado = -1;

  assert (arbol != NULL);
  assert (arbol->raiz != NULL);
  if (arbol->raiz->n_hijos != 0) { 
    imprimir_nodos_arbol(arbol->raiz, 0);
    resultado = 0;
  }

  return resultado;
}


/*
 * Esta es la funcion principal
 * No hay que crear casos de prueba o construir abstracciones
 * para nada de lo que hay bajo esta linea
 */
void procesar_comandos(filep)
     FILE *filep;
{
  char *ptr;
  char buf[BUFSIZ];
  char comando[BUFSIZ], arg1[BUFSIZ], arg2[BUFSIZ], arg3[BUFSIZ];
  ARBOL *miarbol = NULL;

  while (fgets(buf, sizeof(buf), filep) != NULL) {
    if ( (ptr = strchr(buf, '\n')) != NULL)  
      *ptr = '\0';
    printf("\nProcesando linea `%s':\n", buf);
    *comando = '\0';  
    *arg1 = '\0';  
    *arg2 = '\0';  
    *arg3 = '\0';
    sscanf(buf, "%s %s %s %s", comando, arg1, arg2, arg3);
    if (strcmp(comando, "raiz") == 0)
        miarbol = crear_arbol(strdup(arg1), strdup(arg2));
    else if (strcmp(comando, "hijo") == 0)
         aniadir_hijo_arbol(miarbol, arg1, strdup(arg2), strdup(arg3));
         else if (strcmp(comando, "buscar") == 0)
              buscar_arbol(miarbol, arg1);
              else if (strcmp(comando, "hermanos") == 0) 
                   son_hermanos_arbol(miarbol, arg1, arg2);
                   else if (strcmp(comando, "imprimir") == 0)
                        imprimir_arbol(miarbol);
			else
                            printf("Commando `%s' no reconocido\n", comando);
  }
}

int main(argc, argv)
     int argc; char **argv;
{
  FILE *filep;
  char *file;

  if (argc != 2)
    fprintf(stderr, "Uso: %s Dato\n", *argv);
  else {
    file = argv[1];
    if ((filep = fopen(file, "r")) == NULL)
      perror(file);
    else {
      printf("Procesando fichero de entrada `%s'.\n", file);
      procesar_comandos(filep);
      printf("Fin de fichero de entrada `%s'.\n", file); 
      fclose(filep);
    }
  }
  return 0;
}

