/*
* SCC_seq_main.cpp:
 *
 * Author(s):
 *   Muktikanta Sa   <muktikanta.sa@gmail.com>
 *   
 * Description:
 *   Sequential implementation of a SCC graph
 * Copyright (c) 2017.
 * last Updated: 28/10/2017
 *
*/


#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <pthread.h>


#include"SCC_Graph_seq.cpp"

using namespace std;


FILE *fp;

 int vertexID;
double seconds;
struct timeval tv1, tv2;
TIME_DIFF * difference;
int NTHREADS, ops;
int  total = 0;
enum type {ADDV, ADDE, REMV, REME, CONV, CONE, COUNT};
int optype; // what type of opearations 
char out[30]; // dataset file name,
int naddV=0, naddE=0, nremV=0, nremE=0, nconV=0, nconE=0; 
pthread_mutex_t lock;
typedef struct infothread{
  long tid;
  SCC G;
}tinfo;


void* pthread_call(void* t)
{
        tinfo *ti=(tinfo*)t;
        long Tid = ti->tid;
        SCC G1=ti->G;
	int u, v;
	int other, res;
 	
 	fp = fopen(out, "r");
 	
 	
 	while(!feof(fp)){
 	        fscanf(fp,"%d",&optype); 
		gettimeofday(&tv2,NULL);
		difference = my_difftime (&tv1, &tv2);

		if(difference->secs >= seconds)
			break;
      switch(optype){
     
      case ADDV : 
                       fscanf(fp, "%d", &u);
                      // printf("%d %d\n",ADDV, u); 
                     // pthread_mutex_lock(&lock);
                       res = G1.AddVertexCC(v);
                       //pthread_mutex_unlock(&lock);
                       naddV++;
                       //  cout<< res<<endl;
                       
                    break;
                
      case ADDE : 
                      fscanf(fp, "%d", &u);
                      fscanf(fp, "%d", &v);
                      // printf("%d %d %d\n",ADDE, u, v); 
                      //pthread_mutex_lock(&lock);
                       res = G1.AddGraphEdge(u,v);
                       //pthread_mutex_unlock(&lock);
                       naddE++;
                      // cout<< res<<endl;
                  
                break;
        case REMV :
                     fscanf(fp, "%d", &u);
                     //  printf("%d %d\n",REMV, u);
                     //pthread_mutex_lock(&lock);
                      // res = G1.RemoveVertexCC(v);
                      // pthread_mutex_unlock(&lock);
                       nremV++;
                        // cout<< res<<endl;
                   
                break;
     case REME : 
                      fscanf(fp, "%d", &u);
                      fscanf(fp,"%d", &v);
                       //printf("%d %d %d\n",REME, u, v); 
                       //pthread_mutex_lock(&lock);
                       res = G1.RemoveGraphEdge(u,v);  
                       //pthread_mutex_unlock(&lock);
                       nremE++;
                       // cout<< res<<endl;
                 
                break;                              
        case CONV :
                      fscanf(fp, "%d", &u);
                      // printf("%d %d\n",CONV, u); 
                      //pthread_mutex_lock(&lock);
                       res = G1.ContainsV(u); 
                       //pthread_mutex_unlock(&lock);
                       nconV++;
                         //cout<< res<<endl;
                   
                break;
     case CONE :
                   fscanf(fp, "%d", &u);
                   fscanf(fp, "%d", &v);
                      // printf("%d %d %d\n",CONE, u, v);
                      //pthread_mutex_lock(&lock);
                       res = G1.ContainsE(u,v); 
                      // pthread_mutex_unlock(&lock);
                       nconE++;
                        /// cout<< res<<endl;
                
                break;    
        default: break;                                          
      }
		
	}
	//gettimeofday(&tv2,NULL);
	//difference = my_difftime (&tv1, &tv2);
        total = naddV + naddE + nremV + nremE + nconV + nconE;
	 
			//end of while loop
			
}

int main(int argc, char*argv[])
{
	vertexID = 1;
	int i;
SCC sg;
	if(argc < 3)
	{
		cout << "Enter 3 command line arguments - #threads, #vertices initially, #time in seconds, file name" << endl;
		return 0;
	}

	NTHREADS = atoi(argv[1]);
	int initial_vertices = atoi(argv[2]); 		// initial number of vertices
	seconds = atoi(argv[3]);
			// number of operations each thread going to perform 1k,10k,50k,100k,1k^2
			
 	ops = 0;
strcpy(out,argv[4]);
	
        pthread_mutex_init(&lock, NULL);
	//create initial vertices
	vertexID = initial_vertices + 1;	
	sg.create_initial_vertices(initial_vertices);
//sg.PrintGraph();
	cout << "Number of Threads: " << NTHREADS << endl;
	cout << "Initial graph with " << initial_vertices << " created." << endl;
 pthread_t *thr = new pthread_t[NTHREADS];
	// Make threads Joinable for sure.
    	pthread_attr_t attr;
   	pthread_attr_init (&attr);
   	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
   	
	gettimeofday(&tv1,NULL);
	cout << "timer started . . ." << endl;
        for (i=0;i < NTHREADS;i++)
       	{
       	      tinfo *t =(tinfo*) malloc(sizeof(tinfo));
		t->tid = i;
		t->G = sg;
       		pthread_create(&thr[i], &attr, pthread_call, (void*)t);
        }

	for (i = 0; i < NTHREADS; i++)
      	{
		pthread_join(thr[i], NULL);
	}
	cout << seconds <<  " seconds elapsed" << endl;

    	cout << "Total operations: " << total <<endl;
        gettimeofday(&tv2,NULL);
	difference = my_difftime (&tv1, &tv2);
    	cout<< "Total time:" <<	(double)difference->secs <<endl;
    	
//sg.PrintGraph();
 /*
  bool cycle = sg.checkCycle(vertexID+naddV);
    if(cycle)
      cout<<"cycle is present"<<endl;
    else
      cout<<"cycle is not present"<<endl;  
   */   
	return 0;
}

