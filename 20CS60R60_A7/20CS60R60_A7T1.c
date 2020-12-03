/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 7

                 Task 1 :

The text in file should Look like this :  
4 4    //r1 c1
0 1 1 1
2 2 2 5
3 3 3 3
4 4 4 4
                //new_line
4 4     // r2 c2
1 1 1 1
2 2 2 2
3 3 3 3
4 4 4 4
 
Compiling Command : gcc 20CS60R60_A7T1.c -o main -lpthread
Running command :./main input.txt

*******************************************************************************/
 
#include<stdio.h> 
#include<pthread.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include <time.h>

int r1,c1,r2,c2;
//Each thread computes single element in the resultant matrix 
void *mult(void* arg) 
{ 
    double *data = (double *)arg; 
    double k = 0;
    int i = 0; 
      
    int x =c1; 
    for (i = 0; i < x; i++) 
           k += data[i]*data[i+x]; 
      
    double *matC = (double*)malloc(sizeof(double)); 
         *matC = k; 
      
//Used to terminate a thread and the return value is passed as a pointer 
    pthread_exit(matC); 
} 
  
//Driver code 
int main(int argc , char **argv) 
{ 
    if(argc<2)
    {
        printf("Too less argv's");
        return 1;
    }
    int i,j,k; 
    char a[4],b[4],c[4];
    FILE *fp;
    fp=fopen(argv[1],"r");
    //reading matrix A
    fscanf(fp, "%s %s",a,b);
    r1=atoi(a);
    c1=atoi(b);
    double **matA;
    matA = (double **) malloc(sizeof(double) * r1);
	for (i = 0; i < r1; i++)
		matA[i] = (double *) malloc(sizeof(double) * c1);
    for(int i=0;i<r1;i++)
    {
        for(int j=0;j<c1;j++)
        {
            fscanf(fp,"%s",c);
            matA[i][j]=atoi(c);
        }
    }
    a[0]='\0';b[0]='\0';c[0]='\0';
    // reading matrix B
    
    fscanf(fp, "\n%s %s",a,b);
    r2=atoi(a);
    c2=atoi(b);
    double **matB;
    matB = (double **) malloc(sizeof(double) * r2);
	for (i = 0; i < r2; i++)
		matB[i] = (double *) malloc(sizeof(double) * c2);
    for(int i=0;i<r1;i++)
    {
        for(int j=0;j<c1;j++)
        {
            fscanf(fp,"%s",c);
            matB[i][j]=atoi(c);
        }
    }
    
    
    if(c1 != r2 )
        return 0;
    	printf("row1: %d, col1 = row2: %d, row2: %d, threads: %d\n", r1,c1,c2,(r1*c2));  
    /*// Displaying matA          
    for (i = 0; i < r1; i++){ 
        for(j = 0; j < c1; j++) 
            printf("%lf ",matA[i][j]); 
        printf("\n"); 
    } 
              
    // Displaying matB                 
    for (i = 0; i < r2; i++){ 
        for(j = 0; j < c2; j++) 
            printf("%lf ",matB[i][j]); 
        printf("\n");     
    } */
      
     
    int max = r1*c2; 
      
      
    //declaring array of threads of size r1*c2         
    pthread_t *threads; 
    threads = (pthread_t*)malloc(max*sizeof(pthread_t)); 
      
    int count = 0; 
    double* data = NULL; 
   //take start-time stamp
	clock_t start = clock(); 
    for (i = 0; i < r1; i++) 
        for (j = 0; j < c2; j++) 
               { 
                 
            //storing row and column elements in data  
            data = (double *)malloc((max+1)*sizeof(double)); 
      
            for (k = 0; k < c1; k++) 
                data[k] = matA[i][k]; 
      
            for (k = 0; k < r2; k++) 
                data[k+c1] = matB[k][j]; 
               
             //creating threads 
            pthread_create(&threads[count++], NULL, mult, (void*)(data));

                } 
   	clock_t stop = clock();
	
	double time = (stop - start)/1000;

	printf("time:    %f ms\n", time);
      
    for (i = 0; i < max; i++)  
    { 
      void *k; 
        
      //Joining all threads and collecting return value  
      pthread_join(threads[i], &k); 
      double *matC= (double*)malloc(sizeof(double));
      matC = (double *)k; 
     //print *matC
	free(matC);
      } 
  
     
  	fclose(fp);
  	for (int x = 0; x < r1; x++) {
		free(matA[x]);
	}
	for (int y = 0; y < c1; y++) {
		free(matB[y]);
	}
	free(matA);
	free(matB);
	
  return 0; 
} 
