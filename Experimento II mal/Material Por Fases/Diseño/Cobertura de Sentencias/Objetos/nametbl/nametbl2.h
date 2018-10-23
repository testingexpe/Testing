#ifndef NAMETBLH
#define NAMETBLH

enum tipo_objeto {
     RECURSO,
     SISTEMA,
     NO_INFO};

static char *Nombre_tipo_objeto[] = {
     "SISTEMA",
     "RECURSO",
     "NO_INFO"};

enum tipo_recurso {
     R_SISTEMA,
     DATO,
     FUNCION,
     R_NO_INFO};

static char *Nombre_tipo_recurso[] = {
     "R_SISTEMA",
     "DATO",
     "FUNCION",
     "R_NO_INFO"};


struct elemento_tabla {
  char                  *nombre;
  enum tipo_objeto      objeto;
  enum tipo_recurso     recurso;
};


typedef struct elemento_tabla ELEMENTO;

struct tabla {
  char *raiz;
  int  n_elementos;
};

typedef struct tabla TABLA;

#endif
