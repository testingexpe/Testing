#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <search.h>
#include "nametbl.h"


NT *new_table(void)
{
  NT *ptr;

  ptr = (NT *)malloc(sizeof(NT));
  assert(ptr != NULL);
  ptr->rootp = NULL;
  ptr->numitems = 0;

  return ptr;
}


int how_many(NT *nt)
{
  return nt->numitems;
}

int compare_entry(NTE *p1,
                  NTE *p2)
{
  int len1, len2;
  
  len1 = strlen(p1->name);
  len2 = strlen(p2->name);

  if (len1 < len2)
     return(strncmp(p1->name, p2->name, len1));
  else return(strncmp(p1->name, p2->name, len2));
}


void print_entry(NTE *node)
{
  char name[5]="Name";
  char otype[7]="O_type";
  char rtype[7]="R_type";

  printf("%s    : %s\n", name, node->name);
  printf("%s  : %s\n", otype, object_type_name[node->ot]);
  printf("%s : %s\n", rtype, resource_type_name[node->rt]);
  printf("-----\n");
}


void print_on_last_visit(NTE **nodep,
                         VISIT order,
                         int level)
{
  if (order == postorder || order == leaf)
    (void) print_entry(*nodep);
}


void insert_entry(NT *nt,
                  char *new_name,
                  enum object_type new_ot,
                  enum resource_type new_rt)
{
  NTE *nte, **result;

  nte = (NTE *) malloc(sizeof(NTE));
  assert(nte != NULL);

  nte->name = strdup(new_name);
  assert(nte->name != NULL);
  nte->ot = new_ot;
  nte->rt = new_rt;
  (nt->numitems)++;

  result = (NTE **) tsearch((char *)nte, &(nt->rootp), compare_entry); 
  assert((char *) *result == (char *) nte);
}



NTE *retrieve_entry(NT *nt,
                    char *search_name)
{
  NTE **nte, *result, key;

  key.name = search_name;
  nte = (NTE **) tfind( (char *)&key, &(nt->rootp), compare_entry);
  if (nte != NULL)
    result = *nte;
  else 
    result = NULL;

  return result;
}



int set_obj_type (NT *nt,
                  char *search_name,
                  enum object_type new_ot)
{
  int result;
  NTE *nte;

  result = -1;
  nte = retrieve_entry(nt, search_name);
  if (nte != NULL) {
    result = 0;
    nte->ot = new_ot;
  }

  return result;
}


int set_res_type (NT *nt,
                  char *search_name,
                  enum resource_type new_rt)
{
  int result;
  NTE *nte;

  result = -1;
  nte = retrieve_entry(nt, search_name);
  if (nte != NULL) {
    result = 0;
    nte->rt = new_rt;
  }

  return result;
}

int ins(NT *nt,
        char *new_name)
{
  int result=0;
 
  insert_entry(nt, new_name, OT_NO_INF, RT_NO_INF);

  return result;
}

int tot(NT *nt,
        char *new_name,
        char *new_ot_char)
{
  int result;
  enum object_type new_ot;

  new_ot = -1;
  if (strcmp("SYSTEM", new_ot_char) == 0)
     new_ot = SYSTEM;
  else if (strcmp("RESOURCE", new_ot_char) == 0)
       new_ot = RESOURCE;

  if (new_ot == -1) {
    printf("Object type `%s' not recognised\n", new_ot_char);
    result = -1;
  }
  else {
    result = set_obj_type(nt, new_name, new_ot);
    if (result)
      printf("The name `%s' is not in the Table.\n", new_name);
  }

  return result;
}


int trt(NT *nt,
        char *new_name,
        char *new_rt_char)
{
  int result;
  NTE *element;
  enum resource_type new_rt;

  new_rt = -1;
  element = retrieve_entry (nt,new_name);

  if (strcmp("RT_SYSTEM", new_rt_char) == 0)
     new_rt = RT_SYSTEM;
  else if (strcmp("FUNCTION", new_rt_char) == 0)
       new_rt = FUNCTION;
       else if (strcmp("DATA", new_rt_char) == 0)
            new_rt = DATA;
            else if (strcmp("MEMORY", new_rt_char) == 0)
                 new_rt = MEMORY;

  if ((new_rt == -1) && (element == NULL)) {
    printf("The operation could not be performed.\n"); 
  } else if (new_rt == -1) {
    printf("Resource type `%s' not recognised\n", new_rt_char);
    result = -1;
  }
  else {
    result = set_res_type(nt, new_name, new_rt);
    if (result)
      printf("The name `%s' is not in the Table.\n", new_name);
  }

  return result;
}


void sch(NT *nt,
         char *name)
{
  NTE *nte;

  printf("Search for name `%s':\n", name);
  nte = retrieve_entry(nt, name);
  if (nte != NULL)
    print_entry(nte);
  else printf("The name `%s' is not in the Table.\n", name);
}

void prt(NT *nt)
{
  printf("the Table has the following %d entries:\n", how_many(nt));
  twalk((char *)nt->rootp, print_on_last_visit);
}






/*******************************************************************************
 * From here on is the test harness
 * Do not create any test cases or build abstractions
 * for this part of the code.
 *******************************************************************************/


void process_commands(FILE *filep)

{
  NT *nt;
  char *ptr;
  char buf[BUFSIZ], command[BUFSIZ], name[BUFSIZ], typ[BUFSIZ];

  nt = new_table();
  assert (nt != NULL);

  while (fgets(buf, sizeof(buf), filep) != NULL) {
    if ( (ptr = strchr(buf, '\n')) != NULL)
      *ptr = '\0';
    printf("\nLine '%s' being processed:\n", buf);
    *command = '\0';
    *name = '\0';
    *typ = '\0';
    sscanf(buf, "%s %s %s", command, name, typ);
    if (strcmp(command, "ins") == 0)
      ins(nt, name);
    else if (strcmp(command, "tot") == 0) 
         tot(nt, name, typ);
         else if (strcmp(command, "trt") == 0)
              trt(nt, name, typ);
              else if (strcmp(command, "sch") == 0)
                   sch(nt, name);
                   else if (strcmp(command, "prt") == 0)
                        prt(nt);
                        else printf("Command `%s' not recognised\n", command);
  }
}

int main(int argc, 
         char ** argv)
{
  FILE *filep;
  char *file;

  if (argc != 2)
    fprintf(stderr, "Usage: %s Data\n", *argv);
  else {
    file = argv[1];
    if ((filep = fopen(file, "r")) == NULL)
      perror(file);
    else {
      printf("Input file `%s' being processed.\n", file);
      process_commands(filep);
      printf("End of input file `%s'.\n", file); 
      fclose(filep);
    }
  }
  return 0;
}
