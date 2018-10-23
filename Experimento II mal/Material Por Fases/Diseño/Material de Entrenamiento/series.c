#include<stdio.h>
#include<ctype.h>
#include<fcntl.h>

#define startstr  argv[1]    
#define endingstr argv[2]   
#define stepstr   argv[3]  

#define GFORMAT "%g\n"    
#define IFORMAT "%.0f\n" 

char    *Format = GFORMAT;    
int     Onlyint;             
double  Start;              
double  Ending;            

#define RANGE (Ending-Start) 
#define FZERO   10e-10      
#define fzero(x) (fabs (x) < FZERO)

double  Step = 1.0;        
extern double fabs();
extern double atof();

int isinteger (char *cadena)
{
 int res;
 char cadena2[30];
 
 res = atoi (cadena);
 sprintf (cadena2, "%d", res);
 return (!strcmp (cadena, cadena2));                                           }

int number (char *cadena)
{
 int numero;
 float res;
 char cadena2[30];

 numero = sscanf (cadena, "%f%s", &res, cadena2);
 return (numero == 1);
}



int main (int argc, 
	  char **argv)
{
  long   nitems;                 
  long   item;           
  double value;
  int    nargs = argc - 1;

  switch (nargs)
    {
    case 3:
      if (! number(stepstr))
	{
	  printf("El argumento #3 no es un numero: %s\n", stepstr);
	  exit(1);
	}

    case 2:
      if (! number(startstr))
	{
	  printf("El argumento #1 no es un numero: %s\n", endingstr); 
	  exit(1);
	}
      if (! number(startstr))   
	{
	  printf("El argumento #2 no es un numero: %s\n", endingstr);
	  exit(1);
	}
      break;
    default:
      printf("USO comienzo fin cadencia\n");
      exit(1);
    }

  Start   = atof(startstr);
  Ending  = atof(endingstr);
  Onlyint = isinteger(startstr) && isinteger(endingstr);
  if (nargs == 3)
    {
      Step     = fabs(atof(stepstr));
      if (! fzero(RANGE) && fzero(Step))
	{
	  printf("cadencia no puede ser cero\n");
	  exit(0);
	}
      Onlyint &= isinteger(stepstr);
    }

  if (Onlyint)
    Format = IFORMAT;

  if (fzero(RANGE))
    nitems = 2; 
  else
    nitems = RANGE / Step + 1.0 + FZERO;

  for (item = 0; item < nitems; item++)
    {
      value = Start + Step * item;
      if (fzero(value))
	printf(Format, 0.0);
      else
	printf(Format, value);
    }

  exit(0);
}
