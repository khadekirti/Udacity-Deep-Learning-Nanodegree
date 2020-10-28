/*
ABOUT: 

FULLY CONNECTED NEURAL NETWORK 
4-Layer (784 * 200 * 30 * 10) 

*/
 

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 

 

#define ROW 28 
#define COL 28
#define number_hlayers 4

#include "mnist.h" 


void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
 

double sample_normal_distribution(){
    double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double r = u * u + v * v;
    if (r == 0 || r > 1) return sample_normal_distribution();
    double c = sqrt(-2 * log(r) / r);
    return u * c;
} 


 

double sigmoid(double x) { return 1.0f / (1.0f + exp(-x)); }


double dSigmoid(double x) { return sigmoid(x) * (1 - sigmoid(x)); }
 


int main(){
	clock_t begin = clock();


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
  
    /*
    ---------------------------------------------------------------------------- 
	----------------------------------------------------------------------------
	----------------------------------------------------------------------------
    */  

	/*
		Set Variables 
	*/ 

	double lr =0.01;
    const int epochs  = 100;
    int i,j, k;

    // weights
   	int layers[number_hlayers]=  {784, 200, 30 , 10};
    double  bias1[layers[1]], weight1[layers[1]][layers[0]];
    double  bias2[layers[2]], weight2[layers[2]][layers[1]] ;
    double  bias3[layers[3]], weight3[layers[3]][layers[2]] ;


    // forward propogation 
 	double kdash;
	double hideenlayer1Sigmoid[layers[1]];
	double hideenlayer1[layers[1]];
	double hideenlayer2Sigmoid[layers[2]];
	double hideenlayer2[layers[2]];	
	double output[layers[3]];
	double outputSigmoid[layers[3]]; 
	
	// Backward propogation 
	double deltaOutput[layers[3]];
	double deltaHidden2[layers[2]];
 	double deltaHidden1[layers[1]];
 	double deltaweight3[layers[3]][layers[2]];
 	double deltaweight2[layers[2]][layers[1]];  
 	double deltaweight1[layers[1]][layers[0]]; 

	// Testing 
	int largest_index;
	double largest; 
	int index_label;
	int number_test_passed = 0 ; 
	double accuracy;
 
	int row_test = sizeof(test_image)/ sizeof(test_image[0]);
	int row_train = sizeof(train_image)/ sizeof(train_image[0]);

	//Shuffle 
	int row_train_10 = row_train/10;
	double temp;
	int temp_;  

	 // Mini batch 
 	double mini_batch_train_image[10][784]; 
    int mini_batch_train_label[10][10];  

	printf("%d %d\n", row_test, row_train);


    /*
    ---------------------------------------------------------------------------- 
	----------------------------------------------------------------------------
	----------------------------------------------------------------------------
    */  

	/*
		Initialise Weights and Bias 
	*/  

	for(i = 0; i < layers[1]; i++){
		for(j = 0; j < layers[0]; j++){
			weight1[i][j] = sample_normal_distribution();
		}
	}

	for(i = 0; i < layers[2]; i++){
		for(j = 0; j< layers[1]; j++){
			weight2[i][j] = sample_normal_distribution(); 
		}
	}

	for(i = 0; i < layers[3]; i++){
		for(j = 0; j< layers[2]; j++){
			weight3[i][j] = sample_normal_distribution(); 
		}
	}
 

	for(i=0; i < layers[1]; i++){
		bias1[i] = sample_normal_distribution();
	}

	for(i = 0; i < layers[2]; i++){
		bias2[i] = sample_normal_distribution();
	}

	for(i = 0; i < layers[3]; i++){
		bias3[i] = sample_normal_distribution();
	} 

    int numTrainingSets = 60000; 

    int trainingSetOrder[numTrainingSets]; //= {0,1,2,3,4,5,6,7,8,9};

    for(i = 0; i < numTrainingSets; i++){
         trainingSetOrder[i] = i;
    }  

    /*
    ---------------------------------------------------------------------------- 
	----------------------------------------------------------------------------
	----------------------------------------------------------------------------
    */
    /*
    	Train
    */
    int epoch, iterate,index_train,index_test; 

    for(epoch = 0 ; epoch < epochs; epoch++){


		// Shuffle the data 
		shuffle(trainingSetOrder,numTrainingSets); 
		//printf("Get the shuffle id %d\n", trainingSetOrder[0]);


		//for(int i = 0; i < numTrainingSets; i++ ){
		//	printf("%d\n", trainingSetOrder[i] );
		//}

	    for(iterate = 0; iterate < row_train; iterate = iterate + 10){ 
	    	//printf("iteration %d\n", iterate );
	    	// Generate minibatch 
			for( i = 0 ; i <   10 ; i++ ){
				//printf("mini batch %d\n", i );
				for (j = 0 ; j < 784 ; j++)
				{
					mini_batch_train_image[i][j] =  train_image[trainingSetOrder[i]][j];
					//printf("mini train %f\n",  mini_batch_train_image[i][j]);
				}
				for (k = 0 ; k < 10 ; k++)
				{
					mini_batch_train_label[i][k] =  train_label[trainingSetOrder[i]][k];
					//printf("mini train label %d\n",  mini_batch_train_label[i][k]);

				} 
			}

   

			// Reset the bakward propogation to zero 
		    for (i=0; i<layers[3]; i++) {
		        deltaOutput[i] = 0.0f; 
		    }  

		    for (i=0; i< layers[2]; i++) {
		        	deltaHidden2[j] = 0.0;
		    }       
 
 		    for ( i=0; i< layers[1]; i++){
		        	deltaHidden1[j] = 0.0;
		    } 

		    for ( i=0; i<layers[3]; i++){
		        for ( j=0; j<layers[2]; j++) {
		            deltaweight3[i][j] = 0.0f; 
		        }
		    } 

		    for ( i=0; i<layers[2]; i++) {
		        for ( j=0; j<layers[1]; j++) {
		            deltaweight2[i][j] = 0.0f;
		        }
		    }  
		    for ( i=0; i<layers[1]; i++) {
		        for ( j=0; j<layers[0]; j++) {
		            deltaweight1[i][j] = 0.0f;
		        }
		    }   		 


		   	//printf("Mini Batch Created");

		   	for( index_train = 0 ; index_train < 10; index_train++){
		   		//printf("%d\n",  index_train);

				//Forward Bias
				for(i= 0;i < layers[1] ;i++){
					kdash = 0.0; 
					for(j=0;j < layers[0]; j++){
						kdash += weight1[i][j] * mini_batch_train_image[index_train][j];
					}
					hideenlayer1[i] = kdash + bias1[i];
					hideenlayer1Sigmoid[i] = sigmoid(hideenlayer1[i]);
				}

				for(i=0; i < layers[2]; i++){
					kdash = 0.0;
					for(j=0; j<layers[1]; j++){
						kdash += weight2[i][j]*hideenlayer1Sigmoid[j];
					}
					hideenlayer2[i] = kdash + bias2[i];
					hideenlayer2Sigmoid[i] = sigmoid(hideenlayer2[i]);
				}
 			
				for(i=0; i < layers[3]; i++){
					kdash = 0.0;
					for(j=0; j<layers[2]; j++){
						kdash += weight3[i][j]*hideenlayer2Sigmoid[j];
					}
					output[i] = kdash + bias3[i];
					outputSigmoid[i] = sigmoid(output[i]);
				}

				//Backward Bias
				for(i = 0; i< layers[3]; i++)
				{
					deltaOutput[i] += (outputSigmoid[i] - mini_batch_train_label[index_train][i] )*dSigmoid(output[i]);  
				}

				for(i=0; i< layers[3]; i++){
					for(j=0; j < layers[2]; j++){
						deltaweight3[i][j] += deltaOutput[i]* hideenlayer2Sigmoid[j];
					}
				}

			    for ( i = 0; i< layers[2]; i++) {
			    	kdash = 0.0;
			        for( j=0; j< layers[3]; j++) {
			        	kdash += deltaOutput[j]* weight3[j][i];  
			        }
			        deltaHidden2[i] += kdash * dSigmoid(hideenlayer2[i]);
			    }

			    for ( i=0; i<layers[2]; i++) {
			        for ( j=0; j<layers[1]; j++) {
			            deltaweight2[i][j] +=  deltaHidden2[i] * hideenlayer1Sigmoid[j] ; 
			        }
			    } 


			    for ( i = 0; i< layers[1]; i++) {
			    	kdash = 0.0;
			        for( j=0; j< layers[2]; j++) {
			        	kdash += deltaHidden2[j]* weight2[j][i];  
			        }
			        deltaHidden1[i] += kdash * dSigmoid(hideenlayer1[i]);
			    }


			    for ( i=0; i<layers[1]; i++) {
			        for ( j=0; j<layers[0]; j++) {
			            deltaweight1[i][j] +=  deltaHidden1[i] * mini_batch_train_image[index_train][j] ; 
			        }
			    }  
		   	}

		    // Based on this, change the weight and the bias
			for (i=0; i < layers[1] ; i++) 
			{
				for(j = 0;  j < layers[0] ; j++)
				{
					 weight1[i][j] = (weight1[i][j] - (lr * deltaweight1[i][j])/10.0) ;
				}
					bias1[i] = (bias1[i]  - (lr * deltaHidden1[i])/10.0); 
			} 


			for (i=0; i < layers[2] ; i++){
				for(j = 0;  j < layers[1] ; j++)
				{

					weight2[i][j] = (weight2[i][j] - (lr * deltaweight2[i][j]/10.0));
					//printf(" weight2 change %f\n", weight2[i][j]);
		 		}   

		 		bias2[i] = (bias2[i]  - (lr * deltaHidden2[i]/10.0));  
			}

			for (i=0; i < layers[3] ; i++){
				for(j = 0;  j < layers[2] ; j++)
				{

					weight3[i][j] = (weight3[i][j] - (lr * deltaweight3[i][j]/10.0));
					//printf(" weight2 change %f\n", weight2[i][j]);
		 		}   

		 		bias3[i] = (bias3[i]  - (lr * deltaOutput[i]/10.0));  
			}	 
		} 


		 /*
	    ---------------------------------------------------------------------------- 
		----------------------------------------------------------------------------
		----------------------------------------------------------------------------
	    */

	    /*
	    	Test
	    */ 	

		int index_test;
		number_test_passed = 0;
 
		if(epoch % 10 == 0) {

		    for(index_test = 0; index_test < row_test ;index_test++){

			    	// Forward Bias 

					for(i=0;i < layers[1] ;i++){
						kdash = 0.0; 
						for(j=0;j < layers[0]; j++){
							kdash += weight1[i][j] * test_image[index_test][j];
						}
						hideenlayer1[i] = kdash +  bias1[i];
						hideenlayer1Sigmoid[i] = sigmoid(hideenlayer1[i]);
					}

					for(i=0; i < layers[2]; i++){
						kdash = 0.0;
						for(j=0; j<layers[1]; j++){
							kdash += weight2[i][j]*hideenlayer1Sigmoid[j];
						}
						hideenlayer2[i] = kdash + bias2[i];
						hideenlayer2Sigmoid[i] = sigmoid(hideenlayer2[i]);
					} 

					for(i=0;i < layers[3] ;i++){
						kdash = 0.0; 
						for(j=0;j < layers[2]; j++){
							kdash += weight3[i][j] *  hideenlayer2Sigmoid[j];
						}
						output[i] = kdash +  bias3[i];
						outputSigmoid[i] = sigmoid(output[i]);
					} 					

					// Checking the reult 

					largest_index = 0;
					largest = outputSigmoid[0]; 

					for (i = 1; i < 10 ; i++) {
						if (largest < outputSigmoid[i]){
							largest_index = i;
							largest = outputSigmoid[i];
						}
						if (test_label[index_test][i] == 1){
							index_label = i;
						}
					}

					if (largest_index == index_label){
						number_test_passed += 1; 
					}
			    }

    			printf("epoch %d\n", epoch);
			    printf("number_test_passed %d\n", number_test_passed);
			    printf("row_test %d\n",  row_test);
			    //accuracy = number_test_passed * 1.0/row_test;  
			}  
 }
clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
printf("%f\n",  time_spent); 
 
}

 