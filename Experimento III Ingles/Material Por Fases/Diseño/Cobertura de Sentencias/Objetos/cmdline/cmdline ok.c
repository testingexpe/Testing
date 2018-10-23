#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cmdline.h"

static struct keyword_entry keyword_table[] = {
  {"-?",		2, HELP},
  {"-help",		2, HELP},
  {"-measure",		3, MEASURE},
  {"GKOM",		3, GKOM},
  {"LKOM",		3, LKOM},
  {"LKHM",		3, LKHM},
  {"GKHM",		3, GKHM},
  {"LIHE",		2, LIHE},
  {"GIHE",		2, GIHE},
  {"-all",		3, ALL},
  {"-max",		3, MAX},
  {"-average", 	        3, AVERAGE},
  {"-min",		3, MIN},
  {"-below",		3, BELOW},
  {"-above",		3, ABOVE},
};
static int keyword_table_size = sizeof(keyword_table) /
                                sizeof(struct keyword_entry);


void usage(char *progn)
{
  fprintf(stderr,
    "Usage: %s -measure <MEASURE> [ Search-option ] File [ File ... ]\n", progn);
  exit(1);
}

static int is_str_keyword(char *arg,
                          char *valid_arg,
                          int min_required_len)
{
  int arglen, rc;

  rc = 0;
  arglen = strlen(arg);
  if ((strncmp(arg, valid_arg, arglen) == 0) && (arglen>=min_required_len))
      rc = 1;

  return rc;
}


static int code_string(char *str)
{
  int i, rc;

  rc = -1;
  for (i = 0; i < keyword_table_size; ++i) {
  if (is_str_keyword(str, keyword_table[i].keyword, keyword_table[i].min_len)) {
      rc = keyword_table[i].id;
      break;
    }
  }
  return rc;
}


void print_command(struct command *cmd)
{

  if (cmd->measure == NULL)
    fprintf(stderr, "No measure option given\n");
  else if (strcmp(cmd->measure, "-help") == 0)
    usage("cmdline");
  else {
    printf("Command summary is:\n");
    if (cmd->measure)
      printf("Measure:  %s\n", cmd->measure);
    if (cmd->search) {
      printf("Search: %s\n", cmd->search);
      if (strcmp(cmd->search, "-below") == 0 || strcmp(cmd->search, "-above") == 0)
        printf("Number:  %.1f\n", cmd->number);
    }
  }
}

void print_inputfiles(int argi,
                      int argc,
                      char **argv)
{
   int i;

   if (argi == 0)
      printf("No input files given\n");
   else if (argi > 0)
	{
      printf("Number of input files: %d\n", argc - argi);
      printf("Input files are:\n");
      for (i = argi ; i < argc; ++i)
        printf("   %s\n", argv[i]);
   }
}


int process_switches(int argc,
                      char **argv,
                      struct command *cmd)
{
  int i, rc = 0, position = 0;

  cmd->number = 0.0;
  cmd->measure = NULL;
  cmd->search = NULL; 

  for (i = 1; i < argc; ++i) {
    switch (code_string(argv[i])) {

    case HELP:
      cmd->measure = "-help";
      position = -1;
      break;

    case MEASURE:
      if (cmd->measure != NULL) {
        fprintf(stderr, "Too many measure options given\n");
	rc = -1;
      }
      else {
	if (++i >= argc) {
          fprintf(stderr, "No measure given\n");
	  rc = -1;
	}
	else {
	  switch (code_string(argv[i])) {
	  case GKOM: 
	    cmd->measure = "GKOM";
	    break;
	  case LKOM:
	    cmd->measure = "LKOM";
	    break;
	  case LKHM:
	    cmd->measure = "LKHM";
	    break;
	  case GKHM:
	    cmd->measure = "GKHM";
	    break;
	  case LIHE:
	    cmd->measure = "LIHE";
	    break;
	  case GIHE:
	    cmd->measure = "GIHE";
	    break;
	  default: 
            fprintf(stderr, "Invalid measure option given\n"); 
	    rc = -1;
	    break;
	  }
	} 
      }
      break;

    case ALL:
      if (cmd->search != NULL) {
	fprintf(stderr, "Too many search options given\n");
	rc = -1;
      }
      else
	cmd->search = "-all";
      break;

    case MAX:
      if (cmd->search != NULL) {
	fprintf(stderr, "Too many search options given\n");
	rc = -1;
      }
      else
	cmd->search = "-max";
      break;

    case AVERAGE:
      if (cmd->search != NULL) {
	fprintf(stderr, "Too many search options given\n");
	rc = -1;
      }
      else
	cmd->search = "-average";
      break;

    case MIN:
      if (cmd->search != NULL) {
	fprintf(stderr, "Too many search options given\n");
	rc = -1;
      }
      else
	cmd->search = "-min";
      break;
    case BELOW:
      if (cmd->search != NULL) {
	fprintf(stderr, "Too many search options given\n");
	rc = -1;
      }
      else {
	cmd->search = "-below";
	++i;
        if (i >= argc ||
           !(isdigit(*argv[i]) || (*argv[i]=='-' && isdigit(*(argv[i]+sizeof(char)))))) {               
	  fprintf(stderr, "No number given\n");
	  rc = -1;
	}
	else {
	  cmd->number = atof(argv[i]);
	}
      }
      break;

    case ABOVE:
      if (cmd->search != NULL) {
        fprintf(stderr, "Too many search options given\n");
	rc = -1;
      }
      else {
	cmd->search = "-above";
	++i;
        if (i >= argc ||
            !(isdigit(*argv[i]) || ((*argv[i])=='-' && isdigit(*(argv[i]+sizeof(char)))))) {
	  fprintf(stderr, "No number given\n");
	  rc = -1;
	}
	else {
	  cmd->number = atof(argv[i]);
	}
      }
      break;

    default:
      position = i;
      break;
    }

    if ( (rc < 0) || (position == i) )
       break;
  }

  if (rc < 0)
    fprintf(stderr, "Option arguments are incorrect\n");
  else if (cmd -> measure == NULL)
    print_command(cmd);
  else if (position == 0)
   print_inputfiles(position, argc, argv);
  else  {
    print_command(cmd);
    print_inputfiles(position, argc, argv);
  }
  return rc;
}




/**************************************************************
*************************************************************** 
 * This is the test harness.
 * Do not create test data for this or build abstractions, etc.
***************************************************************
 **************************************************************/
int main(int argc,
         char **argv)
{
  struct command a;
  int rc;

  if (argc == 1) {
    usage("cmdline");
    rc = -1;
  }
  else
    rc = process_switches(argc, argv, &a);
    
  return rc;
}
