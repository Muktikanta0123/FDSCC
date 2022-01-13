/*
* hoh_main.cpp:
 *
 * Author(s):
 *   Muktikanta Sa   <muktikanta.sa@gmail.com>
 *   
 * Description:
 *   Lazy implementation of a SCC graph
 * Copyright (c) 2017.
 * last Updated: 28/10/2017
 *
*/


#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <pthread.h>


#include"SCC_lazy.cpp"

using namespace std;


FILE *fp;

atomic< long> vertexID;
double seconds;
struct timeval tv1, tv2;
TIME_DIFF * difference;
int NTHREADS;
long ops;
long  total = 0, total1=0;
enum type {ADDV, ADDE, REMV, REME, CONV, CONE, COUNT};
int optype; // what type of opearations 
char out[30]; // dataset file name,
long naddV=0, naddE=0, nremV=0, nremE=0, nconV=0, nconE=0; 
long naddV1=0, naddE1=0, nremV1=0, nremE1=0, nconV1=0, nconE1=0; 
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
	long u, v;
	long other, res;
 	
 	//fp = fopen(out, "r");
 	
 	long i=0;
 	while(true){
 	        //fscanf(fp,"%d",&optype); 
		gettimeofday(&tv2,NULL);
		difference = my_difftime (&tv1, &tv2);

		if(difference->secs >= seconds)
			break;
	//cout<<optype<<" ";	
	//int optype = rand()%6;
		i++;
	int optype;
	if(i%5==0) optype = 3;
	else optype = 1;
      switch(optype){
     
      case ADDV : 
                       //fscanf(fp, "%d", &u);
                      // printf("%d %d\n",ADDV, u); 
                      //pthread_mutex_lock(&lock);
                      u = (rand() % (vertexID));
                       res = G1.AddVertexCC(u);
                       //pthread_mutex_unlock(&lock);
                       vertexID++;
                       if(res == true)
                       naddV++;
                       else naddV1++;
                       //  cout<< res<<endl;
                       
                    break;
                
      case ADDE : 
                     // fscanf(fp, "%d", &u);
                      //fscanf(fp, "%d", &v);
                      // printf("%d %d %d\n",ADDE, u, v); 
                      //pthread_mutex_lock(&lock);
                      u = (rand() % (vertexID));
                      v = (rand() % (vertexID));
                     //  res = G1.AddGraphEdge(u,v);
                       //pthread_mutex_unlock(&lock);
                       if(res == true)
                       naddE++;
                       else naddE1++;
                      // cout<< res<<endl;
                      //pthread_mutex_lock(&lock);
                      G1.RemoveCC();
                      //pthread_mutex_unlock(&lock);
                  
                break;
        case REMV :
                     //fscanf(fp, "%d", &u);
                     //  printf("%d %d\n",REMV, u);
                     //pthread_mutex_lock(&lock);
                      // res = G1.RemoveVertexCC(v);
                      // pthread_mutex_unlock(&lock);
                      // nremV++;
                        // cout<< res<<endl;
                   
                break;
     case REME : 
                      //fscanf(fp, "%d", &u);
                      //fscanf(fp,"%d", &v);
                       //printf("%d %d %d\n",REME, u, v); 
                       //pthread_mutex_lock(&lock);
                       u = (rand() % (vertexID));
                       v = (rand() % (vertexID));
                      // res = G1.RemoveGraphEdge(u,v);  
                       //pthread_mutex_unlock(&lock);
                       if(res == true)
                       nremE++;
                       else nremE1++;
                       // cout<< res<<endl;
                        //pthread_mutex_lock(&lock);                       
                       G1.RemoveCC();
                        //pthread_mutex_unlock(&lock);                 
                break;                              
        case CONV :
                      //fscanf(fp, "%d", &u);
                      // printf("%d %d\n",CONV, u); 
                      //pthread_mutex_lock(&lock);
                      u = (rand() % (vertexID));
                       res = G1.ContainsV(u); 
                       //pthread_mutex_unlock(&lock);
                       if(res == true)
                       nconV++;
                       else nconV1++;
                         //cout<< res<<endl;
                         G1.RemoveCC();
                   
                break;
     case CONE :
                   //fscanf(fp, "%d", &u);
                   //fscanf(fp, "%d", &v);
                      // printf("%d %d %d\n",CONE, u, v);
                      //pthread_mutex_lock(&lock);
                      u = (rand() % (vertexID));
                      v = (rand() % (vertexID));
                       res = G1.ContainsE(u,v); 
                       //pthread_mutex_unlock(&lock);
                       if(res == true)
                       nconE++;
                       else nconE1++;
                        /// cout<< res<<endl;
                        G1.RemoveCC();
                
                break;    
        default: break;                                          
      }
		
	}
	//gettimeofday(&tv2,NULL);
	//difference = my_difftime (&tv1, &tv2);
        total = naddV + naddE + nremV + nremE + nconV + nconE;
        total1 = naddV1 + naddE1 + nremV1 + nremE1 + nconV1 + nconE1;
	 
			//end of while loop
			
}

int main(int argc, char*argv[])
{
	vertexID.store( 1);
	long i;
SCC sg;
	if(argc < 4)
	{
		cout << "Enter 3 command line arguments - #threads, #vertices initially, #time in seconds, dataset file name" << endl;
		return 0;
	}

	NTHREADS = atoi(argv[1]);
	long initial_vertices = atoi(argv[2]); 		// initial number of vertices
	seconds = atoi(argv[3]);
			// number of operations each thread going to perform 1k,10k,50k,100k,1k^2
			
 	ops = 0;
//strcpy(out,argv[4]);
	
        pthread_mutex_init(&lock, NULL);
	//create initial vertices
	vertexID.store(initial_vertices + 1);	
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
    	cout<<"naddV + naddE + nremV + nremE + nconV + nconE:"<<naddV<<"+"<<naddE <<"+"<< nremV <<"+"<< nremE <<"+"<< nconV <<"+"<< nconE<<"="<<total <<endl;
    	cout<<"naddV1 + naddE1 + nremV1 + nremE1 + nconV1 + nconE1:"<<naddV1<<"+"<<naddE1 <<"+"<< nremV1 <<"+"<< nremE1 <<"+"<< nconV1 <<"+"<< nconE1<<"="<<total1 <<endl;
    	cout<<total1<<"-"<<total<<"="<<total1-total<<endl;
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

