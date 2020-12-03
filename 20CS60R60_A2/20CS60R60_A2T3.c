/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 2

                 Task 3 :

The text in file should Look like this :
5
0:3,4
1:2,4
2:1,4
3:0
4:0,1,2
7
0:0,1;5,0;
1:4,2;6,2;
2:1,1;
3:3,0;6,2;
4:2,0;

*******************************************************************************/
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 40
#define int long long

/***********************************************************************
 * 
 *                              GRAPH CREATION
 * 
 * 
**********************************************************************/

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
    int p;
    struct contents *next;
};
  
//A structure to represent a graph.A graph is an array of adjacency lists.Size of array will be V (number of vertices in graph) 
struct Graph 
{ 
    int V; 
    struct AdjList* array; 
    int* visited;
}; 

struct Graph* createGraph(int V) 
{ 
    struct Graph* graph =  (struct Graph*) malloc(sizeof(struct Graph)); 
    graph->V = V; 
    // Create an array of adjacency lists.  Size of array will be V 
    graph->array =  (struct AdjList*) malloc(V * sizeof(struct AdjList));
    //visited array Initialization for BFS
    graph->visited =(int *) malloc(V *sizeof(int));
    int i; 
    for (i = 0; i < V; ++i) 
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
    // Add an edge from src to dest.  A new node is  
    // added to the adjacency list of src.  The node 
    // is added at the beginning 
    struct AdjListNode* newNode = newAdjListNode(dest); 
    newNode->next = graph->array[src].head; 
    graph->array[src].head = newNode; 
  
} 
// A utility function to create a new content list node and its privacy flag
struct contents* addContentNode(int content,int p)
{
    struct contents *newNode=(struct contents* )malloc(sizeof(struct contents));
    newNode->c=content;
    newNode->p=p;
    newNode->next=NULL;
    return newNode;
}
// Adds an edge to an content list for the user i
void addContent(struct Graph* graph,int src, int content,int p)
{
    //every content node is added in the beginning of previous one
    struct contents *newNode=addContentNode(content,p);
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
 *                              BFS
 * 
 * **********************************************************************/
 //structure for queue
 struct queue {
  int items[SIZE];
  int front;
  int rear;
};
//creating queue
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
//to check if queue is empty or not
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
    printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        //printf("resetting queue");
      q->front = q->rear = -1;
    }
  }
  return item;
}
//a functin to find top k min distance content node with privacy view edge
void topKMinDist(struct Graph* graph, int startVertex,int k,int no_of_contents)
 {
  struct queue* q = createQueue();
  int content_count=0;
  //an array to store information if that content is already recommended or not
  int store_content[no_of_contents+1];
  for(int i=0;i<=no_of_contents;i++)
    store_content[i]=0;
  //array to keep track of level
  int level[graph->V];
  level[startVertex]=0;
  
  graph->visited[startVertex] = 1;
  
  enqueue(q, startVertex);
  
      while (!isEmpty(q))
      {
    
        int currentVertex = dequeue(q);
        //taking each adjacent vertex of current vertex one by one
        struct AdjListNode * temp = graph->array[currentVertex].head;

        while (temp) 
        {
          int adjVertex = temp->dest;
            
          if (graph->visited[adjVertex] == 0) 
          {
            graph->visited[adjVertex] = 1;
            level[adjVertex]=level[currentVertex]+1;
            struct contents *temp1=graph->array[adjVertex].content_arr;
            //temp1 is poiter to through each content
            while(1)
            {
                if( content_count<k &&  temp1 !=NULL)
                {
                    
                    int content=temp1->c;
                    //to match if the content is not its own content
                    int match=content_match(graph,startVertex,content);
                    if(match!=1)
                    {
                      if(store_content[content]==0)
                      {
                          store_content[content]=1;
                          //a switch over the privacy flags
                          switch(temp1->p)
                          {
                              case 0:
                                printf("\nContent %lld @ Distance 1 ",content);
                                break;
                              case 1:
                                if((level[adjVertex] - level[startVertex])==1)
                                    printf("\nContent %lld @ Distance 1 ",content);
                                else if(level[adjVertex] -level[currentVertex]==1)
                                      printf("\nContent %lld @ Distance %lld ",content,level[currentVertex]+1);
                                     else
                                      printf("\nContent %lld @ Distance %lld ",content,level[adjVertex]+1);
                                break;
                              case 2:
                                if((level[adjVertex] - level[startVertex])<=2)
                                    printf("\nContent %lld @ Distance 1 ",content);
                                else if(level[adjVertex] -level[currentVertex]==2)
                                      printf("\nContent %lld @ Distance %lld ",content,level[currentVertex]+1);
                                     else
                                      printf("\nContent %lld @ Distance %lld ",content,level[adjVertex]+1);
                                break;
                          }
                          content_count++;
                          temp1= temp1->next;
                      }
                      else
                        temp1=temp1->next;
                    }
                    else
                      temp1= temp1->next;
                }
                else
                    break;
                    
            }
            //if k number of contents printed then return
            if(content_count==k)
                return;

            enqueue(q, adjVertex);
          }
          temp = temp->next;
          
        }
      }
  
}
 
 /***********************************************************************
 *                              
 *                          MAIN FUNCTION
 * 
 * 
 * **********************************************************************/
int main (int argc, char **argv)
{

  int i = 0,n=0,p=0;
  int u,k;
  char ch, txt_file[100000];
  FILE *file;
  char  *filename;
  if(argc<2)
{
    printf("Missing filename\n");
    return(1);
}
else
{
    filename=argv[1];
    u=atoi(argv[2]);
    k=atoi(argv[3]);
}
//opening the file a2_task3.txt
  file = fopen (filename, "r");

//copying file contents into character array
  while ((ch = fgetc (file)) != EOF)
    txt_file[i++] = ch;
  txt_file[i] = '\0';
  
i=0;  
int no_of_user=0;
char nouser[5];
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
//for graph creation taking every vertex
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
char nocontent[5];
x=0;
while(txt_file[i]!='\n')
{
 nocontent[x++]=txt_file[i++];
}
nocontent[x]='\0';
no_of_contents=atoi(nocontent);
i+=2;
//for content nodes creation taking every content for every user
for(user_count=0;user_count<no_of_user;user_count++)
{
    while(txt_file[i]!=':')
      i++;
    i++;
    int c=0,content_per_user=0;
    char new_line[100];
    for(;txt_file[i]!='\n';)
        new_line[c++]=txt_file[i++];
    new_line[c]='\0';
    c=0;
    while(new_line[c])
    {
        if(new_line[c++]==';')
        content_per_user++;
    }
    c=0;
      for(int k=0;k<content_per_user;k++)
      {
          while(new_line[c]!=',')
            n=n*10+new_line[c++]-'0';
          while(new_line[c]!=';')
            p=new_line[c++]-'0';
        addContent(graph,user_count,n,p);
        c++;
        n=0;
        p=0;
      }
      
    i++;
    
}

if(k>no_of_contents  || u>no_of_user)
{
    printf("These Many Contents OR This user Not Present");
    return 1;
}

topKMinDist(graph,u,k,no_of_contents);

fclose(file);
return 0;
}