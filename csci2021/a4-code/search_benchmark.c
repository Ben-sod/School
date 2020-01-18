#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "search.h"


int main(int argc, char *argv[]){

  int min = atoi((argv[1]));
  int max = atoi((argv[2]));
  int rep = atoi((argv[3]));


  int do_array=0,
      do_barray=0,
      do_list=0,
      do_tree=0;

  if(argv[4]!=NULL){
    char *format=argv[4];
    if(strchr(format,'a')){
      do_array=1;
    }
    if(strchr(format,'b')){
      do_barray=1;
    }
    if(strchr(format,'l')){
      do_list=1;
    }
    if(strchr(format,'t')){
      do_tree=1;
    }
  }
  else{
    do_array=1;
    do_barray=1;
    do_list=1,
    do_tree=1;
  }

  clock_t begin, end;
  double cpu_time;


  printf("LENGTH");
  printf("SEARCHES \t");
  printf("array \t");
  printf("list \t");
  printf("binary \t");
  printf("tree \t");


    //loop that iterates from min size to max size
    for(int size=min;size<=max;size++){
      //loop to generate the 2^size
      int power=2;
      for(int i=0;i<size;i++){
        power=power*2;
      }
      int length=power;

      if(do_array){
        int *arr= make_evens_array(length);

        //begin clock before searches
        begin=clock();

        //loop to iterate through the reps
        for(int j=0;j<rep;j++){
          //loop to iterate through the search querys
          for(int k=0;k<length*2;k++){
            linear_array_search(arr, length, k);
          }
        }
      //after reps loop, end clock and print out the
        end=clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC; //calculating cpu time
        printf("%5e",cpu_time);
        free(arr); //freeing the memory

      }

      if(do_barray){
        int *barr= make_evens_array(length);

        //begin clock before searches
        begin=clock();

        //loop to iterate through the reps
        for(int j=0;j<rep;j++){
          //loop to iterate through the search querys
          for(int k=0;k<length*2;k++){
            linear_array_search(barr, length, k);
          }
        }
        //after reps loop, end clock and print out the
        end=clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC; //calculating cpu time
        printf("%5e",cpu_time);
        free(barr); //freeing the memory
      }


      if(do_tree){
        bst_t *tree= make_evens_tree(length);

        //begin clock before searches
        begin=clock();

        //loop to iterate through the reps
        for(int j=0;j<rep;j++){
          //loop to iterate through the search querys
          for(int k=0;k<length*2;k++){
            binary_tree_search(tree, length, k);
          }
        }
        //after reps loop, end clock and print out the
        end=clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC; //calculating cpu time
        printf("%5e",cpu_time);
        bst_free(tree); //freeing the memory

      }

      if(do_list){
        list_t *list= make_evens_list(length);

        //begin clock before searches
        begin=clock();

        //loop to iterate through the reps
        for(int j=0;j<rep;j++){
          //loop to iterate through the search querys
          for(int k=0;k<length*2;k++){
            linkedlist_search(list, length, k);
          }
        }
        //after reps loop, end clock and print out the
        end=clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC; //calculating cpu time
        printf("%5e",cpu_time);
        list_free(list); //freeing the memoryd
      }


    }
  }


//////////////////////////////////////////////////////////////////////////////////////////////////////
