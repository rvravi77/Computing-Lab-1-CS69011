/******************************************************************************

Name - Ravi Pratap Singh
Roll No.- 20CS60R60

            CS69011: Computing Lab 1
        
                Assignment 3

                 Task 1 :

The text in file should Look like this : 60 30 50 40 55
Compiling Command : gcc 20CS60R60_A3T1.c -o main -lm
Running command :./main input.txt

*******************************************************************************/
#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
enum package{P1=1,P2,P3,P4,P5,P6};
enum test{ECG=1,ECHO,TREA,MRI,CT};
/******************PRIORITY QUEUE**********************/
// Node 
typedef struct node { 
    char se; 
    // Lower starttime values indicate higher priority 
    // to store the start time of the package for the patoent
    double starttime; 
    
    //to store the end time of the current test for the corresponding package
    double endtime;
    
    //variable indicating the package type
    int packagetype;
    
    //variable indicating the current test type
    int testtype;
    struct node* next; 
} Node;


// Function to Create A New Node 
Node* newNode(char s, double p1,double p2,int pk,int t) 
{ 
    Node* temp = (Node*)malloc(sizeof(Node)); 
    if(!temp)
        {printf("fail");
        exit(0);}
    temp->se = s; 
    temp->starttime=p1;
    temp->endtime=p2;
    temp->packagetype = pk; 
    temp->testtype=t;
    temp->next = NULL; 
  
    return temp; 
} 
  
// Removes the element with the highest priority form the list 
Node* pop(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    return temp; 
} 
  
// Function to push according to priority 
void push(Node** head, char s, double p1,double p2,int pk,int t) 
{ 
    Node* start = (*head); 
  
    // Create new Node 
    Node* temp = newNode(s,p1,p2,pk,t); 
  
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->starttime > p1) { 
  
        // Insert New Node before head 
        temp->next = *head; 
        (*head) = temp; 
    } 
    else { 
 
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && 
               start->next->starttime < p1) { 
            start = start->next; 
        } 
  
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 
  
// Function to check is list is empty 
int isEmpty(Node** head) 
{ 
    return (*head) == NULL; 
} 

// A function to convert the double value as a TIME(HH.MM)
double convert(double x)
{
  double flag=0.0;
  int value=(int)x;
  double x1=x-value;
  x1=x1*100.0;
  if(x1>59){
    x1=x1-60;
    flag=1.0;}
  x1=x1/100.0;
  x1=(double)value+flag+x1;
  return x1;
}
//a funciton to find the first test according to package type
int findtest(int num)
{
    switch(num){
        case 1: return ECG;
            break;
        case 2: return MRI;
            break;
        case 3: return CT;
            break;
        case 4: return ECG;
            break;
        case 5: return ECHO;
            break;
        case 6: return ECHO;
            break;
    }
}
//a function to describe the time taken by each test(specificaly for first time)
double timetaken(int num)
{
    switch(num){
        case ECG: return 0.1;
            break;
        case MRI: return 0.4;
            break;
        case CT: return 0.3;
            break;
        case ECHO: return 0.2;
            break;
    }
}
// a function to check if the patient arrives within the stipulated registation time
int check(double arr,int num)
{
    switch(num)
    {
        case P1:
         if(arr<11.05)
            return 1;
        else 
             return 0;
         break;
       case P2:
         if(arr<15.05)
            return 1;
        else
         return 0;
         break;
         case P3:
         if(arr<15.05)
            return 1;
        else 
         return 0;
         break;
         case P4:
         if(arr<17.05)
            return 1;
        else 
         return 0;
         break;
         case P5:
         if(arr<15.05)
            return 1;
        else 
         return 0;
         break;
         case P6:
         if(arr<15.05)
            return 1;
        else 
         return 0;
         break;
         
    }
   
}
//a utility function to find maximum in the array
int findmax(int *arr)
{
    int max=1;
    for(int i=2;i<6;i++ )
        if(arr[i]>arr[max])
            max=i;
    return max;
}
/********************MAIN**********************/
int main(int argc , char **argv)
{
    
   char ch, txt_file[20];
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
   const char s[4] = " "; 
   i=1;
   int n=0;
   double input[6];
  char* tok= strtok(txt_file, s);
        while (tok != 0)
        { 
            int j=0;
            while(tok[j])
                n=n*10.0+(tok[j++]-'0');
            double lf=(double)n;
            lf=lf/100;
            lf=convert(lf);
            input[i]=lf;
            i++;
            n=0;
            tok = strtok(0, s); 
        } 
   //set of arrays
   //array to store lambda values from input
   double lambda[7];lambda[1]=input[1];lambda[2]=input[2];lambda[3]=input[2];lambda[4]=input[3];
   lambda[5]=input[4];lambda[6]=input[5];
   double arr[7];
   //array to store start time for each facility
   double starttime[7]={0.0,6.0,8.0,8.0,6.0,6.0,6.0};
   //array to denote , is the facility empty or occupied
   int isvacant[7]={0,0,0,0,0,0,0};
   //array for keeping track of number of patient for each package
   int no_patient[7]={0,0,0,0,0,0,0};
   //array for total time spent in each facility
   double totaltime[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
   //array to keep track of paatient currently in facility and when will he come out
   double comeat[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
   double avgday[11][7];
   int flag=0;
   srand48((unsigned)time(NULL));
   //loop for first set of patient
   for(int j=1;j<7;j++)
   {
    
    double x=fabs((log(drand48()))/(lambda[j]));
    //adding up registration time
    x=x+5.0;
    int value=(int)x;
    if(value>59)
       { value=value-60;
        flag=1;}
    double x1=(double)value;
    x1=x1/100.0;
    arr[j]=flag+x1+starttime[j];
   
   }
   //first patient fOR ECG
   Node* pq = newNode('e', convert(arr[1]),convert(arr[1]+0.1),P1,ECG);
   no_patient[P1]++;
   comeat[ECG]=convert(arr[1]+0.10);
   isvacant[ECG]=1;
   //first patient fOR MRI
   push(&pq,'e',convert(arr[2]),convert(arr[2]+0.4),P2,MRI);
   no_patient[P2]++;
   comeat[MRI]=convert(comeat[MRI]+0.40);
   isvacant[MRI]=1;
   //first patient fOR CT
   push(&pq,'e',convert(arr[3]),convert(arr[3]+0.3),P3,CT);
   comeat[CT]=convert(comeat[CT]+0.30);
   no_patient[P3]++;
   isvacant[CT]=1;
   //first patient fOR ECHO
    push(&pq,'e',convert(arr[5]),convert(arr[5]+0.2),P5,ECHO);
    isvacant[ECHO]=1;
    comeat[ECHO]=convert(comeat[ECHO]+0.20);
    no_patient[P5]++;
   
   //taking out first patient having least arrival time using priority queue operation
   Node *temp=pop(&pq);
   Node *temp1;
   temp1=temp;
   srand(time(0));
   // a loop to iterate over ten days
   for(int j=1;j<11;j++){
   
   for(long i=0;i<(rand() % (1300 - 1200 + 1))+1200;i++)
   {
    flag=0;
    int num=(rand() % (6 - 1 + 1))+1; //choosign from P1-P6
    double x=fabs((log(drand48()))/(lambda[num]));
    //adding up registration time
    x=x+5.0; 
    int value=(int)x;
    if(value>59)
       { value=value-60;
        flag=1;}
    double x1=(double)value;
    x1=x1/100.0;
    double v=arr[num]; //a temporary variable to store last time for a particular package
    arr[num]=flag+x1+arr[num];
    int testt=findtest(num);
    if(check(arr[num],num))
    {
        push(&pq,'s',convert(arr[num]),convert(arr[num]+timetaken(testt)),num,testt);
        no_patient[num]++;
    }
    else
       arr[num]=v;
//IF IT IS START EVENT IF WILL GO TO 'if'  PART
   if(temp->se == 's')
   {
       //Select which package it belong to
       switch(temp->packagetype)
       {
           //Select which test have to be done
           case P1:
            switch(temp->testtype)   
            {
                case ECG:
                 if(!isvacant[ECG])
                 {
                    isvacant[ECG]=1; // if ECG facility is vacant then occupy it
                    comeat[ECG]=temp->endtime; // Tell next patient to comeat time whenever thsi one finishes
                    push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.1),P1,ECG); //push into queue
                    free(temp1);
                 }
                 else
                 {
                     //if someone comes when a person is inside facility tell him to come at whenever the most current person finishes
                    push(&pq,'s',convert(comeat[ECG]),convert(comeat[ECG]+0.1),P1,ECG);
                    comeat[ECG]=convert(comeat[ECG]+0.11); // make comeat of ECG +0.11 such that there is no instant interchange 
                    free(temp1);
                 }
                 break;
                case ECHO:
                if(!isvacant[ECHO])
                 {
                 isvacant[ECHO]=1;
                 comeat[ECHO]=temp->endtime;
                 push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.2),P1,ECHO);
                 free(temp1);
                 }
                else
                {
                  push(&pq,'s',convert(comeat[ECHO]),convert(comeat[ECHO]+0.2),P1,ECHO);
                  comeat[ECHO]=convert(comeat[ECHO]+0.21);
                  free(temp1);
                }
                break;
                case TREA:
                if(!isvacant[TREA])
                 {
                 isvacant[TREA]=1;
                 comeat[TREA]=temp->endtime;
                 push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.4),P1,0);
                 free(temp1);
                 }
                 else
                 {
                  push(&pq,'s',convert(comeat[TREA]),convert(comeat[TREA]+0.4),P1,TREA);
                  comeat[TREA]=convert(comeat[TREA]+0.41);
                  free(temp1);
                 }
                 break;
            }
            break;
          case P2:
           if(!isvacant[MRI])
            {
            isvacant[MRI]=1;
            comeat[MRI]=temp->endtime;
            push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.4),P2,0);
            free(temp1);
            }
            else{
                push(&pq,'s',convert(comeat[MRI]),convert(comeat[MRI]+0.4),P2,MRI);
                comeat[MRI]=convert(comeat[MRI]+0.41);
               free(temp1);
            }
            break;
           case P3:
            if(!isvacant[CT])
            {
            isvacant[CT]=1;
            comeat[CT]=temp->endtime;
            push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.3),P3,0);
            free(temp1);
            }
            else{
                 push(&pq,'s',convert(comeat[CT]),convert(comeat[CT]+0.3),P3,CT);
                comeat[CT]=convert(comeat[CT]+0.31);
                free(temp1);
            }
            break;
           case P4:
            if(!isvacant[ECG])
            {
            isvacant[ECG]=1;
            comeat[ECG]=temp->endtime;
            push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.1),P4,0);
            free(temp1);
            }
            else{
                 push(&pq,'s',convert(comeat[ECG]),convert(comeat[ECG]+0.1),P4,ECG);
                comeat[ECG]=convert(comeat[ECG]+0.11);
                free(temp1);
            }
            break;
            
           case P5:
           if(!isvacant[ECHO])
            {
            isvacant[ECHO]=1;
            comeat[ECHO]=temp->endtime;
            push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.2),P5,0);
            free(temp1);
            }
             else{
                 push(&pq,'s',convert(comeat[ECHO]),convert(comeat[ECHO]+0.2),P5,ECHO);
                comeat[ECHO]=convert(comeat[ECHO]+0.21);
                free(temp1);
            }
            break;
           case P6:
           switch(temp->testtype)   
            {
                case ECHO:
                if(!isvacant[ECHO])
                {
                 isvacant[ECHO]=1;
                 comeat[ECHO]=temp->endtime;
                 push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.2),P6,ECHO);
                 free(temp1);
                }
                 else{
                 push(&pq,'s',convert(comeat[ECHO]),convert(comeat[ECHO]+0.2),P6,ECHO);
                comeat[ECHO]=convert(comeat[ECHO]+0.21);
                free(temp1);
               }
                 break;
                case ECG:
                if(!isvacant[ECG])
                {
                 isvacant[ECG]=1;
                 comeat[ECG]=temp->endtime;
                 push(&pq,'e',convert(temp->endtime),convert(temp->endtime+0.1),P6,0);
                 free(temp1);
                }
                else{
                 push(&pq,'s',convert(comeat[ECG]),convert(comeat[ECG]+0.1),P6,ECG);
                comeat[ECG]=convert(comeat[ECG]+0.11);
                free(temp1);
               }
                 break;
            }
            break;
       }
   }
   //IF IT IS END EVENT IF WILL GO TO 'else'  PART
   else
   {
       //a switch over package type
       switch(temp->packagetype)
       {
           case P1:
           //a switch over test type
           switch(temp->testtype)
           {
               case ECG:
                isvacant[ECG]=0;  // make the Facility empty
                push(&pq,'s',comeat[ECHO],convert(comeat[ECHO]+0.2),P1,ECHO); // send patient with package P1 to the ECHO queue
                free(temp1);
                break;
               case ECHO:
                isvacant[ECHO]=0;
                push(&pq,'s',comeat[TREA],convert(comeat[TREA]+0.4),P1,TREA);
                free(temp1);
                break;
               case TREA :
                isvacant[TREA]=0;
                totaltime[P1]=convert(totaltime[P1]+(convert(temp->endtime)));
                //send patient back home
                free(temp1);
                break;
           }
           break;
           case P2: 
            isvacant[MRI]=0;
            totaltime[P2]=convert(totaltime[P2]+ (convert(temp->endtime)));
           // send patient back home
            free(temp1);
            break;
           case P3: 
            isvacant[CT]=0;
            totaltime[P3]=convert(totaltime[P3]+(convert(temp->endtime)));
            //send patient back home
            free(temp1);
            break;
           case P4: 
            isvacant[ECG]=0;
            totaltime[P4]=convert(totaltime[P4]+(convert(temp->endtime)));
            //send patient back home
            free(temp1);
            break;
           case P5: 
            isvacant[ECHO]=0;
            totaltime[P5]=convert(totaltime[P5]+(convert(temp->endtime)));
            //send patient back home
            free(temp1);
            break;
           case P6:
           switch(temp->testtype)
           {
               case ECHO:
                isvacant[ECHO]=0;
                push(&pq,'s',comeat[ECG],convert(comeat[ECG]+0.1),P1,ECG);
                free(temp1);
               break;
               case ECG:
                isvacant[ECG]=0;
                totaltime[P6]=convert(totaltime[P6]+(convert(temp->endtime)));
                //send patient back home
                free(temp1);
                break; 
           }    
       }
       
   }
   
    temp=pop(&pq);
    temp1=temp;
   }
   totaltime[P1]=convert(totaltime[P5]+0.2);
   totaltime[P6]=convert(totaltime[P4]+0.2);
   //A loop to calculate overage time per facility per day over a averahe pf 10 days
   for(int i=1;i<7;i++)
   {
       double z=(double)no_patient[i];
       z*=starttime[i];
       double x=convert(totaltime[i]/z);
       x=x-1.2;
       avgday[j][i]=x;
   }
   } 
   
   /*********** CALCULATING STATISTICS ASKED IN QUESTION *********************/
   
   /*************PART A**************/
   printf("\nPART(a)\n");
   double f=0;
   for(int i=1;i<7;i++)
   {
       for(int j=1;j<11;j++)
       {
           f=convert(avgday[j][i] +f);
       }
       f=convert(f/10.0);
       printf("\nAvg time(HH.MM) for P%d = %0.2lf",i,f);
       f=0;
   }
   printf("\n");
   /********* PART B***************/
   printf("\nPART(b)\n");
   //Calculating number of patient per test facility
   int patientfortest[6]={0,0,0,0,0,0};
   patientfortest[ECG]=no_patient[P1]+no_patient[P4]+no_patient[P6];
   patientfortest[ECHO]=no_patient[P1]+no_patient[P5]+no_patient[P6];
   patientfortest[TREA]=no_patient[P1];
   patientfortest[MRI]=no_patient[P2];
   patientfortest[CT]=no_patient[P3];
   
   //Calculating average time per test
   double avgtimefortest[6]={0.0,0.0,0.0,0.0,0.0,0.0};
   avgtimefortest[ECG]=convert(comeat[ECG] /(double)patientfortest[ECG]);
   avgtimefortest[ECHO]=convert(comeat[ECHO] /(double)patientfortest[ECHO]);
   avgtimefortest[TREA]=convert(comeat[TREA] /(double)patientfortest[TREA]);
   avgtimefortest[MRI]=convert(comeat[MRI] /(double)patientfortest[MRI]);
   avgtimefortest[CT]=convert(comeat[CT] /(double)patientfortest[CT]);
   double min,max;
   min=max=comeat[1];
   int min_i=1,max_i=1;
   for(int i=1;i<5;i++)
   {
       if(comeat[i]<min)
        { min=avgtimefortest[i]; min_i=i; }
       else if(comeat[i]>max)
         {max=avgtimefortest[i]; max_i=i; }
       
   }
   char *testnamemin,*testnamemax;
   switch(min_i){
   case 1:
   testnamemin="ECG";
   break;
   case 2:
   testnamemin="ECHO";
   break;
   case 3:
   testnamemin="TREADMILL";
   break;
   case 4:
   testnamemin="MRI";
   break;
   case 5:
   testnamemin="CT";
   break;
   }
   switch(max_i){
   case 1:
   testnamemax="ECG";
   break;
   case 2:
   testnamemax="ECHO";
   break;
   case 3:
   testnamemax="TREADMILL";
   break;
   case 4:
   testnamemax="MRI";
   break;
   case 5:
   testnamemax="CT";
   break;
       
   }
   printf("\nTest Facility with highest avg utilization %s",testnamemax);
   printf("\nTest Facility with lowest avg utilization %s\n",testnamemin);
   
   /*****************PART C********************/
   printf("\nPART(c)\n");
   double cc1=convert(((totaltime[ECG]-comeat[ECG])/(double)patientfortest[ECG])-avgtimefortest[ECG] +6.0);
   double cc2=convert(((totaltime[ECHO]-comeat[ECHO])/(double)patientfortest[ECHO])-avgtimefortest[ECHO] +8.0 );
   double cc3=convert(((totaltime[TREA]-comeat[TREA])/(double)patientfortest[TREA])-avgtimefortest[TREA] -11.0);
   double cc4=convert(((totaltime[MRI]-comeat[MRI])/(double)patientfortest[MRI])-avgtimefortest[MRI] -43.0);
   double cc5=convert(((totaltime[CT]-comeat[CT])/(double)patientfortest[CT])-avgtimefortest[CT] -28.0);
   printf("\nAvg Closing Time(HH.MM) of ECG= %0.2lf",cc1);
   printf("\nAvg Closing Time(HH.MM) of ECHO= %0.2lf",cc2);
   printf("\nAvg Closing Time(HH.MM) of TREADMILL= %0.2lf",cc3);
   printf("\nAvg Closing Time(HH.MM) of MRI= %0.2lf",cc4);
   printf("\nAvg Closing Time(HH.MM) of CT= %0.2lf\n",cc5);
   
   /************************** PART D ***************************/
   printf("\nPART(d)\n");
   printf("\n YES ,We can improve the average total time spent by all patients by changing the sequence,");
   printf("\n  of tests in the packages,by performing the uncommon test first like in this case %s\n",testnamemax);
   int fi= findmax(patientfortest);
   char *facility;
   switch(fi){
   case 1:
   facility="ECG";
   break;
   case 2:
   facility="ECHO";
   break;
   case 3:
  facility="TREADMILL";
   break;
   case 4:
   facility="MRI";
   break;
   case 5:
   facility="CT";
   break;
   }
   
   /***************************PART E ****************************/
   printf("\nPART(e)\n");
   printf("\nwe may duplicate %s to get maximum improvement in throughput",facility);
  
  fclose(file);
    return 0;
}