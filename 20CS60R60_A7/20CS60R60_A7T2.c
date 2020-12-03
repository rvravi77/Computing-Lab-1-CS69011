/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 7

                 Task 2 :

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
 
Compiling Command : gcc 20CS60R60_A7T2.c -o main -lpthread
Running command :./main input.txt

*******************************************************************************/
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
#define CORE 4
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
	double **matA;
	double **matB;
	double **matC;
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

/* matrix multiplication for the appropriate line of data.matA */
void mat_mult (int line) {
	for (int z = 0; z < data.z; z++) {
		for (int y = 0; y < data.y; y++) {
			data.matC[line][z] += data.matA[line][y] * data.matB[y][z];
		}
	} 
}

/* in data.thread_array are the numbers of lines (of data.matA) to be calated for each thread */
void *thread_function() {

	mutex_lock(& data.mutex);
	while (data.thread_array[data.thread_nr] != 0) {

		mat_mult(data.line);
		data.line++;
		data.thread_array[data.thread_nr]--;
	}
	data.thread_nr++;
	mutex_unlock(& data.mutex);
	pthread_exit((void *)pthread_self());	
}


int main (int argc, char **argv) {
   
	if (argc<2) {
		printf("arguments: <./program> <input.txt>");
		exit(EXIT_FAILURE); 
	}
	
	char a[8],b[8],c[8];
    FILE *fp;
    fp=fopen(argv[1],"r");
    fscanf(fp, "%s %s",a,b);
 
    data.x = atoi(a);
	data.y = atoi(b);
    //read matrix A 
    
    if ((data.matA = malloc(sizeof(double*) * data.x)) == NULL) {
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for(int x = 0; x < data.x; x++) {
		if ((data.matA[x] = malloc(sizeof(double) * data.y)) == NULL) {
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
	if ((data.matB = malloc(sizeof(double*) * data.y)) == NULL) {
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for (int y = 0; y < data.y; y++) {
		if ((data.matB[y] = malloc(sizeof(double) * data.z)) == NULL) { \
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

	arg_insert(CORE);
	//init matrix C 
	if ((data.matC = calloc(data.x, sizeof(double*))) == NULL) {
		perror("calloc error");
		exit(EXIT_FAILURE);
	}
	for (int x = 0; x < data.x; x++) {
		if ((data.matC[x] = calloc(data.z, sizeof(double))) == NULL) {
			perror("calloc error");
			exit(EXIT_FAILURE);
		}
	}
	
	int thread_arr[data.n];
	thread_lines(thread_arr);
	
	pthread_t pid[data.n];

	//take start-time stamp
	clock_t start = clock();

	for (int i = 0; i < data.n; i++) {
		if (pthread_create(&(pid[i]), NULL, thread_function, NULL) != 0) {
				perror("pthread_create error");
				exit(EXIT_FAILURE);
		}
	}
	
	for (int i = 0; i < data.n; i++) {
		if ((pthread_join(pid[i], NULL)) != 0 ) {
			perror("pthread_join error");
			exit(EXIT_FAILURE);
		}
	}
	
	//take end-time stamp
	clock_t stop = clock();
	
	double time = (stop - start)/1000;

	printf("time:    %f ms\n", time);	
	free_mat();
	
	printf("row1: %d, col1 = row2: %d, row2: %d, threads: %d\n", data.x, data.y, data.z, data.n);
	fclose(fp);
	return (EXIT_SUCCESS);
}