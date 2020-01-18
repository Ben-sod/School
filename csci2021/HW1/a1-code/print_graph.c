#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_graph(int *data, int len, int max_height){

  //computing max value in array
  int max=0;
  for(int i=0;i<len;i++){
    if(data[i]> max){
      max=data[i];
    }
  }
  //computing min value in array
  int min;
  for(int i=0;i<len;i++){
    if(i==0){
        min=data[i];
    }
    else if(data[i]<min){
      min=data[i];
    }
  }
  //finding range and units per height
  int range= max-min;
  float units_per_height=(float)range/(float)max_height;

  printf("length:  %d\n",len);
  printf("min:  %d\n",min);
  printf("max:  %d\n",max);
  printf("range:  %d\n",range);
  printf("max_height:  %d\n",max_height);
  printf("units_per_height:  %.2f\n",units_per_height);

  printf("     +----+----+----+----+----+----+----+----+----+----\n");

  for(int j=max_height;j>-1;j--){
    int level=j;
    int cutoff= min+level*units_per_height;
    printf("%3d |",(int)level*(int)units_per_height);
    for(int i=0;i<len;i++){
      if(data[i]>=cutoff){
        printf("x");
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }


  printf("     +----+----+----+----+----+----+----+----+----+----\n");
  printf("     0    5    10   15   20   25   30   35   40   45   ");
}
