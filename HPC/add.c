#include <stdio.h>
#include <math.h> 

 
int add(int a, int b) {
    return a + b;
} 


double sample_normal_distribution(){
    double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
    double r = u * u + v * v;
    if (r == 0 || r > 1) return sample_normal_distribution();
    double c = sqrt(-2 * log(r) / r);
    return u * c;
} 



//double sample_normal_distribution() { 
//	return ((double)rand())/((double)RAND_MAX); 
//}

 

double sigmoid(double x) { return 1.0f / (1.0f + exp(-x)); }


double dSigmoid(double x) { return sigmoid(x) * (1 - sigmoid(x)); }
 

int get_row_col( int a , int b){
	int row = a / b;
	int column = b/sizeof(double);
	printf("%d %d \n", row, column );
	return 0;
}

/*




*/


