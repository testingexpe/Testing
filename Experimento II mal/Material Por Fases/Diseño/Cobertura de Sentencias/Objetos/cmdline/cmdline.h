#ifndef CMDLINEH
#define CMDLINEH

#define AYUDA   0
#define MEDIDA  1
#define GKOM	2
#define LKOM	3
#define LKHM	4
#define GKHM	5
#define LIHE	6
#define GIHE	7
#define TODAS	8
#define MAX	9
#define PROMEDIO 10
#define MIN	11
#define INF     12
#define SUP     13

struct entrada_opcion {
  char *opcion; 
  int long_min;
  int identificador;
};

struct comando {
  double numero;
  char *medida;
  char *busqueda;
};

#endif
