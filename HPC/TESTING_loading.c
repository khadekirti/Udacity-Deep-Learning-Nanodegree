/*
ABOUT: 

TESTING CALCULATION vs Python Code
*/ 

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
  

#define ROW 28 
#define COL 28
#define number_hlayers 3

#include "mnist.h" 


int main(){


    /*
    ---------------------------------------------------------------------------- 
	----------------------------------------------------------------------------
	----------------------------------------------------------------------------
    */ 

   	/*
   		Load Data 
	*/
 
	/* test_image test_labels train_image train_labels */
	load_mnist();
	printf("%d" , test_label[0][0]);
  
    /*
    ---------------------------------------------------------------------------- 
	----------------------------------------------------------------------------
	*/ 
}

