#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<errno.h>
#include <sys/sem.h>/*  This file is necessary for using shared
			    memory constructs
			*/
			
#define P(s) semop(s, &pop, 1)  /* pop is the structure we pass for doing
				   the P(s) operation */
#define V(s) semop(s, &vop, 1)  /* vop is the structure we pass for doing
				   the V(s) operation */
				   
struct sembuf pop,vop;
int get_read_lock(int trainid,int *ss ) // The calling process blocks until it gets a read lock on the given train
{
   switch(trainid)
   {
    case 0:
        P(ss[1]); //if no process have write lock
        P(ss[0]); //get read lock 
        V(ss[1]); // release write lock
        return 1;
    case 1:
        P(ss[3]);
        P(ss[2]);
        V(ss[3]);
        return 1;
    case 2:
        P(ss[5]);
        P(ss[4]);
        V(ss[5]);
        return 1;
   }
}

int get_write_lock(int trainid,int *ss ) // The calling process blocks until it gets a write lock on the given train
{
 switch(trainid)
 {
  case 0:
     // if no process has read lock 
    for(int i=0;i<4;i++){P(ss[0]); V(ss[0]);}
    P(ss[1]); //get write lock 
    //for(int i=0;i<4;i++){V(ss[0]);} //release read lock
    return 1;
  case 1:
     // if no process has read lock 
    for(int i=0;i<4;i++){P(ss[2]); V(ss[2]);}
    P(ss[3]);
   // for(int i=0;i<4;i++){V(ss[2]);}
    return 1;
  case 2:
    // if no process has read lock
    for(int i=0;i<3;i++){P(ss[4]); V(ss[4]);}
    P(ss[5]); 
    //for(int i=0;i<4;i++){V(ss[4]);}
    return 1;
 }
}
int release_read_lock( int trainid,int *ss )
{
    switch(trainid)
    {
       case 0:
            V(ss[0]);
            return 1;
        case 1:
            V(ss[2]);
            return 1;
        case 2:
            V(ss[4]);
            return 1;
    }
   
}
int release_write_lock(int trainid,int *ss )
{
    switch(trainid)
    {
        case 0:
            V(ss[1]);
            return 1;
        case 1:
            V(ss[3]);
            return 1; 
        case 2:
            V(ss[5]);
            return 1;
    }
}

struct train{
 int train_id;
 int AC2, AC3, SC; // No. of available berths
 struct reservation *rlist;
};

struct reservation{
 int pnr;
char passenger_name[20], age[3], sex[2];
char class[4]; // AC2, AC3, or SC
int status; // waitlisted or confirmed
};


struct train * init_val(struct train * x)
{
   for(int i=0;i<3;i++)
    {
        x[i].train_id=i;
        x[i].AC2 = x[i].AC3 =x[i].SC =10;
        x[i].rlist=(struct reservation *)malloc(1000 *sizeof(struct reservation));
    }
  return x;
}
struct reservation * add_passenger(struct train *d,int train_id,char *name,char *age,char *sex,char *class,int idx)
{
   
    d[train_id].rlist[idx].pnr= (idx*10) + train_id;
	printf("Passenger %s is alloted  pnr:%d\n",name,d[train_id].rlist[idx].pnr);
    strcpy(d[train_id].rlist[idx].passenger_name,name);
    strcpy(d[train_id].rlist[idx].age,age);
    strcpy(d[train_id].rlist[idx].sex,sex);
    strcpy(d[train_id].rlist[idx].class,class);
    
    if(idx < 10)
        d[train_id].rlist[idx].status=1; //confirmed=1
    else
         d[train_id].rlist[idx].status=0; //waitlisted=0
  
    return d[train_id].rlist;
}
  
int main(int argc , char **argv)
{
   char *filename1,*filename2,*filename3,*filename4;
   if(argc<2)
    return 0;
   else{
       filename1=argv[1];
       filename2=argv[2];
       filename3=argv[3];
       filename4=argv[4];
   }
   
   int  shmid;
   struct train *a, *b,*c,*d,*i;
   struct train *rail_data;
   int size=sizeof(rail_data) *3;
	//shared memory
   shmid = shmget(IPC_PRIVATE, size, 0777|IPC_CREAT);
			/* We request an array of 3 struct train */
 	//initializing memory for every train
   i = (struct train *) shmat(shmid, 0, 0);//attach
   i=init_val(i);

   //shared memory for semaphore
   int  shmid1,*ss,*ss1,*ss2,*ss3,*ss4;

   shmid1 = shmget(IPC_PRIVATE, 6*sizeof(int), 0777|IPC_CREAT);
   ss = (int *) shmat(shmid1, 0, 0);//attach
	
   ss[0] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT); //read
   ss[1] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);  //write

   ss[2] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT); //read
   ss[3] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);  //write

   ss[4] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT); //read
   ss[5] = semget(IPC_PRIVATE, 1, 0777|IPC_CREAT);  //write

	/* The following system calls sets the values of the semaphores
	   semid1 and semid2 to  0 and 1 respectively. */
  semctl(ss[0], 0, SETVAL, 4);
  semctl(ss[1], 0, SETVAL, 1);

  semctl(ss[2], 0, SETVAL, 4);
  semctl(ss[3], 0, SETVAL, 1);

  semctl(ss[4], 0, SETVAL, 4);
  semctl(ss[5], 0, SETVAL, 1);

  pop.sem_num = vop.sem_num = 0;
  pop.sem_flg = vop.sem_flg = 0;
  pop.sem_op = -1 ; vop.sem_op = 1 ;

  //shared memory for index of array
  int  shmid2;
  shmid2 = shmget(IPC_PRIVATE, 3*sizeof(int), 0777|IPC_CREAT);
  int *idx,*idx1,*idx2,*idx3,*idx4;
  idx=(int *) shmat(shmid2, 0, 0);
  idx[0]=0;idx[1]=0;idx[2]=0;
  int id1=fork();
  int id2=fork();
  int id3=fork();
    if(id1==0)
    {
        if(id2==0)
        {
            if(id3==0)
             {
            d = (struct train *) shmat(shmid, 0, 0);
	        idx1=(int *) shmat(shmid2, 0, 0);
	        ss1 = (int *) shmat(shmid1, 0, 0);
	        char buffer[50],buffer2[50],action[10],fname[10],lname[10],class[4],age[3],sex[2],train_id[2],pnr[10];
	        FILE *fp;
	        fp=fopen(filename1,"r");
	        printf("Opened File %s \n",filename1);
	        int ch=0;
	        char *token;
	        while( EOF != fscanf(fp, "%[^\n]\n", buffer))
	        {
	         strcpy(buffer2,buffer);
             token=strtok(buffer," ");
             while( token != NULL )
              {
                ch++;
                token=strtok(NULL," ");
              }
	         if(ch==7)
	            {    
	                if(get_read_lock(atoi(train_id),ss1))
	                {   //printf("got read lock in in1\n");
	                    release_read_lock(atoi(train_id),ss1);
	                   // printf("release read lock in in1\n");
	                    sscanf(buffer2,"%s %s %s %s %s %s %s",action,fname,lname,age,sex,train_id,class);
	                    strcat(fname,lname);
	                    if(get_write_lock(atoi(train_id),ss1))
	                      {   
	                         // printf("got write lock in in1\n");
	                          d[atoi(train_id)].rlist=add_passenger(d,atoi(train_id),fname,age,sex,class,idx1[atoi(train_id)]);
	                          idx1[atoi(train_id)]++;
	                      }
	                    
	                    printf("---Reservation Confirmed of %s---\n",fname);
	                    action[0]='\0'; fname[0]='\0'; lname[0]='\0'; age[0]='\0'; sex[0]='\0'; train_id[0]='\0'; class[0]='\0';
	                    release_write_lock(atoi(train_id),ss1);
	                    sleep(2);
	                }
	                
	            }
	            //if someone cancels reservation
	         if(ch==2)
	            {
	               sscanf(buffer2,"%s %s",action,pnr);
	               int tid,pos;
	               tid=atoi(pnr)%10;
	               pos= atoi(pnr)/10;
	              
	               get_read_lock(tid,ss1);
	              // printf("got read lock in in1  for cancel\n");
	               release_read_lock(tid,ss1);
	              // printf("release read lock in in1  for cancel\n");
	               sleep(1);
	               get_write_lock(tid,ss1);
	               //printf("got write lock in in1  for cancel\n");
	               d[tid].rlist[pos].status=2; //reservation cancelled
	               printf("***Cancellation Confirmed of pnr %s***\n",pnr);
	               sleep(2);
	               for(int i=0;i<idx1[tid];i++)
                    {
	                   if(d[tid].rlist[i].status == 0)
	                    {
	                     d[tid].rlist[i].status=1;
	                     //printf(" Passenger %s from Waitlist to confirmed\n",d[tid].rlist[i].passenger_name);
	                     break;
	                    }
                    }
	               action[0]='\0'; pnr[0]='\0';
	               release_write_lock(tid,ss1);
	           }
	        ch=0;
	        buffer[0]='\0'; buffer2[0]='\0'; token=NULL;
	        }
	       fclose(fp);
	       
             }
             else
             {
                
	        b = (struct train *) shmat(shmid, 0, 0);
	        idx2=(int *) shmat(shmid2, 0, 0);
	        ss2 = (int *) shmat(shmid1, 0, 0);
	        char buffer[50],buffer2[50],action[10],fname[10],lname[10],class[4],age[3],sex[2],train_id[2],pnr[10];
	        FILE *fp;
	        fp=fopen(filename2,"r");
	        printf("Opened File %s \n",filename2);
	        int ch=0;
	        char *token;
	        while( EOF != fscanf(fp, "%[^\n]\n", buffer))
	        {
	         strcpy(buffer2,buffer);
             token=strtok(buffer," ");
             while( token != NULL )
              {
                ch++;
                token=strtok(NULL," ");
              }
	         if(ch==7)
	            {    
	                if(get_read_lock(atoi(train_id),ss2))
	                {   //printf("got read lock in in2\n");
	                    release_read_lock(atoi(train_id),ss2);
	                    //printf("release read lock in in2\n");
	                    sscanf(buffer2,"%s %s %s %s %s %s %s",action,fname,lname,age,sex,train_id,class);
	                    strcat(fname,lname);
	                    if(get_write_lock(atoi(train_id),ss2))
	                      {   
	                         // printf("got write lock in in2\n");
	                          b[atoi(train_id)].rlist=add_passenger(b,atoi(train_id),fname,age,sex,class,idx2[atoi(train_id)]);
	                          idx2[atoi(train_id)]++;
	                         
	                      }
	                    printf("---Reservation Confirmed of %s---\n",fname);
	                    action[0]='\0'; fname[0]='\0'; lname[0]='\0'; age[0]='\0'; sex[0]='\0'; train_id[0]='\0'; class[0]='\0';
	                    release_write_lock(atoi(train_id),ss2);
	                    sleep(2);
	                }
	                
	            }
	            //if someone cancels reservation
	         if(ch==2)
	            {
	               sscanf(buffer2,"%s %s",action,pnr);
	               int tid,pos;
	               tid=atoi(pnr)%10;
	               pos= atoi(pnr)/10;
	              
	               get_read_lock(tid,ss2);
	               release_read_lock(tid,ss2);
	               sleep(1);
	               get_write_lock(tid,ss2);
	               b[tid].rlist[pos].status=2; //reservation cancelled
	               printf("***Cancellation Confirmed of pnr %s***\n",pnr);
	               sleep(1);
	               for(int i=0;i<idx2[tid];i++)
                    {
	                   if(b[tid].rlist[i].status ==0 )
	                    {
	                     b[tid].rlist[i].status=1;
	                    // printf("Moved Passenger %s from Waitlist to confirmed\n",b[tid].rlist[i].passenger_name);
	                     break;
	                    }
                    }
	               action[0]='\0'; pnr[0]='\0';
	               release_write_lock(tid,ss2);
	           }
	        ch=0;
	        buffer[0]='\0'; buffer2[0]='\0'; token=NULL;
	        }
	         
	        fclose(fp);
	        
	    }
             
        }
        else
        {
            if(id3==0)
            {
                
	        c = (struct train *) shmat(shmid, 0, 0);
	        idx3=(int *) shmat(shmid2, 0, 0);
	         ss3 = (int *) shmat(shmid1, 0, 0);
	        char buffer[50],buffer2[50],action[10],fname[10],lname[10],class[4],age[3],sex[2],train_id[2],pnr[10];
	        FILE *fp;
	        
	        fp=fopen(filename3,"r");
	        printf("Opened File %s \n",filename3);
	        int ch=0;
	        char *token;
	        while( EOF != fscanf(fp, "%[^\n]\n", buffer))
	         {
	         strcpy(buffer2,buffer);
             token=strtok(buffer," ");
             while( token != NULL )
              {
                ch++;
                token=strtok(NULL," ");
              }
	         if(ch==7)
	            {    
	                if(get_read_lock(atoi(train_id),ss3))
	                {   //printf("got read lock in in3\n");
	                    release_read_lock(atoi(train_id),ss3);
	                    //printf("release read lock in in3\n");
	                    sscanf(buffer2,"%s %s %s %s %s %s %s",action,fname,lname,age,sex,train_id,class);
	                    strcat(fname,lname);
	                    if(get_write_lock(atoi(train_id),ss3))
	                      {   
	                         // printf("got write lock in in3\n");
	                          c[atoi(train_id)].rlist=add_passenger(c,atoi(train_id),fname,age,sex,class,idx3[atoi(train_id)]);
	                          idx3[atoi(train_id)]++;
	                         
	                      }
	                    printf("---Reservation Confirmed of %s---\n",fname);
	                    action[0]='\0'; fname[0]='\0'; lname[0]='\0'; age[0]='\0'; sex[0]='\0'; train_id[0]='\0'; class[0]='\0';
	                    release_write_lock(atoi(train_id),ss3);
	                     sleep(2);
	                }
	                
	            }
	            //if someone cancels reservation
	         if(ch==2)
	            {
	               sscanf(buffer2,"%s %s",action,pnr);
	               int tid,pos;
	               tid=atoi(pnr)%10;
	               pos= atoi(pnr)/10;
	              
	               get_read_lock(tid,ss3);
	               release_read_lock(tid,ss3);
	               sleep(1);
	               get_write_lock(tid,ss3);
	               c[tid].rlist[pos].status=2; //reservation cancelled
	               printf("***Cancellation Confirmed of pnr %s***\n",pnr);
	               sleep(2);
	               for(int i=0;i<idx3[tid];i++)
                    {
	                   if(c[tid].rlist[i].status == 0 )
	                    {
	                     c[tid].rlist[i].status=1;
	                     //printf("Moved Passenger %s from Waitlist to confirmed\n",c[tid].rlist[i].passenger_name);
	                     break;
	                    }
                    }
	               action[0]='\0'; pnr[0]='\0';
	               release_write_lock(tid,ss3);
	           }
	        ch=0;
	        buffer[0]='\0'; buffer2[0]='\0'; token=NULL;
	        }
	        fclose(fp);
	        
            }
            else
            {
                 a = (struct train *) shmat(shmid, 0, 0);
	         idx4=(int *) shmat(shmid2, 0, 0);
	          ss4 = (int *) shmat(shmid1, 0, 0);
	        char buffer[50],buffer2[50],action[10],fname[10],lname[10],class[4],age[3],sex[2],train_id[2],pnr[10];
	        FILE *fp;
	        
	        fp=fopen(filename4,"r");
	        printf("Opened File %s \n",filename4);
	        int ch=0;
	        char *token;
	        while( EOF != fscanf(fp, "%[^\n]\n", buffer))
	        {
	         strcpy(buffer2,buffer);
             token=strtok(buffer," ");
             while( token != NULL )
              {
                ch++;
                token=strtok(NULL," ");
              }
	         if(ch==7)
	            {    
	                if(get_read_lock(atoi(train_id),ss4))
	                {   //printf("got read lock in in4\n");
	                    release_read_lock(atoi(train_id),ss4);
	                    //printf("release read lock in in4\n");
	                    sscanf(buffer2,"%s %s %s %s %s %s %s",action,fname,lname,age,sex,train_id,class);
	                    strcat(fname,lname);
	                    if(get_write_lock(atoi(train_id),ss4))
	                      {   
	                         // printf("got write lock in in4\n");
	                          a[atoi(train_id)].rlist=add_passenger(a,atoi(train_id),fname,age,sex,class,idx4[atoi(train_id)]);
	                          idx4[atoi(train_id)]++;
	                          
	                      }
	                    printf("---Reservation Confirmed of %s---\n",fname);
	                    action[0]='\0'; fname[0]='\0'; lname[0]='\0'; age[0]='\0'; sex[0]='\0'; train_id[0]='\0'; class[0]='\0';
	                    release_write_lock(atoi(train_id),ss4);
	                    sleep(2);
	                }
	                
	            }
	            //if someone cancels reservation
	         if(ch==2)
	            {
	               sscanf(buffer2,"%s %s",action,pnr);
	               int tid,pos;
	               tid=atoi(pnr)%10;
	               pos= atoi(pnr)/10;
	              
	               get_read_lock(tid,ss4);
	               release_read_lock(tid,ss4);
	               sleep(1);
	               get_write_lock(tid,ss4);
	               a[tid].rlist[pos].status=2; //reservation cancelled
	               printf("***Cancellation Confirmed of pnr %s***\n",pnr);
	               sleep(1);
	               for(int i=0;i<idx4[tid];i++)
                    {
	                   if(a[tid].rlist[i].status == 0 )
	                    {
	                     a[tid].rlist[i].status=1;
	                     //printf("Moved Passenger %s from Waitlist to confirmed\n",a[tid].rlist[i].passenger_name);
	                     break;
	                    }
                    }
	               action[0]='\0'; pnr[0]='\0';
	               release_write_lock(tid,ss4);
	           }
	        ch=0;
	        buffer[0]='\0'; buffer2[0]='\0'; token=NULL;
	        }
	        fclose(fp); 
	        
            }
        }
    
    }
    else
    {
        if(id2==0)
        {
            if(id3==0)
             {
                // printf("Dummy process%d %d\n",getpid(),getppid());
             }
             else
             {
                 //printf("Dummy process%d %d\n",getpid(),getppid());
             }
        }
        else
        {
            if(id3==0)
            {
                //printf("Dummy process%d %d\n",getpid(),getppid());
            }
            else
            {
                	semctl(ss[0], 0, IPC_RMID, 0);
	            	semctl(ss[1], 0, IPC_RMID, 0);
		            semctl(ss[2], 0, IPC_RMID, 0);
		            semctl(ss[3], 0, IPC_RMID, 0);
		            semctl(ss[4], 0, IPC_RMID, 0);
		            semctl(ss[5], 0, IPC_RMID, 0);
	                shmctl(shmid, IPC_RMID, 0);
	                shmdt(ss);//detach
                	shmdt(i);//detach
                    shmdt(idx); //detach
                    shmdt(a); //detach
                    shmdt(c);//detach
                    shmdt(b);//detach
                    shmdt(d);//detach
                    shmdt(ss1);//detach
                    shmdt(ss2);shmdt(ss3);shmdt(ss4); //detach
                    shmdt(idx1);shmdt(idx2);shmdt(idx3); shmdt(idx4); //detach
                    
            }
        }
    }
    	while(wait(NULL)!=-1 ||  errno!=ECHILD)
    	{
    	    printf("***********Parent Waited for a child to finish*************\n");
    	}
}
