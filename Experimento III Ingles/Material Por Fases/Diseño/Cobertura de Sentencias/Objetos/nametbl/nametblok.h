#ifndef NAMETBLDOTH
#define NAMETBLDOTH

enum object_type {
     SYSTEM,
     RESOURCE,
     OT_NO_INF};

static char *object_type_name[] = {
     "SYSTEM",
     "RESOURCE",
     "OT_NO_INF"};

enum resource_type {
     RT_SYSTEM,
     DATA,
     FUNCTION,
     RT_NO_INF};

static char *resource_type_name[] = {
     "RT_SYSTEM",
     "DATA",
     "FUNCTION",
     "RT_NO_INF"};


struct name_table_entry {
  char                  *name;
  enum object_type      ot;
  enum resource_type    rt;
};


typedef struct name_table_entry NTE;

  struct name_table {
  char *rootp;
  int  numitems;
};

typedef struct name_table NT;

#endif
