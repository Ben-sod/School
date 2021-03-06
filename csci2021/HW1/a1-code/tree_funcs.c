#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void bst_init(bst_t *tree){
  tree->root=NULL;
  tree->size=0;
}
// Initialize the given tree to have a null root and have size 0.


int bst_insert(bst_t *tree, char name[]){

  if(tree->root==NULL){
    //tree is empty, add new note to the root
    node_t *insNode= malloc(sizeof(node_t));
    strcpy(insNode->name,name);
    tree->root=insNode;
    insNode->left=NULL;
    insNode->right=NULL;
    tree->size++;
    return 1;
  }

  //declaring note to traverse the tree
  node_t *finder=tree->root;
  while(1){
    int num=strcmp(name,finder->name);
    if(num==0){
      return 0; //node is equal to an existing node so we do nothing
    }
    else if(num<0){ //going left in traversing the tree
      if(finder->left==NULL){ //if null pointer is found put new node here
        node_t *insNode= malloc(sizeof(node_t));
        strcpy(insNode->name,name);
        insNode->left=NULL;
        insNode->right=NULL;
        tree->size++;
        return 0;
      }
      else{
        finder=finder->left;
      }

    }
    else if(num>0){ //going right in traversing the tree
      if(finder->right==NULL){ //if a null pointer is found, put new node here
        node_t *insNode= malloc(sizeof(node_t));
        strcpy(insNode->name,name);
        insNode->right=NULL;
        insNode->right=NULL;
        tree->size++;
        return 0;e
      }
      else{
        finder=finder->right;
      }
    }
  }
}
// Inserts given name into a binary search tree. Uses an ITERATIVE
// (loopy) approach to insertion which starts a pointer at the root of
// the tree and changes its location until the correct insertion point
// is located. Returns 1 if a new node is created and 0 if a duplicate
// name is found in the tree already in which case the name is not
// added.

int bst_find(bst_t *tree, char name[]){
  //declaring finder node and setting it to the root of the tree
  node_t *finder=tree->root;
  //comparing finder name to the name we are looking for
  int num= strcmp(finder->name,name);
  if(num==0){ //if tree root is equal
    return 1;
  }
  while(1){
    int num= strcmp(finder->name,name);
    if(num>0){
      if(finder->right==NULL){ //if we reach end of a branch, return 0
        return 0;
      }
      else{
        finder=finder->right;//traverse right
      }
    }
    if(num<0){
      if(finder->left==NULL){
        return 0;
      }
      else{
        finder=finder->left; //traverse right
      }
    }
  }
}
// Determines whether name is present or not in the tree using binary
// search. Returns 1 if name is present and 0 otherwise. Uses an
// ITERATIVE (loopy) approach which starts a pointer at the root of
// the tree and changes it until the search name is found or
// determined to not be in the tree.

void bst_clear(bst_t *tree){
  node_remove_all(tree->root); //calling remove all function
  tree=NULL;
}
// Eliminate all nodes in the tree setting its contents empty. Uses
// recursive node_remove_all() function to free memory for all nodes.

void node_remove_all(node_t *cur){
  if(cur!=NULL){
    node_remove_all(cur->left); //recuirsively removing all branches and nodes
    node_remove_all(cur->right);
    cur->left=NULL;
    cur->right=NULL;
    free(cur); //freeing data
  }
}
// Recursive helper function which visits all nodes in a tree and
// frees the memory associated with them. This requires a post-order
// traversal: visit left tree, visit right tree, then free the cur
// node.


void bst_print_revorder(bst_t *tree){
  node_print_revorder(tree->root,0); //calling helper funciton

}
// Prints the elements of the tree in reverse order at differing
// levels of indentation which shows all elements and their structure
// in the tree. Visually the tree can be rotated clockwise to see its
// structure.

void node_print_revorder(node_t *cur, int indent){
  if(cur!=NULL){ //checking if the passed in node is null

    node_print_revorder(cur->right,indent+1); //recursively printing right branches

    for(int i=0;i<indent;i++){
      printf("  ");//printing spaces
    }
    printf("%s\n",cur->name);//pringting name

    node_print_revorder(cur->left,indent+1); //recursively printing left branches
  }

}
// Recursive helper function which prints all elements in the tree
// rooted at cur in reverse order: traverses right subtree, prints cur
// node, traverses left tree. Parameter 'indent' indicates how far to
// indent (2 spaces per indent level).


void bst_print_preorder(bst_t *tree){
  node_write_preorder(tree->root, stdout,0); //calling helper function
}
// Print all the data in the tree in pre-order with indentation
// corresponding to the depth of the tree. Makes use of
// node_write_preorder() for this.

void bst_save(bst_t *tree, char *fname){
  FILE *fp; //opening file
  fp = fopen(fname,"r+");

  node_write_preorder(tree->root, fp, 0); //calling helper function
  fclose(fp);//closing file

}
// Saves the tree by opening the named file, writing the tree to it in
// pre-order with node_write_preorder(), then closing the file.

void node_write_preorder(node_t *cur, FILE *out, int depth){

  if(cur!=NULL){

    node_write_preorder(cur->left,out,depth+1); //recursively traversing left branches

    for(int i=0;i<depth;i++){
      fprintf(out,"  ");
    }
    fprintf(out,"%s\n",cur->name);

    node_write_preorder(cur->right,out,depth+1);//recursively traversing left branches
  }

}
// Recursive helper function which writes/prints the tree in pre-order
// to the given open file handle. The parameter depth gives how far to
// indent node data, 2 spaces per unit depth. Depth increases by 1 on
// each recursive call. The function prints the cur node data,
// traverses the left tree, then traverses the right tree.

int bst_load(bst_t *tree, char *fname ){
  bst_clear(tree); //clears and possible remaining tree
  //opening file
  FILE *fp;
  fp=fopen(fname, "r");
  //chekcing if file opened
  if(fp==NULL){
    return 0;
  }
  //reading in strings from file
  char string[100];
  while(fscanf(fp, "%s",string)!=EOF){
    bst_insert(tree, string);
  }
  return 1;

}
// Clears the given tree then loads new elements to it from the
// named. Repeated calls to bst_insert() are used to add strings read
// from the file.  If the tree is stored in pre-order in the file, its
// exact structure will be restored.  Returns 1 if the tree is loaded
// successfully and 0 if opening the named file fails in which case no
// changes should be made to the tree. 
