#include <stdio.h>
#include <string.h>
#include <assert.h>



int 	Ignore   = 0;
int 	Mincount = 0;
int 	Alpha = 0;
char	MapAllowed[256];

typedef struct tnode
{
  char   *contents;
  int    count;
  struct tnode *left;
  struct tnode *right;
} TNODE;

void treeprint(TNODE *tree)
{
  if (tree != NULL)
    {
      treeprint(tree->left);
      if (tree->count > Mincount)
	printf("%7d\t%s\n", tree->count, tree->contents);
      treeprint(tree->right);
    }
}

TNODE *install(char *string, 
               TNODE *tree) 
{
  int cond;
  assert(string != NULL);
  if (tree == NULL)
    {
      if (tree = (TNODE *) calloc(1, sizeof(TNODE)))
	{
	  tree->contents = strdup(string);
	  tree->count = 1;
	}
    }
  else
    {
      cond = strcmp(string, tree->contents);
      if (cond < 0)
	tree->left = install(string, tree->left);
      else if (cond == 0)
	tree->count++;
      else
	tree->right = install(string, tree->right);
    }
  return(tree);
}


char *getword(FILE *ioptr) 
{
  static char string[1024];
  char *ptr = string;
  register int c;
  assert(ioptr != NULL);
  for (;;)
    {
      c = getc(ioptr);
      if (c == EOF)
	if (ptr == string)
	  return(NULL);
	else
	  break;
      if (!MapAllowed[c])
	if (ptr == string)
	  continue;
	else
	  break;
      *ptr++ = MapAllowed[c];
    }
  *ptr = '\0';
  return(string);
}

void tokens(FILE *ioptr) 
{
  TNODE *root = NULL;
  char *s;
  assert(ioptr != NULL);
  while (s = getword(ioptr))
    root = install(s, root);
  treeprint(root);
}

int main(int argc, 
	 char **argv) 
{
  int c, errcnt = 0;
  extern char *optarg;
  while ((c = getopt(argc, argv, "ac:im:")) != EOF)
    switch(c)
      {
      case 'a': Alpha = 0; break;
      case 'c':
	while (*optarg)
	  {
	    MapAllowed[*optarg] = *optarg;
	    optarg++;
	  }
	break;
      case 'i': Ignore = 1; break;
      case 'm': Mincount = atoi(optarg); break;
      default: errcnt++;
      }
  if (errcnt)
    {
      fprintf(stderr, "Uso: %s [-i] [-c chars] [-m cuenta]\n", *argv);
      return(1);
    }

  for (c = 'a'; c <= 'z'; c++)
    MapAllowed[c] = c;
  for (c = 'A'; c <= 'Z'; c++)
    MapAllowed[c] = Ignore ? c - 'A' + 'a' : c;
  if (!Alpha)
    for (c = '0'; c <= '9'; c++)
      MapAllowed[c] = c;
  tokens(stdin);
  return(0);
}

