/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 4

                 Task 2 :

The text in file should Look like this :  
6 3 4
8 4 8 4 8 4
1 1 1 1 1000 1000
4 8 4
1   100 100
100 1   100
100 100 1
100 1   100
1   1   1
1   1   1
 
Compiling Command : gcc -Wall 20CS60R60_A4T2.c -o main -lglpk -lm
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
   
   int nm[3],mn=0;
   
   // for storing the value of n  , m and k
   const char s[15] = " "; 
   int num=0;
   char new_line[100];
   int d;
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
  
   
    
    int row_count=nm[0]; 
    int col_count=nm[1];
    int k_val=nm[2];
    printf("n = %d , m = %d  k = %d\n",row_count,col_count,k_val);
    
    // fro storing the supply array
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
 
 
 // for storing the fixed cost array
 
  double fixed_cost[row_count];
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
            fixed_cost[mn++]=(double)num;
            num=0;
            tok2 = strtok(0, s); 
        } 
    printf("f vector = ");
    for(int i=0;i<row_count;i++)
        printf("%0.1lf ",fixed_cost[i]);
    printf("\n");
 
 
 // for storing the demand array
 
    double demand[col_count];
    mn=0;
    i++;
    for(d=0;txt_file[i]!='\n';)
        new_line[d++]=txt_file[i++];
    new_line[d]='\0';
    
    char *tok3= strtok(new_line, s);

     while (tok3 != 0)
        { 
            int j=0;
            while(tok3[j]!='\0')
                num=num*10+(tok3[j++]-'0');
            demand[mn++]=(double)num;
            num=0;
            tok3 = strtok(0, s); 
        } 
    printf("d vector = ");
    for(int i=0;i<col_count;i++)
        printf("%0.1lf ",demand[i]);
    printf("\n");
        
    // for storing the cost array of spuulying one unit of soldier from unit i to battleground j
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
     
     
     --------------------------LP FORMULATION ----------------------------------------
     
     */
    
    // object in which we store our problem statement
	glp_prob *lp;
	lp = glp_create_prob();
	glp_set_prob_name(lp, "a4t1");

	// set objective function
	int coef=row_count*col_count;
	glp_set_obj_dir(lp, GLP_MIN);
	// adding number of columns as (n*m ) + n , where last added  n is integer variable 
	glp_add_cols(lp, (coef+row_count));
	char name[10];
  

	for(int i=1;i<=row_count;i++){
	    for(int j=1;j<=col_count;j++){
	        
		sprintf(name, "x_%d",(col_count*(i-1))+j);
		//printf(" - -- - %s\n",name);
		glp_set_col_name(lp, (col_count*(i-1))+j,name);
	    glp_set_col_bnds(lp, (col_count*(i-1))+j, GLP_DB, 0.0, 1.0);
        // setting coefficients of first (n*m) variable as cost_ij * demand_j
		double obj_coef;
		obj_coef=cost[i-1][j-1]*demand[j-1];
	    	glp_set_obj_coef(lp, (col_count*(i-1))+j,obj_coef);
	                        }
                    	}
    for(int i=1;i<=row_count;i++)
    {
       sprintf(name, "x_%d",coef+i);
		//printf(" - -- - %s\n",name);
		glp_set_col_name(lp, coef+i, name);
		glp_set_col_kind(lp, coef+i, GLP_BV);  // y_i is boolean variable
        // coefficient of y_i is fixed_cost for setting up the unit i
	    glp_set_obj_coef(lp, coef+i, fixed_cost[i-1]);
    }
    
    
	// set row bounds
	// total number of rows are n+m+1 
	glp_add_rows(lp, row_count+col_count+1);
	char row_name[10];
	int j;
	// setting row bounds for {  d_j* x_ij  <= u_i *y_i  } for all i
	for(j=0;j<row_count;j++)
        {
    	sprintf(row_name,"row%d",j+1);
    	glp_set_row_name(lp, j+1, row_name);
    	// took the upper bound as 0 for the simplified equation { d_j* x_ij - u_i *y_i } <=0
    	glp_set_row_bnds(lp, j+1, GLP_UP, 0.0, 0.0);
         }
        j=row_count;
    // setting row bounds for x_ij >=1
     for(int k=0;k<col_count;k++)
        {
	   sprintf(row_name,"row%d",j+1);
	   glp_set_row_name(lp, j+1, row_name);
	   glp_set_row_bnds(lp, j+1, GLP_LO, 1.0,0.0);
	   j++;
       }
       
     // setting row bounds for y_i <= k
    sprintf(row_name,"row%d", row_count+col_count+1);
    glp_set_row_name(lp, row_count+col_count+1, row_name);
	glp_set_row_bnds(lp, row_count+col_count+1, GLP_UP, 0.0,k_val);
	
	// set row coefficients

	int ia[1+1000], ja[1+1000];
	double ar[1+1000];

    int e=0,f=0,g=0; 
    int a,b;
    
    // row bounds for for d_j*x_ij - u_i *y_i <= 0
	for(a=1;a<=row_count;a++){

	double temp_mat[(row_count*col_count)+1];
	    for(int i=1;i<=row_count;i++)
	    {
	        for(int j=1;j<=col_count;j++)
	        {
	            if(i==a)
	                // for x_ij coefficients are d_j
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
        // for -u_i * y_i    
	    // for y_i coefficients is u_i
		    ia[++e] = a;
		    ja[++f] = coef+a;
		    int x; 
		    x=(int)supply[a-1];
		    x=(-1)*x;
		    double x1= (double)x;
		    ar[++g] = x1;
	            
	  }
   
   	// setting coefficients for x_ij >= 1
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

     // setting coefficients for  y_j <= k 
     for(int i=1;i<=row_count;i++){
	    for(int j=1;j<=col_count;j++)
	    {
		    ia[++e] = row_count+col_count+1;
		    ja[++f] = col_count*(i-1)+j;
		    ar[++g] = 0.0;
	    }
	}
	for(int i=1;i<=row_count;i++){
	    
		    ia[++e] = row_count+col_count+1;
		    ja[++f] = coef+i;
		    ar[++g] = 1.0;
	}

   // loading matrix for ia , ja, ar 
  	glp_load_matrix(lp, g, ia, ja, ar);

	// solves and gives a frational result ( variables are fraction )
	glp_simplex(lp, NULL);

    // call integer solver
	// interger optimizer -> again starts solving from where previously left
	// and converege to integer solution 
	
    glp_intopt(lp, NULL);
	double z, sol[coef+row_count] ;
	
	// mix integer programming solution
	z = glp_get_obj_val(lp);
	
	printf("\noptimal value = %g\n",z);
	for(int i=0;i<coef+row_count;i++){
		sol[i+1] = glp_mip_col_val(lp,i+1);
	}
	// printinf y_1  to y_n
    printf("\n Fcilities opened :");
    for(int i=coef;i<coef+row_count;i++)
       { printf(" %g     ",sol[i+1]);}
    
    printf("\n");
	
	printf("\nMatrix of optimal allocation of soldiers from unit location i to battleground j \n");
	for(int i=1;i<=row_count;i++)
	{
	    {for(int j=1;j<=col_count;j++)
    		printf(" %g   ",sol[col_count*(i-1)+j] * demand[j-1]);}
    	printf("\n");
	} 

	glp_delete_prob(lp);
	return 0;   
}

