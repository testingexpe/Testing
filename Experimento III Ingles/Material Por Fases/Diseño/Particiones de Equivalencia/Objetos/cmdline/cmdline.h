#ifndef CMDLINEDOTH
#define CMDLINEDOTH

#define HELP    0
#define MEASURE 1
#define GKOM	2
#define LKOM	3
#define LKHM	4
#define GKHM	5
#define LIHE	6
#define ALL	7
#define MAX	8
#define AVERAGE 9
#define MIN	10
#define BELOW   11
#define ABOVE   12

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
