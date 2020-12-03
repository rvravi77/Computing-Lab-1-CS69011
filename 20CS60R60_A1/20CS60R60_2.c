/******************************************************************************
Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 1

            Task 1 Algorithm 2 :

The input text in file should Look like this :

0:1,2,3,4,5,6,7,8,9,10
1:1,2,3,4,11,22,7,8,9,10 (after 10 there should be newline )

*******************************************************************************/


#include <stdio.h>

#define int unsigned long long 

//a prime number
int p = 107;

//Structture declaration for hashing
struct array{
    int arr[100000]; // an unordered array
    int x; 
    int L,R; //declare L and R which will store end and start index of repeated string
};
struct array a;

// A utility function to find if hash value is present (a match)
int find (int hash)
{
  for (int i = 0; i < sizeof(unsigned long long); i++)
    {
      if (hash == a.arr[i])
	return 1;
      else
	return 0;
    }
}

//Function to Find if there exist any Common substring

int ok (int k, int *S,int n)
{
  int hash = 0, mul = 1;
  a.x=0;
  // compute hash of first substring of length k 
   for (signed i = k-1; i >-1; --i)
    {
      hash = hash + (S[i]) * mul;
      mul = mul * p;
    }
  //mul=p^k
  
  //insert hash into unordered array
  a.arr[a.x++] = hash;

  //Consider all other substring of length k
  for (int i = k; i < n; i++)
    {
      hash *= p;
      hash -= (S[i - k]) * mul;
      hash += S[i];

      //if hash is already present in table , store start and end indices in L and R and retuen true
      if (find (hash))
	{
	  a.L = i - k + 1;
	  a.R = i;
	  a.x = 0;
	  return 1;
	} 
	//else add hash value to the table
      a.arr[a.x++] = hash;

    }
  return 0;
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
//To check if file in empty
int isempty;
if (file!=NULL) {
    fseek (file, 0, SEEK_END);
    isempty = ftell(file);

    if (isempty==0) {
        printf("file is empty\n");
        return 0;
    }
}
//opening the file task1_input.txt
file=fopen(filename,"r");

//copying file contents into character array
  while ((ch = fgetc (file)) != EOF)
    txt_file[i++] = ch;
  txt_file[i] = '\0';		//Set the terminating character

//Defining a  character array(path sequence) for each user
  char user0[1000], user1[1000];

  i = 2;			// Variable assignment for extracting only path sequence

//copying path sequence for user0
  while (txt_file[i] != '\n')
    {
      user0[u0] = txt_file[i];
      i++;
      u0++;
    }

user0[u0]=',';
u0++;
user0[u0]='\0';//terminating character
//copying path sequence for user1
  i += 3;
  while (txt_file[i] != '\n')
    {
      user1[u1] = txt_file[i];
      i++;
      u1++;
    }
 
user1[u1]=',';
u1++;
user1[u1]='\0';//terminating character

//Converting char array into Interger array 
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


//concatnating both array into one to boil down the problem into 
// longest repeating substring of array

 for(int i=count0,j=0;i<count0+count1;i++,j++)
    arr0[i]=arr1[j];
    
//binary search on length length l
  a.L = -1, a.R = -1;
  int low = 0, up = (count0+count1);
  int l=up;
 while (low <= up)
    {
      int mid = low +(up - low) / 2;
      if (ok (mid, arr0,l))
	    low = mid + 1;
      else
	    up = mid - 1;
    }
    
 // to print the common subpath
  if (a.L == -1)
    printf ("not found any string");
  else
    {
      for(int i=a.L;i<=a.R;i++)
        printf("%llu ",arr0[i]);
    }
    
//closing File
  fclose (file);
  return 0;
}
