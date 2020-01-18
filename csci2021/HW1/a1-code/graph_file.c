#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deltas.h"
#include <sys/stat.h>

int main(int argc, char *argv[]){
  char *format = argv[1];
  char *fname = argv[2];
  char *number=argv[3];

  int num=atoi(number);

  int data_len = -1;
  int *data_vals = NULL;
  if( strcmp("text", format)==0 ){
    printf("Reading text format\n");
    data_vals = read_text_deltas(fname, &data_len);
    print_graph(data_vals, data_len, num);
  }
  else if( strcmp("int", format)==0 ){
    printf("Reading binary int format\n");
    data_vals = read_int_deltas(fname, &data_len);
    print_graph(data_vals, data_len, num);
  }

  else{
    printf("Unknown format '%s'\n",format);
    return 1;
  }

  free(data_vals);

  return 0;
}
