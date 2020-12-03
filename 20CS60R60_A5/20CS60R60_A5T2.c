/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 5

                 Task 2 :

The text in file should Look like this :  
7,5
4,5
6,2
8,7
5,9
2,9
 
Compiling Command : gcc  20CS60R60_A5T2.c -o main
Running command :./main input.txt

*******************************************************************************/
#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<limits.h>

#define MAX_N 11
//structure for storing process tree
struct proc_info
{
  int p;
  int n;
  int fd[2];
  int cfd[100][2];
};
// structure for storing each process node information for leader election
struct leader
{
  int wsp;
  int wrp;
  int *rec;
  int vp;
  int state;
};
//structure to initialize the process node for leader election
struct leader *init_leader (int childs, int p)
{
  struct leader *a = (struct leader *) malloc (sizeof (struct leader));
  a->wsp = 0;
  a->wrp = 0;
  a->rec = (int *) malloc (childs * sizeof (int));
  for (int i = 0; i < childs; i++)
    a->rec[i] = 0;
  a->vp = p;
  a->state = 0;			//sleep -0 ,leader -1,lost-2
  return a;
}
//AdjListNode for graph
struct AdjListNode
{
  int dest;
  struct AdjListNode *next;
};
//agjacnecy list for graph
struct AdjList
{
  int childs;
  struct AdjListNode *head;
};
static int sum=0;
//declaring graph structure
struct Graph
{
  int V;
  struct AdjList *array;
};
//initializing adjacency list node
struct AdjListNode * newAdjListNode (int dest)
{
  struct AdjListNode *newNode =
    (struct AdjListNode *) malloc (sizeof (struct AdjListNode));
  newNode->dest = dest;
  newNode->next = NULL;
  return newNode;
}
//creating graph of required size
struct Graph *createGraph (int V)
{
  struct Graph *graph = (struct Graph *) malloc (sizeof (struct Graph));
  graph->V = V;

  graph->array = (struct AdjList *) malloc (V * sizeof (struct AdjList));

  int i;
  for (i = 0; i < V; ++i)
    {
      graph->array[i].head = NULL;
      graph->array[i].childs = 0;
    }

  return graph;
}
// a function to add node to graph
void addEdge (struct Graph *graph, int src, int dest)
{
  struct AdjListNode *newNode = newAdjListNode (dest);
  newNode->next = graph->array[src].head;
  graph->array[src].head = newNode;
  graph->array[src].childs += 1;

}

/*
 *
 *BUILD SUB TREE
 * 
 */
int buildSubTree (struct proc_info *R, struct proc_info *R1, struct Graph *g)
{
    
  if (R->n == 0)
    return 1;

  struct AdjListNode *pCrawl = g->array[R->p].head;
  int i=0;
  int reads;
  while (pCrawl!=NULL)
  {
   
    int *A=(int*)malloc(2*sizeof(int));
    int *B=(int*)malloc(2*sizeof(int));
      //creating pipes
    if (pipe(A) == -1)	return 2;
    if (pipe(B) == -1)	return 3;
    
    //setting child file descriptors
    R->cfd[i][0] = B[0];
   
    R->cfd[i][1] = A[1];
    
     i++; 
      //Initialize R1
      int x = pCrawl->dest;
      
      R1->p = x;
      R1->n = g->array[pCrawl->dest].childs;
      R1->fd[0] = A[0];
      R1->fd[1] = B[1];
      
     //child pipeFDs of R’ to all zeros
      for(int i=0;i<R1->n;i++)
     {
	    R1->cfd[i][0] = 0;
	    R1->cfd[i][1] = 0;
     }
     // printf("Parent : %d  Child  :%d\n",R->p,x);
      int process = fork ();
      //child process
      if (process == 0)
      {
        
         //closing the unnecessary file descriptors
         close (R->cfd[i][0]);
	     close ( R->cfd[i][1]);
         //Copy all fields from R1 to R;
	    R->p = R1->p;
	    R->n = R1->n;
	    R->fd[0] = R1->fd[0];
	    R->fd[1] = R1->fd[1];
	   //copying child's FD
	   for(int i=0;i<R1->n;i++)
	        {
	            R->cfd[i][0] = R1->cfd[i][0];
	            R->cfd[i][1] = R1->cfd[i][1];
	        }
	      
	     int y=buildSubTree (R, R1, g);
	    
	     return 4;
	  }
	  //parent process
      else
	  {
	      //closing the unnecessary file descriptors
	     close ( R1->fd[1]);
	     close (R1->fd[0]);
	       sum+=R->p+R1->p;
	    while( wait (NULL) !=-1 || errno !=ECHILD){
	    // printf("%d Waited for a child %d to finish\n",R->p,R1->p);
	    }
      }
      
      pCrawl = pCrawl->next;
    }
}

int count(struct leader *);
int mini(int ,int);

int main (int argc, char **argv)
{
  int neigh,max=0,n=0;
  int graph_mat[100][2];
  FILE *file;
  char new_line[100];
  const char s[100] = ",";
  int row=0,col=0,x=INT_MAX;
  char  *filename;
   if(argc<2)
    {
    printf("Missing filename\n");
    return(1);
    }
    else
    filename=argv[1];
  //opning file
  file = fopen (filename, "r");
  //getting input
  while (fgets (new_line,10,file))
    {
      char* tok= strtok(new_line, s); 
      while (tok != 0)
        { 
            n=atoi(tok);
            if(n>max)
               { max=n; }
            graph_mat[row][col++]=n;
            if(n<x){x=n;}
            n=0;
            tok = strtok(0, s); 
        } 
    row++;
    col=0;
    }
  //createGraph
  struct Graph *graph = createGraph (max + 1);
  for(int i=0;i<row;i++)
       addEdge (graph,graph_mat[i][1],graph_mat[i][0]);
    
    //finding root
    int root;   //find_root((int *)graph_mat,row);
    int f;
    for(int i=0;i<row;i++)
    { 
        f=0;
        int flag=graph_mat[i][1];
        for(int j=0;j<row;j++)
        {
            if(flag == graph_mat[j][0]);
            {
                f=1;
                break;
            }
        }
        root=flag;
    }

  struct proc_info *pid = (struct proc_info *) malloc (sizeof (struct proc_info));
  pid->p = root;
  pid->n = graph->array[root].childs;
  pid->fd[0] = -1;
  pid->fd[1] = -1;

   for(int i=0;i<pid->n;i++)
    {
      pid->cfd[i][0] = 0;
      pid->cfd[i][1] = 0;
    }
  //building process tree
  struct proc_info *pid1=(struct proc_info *) malloc (sizeof (struct proc_info));
  int code = buildSubTree (pid, pid1, graph);
  free (graph);
 
  int wake='w';
  int receive;
  int leader=x;
  struct leader *l=init_leader(pid->n,pid->p);
  //leader election algorithm
  if(pid->fd[0]==-1 && pid->fd[1]==-1)
    {   
        sum+= pid->p+pid->p;
        sum=sum-leader;
        printf("I am leader %d leader and sum of IDs of agent is %d",leader,sum );
        exit(0);
        l->wsp=1;
        for(int i=0;i<pid1->n;i++)
            {
                if(read (pid1->cfd[i][0], &wake, sizeof (int)) == -1) { return 1 ;}
               // printf("Wrote1 :%c\n",wake);
            }
   
    }
    else{
        
        int i=0;
        if(count(l) >1)
        {   
            while(count(l) >1)
            {
            if (write (pid->cfd[i][1], &wake , sizeof (int)) == -1) { return 4 ; }
               // printf("read4 :%c\n",wake);
            if(!(l->wsp))
                {
                l->wsp=1;
                for(int i=0;i<pid->n;i++)
                   if (read (pid1->cfd[i][0], &wake, sizeof (int)) == -1) { return 5 ; } 
                }
            l->rec[i]=1;
            l->vp=mini(l->vp,pid->p);
            i++;
                
            }
        }
      if(count(l)==1)
        {
            if (read (pid1->cfd[0][0], &wake, sizeof (int)) == -1) { return 5 ; }
            //printf("wrote5 :%c\n",wake);
            l->rec[i]=1;
        }
        
         if(count(l)==0)
         {
            if (write (pid1->fd[1], &receive , sizeof (int)) == -1) { return 6 ; }
             //printf("read c :%c\n",receive);
         }
        
        l->vp=mini(l->vp,receive);
        if((l->vp) == (pid1->p))
            l->state=1;
        else    
            l->state=2;
            
        if(l->state == 1)
             leader=l->vp;
        
         for(int i=0;i<pid1->n;i++)
            {
               if (read(pid1->cfd[i][0], &wake, sizeof (int)) == -1) { return 7; }  
            }
   
   }

    //parent waits for all child to exit 
    pid_t child_pid, wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0);
    fclose(file);
    return 0;
   
}
 /*
 *
 *UTILITY FUNC
 * 
 */
 //to count no. of adjacent nodes
 int count(struct leader *a)
{
    int size= sizeof(a->rec)/ sizeof(a->rec[0]);
    int count=0;
    for(int i=0;i<size;i++)
    {
        if((a->rec[i]) == 0)
            count++;
    }
return count;
}

//a function to count minimum of two values
int mini(int a,int b)
{
    if(a<b)
        return a;
    else
        return b;
}

