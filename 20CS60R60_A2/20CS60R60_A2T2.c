/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 2

                 Task 2 :

The text in file should Look like this :
5
0:3,4
1:2,4
2:1,4
3:0
4:0,1,2
7
0:0,5
1:4,6
2:1
3:3,6
4:2

*******************************************************************************/
#include <stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#define SIZE 100
#define int long long

/***********************************************************************
 * 
 *                              GRAPH CREATION
 * 
 * 
 * **********************************************************************/

// A structure to represent an adjacency list node 
struct AdjListNode 
{ 
    int dest; 
    struct AdjListNode* next; 
}; 
// A structure to represent an adjacency list 
struct AdjList 
{ 
    struct AdjListNode *head; 
    struct contents *content_arr;
}; 
//A structure to represent content list
struct contents
{
    int c;
    struct contents *next;
};
// A structure to represent a graph. A graph is an array of adjacency lists. 
// Size of array will be V (number of vertices in graph)
struct Graph 
{ 
    int V; 
    struct AdjList* array; 
    int* visited; // for bfs
}; 
//creating graph
struct Graph* createGraph(int V) 
{ 
    struct Graph* graph =  (struct Graph*) malloc(sizeof(struct Graph)); 
    graph->V = V; 
  
    // Create an array of adjacency lists.  Size of array will be V   
    graph->array =  (struct AdjList*) malloc(V * sizeof(struct AdjList)); 
    //creating an visited array 
    graph->visited =(int *) malloc(V *sizeof(int));
    // Initialize each adjacency list as empty by  making head as NULL  and visited array by 0
    for (int i = 0; i < V; ++i) 
    {
        graph->array[i].head = NULL; 
        graph->visited[i]=0;
    }
  
    return graph; 
} 
 // A utility function to create a new adjacency list node 
struct AdjListNode* newAdjListNode(int dest) 
{ 
    struct AdjListNode* newNode = 
     (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); 
    newNode->dest = dest; 
    newNode->next = NULL; 
    return newNode; 
} 
  
// Adds an edge to an undirected graph 
void addEdge(struct Graph* graph, int src, int dest) 
{ 
    // Add an edge from src to dest.  A new node is added to the adjacency list of src.The node is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode; 
} 
// A utility function to create a new content list node 
struct contents* addContentNode(int content)
{
    struct contents *newNode=(struct contents* )malloc(sizeof(struct contents));
    newNode->c=content;
    newNode->next=NULL;
    return newNode;
}
// Adds an edge to an content list for the user i
void addContent(struct Graph* graph,int src, int content)
{
    //every content node is added in the beginning of previous one
    struct contents *newNode=addContentNode(content);
    newNode->next=graph->array[src].content_arr;
    graph->array[src].content_arr=newNode;
   
}
// a utility function to match content that it is not same as of the user as a user don't want to see his own content
int content_match(struct Graph *graph,int src,int content)
{
  struct contents *temp=graph->array[src].content_arr;
  while(temp != NULL )
    {
        if(temp->c ==content )
            return 1;
        else
           temp = temp->next;
    }
    return 0;
}


/***********************************************************************
 * 
 *                      BFS(For distance finding)
 * 
 * 
 * **********************************************************************/
 //an structure for queue
 struct queue {
  int items[SIZE];
  int front;
  int rear;
};
//function to create a queue
 struct queue* createQueue() 
 {
  struct queue* q = (struct queue*)malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}
// Adding elements into queue
void enqueue(struct queue* q, int value) {
  if (q->rear == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

// Removing elements from queue
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    //printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        //resetting queue
      q->front = q->rear = -1;
    }
  }
  return item;
}
// a unility function to make visited as null as we are calling bfs for every user
void make_visited_null(struct Graph *graph)
{
 for (int i = 0; i < graph->V; ++i) 
        graph->visited[i]=0;
}
// a function to find distance between the content node and every other user
int  distanceContentNodes(struct Graph* graph, int startVertex,int content)
 {
  //if the content is its own content then return 1 as the distance to its own content is 1  
  if(content_match(graph,startVertex,content))
   {
       make_visited_null(graph);
       return 1;
   }
  struct queue* q = createQueue();
  int level[graph->V];
  level[startVertex]=0;
  // a simple BFS strategy
  graph->visited[startVertex] = 1;
  
  enqueue(q, startVertex);
      while (!isEmpty(q))
      {
        int currentVertex = dequeue(q);
        struct AdjListNode * temp = graph->array[currentVertex].head;
        while (temp) 
        {
          int adjVertex = temp->dest;
            
          if (graph->visited[adjVertex] == 0) 
          {
            graph->visited[adjVertex] = 1;
            level[adjVertex]=level[currentVertex]+1;
            if(content_match(graph,adjVertex,content)==1)
               {
                   make_visited_null(graph);
                   //return the level of vertex to which the content node is adjacent to
                   return (level[adjVertex]+1);
               }
            enqueue(q, adjVertex);
          }
          temp = temp->next;
        }
      }
    make_visited_null(graph);
    return 0; 
}
 /***********************************************************************
 * 
 *                       Print TOP l(using Max Heap)
 * 
 * 
 * **********************************************************************/
// a simple max heapify function
 void maxheapify(int a[], int i, int *heapsize)
{
    int temp, largest, left, right, k;
    left = (2*i+1);
    right = ((2*i)+2);
    if (left >= (*heapsize))
        return;
    else {
        if (left < (*heapsize) && a[left] > a[i]) 
            largest = left;
        else
            largest = i;
        if (right < (*heapsize) && a[right] > a[largest])  
            largest = right;
        if (largest != i) {
            temp = a[i];
            a[i] = a[largest];
            a[largest] = temp;
            maxheapify(a, largest, heapsize);
        }
    }
}
// function to extract max out of heap
int extractmax(int *arr,int *heapsize)
{
    if((*heapsize)<=0)
    {
      return INT_MAX;  
    }
    if((*heapsize)==1)
    {
        (*heapsize)--;
        return arr[0];
    }
    int max=arr[0];
    arr[0]=arr[(*heapsize)-1];
    (*heapsize)--;
    maxheapify(arr,0,heapsize);
    return max;
}

//function to build mac heap 
void buildmaxheap(int *a, int n,int *heapsize)
{
    for (int j = n/2; j >= 0; j--) 
    {
        maxheapify(a, j, heapsize);
    }
}
//function to print top l connected users along with their distances
void connectedUserNodes(struct Graph *graph,int l,int no_of_user,int *distances)
{
    int t,index,j=0;
    int m[no_of_user];
    int heapsize = no_of_user;
    //copying the required row from user_dist matrix as not to alter original array also removing the useless nodes like 0
    //which represent self node and directly connected User Nodes
    for(int i=0;i<no_of_user;i++)
    {
        if(distances[i]!=0)
        {
          m[j]=distances[i];
          j++;
        }
    }
    int id[no_of_user];
    //an array to keep track for which user we are printing
    for(int i=0;i<no_of_user;i++)
        id[i]=0;
    buildmaxheap(m, no_of_user,&j);
    for(int i=0;i<l;i++)
    {
        //extract max distance (i.e max number of contents matched) user
        t=extractmax(m,&j);
        for(int i=0;i<no_of_user;i++)
           {
               if(distances[i]==t && id[i]==0 )
               {
                id[i]=1;
                index=i;
                break;
               }
           }
        printf("\nUser %lld @ Distance %lld ",index,t);
        
    }
}
 /***********************************************************************
 * 
 *                              MAIN FUNCTION
 * 
 * 
 * **********************************************************************/
int main (int argc, char **argv)
{

  int i = 0,n=0;
  int u,k,l;
  char ch, txt_file[100000];
  FILE *file;
  char  *filename;
  if(argc<2)
{
    printf("TOO LESS ARGV's\n");
    return(1);
}
else
{
    filename=argv[1];
    u=atoi(argv[2]);
    k=atoi(argv[3]);
    l=atoi(argv[4]);
}
//opening the file a2_task2.txt
  file = fopen (filename, "r");

//copying file contents into character array
  while ((ch = fgetc (file)) != EOF)
    txt_file[i++] = ch;
  txt_file[i] = '\0';
  
i=0;  
int no_of_user=0;
char nouser[4];
int x=0;
while(txt_file[i]!='\n')
 {
    nouser[x++]=txt_file[i++];
 }
 nouser[x]='\0';
 no_of_user=atoi(nouser);
int user_count;

struct Graph* graph = createGraph(no_of_user);
i+=2;
// for graph creation
for(user_count=0;user_count<no_of_user;user_count++)
{
    while(txt_file[i]!=':')
      i++;
    i++;
      const char s[100] = ","; 
      char new_line[100];
      int c;
      for(c=0;txt_file[i]!='\n';)
        new_line[c++]=txt_file[i++];
       new_line[c]='\0';
      char* tok= strtok(new_line, s);
        while (tok != 0)
        { 
            int j=0;
            while(tok[j]!='\0')
                n=n*10+(tok[j++]-'0');
            addEdge(graph,user_count,n);
            n=0;
            tok = strtok(0, s); 
        } 
    i++;
    
}

int no_of_contents=0;
char nocontent[4];
x=0;
while(txt_file[i]!='\n')
{
 nocontent[x++]=txt_file[i++];
}
nocontent[x]='\0';
no_of_contents=atoi(nocontent);
i+=2;
//for content links creation
for(user_count=0;user_count<no_of_user;user_count++)
{
    while(txt_file[i]!=':')
      i++;
    i++;
     const char s[100] = ","; 
      char new_line[100];
      int c;
      for(c=0;txt_file[i]!='\n';)
        new_line[c++]=txt_file[i++];
       new_line[c]='\0';
      char* tok= strtok(new_line, s);
        while (tok != 0)
        { 
            int j=0;
            while(tok[j]!='\0')
                n=n*10+(tok[j++]-'0');
            addContent(graph,user_count,n);
            n=0;
            tok = strtok(0, s); 
        } 
    i++;
    
}

if(k>no_of_contents)
{
    printf("These Many Contents Not Present");
    return 1;
}

int content_dt_matrix[no_of_user][no_of_contents];
//creating matrix for distance of every node to evry other content
for(int v=0;v<no_of_user;v++)
{
    for(int u=0;u<no_of_contents;u++)
    {
        content_dt_matrix[v][u]=distanceContentNodes(graph,v,u);
    }
}
//storing the row for the required user
int user_dist[no_of_user];
for(int i=0;i<no_of_user;i++)
    user_dist[i]=0;
for(int j=0;j<no_of_contents;j++)
{
    if(content_dt_matrix[u][j]<=k)
    {
        for(int i=0;i<no_of_user;i++)
        {
            if(content_dt_matrix[i][j]<=k)
                user_dist[i]+=1;
        }
    }
}

user_dist[u]=0;
//making user's distance to directly connected user as 0
struct AdjListNode * temp = graph->array[u].head;
while(temp)
{
    user_dist[temp->dest]=0;
    temp=temp->next;
}

connectedUserNodes(graph,l,no_of_user,user_dist);

fclose(file);
return 0;
}