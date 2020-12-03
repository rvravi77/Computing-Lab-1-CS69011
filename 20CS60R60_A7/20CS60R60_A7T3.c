/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 7

                 Task 3 :

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
 
Compiling Command : gcc 20CS60R60_A7T3.c -o main -lpthread
Running command :./main input.txt 4

*******************************************************************************/
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# define BufferSize 10
void *Producer();
void *Consumer();

int BufferIndex=0;
int  *BUFFER;

pthread_cond_t Buffer_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty=PTHREAD_COND_INITIALIZER;
void mutex_init(pthread_mutex_t *mutex) {
	if (pthread_mutex_init(mutex, NULL) != 0) {
        printf("pthread_mutex_init error");
		exit(EXIT_FAILURE);
    }
}


void mutex_lock(pthread_mutex_t *mutex){
	if (pthread_mutex_lock(mutex) != 0) {
		perror("pthread_mutex_lock error");
		exit(EXIT_FAILURE);
	}
}

void mutex_unlock(pthread_mutex_t *mutex) {
	if (pthread_mutex_unlock(mutex) != 0) {
		perror("pthread_mutex_unlock error");
		exit(EXIT_FAILURE);
	}
}

typedef struct {
	int x;
	int y;
	int z;
	int n;
	int **matA;
	int **matB;
	int **matC;
	int *thread_array; //includes, how many lines of data.matA each thread has to calculate
	int line;
	int thread_nr;
	pthread_mutex_t mutex;
} data_struct;

data_struct data;


void arg_insert(int n){
	
	data.n = n;
	data.line = 0;
	data.thread_nr = 0;
	mutex_init(&data.mutex);
}

/* free of malloc/calloc - data.matA, data.matB, data.matC */
void free_mat() {
	for (int x = 0; x < data.x; x++) {
		free(data.matA[x]);
		free(data.matC[x]);
	}
	for (int y = 0; y < data.y; y++) {
		free(data.matB[y]);
	}
	free(data.matA);
	free(data.matB);
	free(data.matC);
}


/* calculates, how many lines of data.matA each thread has to calculate -> the result is written in in the array: thread_arr where the pointer data.thread_arrays (struct) points to */
void thread_lines(int thread_arr[data.n]) {
	for (int i = 0; i < data.n; i++) {
		data.thread_array = thread_arr;
		thread_arr[i] = (i<(data.x % data.n)) ? ((data.y/data.n)+1) : (data.y/data.n);
//		printf("thread <%d> = %d lines\n", i, data.thread_array[i]);
	}
}

int main(int argc, char **argv)
{   
    if (argc<2) {
		printf("arguments: <./program> <input.txt> <threads>");
		exit(EXIT_FAILURE); 
	}
	
	char a[8],b[8],c[8];
    FILE *fp;
    fp=fopen(argv[1],"r");
    fscanf(fp, "%s %s",a,b);
 
    data.x = atoi(a);
	data.y = atoi(b);
    //read matrix A 
    
    if ((data.matA = malloc(sizeof(int*) * data.x)) == NULL) {
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for(int x = 0; x < data.x; x++) {
		if ((data.matA[x] = malloc(sizeof(int) * data.y)) == NULL) {
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
	}
	for (int x = 0; x < data.x; x++) {
		for (int y = 0; y < data.y; y++) {
		    fscanf(fp,"%s",c);
			data.matA[x][y] =atoi(c);
		}
	}
	a[0]='\0';b[0]='\0';c[0]='\0';
    fscanf(fp, "\n%s %s",a,b);
    data.y=atoi(a);
    data.z = atoi(b);
    // read matrix B
	if ((data.matB = malloc(sizeof(int*) * data.y)) == NULL) {
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for (int y = 0; y < data.y; y++) {
		if ((data.matB[y] = malloc(sizeof(int) * data.z)) == NULL) { \
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
	}
	for (int y = 0; y < data.y; y++) {
		for (int z = 0; z < data.z; z++) {
		    fscanf(fp,"%s",c);
			data.matB[y][z] = atoi(c);
		}
	}

	arg_insert(atoi(argv[2]));
	//init matrix C 
	if ((data.matC = calloc(data.x, sizeof(int*))) == NULL) {
		perror("calloc error");
		exit(EXIT_FAILURE);
	}
	for (int x = 0; x < data.x; x++) {
		if ((data.matC[x] = calloc(data.z, sizeof(int))) == NULL) {
			perror("calloc error");
			exit(EXIT_FAILURE);
		}
	}
	int thread_arr[data.n];
	thread_lines(thread_arr);
   
    pthread_t ptid,ctid[data.n];

    BUFFER=(int *) malloc(sizeof(int) * BufferSize); 
    
    //creating thread
   
		if (pthread_create(&(ptid), NULL, Producer, NULL) != 0) {
				perror("pthread_create error");
				exit(EXIT_FAILURE);
		}

	 for (int i = 0; i < data.n; i++){
	     	if (pthread_create(&(ctid[i]), NULL, Consumer, NULL) != 0) {
				perror("pthread_create error");
				exit(EXIT_FAILURE);
		}
	 }
    
    //joining thread   	
	
		if ((pthread_join(ptid, NULL)) != 0 ) {
			perror("pthread_join error");
			exit(EXIT_FAILURE);
		}
	for (int i = 0; i < data.n; i++) 
	{
	    	if ((pthread_join(ctid[i], NULL)) != 0 ) {
			perror("pthread_join error");
			exit(EXIT_FAILURE);
		}
	}
    
    
    return 0;
}

void *Producer()
{    
    while(1)
    {
        
        mutex_lock(&data.mutex);
        if(BufferIndex==BufferSize)
        {                        
            pthread_cond_wait(&Buffer_Not_Full,& data.mutex);
        }                        
        while (data.thread_array[data.thread_nr] != 0)
        {
	    BUFFER[BufferIndex++]=data.line;
	    
		data.line++;
		data.thread_array[data.thread_nr]--;
	   }
        data.thread_nr++;
       
        mutex_unlock(&data.mutex);
        pthread_exit((void *)pthread_self());
        pthread_cond_signal(&Buffer_Not_Empty); 
        
    } 
}

void *Consumer()
{
    while(1)
    {
        mutex_lock(&data.mutex);
        if(BufferIndex==-1)
        {            
            pthread_cond_wait(&Buffer_Not_Empty,& data.mutex);
        }     
        
       for (int z = 0; z < data.z; z++) {
		for (int y = 0; y < data.y; y++) {
			data.matC[BUFFER[BufferIndex]][z] += data.matA[BUFFER[BufferIndex]][y] * data.matB[y][z];
		}
	   } 
	   
        mutex_unlock(&data.mutex);        
        pthread_cond_signal(&Buffer_Not_Full);                
    }    
}
