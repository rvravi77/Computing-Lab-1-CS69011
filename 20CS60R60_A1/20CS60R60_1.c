/******************************************************************************
Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 1

            Task 1 Algorithm 1 :

The text in file should Look like this :

0:1,2,3,4,5,6,7,8,9,10
1:1,2,3,4,11,22,7,8,9,10  // Note: Insert Newline after 10

*******************************************************************************/

#include <stdio.h> 
#include<stdlib.h>

//Function to find common path sequence 
void common_path( int *user0, int *user1, int count0, int count1 ) 
{
    
    int L[count0+1][count1+1]; 
    // To store length of the longest common substring 
    int len = 0; 
  
    // To store the index of the cell which contains the 
    // maximum value. This cell's index helps in building 
    // up the longest common substring from right to left. 
    int row, col;
  
   // Following steps build L[count0+1][count1+1] in bottom up fashion.  
       
   for (int i = 0; i <= count0; i++) { 
        for (int j = 0; j <= count1; j++) 
        { 
            if (i == 0 || j == 0)
                L[i][j] = 0; 
  
            else if (user0[i - 1] == user1[j - 1])
            { 
                L[i][j] = L[i - 1][j - 1] + 1; 
                if (len < L[i][j]) 
                { 
                    len = L[i][j]; 
                    row = i; 
                    col = j; 
                } 
            } 
            else
                L[i][j] = 0; 
        } 
    } 
  
   
    // if true, then no common substring exists 
    if (len == 0) { 
        printf( "No Common Substring" ); 
        return; 
    } 
  
    // allocate space for the longest common substring 
    int * resultLcs = (int*)malloc((len + 1) * sizeof(int)); 
   
    int x=0; //Variable to keep track of the size of LCS for printing
  
    // traverse up diagonally form the (row, col) cell 
    // until resultLcs[row][col] != 0 
    
    while (L[row][col] != 0) { 
        resultLcs[--len] = user0[row - 1]; // or user1[col-1] 
  
        // move diagonally up to previous cell 
        row--; 
        col--; 
        x++;
    } 
  
    // required longest common substring 
    for(int i=0;i<x;i++)
    {
        if(i+1==x )   //To eleminate comma at the end  - Just to give output a neat Look
          printf("%d",resultLcs[i]); 
        else
           printf("%d,",resultLcs[i]); 
    }
   

} 

/*         MAIN FUNCTION             */


int main(int argc, char **argv)
{

int i=0,u0=0,u1=0;
char ch,txt_file[100000]; 
FILE *file;
char *filename;

//check if file name is specified in command
if(argc<2)
{
    printf("Missing filename\n");
    return(1);
}
else
{
    filename=argv[1];
}

//opening the file task1_input.txt
file=fopen(filename,"r");


//copying file contents into character array
while ((ch=fgetc(file)) != EOF)  
    txt_file[i++]=ch;
txt_file[i]='\0';  //Set the terminating character

//Defining a  character array(path sequence) for each user
char user0[1000],user1[1000];

i=2; // Variable assignment for extracting only path sequence

//copying path sequence for user0
while(txt_file[i]!='\n')
    {
        user0[u0]=txt_file[i];
        i++;
        u0++;
    }
    //u0--;
user0[u0]=',';
u0++;
user0[u0]='\0';//terminating character

//copying path sequence for user1
i+=3;
while(txt_file[i]!='\n')
    {
        user1[u1]=txt_file[i];
        i++;
        u1++;
    }
  //u1--;
user1[u1]=',';
u1++;
user1[u1]='\0';//terminating character

//convert array of character to integer
int arr0[1000],arr1[1000],count0=0,count1=0,n=0;
  
  
   //for user0
   for(int i=0,j=0;user0[i];i++,j++)
    {
        if(user0[i]>='0' && user0[i]<='9')
        {
         while(user0[j]!=',')
            {
              n=n*10+(user0[j]-'0');
              j++; 
              i++;
            }
            arr0[count0]=n;
            count0++;
            n=0;
        }
    }

    
    n=0;
    
    //for user1
     for(int i=0,j=0;user1[i];i++,j++)
    {
        if(user1[i]>='0' && user1[i]<='9')
        {
         while(user1[j]!=',')
            {
              n=n*10+(user1[j]-'0');
              j++; 
              i++;
            }
            arr1[count1]=n;
            count1++;
            n=0;
        }
    }


//calling function for finding common path sequence
common_path(arr0, arr1, count0, count1); 

//closing File
fclose(file);
return 0; 
}