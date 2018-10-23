#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "ntree.h"

static TN *init_tree_node (char *key, 
                           char *data,
                           TN   *parent)
{
  TN *node;

  assert(key != NULL);
  assert(data != NULL);
  node = (TN *)malloc(sizeof(TN));
  assert(node != NULL);
  node->key = key;
  node->data = data;
  node->n_children = 0;
  node->parent = parent;
  node->max_children = INITIAL_CAPACITY;
  node->children = (TN **)malloc(sizeof(TN *) * INITIAL_CAPACITY);
  assert(node->children != NULL);

  return node;
}

TREE *t_root(char *key,
	     char *data)
{
  TREE *tree;

  assert(key != NULL);
  assert(data != NULL);
  tree = (TREE *)malloc(sizeof(TREE));
  assert(tree != NULL);
  tree->root = init_tree_node(key, data, NULL);
  assert(tree->root != NULL);

  return tree;
}


static TN *find_node(TN *node,
                     char *key)
{
  int i;
  TN *found, *rc;

  assert(node != NULL);
  assert(key != NULL);
  found = NULL;
  rc = NULL;
  if (strcmp(key, node->key) == 0) {
    rc = node;
  }
  else {
    for (i = 0; i < node->n_children; ++i) {
      found = find_node(node->children[i], key);
      if (found) 
	rc = found;
    }
  }
  return rc;
}


int t_add_child(TREE *tree,
                char *parent_key, 
                char *child_key,
                char *child_data)
{
  TN *parent, *child;
  TN **space;
  int rc;

  assert(tree != NULL);
  assert(parent_key != NULL);
  assert(child_key != NULL);
  parent = find_node(tree->root, parent_key);
  if (parent == NULL) {
    printf("Parent node with key %s is not found\n", parent_key);
    rc = -1;
  }
  else {
    if (parent->n_children == parent->max_children) {  
      bcopy (parent->children, space, sizeof(TN *) * parent->max_children);
      free(parent->children);
      parent->children = space;
      (parent->max_children) *= 2;
    }
    child = init_tree_node(child_key, child_data, parent);
    assert(child != NULL);
    parent->children[parent->n_children] = child;
    ++(parent->n_children);
    rc = 0;
  }

  return rc;
}


int t_search(TREE *tree,
             char *key)
{
  int rc;
  TN *node;

  rc = -1;
  assert(tree != NULL);
  assert(key != NULL);
  node = find_node(tree->root, key);
  if (node != NULL) {
    printf("The contents are: %s\n", node->data);
    rc = 0;
  } else printf("Node with key %s is not found\n", key);

  return rc;
}


int t_are_siblings(TREE *tree,
                   char *key1, 
                   char *key2)
{
  TN *node1, *node2;
  int rc;

  assert(tree != NULL);
  assert(key1 != NULL);
  assert(key2 != NULL);
  rc = 0; 
  node1 = find_node(tree->root, key1);
  node2 = find_node(tree->root, key2);
  if ((node1 == NULL) && (node2 == NULL))
    printf("Node %s and %s are not siblings\n", key1, key2);
  else if (node1 == NULL) {
    printf("Node with key %s is not found\n", key1);
    rc = -1;
  } else {
  printf("Node %s and %s are %s siblings\n", key1, key2,
             (((node1->parent == node2->parent) && (node1!=node2)) ? "" : "not"));
  }
  return rc;
}


static void print_tree_nodes(TN *node,
                             int level)
{
  int i;

  assert(node != (TN *)0);
  for (i=0; i<level; i++)
      printf("    ");
  printf("Node (level %d): key '%s', Contents '%s'\n",
	 level, node->key, node->data);
  for (i = 0; i < node->n_children; ++i) {
    print_tree_nodes(node->children[i], level + 1);
  }
}


int t_print(TREE *tree)
{
  int rc = -1;

  assert(tree != NULL);
  assert(tree->root != NULL);
  if (tree->root->n_children != 0) { 
    print_tree_nodes(tree->root, 0);
    rc = 0;
  }

  return rc;
}


/*
 * This is the test harness
 * Do not create tests for anything below this line,
 * or build abstractions, etc...
 */
void process_commands(FILE *filep)

{
  char *ptr;
  char buf[BUFSIZ];
  char command[BUFSIZ], arg1[BUFSIZ], arg2[BUFSIZ], arg3[BUFSIZ];
  TREE *mytree = NULL;

  while (fgets(buf, sizeof(buf), filep) != NULL) {
    if ( (ptr = strchr(buf, '\n')) != NULL)  
      *ptr = '\0';
    printf("\nLine `%s' being processed:\n", buf);
    *command = '\0';  
    *arg1 = '\0';  
    *arg2 = '\0';  
    *arg3 = '\0';
    sscanf(buf, "%s %s %s %s", command, arg1, arg2, arg3);
    if (strcmp(command, "root") == 0)
        mytree = t_root(strdup(arg1), strdup(arg2));
    else if (strcmp(command, "child") == 0)
         t_add_child(mytree, arg1, strdup(arg2), strdup(arg3));
         else if (strcmp(command, "search") == 0)
              t_search(mytree, arg1);
              else if (strcmp(command, "sibs") == 0) 
                   t_are_siblings(mytree, arg1, arg2);
                   else if (strcmp(command, "print") == 0)
                        t_print(mytree);
			else
                            printf("Command `%s' not recognised\n", command);
  }
}

int main(int argc, 
         char **argv)

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

