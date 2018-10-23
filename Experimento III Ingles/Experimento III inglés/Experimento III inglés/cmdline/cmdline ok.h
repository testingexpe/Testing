#ifndef CMDLINEDOTH
#define CMDLINEDOTH

#define HELP    0
#define MEASURE 1
#define GKOM	2
#define LKOM	3
#define LKHM	4
#define GKHM	5
#define LIHE	6
#define GIHE	7
#define ALL	8
#define MAX	9
#define AVERAGE 10
#define MIN	11
#define BELOW   12
#define ABOVE   13

struct keyword_entry {
  char *keyword; 
  int  min_len;
  int  id;
};

struct command {
  double number;
  char *measure;
  char *search;
};

#endif
