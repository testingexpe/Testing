#ifndef NTREEDOTH
#define NTREEDOTH

struct tree_node {
  char *key;
  char *data;
  struct tree_node *parent;
  int n_children;
  int max_children;
  struct tree_node **children;
};

typedef struct tree_node TN;

struct tree_root {
  struct tree_node *root;
};

typedef struct tree_root TREE;

#define INITIAL_CAPACITY 4

#endif
