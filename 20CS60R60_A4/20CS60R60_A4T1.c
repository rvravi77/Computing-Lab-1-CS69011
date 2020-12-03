/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 4

                 Task 1 :

The text in file should Look like this :  
4 3
8 4 8 4
4 8 4
1 100 100
100 1 100
100 100 1
100 1 100
 
Compiling Command : gcc -Wall 20CS60R60_A4T1.c -o main -lglpk -lm
Running command :./main input.txt

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>
#include <glpk.h>

int main(int argc, char **argv)
{ 
    
    
   char ch, txt_file[1000];
   int i=0;
   FILE *file;
   char  *filename;
   if(argc<2)
    {
    printf("Missing filename\n");
    return(1);
    }
    else
    filename=argv[1];
    //opening the file input.txt
   file = fopen (filename, "r");
   while ((ch = fgetc (file)) != EOF)
    txt_file[i++] = ch;
   txt_file[i] = '\0';
   i=0;
   
   int nm[2],mn=0;
   
   //space character defining for strtok
   const char s[15] = " "; 
   int num=0;
   char new_line[100];
   int d;
   
   // for getting n and m
   for(d=0;txt_file[i]!='\n';)
        new_line[d++]=txt_file[i++];
   new_line[d]='\0';
   
   char* tok= strtok(new_line, s);
        while (tok != 0)
        { 
            int j=0;
            while(tok[j]!='\0')
                num=num*10+(tok[j++]-'0');
            nm[mn++]=num;
            num=0;
            tok = strtok(0, s); 
        } 
  
   
    // storing n and m in row_count and col_count
    int row_count=nm[0]; 
    int col_count=nm[1];
    printf("n = %d , m = %d \n",row_count,col_count);
    
    //for storing supply array 
    double supply[row_count];
    mn=0;
    i++;
    for(d=0;txt_file[i]!='\n';)
        new_line[d++]=txt_file[i++];
    new_line[d]='\0';
    
    char *tok1= strtok(new_line, s);

     while (tok1 != 0)
        { 
            int j=0;
            while(tok1[j]!='\0')
                num=num*10+(tok1[j++]-'0');
            supply[mn++]=(double)num;
            num=0;
            tok1 = strtok(0, s); 
        } 
    printf("u vector = ");
    for(int i=0;i<row_count;i++)
        printf("%0.1lf ",supply[i]);
    printf("\n");
 
    //for storing demand array
    double demand[col_count];
    mn=0;
    i++;
    for(d=0;txt_file[i]!='\n';)
        new_line[d++]=txt_file[i++];
    new_line[d]='\0';
    
    char *tok2= strtok(new_line, s);

     while (tok2 != 0)
        { 
            int j=0;
            while(tok2[j]!='\0')
                num=num*10+(tok2[j++]-'0');
            demand[mn++]=(double)num;
            num=0;
            tok2 = strtok(0, s); 
        } 
    printf("d vector = ");
    for(int i=0;i<col_count;i++)
        printf("%0.1lf ",demand[i]);
    printf("\n");
    
    // for storing cost 2-d array
    double cost[row_count][col_count];
    
    for(int q=0;q<row_count;q++)
    {
    mn=0;
    i++;
    for(d=0;txt_file[i]!='\n' && txt_file[i]!='\0';)
        new_line[d++]=txt_file[i++];
    new_line[d]='\0';
    d=0;
    char *tok3= strtok(new_line, s);
      while (tok3 != 0)
        { 
            int j=0;
            while(tok3[j]!='\0')
                num=num*10+(tok3[j++]-'0');
            cost[q][mn++]=(double)num;
            num=0;
            tok3 = strtok(0, s); 
        }   
    }
    printf("-------c matrix------\n");
    for(int i=0;i<row_count;i++)
    {
        for(int j=0;j<col_count;j++)
        {
            printf("%0.1lf          ",cost[i][j]);
        }
    printf("\n");
    }
    /*
    
    
    ------------------------------LP FORMULATION-------------------------
    
    
    */
    // object in which we store our problem statement
	glp_prob *lp;
	lp = glp_create_prob();
	glp_set_prob_name(lp, "a4t1");

	// set objective function
	int coef=row_count*col_count;
	glp_set_obj_dir(lp, GLP_MIN);
	glp_add_cols(lp, coef);
	char name[10];
  

	for(int i=1;i<=row_count;i++){
	    for(int j=1;j<=col_count;j++){
	        
		sprintf(name, "x_%d%d",i,j);
		//printf(" - -- - %s\n",name);
		glp_set_col_name(lp, (col_count*(i-1))+j,name);
	    glp_set_col_bnds(lp, (col_count*(i-1))+j, GLP_DB, 0.0, 1.0);
	    
        // setting column coef as cost_ij * demand_j
		double obj_coef;
		obj_coef=cost[i-1][j-1]*demand[j-1];
	    glp_set_obj_coef(lp, (col_count*(i-1))+j , obj_coef);
	                        }
                    	}

	// set row bounds
	char row_name[10];
	int j;
	// adding number of rows as row_count +col_count
	glp_add_rows(lp,row_count+col_count);
	
	for(j=0;j<row_count;j++)
        {
    	sprintf(row_name,"row%d",j+1);
    	glp_set_row_name(lp, j+1, row_name);
    	// row bounds to ensure  not more than u_i soldiers are supplied from location i
    	glp_set_row_bnds(lp, j+1, GLP_UP, 0.0, supply[j]);
         }
        j=row_count;
     for(int k=0;k<col_count;k++)
        {
	   sprintf(row_name,"row%d",j+1);
	   glp_set_row_name(lp, j+1, row_name);
	  // row bounds to ensure atleast d_j soldiers are supplied to battleground ??
	   glp_set_row_bnds(lp, j+1, GLP_LO, 1.0,0.0);
	   j++;
       }
	
       
       
	// set row coefficients

	int ia[1+1000], ja[1+1000];
	double ar[1+1000];

    int e=0,f=0,g=0; 
    int a,b;
    
    // row coefficients for d_j * x_ij <= u_i , for all i
	for(a=1;a<=row_count;a++){

	double temp_mat[(row_count*col_count)+1];
	    for(int i=1;i<=row_count;i++)
	    {
	        for(int j=1;j<=col_count;j++)
	        {
	            if(i==a)
	                temp_mat[col_count*(i-1)+j]=demand[j-1];
	           else
	                temp_mat[col_count*(i-1)+j]=0.0;
	        }
	    }
	    for(int s=1;s<=(row_count*col_count);s++)
              {
	          ia[++e] = a;
		  ja[++f] = s;
		  ar[++g] = temp_mat[s];  
                }
	  }
	
	//row coefficients  for x_ij >= 1 , for all j 
	for(b=1;b<=col_count;b++)
	{
	    double temp_mat[(row_count*col_count)+1];
	  
	    for(int i=1;i<=row_count;i++)
	    {
	        for(int j=1;j<=col_count;j++)
	        {
	            if(j==b)
	                temp_mat[col_count*(i-1)+j]=1.0;
	           else
	                temp_mat[col_count*(i-1)+j]=0.0;
	        }
	    }
	    for(int s=1;s<=(row_count*col_count);s++)
              {
	          ia[++e] = a;
		  ja[++f] = s;
		  ar[++g] = temp_mat[s];  
                }
	    a++;
	}

    // loading the matrix ia, ja , ar
	glp_load_matrix(lp, g, ia, ja, ar);

	// solves the lp problem and gives a frational result 
	glp_simplex(lp, NULL);

	double z, sol[coef+1] ;
	//for getting linear programming solution optimal value
	z = glp_get_obj_val(lp);
	
	printf("\noptimal value = %g\n",z);
	for(int i=0;i<coef;i++){
		 sol[i+1]= glp_get_col_prim (lp,i+1);
	}
	// printing the  optimal allocation of soldiers from unit location i to battleground j
	for(int i=1;i<=row_count;i++)
	{
	    {for(int j=1;j<=col_count;j++)
    		printf(" %g   ",sol[col_count*(i-1)+j] * demand[j-1]);}
    	printf("\n");
	} 

	glp_delete_prob(lp);
	return 0;  
}

