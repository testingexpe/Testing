#ifndef NTREEH
#define NTREEH

struct nodo_arbol {
  char *clave;
  char *dato;
  struct nodo_arbol *padre;
  int n_hijos;
  int max_hijos;
  struct nodo_arbol **hijos;
};

typedef struct nodo_arbol NODO;

struct raiz_arbol {
  struct nodo_arbol *raiz;
};

typedef struct raiz_arbol ARBOL;

#define CAPACIDAD_INICIAL 4

#endif
