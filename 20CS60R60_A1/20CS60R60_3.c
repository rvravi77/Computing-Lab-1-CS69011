/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 1

                  Task 2 :

The input text in file should Look like this :

0:1,2,3,4,5,6,7,8,9,10,
1:1,2,3,4,11,22,7,8,9,10, 


*******************************************************************************/
#include <stdio.h>
#define int unsigned long  long

 //a prime number
int p = 107;

//Structture declaration for hashing
struct array{
    int arr[100000]; // an unordered array
    int x; 
    int L,R; //declare L and R which will store end and start index of repeated string
};
struct array a;

// Function to find if hash value existed in hash table
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
int ok (int k, int *S,int n,int lines)
{
  int hash = 0, mul = 1,match=1;
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

      //if hash is already present in set , store start and end indices in L and R and retuen true
      if (find (hash))
	   {
	      match++;
	      if(match==lines) // if we find match in all the path sequence
	        {
                a.L = i - k + 1;
	            a.R = i;
	            a.x = 0;
	            return 1;
	         }
    	}
     a.arr[a.x++] = hash;
    }
  
  return 0;
}

/*         MAIN FUNCTION             */

int main (int argc, char **argv)
{

  int i = 0;
  char ch, txt_file[100000];
  FILE *file;
  char *filename;
  
  if(argc<2)
  {
      printf("missing filename\n");
      return(1);
  }
  else
  {
      filename=argv[1];
  }

//opening the file task1_input.txt
  file = fopen (filename, "r");

//copying file contents into character array
  while ((ch = fgetc (file)) != EOF)
    txt_file[i++] = ch;
  txt_file[i] = '\0';		//Set the terminating character

//Defining a  array(path sequence) to consolidate all path into one
  int all_path[100000],count=0,n=0;

//Finding total number of paths i.e no of lines in file
i=0;
int lines=0;
while(txt_file[i]!='\0')
    {
      if(txt_file[i]=='\n')  
        lines++;
      i++;
    }
i=2;
//loop for joining all path sequences into one
while(txt_file[i]!='\0')
{
   for(int j=i,k=i;txt_file[j]!='\n';j++,k++,i++) 
    {
        if(txt_file[j]>='0' && txt_file[j]<='9')
        {
         while(txt_file[k]!=',')
            {
              n=n*10+(txt_file[k]-'0');
              i++;
              j++; 
              k++;
            }
    
            all_path[count]=n;
            count++;
            n=0;
        }
    }
    i+=2;
}

    
//binary search on length length l
  a.L = -1, a.R = -1;
  int low = 0, up = count;
 while (low <= up)
    {
      int mid = low +(up - low) / 2;
      if (ok (mid, all_path,count,lines))
	    low = mid + 1;
      else
	    up = mid - 1;
    }
    

  if (a.L == -1)
    printf ("not found any string\n");
  else
    {
      for(int i=a.L;i<=a.R;i++)
      printf("%llu, ",all_path[i]);
    }
    printf("\n");
    
//closing File
  fclose (file);
  return 0;
}
