#include "matvec.h"
#include <stdlib.h>

int optimized_matrix_trans_mult_vec(matrix_t mat, vector_t vec, vector_t res){
  if(mat.rows != vec.len){
    printf("mat.rows (%ld) != vec.len (%ld)\n",mat.rows,vec.len);
    return 1;
  }
  if(mat.cols != res.len){
    printf("mat.cols (%ld) != res.len (%ld)\n",mat.cols,res.len);
    return 2;
  }
  for(int i=0; i<mat.cols;i++){
    VSET(res,i,0);
  }
  for(int i=0; i<mat.rows; i++){
    for(int j=0; j<mat.cols; j+=2){ //loop iterates by 2 so that 2 iterations can be done in one iteration
      int elij = MGET(mat,i,j);
      int veci= VGET(vec,i);
      int curr = VGET(res,j);
      //adding 1 to my j values so that they can be piplined, allowing 2 iterations to process at one time
      int elij2 = MGET(mat,i,j+1);
      int veci2= VGET(vec,i);
      int curr2 = VGET(res,j+1);
      //next group of instructions work concurrently as soon as previous code executes
      int prod = elij * veci;
      int next = curr + prod;
      int prod2 = elij2 * veci2;
      int next2 = curr2 + prod2;
      //2 locations are set at the same time
      VSET(res,j, next);
      VSET(res,j+1, next2);

    }
  }
  return 0;
}
