#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int main(int argc, char const *argv[]) {
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

    printf("BST Demo\n");
    printf("Commands:\n");
    printf("  print:          shows contents of the tree in reverse sorted order\n");
    printf("  clear:          eliminates all elements from the tree\n");
     printf("  quit:           exit the program\n");
    printf("  add <name>:     inserts the given string into the tree, duplicates ignored\n");
    printf("  find <name>:    prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
    printf("  preorder:       prints contents of the tree in pre-order which is how it will be saved\n");
    printf("  save <file>:    writes the contents of the tree in pre-order to the given file\n");
    printf("  load <file>:    clears the current tree and loads the one in the given file\n");

  
  char cmd[128];
  bst_t newTree;
  bst_init(&newTree);
  int success;

  while(1){
    printf("BST> ");                 // print prompt
    success=fscanf(stdin,"%s",cmd);
    if(success==EOF){
      printf("\n");
      break;
    }

    if( strcmp("exit", cmd)==0 ){     // check for exit command
      if(echo){
        printf("exit\n");
      }
      break;                          // break from loop
    }

    //Echoing toggle
    int echo = 0;
    if(argc > 1 && strcmp("-echo",argv[1])==0) {
      echo=1;
    }

    //quit command
    if( strcmp("quit", cmd)==0 ){
      if(echo){
        printf("quit\n");
      }
      break;                          // break from loop
    }

    //print command
    else if(strcmp("print",cmd)==0){
      if(echo){
        printf("print\n");
      }
       bst_print_revorder(&newTree);
    }

    //clear command
    else if(strcmp("clear",cmd)==0){
      if(echo){
        printf("clear\n");
      }
      bst_clear(&newTree);
    }

    //add command
    else if( strcmp("add", cmd)==0 ){ // insertion
      fscanf(stdin,"%s",cmd);            // read string to insert
      if(echo){
        printf("add %s\n",cmd);
      }

      success = bst_insert(&newTree, cmd); // call list function
      if(!success){                      // check for success
        printf("insert failed\n");
      }
    }

    //find command
    else if( strcmp("find", cmd)==0 ){     // get command
      fscanf(stdin,"%s",cmd);          // read an integer
      if(echo){
        printf("find %s\n",cmd);
      }
      success=bst_find(&newTree, cmd);
      if(success){
        printf("'%s' is present\n",cmd);
      }
      else{
        printf("not found\n");
      }
    }

    //preorder command
    else if( strcmp("preorder", cmd)==0 ){
      bst_print_preorder(&newTree);
    }
  }
}
