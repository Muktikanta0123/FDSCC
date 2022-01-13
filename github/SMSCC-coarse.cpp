/*
* SMSCC_coarse.cpp:
 *
 * Author(s):
 *   Muktikanta Sa   <muktikanta.sa@gmail.com>
 *   
 * Description:
 *   
 * Copyright (c) 2017.
 * last Updated: 28/10/2017
 *
*/

#include<iostream>
#include<float.h>
#include<stdint.h>
#include<stdio.h>
#include <stdlib.h>
//#include <pthread.h>
#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <vector>
#include <ctime>        // std::time
#include <random>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <sys/time.h>
#include <atomic>
#include<list>


using namespace std;
#ifndef LLIST_H_ 
#define LLIST_H_


#ifdef DEBUG
#define IO_FLUSH                        fflush(NULL)
#endif
//#define MAX 100000

int d;
//int SUCC[MAX];
//int PREC[MAX];
typedef struct{
    int     secs;
    int     usecs;
}TIME_DIFF;

TIME_DIFF * my_difftime (struct timeval * start, struct timeval * end){
	TIME_DIFF * diff = (TIME_DIFF *) malloc ( sizeof (TIME_DIFF) );
 	if (start->tv_sec == end->tv_sec){
        	diff->secs = 0;
        	diff->usecs = end->tv_usec - start->tv_usec;
    	}
   	else{
        	diff->usecs = 1000000 - start->tv_usec;
        	diff->secs = end->tv_sec - (start->tv_sec + 1);
        	diff->usecs += end->tv_usec;
        	if (diff->usecs >= 1000000){
        	    diff->usecs -= 1000000;
	            diff->secs += 1;
	        }
	}
        return diff;
}

/*
class list1{
 public:
        slist_t *Head, *Tail;
        // for initilization of the list
        void init(){
                Head = (slist_t*) malloc(sizeof(slist_t));
                Head ->val = INT_MIN;
                Head ->vnext=NULL;
                Head ->enext=NULL;
                Head ->marked=false;
                //pthread_mutex_init(&Head->lock, NULL);
                Tail = (slist_t*) malloc(sizeof(slist_t));
                Tail ->val = INT_MAX;
                Tail ->vnext=NULL;
                Tail ->enext=NULL;
                Tail ->marked=false;
                //pthread_mutex_init(&Tail->lock, NULL);
                Head->vnext=Tail;
        }
        slist_t* createV(long key){
                slist_t* EHead = (slist_t*) malloc(sizeof(slist_t));
                EHead ->val = INT_MIN;
                EHead ->vnext=NULL;
                EHead ->enext=NULL;
                EHead ->marked=false;
                //pthread_mutex_init(&EHead->lock, NULL);
                slist_t *ETail = (slist_t*) malloc(sizeof(slist_t));
                ETail ->val = INT_MAX;
                ETail ->vnext=NULL;
                ETail ->enext=NULL;
                ETail ->marked=false;
                //pthread_mutex_init(&ETail->lock, NULL);
                EHead->enext=ETail;
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext=NULL;
                temp ->enext=NULL;
                temp ->marked=false;
                //pthread_mutex_init(&temp->lock, NULL);
                temp ->enext=EHead;
                return temp;
        }
        
        
        slist_t* createE(long key){
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext=NULL;
                temp ->enext=NULL;
                temp ->marked=false;
                //pthread_mutex_init(&temp->lock, NULL);
                return temp;
        }
        
        
};*/

 long ccid;
long ccCount;

typedef struct Gnode{
	long val; // data
	struct Gnode *vnext; // pointer to the next vertex
	struct Gnode *enext; // pointer to the next adjancy list
}slist_t;

typedef struct CCnode{
	long ccno; // data
	struct Gnode *vnext; // pointer to the Vertex List of the CC
	struct CCnode *next; // pointer to the the next CC
}cclist_t;

class SCC{
        cclist_t *CCHead, *CCTail; // sentinals for CC list
     public:
        SCC(){
                ccid = 1;
                ccCount = 1;
                CCHead = (cclist_t*) malloc(sizeof(cclist_t));
                CCHead ->ccno = INT_MIN;
                CCHead ->vnext =NULL;
                CCHead ->next=NULL;
                CCTail = (cclist_t*) malloc(sizeof(cclist_t));
                CCTail ->ccno = INT_MAX;
                CCTail ->vnext = NULL;
                CCTail ->next = NULL;
                CCHead->next = CCTail;
                
        }
        cclist_t* createCCNode(long key){
                /*sentinal */
                slist_t* VHead = (slist_t*) malloc(sizeof(slist_t));
                VHead ->val = INT_MIN;
                VHead ->vnext=NULL;
                VHead ->enext=NULL;
                /*sentinal */                
                slist_t *VTail = (slist_t*) malloc(sizeof(slist_t));
                VTail ->val = INT_MAX;
                VTail ->vnext=NULL;
                VTail ->enext=NULL;
                /*new vertex created and inserted in between VHead and VTail */                
                slist_t* EHead = (slist_t*) malloc(sizeof(slist_t));
                EHead ->val = INT_MIN;
                EHead ->vnext=NULL;
                EHead ->enext=NULL;
                slist_t *ETail = (slist_t*) malloc(sizeof(slist_t));
                ETail ->val = INT_MAX;
                ETail ->vnext=NULL;
                ETail ->enext=NULL;
                EHead ->enext=ETail;
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext=NULL;
                temp ->enext =NULL;
                temp ->enext =EHead;
                /* creation of intial vertex, itself a new connected components */
                temp ->vnext =VTail; 
                VHead->vnext =temp;
                /* cc node creation */
                cclist_t * temp1 = (cclist_t*) malloc(sizeof(cclist_t));
                temp1 ->ccno = ccid++;
                temp1 ->vnext =VHead; 
                temp1 ->next =NULL;
                return temp1;
        }
        cclist_t* createNewCCNodeWithOldVertex(slist_t * oldV, long cc_num){
                /*sentinal */
                slist_t* VHead = (slist_t*) malloc(sizeof(slist_t));
                VHead ->val = INT_MIN;
                VHead ->vnext =NULL;
                VHead ->enext =NULL;
                /*sentinal */                
                slist_t *VTail = (slist_t*) malloc(sizeof(slist_t));
                VTail ->val = INT_MAX;
                VTail ->vnext =NULL;
                VTail ->enext =NULL;
                /* old vertex inserted in between VHead & VTail, itself a new connected components */
                VHead->vnext =oldV;
                oldV->vnext =VTail;;
                /* cc node creation */
                cclist_t * temp1 = (cclist_t*) malloc(sizeof(cclist_t));
                temp1 ->ccno = cc_num;
                temp1 ->vnext =VHead;
                temp1 ->next =NULL;
                return temp1;
        }
         void locateV(slist_t* head, slist_t* tail, slist_t ** pred, slist_t ** curr, long key){
               (*pred)=head;
                (*curr) = (*pred)->vnext; 
                while((*curr) != tail && (*curr)->val < key){
                   (*pred) = (*curr);
                    (*curr) = (*curr)->vnext;
                 }
        }
        /* Insert old vertex into the newly created cc, sorted place*/
        void InsertOldVertexInNewCC(cclist_t* cc, slist_t* pred, slist_t * oldV){
                slist_t *pred1, *curr1;
                locateV(cc->vnext, NULL, &pred1, &curr1, oldV->val);
//                  cout<<"before insert curr->val, pred and ccno:"<<curr1->val<<" "<<pred1->val<<" "<<cc->ccno<<endl;
               /* old vertex inserted in between pred & curr in the cc */
                if(curr1->val != oldV->val){
                        oldV->vnext =curr1;
                        pred1->vnext =oldV;
                }
       }
        
        /*we don't need this method*/
        slist_t* createV(long key){
                slist_t* EHead = (slist_t*) malloc(sizeof(slist_t));
                EHead ->val = INT_MIN;
                EHead ->vnext =NULL;
                EHead ->enext =NULL;
                //EHead ->marked =false;
                //pthread_mutex_init(&EHead->lock, NULL);
                slist_t *ETail = (slist_t*) malloc(sizeof(slist_t));
                ETail ->val = INT_MAX;
                ETail ->vnext =NULL;
                ETail ->enext =NULL;
               // ETail ->marked =false;
                //pthread_mutex_init(&ETail->lock, NULL);
                EHead ->enext =ETail;
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext =NULL;
                temp ->enext =NULL;
                //temp ->marked =false;
                //pthread_mutex_init(&temp->lock, NULL);
                temp ->enext =EHead;
                return temp;
        }
        
        slist_t* createE(long key){
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext =NULL;
                temp ->enext =NULL;
                return temp;
        }
       
        /*finds the key present in the CC, returns the reference of that cc ,  same as findCCLocate method, wait-free*/
         bool findCC(cclist_t **n, long key){
                 cclist_t *temp1 = CCHead->next;
	         slist_t *temp2;
	         while(temp1 != CCTail){
		        temp2 = temp1->vnext;
		        while(temp2->vnext != NULL && temp2->val <= key ){
		                if(temp2->val == key ){
		                 (*n) = temp1;
		                 return true;
		                 }
			        temp2 = temp2->vnext;
		        }
                	temp1 = temp1->next;
	           }
	       return false;    
        }
     /*it will return the reference to the pred & curr cc and the references to the pred & curr vertex having val=key, wait-free*/
         bool findCCLocate(cclist_t **n1, cclist_t** n2, slist_t **pred, slist_t ** curr, long key){
                 cclist_t *temp, *temp1;
	         slist_t *temp2, *temp3 ;
	         temp = CCHead;
	         temp1 = CCHead->next;
                 while(temp1->next != NULL){
		        temp2 = temp1->vnext;
		        temp3 = temp2->vnext;
		       // if(temp3->vnext){
		        //cout<<" "<<temp3->val;
		        while(temp3->vnext  != NULL  && temp3->val <= key){
		                if(temp3->val == key ){
		                 (*n1) = temp; // pred cc
		                 (*n2) = temp1; // curr cc
		                 (*pred) = temp2; // pred vertex
		                 (*curr) = temp3; // curr vertex
		                 return true;
		                 }
                         temp2 = temp3; 
 		        temp3 = temp3->vnext;
		        }
		        //}
		        temp = temp1;
                	temp1 = temp1->next;
	           }
	       return false;    
        }
       /*wait-free cntains v*/
        bool ContainsV(long key){
                slist_t *pred, *curr;
                cclist_t *currn, *predn;
                bool flag =  findCCLocate(&predn, &currn, &pred, &curr, key);
                if(flag == true  ){
                        //cout<< "the cc is:"<< currn->ccno; 
                        return true; //found it, return success, true
                }
                else{
                        return false; // key not present
                    }    
        }
         /* Add CCNode at the begining, just after CCHead */
        bool AddCC(long key){
                cclist_t *newcc = createCCNode(key);  
                  newcc ->next = CCHead->next;
                  CCHead ->next=newcc;
                  long t = ccCount +1;
                  ccCount=t;
               return true; 
          }
               
        /* New vertex is created with new CC and added at the CCHead */
        bool AddVertexCC(long key){
                slist_t* pred, *curr;
                cclist_t *n;
                bool flag = findCC(&n, key); // find the vertex , it is wait-free search, same as findCCLocate method
                if(flag == false){
                return AddCC(key);
             } 
             return false;  
           }  
           /* Delete the vertex from the CC list */
          
         bool RemoveVertexCC(cclist_t ** cc1, slist_t **adj, long key){
                slist_t* predv, *currv, *p;
                 cclist_t* predc, *currc;
                cclist_t *n;
                bool flag = findCCLocate(&predc, &currc, &predv, &currv, key);
                //bool flag = findCC(&n, key); 
                if(flag == true){
                       (*adj) = currv;
                       predv ->vnext = currv->vnext ; // physical delete
                       (*cc1) = currc;
                        return true; // return true
                }
               else
                return false;
               
              }
         int d;                                            
    bool RemoveSCCV(long key, long MAX){
    cclist_t * cc;
    slist_t * adj;
    slist_t * temp1, *temp2;
     slist_t* predv, *currv;
     cclist_t* predc, *currc;
     bool f = RemoveVertexCC(&cc, &adj, key);
     if(f == true)
      return findSCCafterRemE(cc->vnext, cc->ccno, MAX);
      else
       return false;
    // cout<<"f "<<f<<endl;
    /*
    if(f == true)
      {
      
       //
       bool flag = findSCCafterRemE(cc->vnext, cc->ccno, MAX);
      // cout<<"hello1 "<<flag<<" ";;
       //cin >>d;
       if(flag ==  true){
       //cout<<"hello2 ";
              temp1 = adj->enext->enext;
              while(temp1->enext  != NULL){
                 //cout<<temp1->val<<endl;
                 if(temp1->val < 0){ // only incoming edges
                   bool flag1 = findCCLocate(&predc, &currc, &predv, &currv, (-1)*temp1->val);
                   if(flag1 == true){
                   // cout<<"holl3 "<< flag1<<" "<<currv->val<<" ";
                    RemoveE(currv->enext , key); // delete
                    }
                    
                 }
                 else{ 
                  bool flag1 = findCCLocate(&predc, &currc, &predv, &currv, temp1->val);
                   if(flag1 == true){
                   //cout<<"holl4 "<< flag1<<" "<<currv->val<<" ";
                    RemoveE(currv->enext , (-1)*key); // delete
                    }
                 }
               temp1 = temp1->enext;
              }           
        }
        return true;       
      }
      else
      return false;
    
    */
    }                                                     
  /* it will detele all empty CCs, means if no vertex present in a cc*/
    void RemoveCC(){
         cclist_t* pred = CCHead; 
         cclist_t* curr = pred->next;
         while(curr != CCTail){
               if(curr->vnext->vnext-> vnext == NULL){ // empty cc, only VHead->vnext = VTail                   
                   pred ->next=curr->next; // physical delete
                   curr = pred->next;
                  long t = ccCount-1;
                  ccCount=t;  // decrement the cc counter 
                continue;
                 }
               
                else{
                pred = curr; 
                curr = curr->next;
               }  
         }
        } 
               /*finds the location of the key, it returns the pred(:n1) & curr(:n2)*/         
    void locateE(slist_t* EHead, slist_t ** n1, slist_t ** n2, long key){
                  //         cout<<"loc begins"<<endl;
                slist_t* pred = EHead; 
                slist_t* curr = pred->enext; 
                while(curr->enext != NULL && curr->val < key){
                        pred = curr;
                        curr = curr->enext;
                       }
                (*n1) = pred;
                (*n2) = curr;
        }   

        /*adds an edge in the vertex list*/  
  bool AddE(slist_t * EHead, long key){
           slist_t* pred, * curr;
            locateE(EHead, &pred, &curr, key);
            slist_t *newe = createE(key);
             if((curr->val != key) ){
                        newe ->enext=curr;   
                        pred ->enext =newe ;
                        return true;
                }
                else{
                 return false;
                } 
          }  

        /*Removes the edge node in the edge list of a given vertex*/        
        bool RemoveE(slist_t * EHead, long key){
           slist_t* pred, * curr;
            locateE(EHead, &pred, &curr, key);
            if(curr->val == key  ){
                        pred ->enext=curr->enext; // physical delete
                        return true;
                }
                else{
                        return false;
         }
         }
        /*checking a vertex present in the same cc, this function is used in DFSFW and DFSBW*/
        bool findVSameCC(slist_t *slHead, slist_t**pred, slist_t **curr, long key){
                slist_t*temp = slHead->vnext, *temp1=slHead;
                while(temp->vnext != NULL && temp->val <= key){
                   if(temp->val == key){
                     (*pred) = temp1;
                     (*curr) = temp;
                     return true;
                   } 
                  temp1 = temp;  
                  temp = temp->vnext;   
                }    
              return false;  
        }

        /*not used*/
        void findVInCC(slist_t *slHead, long j, slist_t **oldV){
           slist_t *temp = slHead->vnext;
           while(temp->vnext != NULL){
              if(temp->val == j){
                 (*oldV) = temp;
                 return;            
              }
             temp = temp->vnext;
           }
        }
        int z;
        /*DFS of the forward traversal*/
        void DFSFW(slist_t * slHead, slist_t* it_edge, long num_cc, long cc, long **SUCC){
             slist_t * curr, *pred;//used for iteration of each vertex in findVSameCC
             cclist_t *predn, *currn;
             //cout<<"it_edge->val:"<<it_edge->val<<endl;
             //cin>>z;
             (*SUCC)[it_edge->val] = num_cc; // asign the new num_cc to the vertex
             /*iterate over all out going edges in the edge list of the vertex(i->val > 0)*/
             for(slist_t* i = it_edge->enext->enext; i->enext != NULL; i = i->enext){
             //cout<<"i->val:"<<i->val<<endl;
                     if(i->val > 0 ){ //check for out-going edge and the vertex is belongs to same cc   //&& findVSameCC(slHead, &pred, &curr, i->val)
                     bool flag = findCCLocate(&predn, &currn, &pred, &curr, i->val);
                     //cout<<"i->val:"<<i->val<<endl;
                     //cin>>z;
                        if((*SUCC)[i->val] == cc && flag == true){ // check for old cc equal to same as cc // || (*SUCC)[i->val] == 0
                          // cout<<
                           DFSFW(slHead, curr, num_cc, cc, &(*SUCC));  
                     }
                   }  
            }
        }
        /*DFS of the backward traversal*/      
        void DFSBW(slist_t * slHead, slist_t* it_edge, long num_cc, long cc, long **PREC){
         slist_t * curr, *pred;//used for iteration of each vertex in findVSameCC
         cclist_t *predn, *currn;
             (*PREC)[it_edge->val] = num_cc;// asign the new num_cc to the vertex
              /*iterate over all in coming edges in the edge list of the vertex(i->val < 0)*/
             for(slist_t* i=it_edge->enext->enext; i->enext !=NULL; i = i->enext){
                if(i->val < 0 ){ // check for in-coming edge and the vertex is belongs to same cc   //&& findVSameCC(slHead, &pred, &curr, i->val*(-1))
                        bool flag = findCCLocate(&predn, &currn, &pred, &curr, i->val*(-1));
                        if((*PREC)[i->val*(-1)] == cc && flag == true){
                      //|| (*PREC)[i->val*(-1)] == 0
                           DFSBW(slHead, curr, num_cc, cc, &(*PREC));  
                     }
                   }  
                }
        }
        
        bool findSCCafterRemE(slist_t * slHead, long cc, long MAX){
           
          long num_cc =  ccid;// getting last cc id of SCC
          slist_t * it, *pred, *oldV,*curr;
          cclist_t * newcc;
          //slist_t *pred, *curr;
          cclist_t *currn, *predn;
          long *SUCC = new long[MAX];
          long *PREC = new long[MAX];
          for(long i = 0; i < MAX; i++){
             SUCC[i] =cc; PREC[i] = cc;
          }
          //int st = 1; // test for the first vertex of the SCC
         // cclist_t *newcc;
          /*iterate over all vertices of that SCC*/
          for(it = slHead->vnext; it->vnext != NULL; it = it ->vnext){
           //cout<<"it->val:"<<it->val<<endl;
                 if( (SUCC[it->val] == 0) || (SUCC[it->val] == cc)){ // if cc == old or zero
                   // cout<<"it->val:"<<it->val<<endl;
                    DFSFW(slHead, it, num_cc, cc, &SUCC); // call the DFS on the forward vertices
                    DFSBW(slHead, it, num_cc, cc, &PREC); // call the DFS on the backward vertices
             //   for(int c=1; c<=5; c++){
             //     cout<<SUCC[c]<<" " <<PREC[c]<<endl;
             //   }   
               // PrintSCC();        
           //  cout<<"SUCC[it->val] and PREC[it->val]:"<<  SUCC[it->val] <<" "<< PREC[it->val]  <<endl;
                   int  st = 1; // test for the first vertex of the SCC
                    for(long j = 1; j < MAX ; j++) { // iterate over all vertices            
                        if(findCCLocate(&predn, &currn, &pred, &curr, j)){ // vertex belongs to same cc
                            if(SUCC[j] != PREC[j]) // check for unprocessed vertex for next cc 
                              SUCC[j] = PREC[j] =cc; // assign the old cc if not same cc
                            else{ // add all in to the SCC graph with new CC node having ccid = num_cc
                               if(SUCC[j] == num_cc && PREC[j] == num_cc){
                                 if(st == 1){ // for the first vertex for the new cc node creation
                                  st++;
                               // cout<<"first 1!!"<<j<<endl;
                               // findVInCC(slHead, j, &oldV); // search the vertex 
                               // newcc = createNewCCNodeWithOldVertex(oldV, num_cc); // create a new cc for the first  
                                    slist_t *pred2, *curr2;
                                        cclist_t *currn2, *predn2;
                                    bool flag = findCCLocate(&predn2, &currn2, &pred2, &curr2, j);// find the location of the vertex in the vertex list
                                      /* create new cc with key curr->val and insert at the begining of CCHead  */
                                       newcc = createCCNode(j);
                                       cclist_t * cp = CCHead->next;
                                       if(flag == true && CCHead->next == cp ){ // check for validation
                                       newcc->next=cp;
                                       CCHead->next=newcc;
                                       /*add all its edges to newcc*/
                                      slist_t *p = newcc->vnext->vnext;
                                      p->enext=curr2->enext;
                                        //cout<<"after cc-flag and curr2->val, pred2 and currn2:"<<flag<<" "<<curr2->val<<" "<<pred2->val<<" "<<currn2->ccno<<endl;
                                      changePtr(&pred2, &curr2);
                                      //PrintSCC();
                                      long t = ccCount+1; 
                                      ccCount=t;
                                 }
                                 }
                                 else{ // rest vertcies are added to the newly created cc
                                //  cout<<"Else first!!"<<j<<endl;
                                   slist_t *pred1, *curr1;
                                   cclist_t *currn1, *predn1;
                                   bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, j);// find the location of the vertex in the vertex list  
                             // cout<<"flag1 and curr1->val and currn1:"<<flag1<<" "<<curr1->val<<" "<<currn1->ccno<<endl;
                                  if(flag1 == true && currn1->ccno != newcc->ccno){
                                  //cout<<"before insert-flag1 and curr1->val, pred1 and currn1:"<<flag1<<" "<<curr1->val<<" "<<pred1->val<<" "<<currn1->ccno<<endl;
                                  slist_t *p = pred1, *c = curr1->vnext;
                                  curr1->vnext =NULL;
                                  InsertOldVertexInNewCC(newcc, pred1, curr1); // insert the vertex in the newcc
                                  p->vnext =c ;
                                }
                               } 
                               }
                            }  
                      }
                    }
                    num_cc = num_cc + 1;
                    ccid=num_cc;
                    //st = true;
                }
                
            }
            return true;               
        }
        
        
           bool RemoveGraphEdge(long key1, long key2, long MAX){
                pthread_mutex_lock(&lock);
                 slist_t* pred1, *curr1, *pred2, *curr2;
                cclist_t *predn1, *currn1, *predn2, *currn2;
                /*it will return the reference to the predn1 &currn1 and the references to the pred1 & curr1 
                of the vertex  having val=key1*/                
                bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                bool flag2 = findCCLocate(&predn2, &currn2, &pred2, &curr2, key2);
                if(flag1 == false || flag2 == false)
                 pthread_mutex_unlock(&lock);
                       return false;
                flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                if(flag1 == false)
                 pthread_mutex_unlock(&lock);
                     return false;
                             /*Removes the edge node in the edge list of a given vertex*/        
              bool flag =  RemoveE(curr1->enext, key2) && RemoveE(curr2->enext, (-1)*key1);  
                                
              if(flag == true){
               pthread_mutex_unlock(&lock);
                  return updateSCCAfterRemoeveE(predn1, currn1, predn2, currn2, MAX);//, curr1, curr2, key1, key2);
              
              }
              pthread_mutex_unlock(&lock);
            return false;  
        }
        
        /* check if both vertices are not present in the same CC, do nothing. Else call the limited version of Tarjan's algo to find the all scc, process the vertecies and edges which are in that S
        CC*/
        bool updateSCCAfterRemoeveE(cclist_t *predcc1, cclist_t *currcc1, cclist_t *predcc2, cclist_t *currcc2, long MAX){
         //cout<<"cc1 and cc2:"<<currcc1->ccno<< " "<<currcc2->ccno<<endl;
         if(currcc1->ccno != currcc2->ccno){
              return true;          // do nothing
         }
         else{ /* find all scc, process on the verticess and edges present in that SCC*/
             bool flag = findSCCafterRemE(currcc1->vnext, currcc1->ccno, MAX);
                 //cout<<flag;
             if(flag == true){
     //        cout<<"t";
               //predcc1 -> next = currcc1->next; // remove old cc from the cc-list
               return true;
             }
         }
         return false; 
        }
         /* not used, check is there any in-coming edge from cc2 to cc1, used in the findSCCafterAddE() method*/
        bool findVEdgeDiffCC(slist_t* slHead1, slist_t * slHead2){
           slist_t * it_out, *it_in;
           slist_t *temp;
           /*iterate over all vertices of cc1(:slHead1)*/
           for(it_out = slHead1->vnext; it_out->vnext != NULL; it_out = it_out->vnext){
                      /*iterate over all vertices of cc2(:slHead2)*/
               for(it_in = slHead2->vnext; it_in->vnext != NULL ; it_in = it_in->vnext){
                   temp = it_in->enext->enext;
                              /*iterate over all edges of it_in vertex in cc2*/
                  while(temp->enext!=NULL ){
                     if(temp->val == it_out->val)
                       return true; // found in-coming edge from cc2 to cc1
                   temp = temp->enext;
                  }
                }
              }
            return false;    
        }
        void findSCCafterAddE(cclist_t *predcc1, cclist_t *currcc1, cclist_t *predcc2, cclist_t *currcc2, long MAX){

          slist_t * it, *oldV,*curr;
          
         // if(!findVEdgeDiffCC(currcc1->vnext, currcc2->vnext)){
           //     cout<<"different cc!!"<<endl;
             // return false;// no change in the cc    
            //}
          //else{  
         //  cout<<currcc1->ccno <<" "<<currcc2->ccno<<endl;
              addAllAffectedSCC(currcc1, currcc2, MAX);
                /*iterate over all vertices of cc1(:slHead1)*/
                //for(it = currcc1->vnext->vnext; it->vnext != NULL; it = it->vnext){ 
                //cout<<it->val<<" "<<endl; cin >>d;
                      // InsertOldVertexInNewCC(currcc2, it); // insert the vertex in to the cc2
                //}
                
               // remove cc1 from the cc list
               //predcc1->next = currcc1->next; 
            //return true;
            //}               
        }
        
            /* detection of reachable path from the any vertex from cc2 to any vertex of cc1.*/  
      bool isReachable(cclist_t *cc1, cclist_t *cc2, long MAX){
        slist_t *pred, *curr;
        cclist_t *currn, *predn;

        slist_t *temp3, *temp1, *it, *temp2 = cc2->vnext->vnext;
      //  if(checkEqual(cc1, cc2)) // base case, only for two CCs
        //  return true;
       while(temp2->vnext != NULL ){ // iterate over each vertex of cc2
       //cout<<"temp2:"<<temp2->val<<endl;
             //int p;cout<<"hello"<<p<<endl; cin>>p;           
        temp1 = temp2; //temp1 used for iteration of all vertices and edges in cc2  
        bool *  visited = new bool[MAX]; // create a visiting array
        for(long i =0; i < MAX; i++) // intially all vertices are false
                visited[i] = false;
        list<long> que; // queue for processing the reachablity
        visited[temp1->val] = true;//first item is visited
        que.push_back(temp1->val);// push in to the que
        //cout<<"temp1:"<<temp1->val<<endl;
            // cout<<"h"<<temp1->val<<endl; 
            //cin>>p;   
        while(!que.empty()){ // check until que is not empty
//cout<<"s:"<<s<<endl;
            long s = que.front(); // get the front item of the que
                que.pop_front(); // pop from the que
            //bool flag1 = Contains(Head, Tail, &temp, s);  
          bool flag = checkVPresentInCC(cc1, s);//checks whether s is present in the cc1 
          //cout<<"flag-checkVPresentInCC and s:"<<flag<<" "<<s<<endl;
          
          if(flag == true)
            return true;// is rechable
          bool flag1 = findCCLocate(&predn, &currn, &pred, &curr, s);// find the location of the vertex in the vertex list
          //cout<<"flag1-findCCLocate and curr:"<<flag1<<" "<<curr<<endl;
         // if(flag1 == true && currn->ccno != cc2->ccno){
         if(flag1 == true ){
         //cout<<"flag1-findCCLocate and curr:"<<flag1<<" "<<curr<<endl;
          temp3 = curr->enext; // Ehead
          for(it = temp3->enext; it->enext != NULL; it = it ->enext){ // iterate over each edges of the vertex it
          if(it->val > 0){ // only +ve values(only out going edges)
            bool flag2 = checkVPresentInCC(cc1, it->val);//checks whether s is present in the cc1 
            //cout<<"flag2-checkVPresentInCC:"<<flag2<<endl;
          
          if(flag2 == true)
            return true;// is rechable
             //if(checkVPresentInCC(cc1, it->val)) // check if the vertex is present in the cc1
             
               //  return true; // reachable
             if(!visited[it->val]){ // check if not visited
                 visited[it->val] = true; // mark as visited
                 que.push_back(it->val); // push into que
                 }
                } 
               } // end of for loop   
               }
             }// end of while que != empty
           temp2 = temp2->vnext;
           }// end of outer while loop
            // int p;cout<<"hello"<<p<<endl; cin>>p;               
        return false;                
     }
        
        bool updateSCCAfterAddE(cclist_t *predcc1, cclist_t *currcc1, cclist_t *predcc2, cclist_t *currcc2, long MAX){
        
         if(currcc1->ccno == currcc2->ccno){
              return true;          // do nothing
         }
         else{ /* find all sccs which are affecting, process all vertices and edges present in all the SCCs*/
       //         cout<<currcc1->ccno <<" "<<currcc2->ccno<<endl;
             bool reach =  isReachable(currcc1, currcc2, MAX); // check is there a path from from currcc2 to currcc1
   //int p;cout<<"reach"<<reach<<endl; cin>>p;
    // cout<<" reach:"<<reach;//<<endl;
             if(reach == false){
                              
               return true;// no change in SCC, so return
              } 
             /*if there is a path from currcc2 to currcc1, we need to update all scc, which are affected*/  
               bool flag = findSCCafterRemE(currcc1->vnext, currcc1->ccno, MAX);
             if(flag == true){
               //predcc1 -> next = currcc1->next; // remove old cc from the cc-list
               return true;
             }
            // findSCCafterAddE(predcc1, currcc1, predcc2, currcc2);
            // bool flag = findSCCafterAddE(predcc1, currcc1, predcc2, currcc2);
             //if(flag == true){
              // predcc1 -> next = currcc1->next; // remove old cc from the cc-list
              // return true;
            // }
            //return true;
         }
         return false;
 
        }
        
      
      /*checks whether a vertex is present in the cc, this method used in the isReachable*/ 
      bool checkEqual(cclist_t * cc1, cclist_t * cc2){
        slist_t *temp, *temp_e, *temp1, *temp2 = cc2->vnext->vnext;
         while(temp2->vnext!=NULL){ // iterate over each vertex of cc2
          temp1 = cc1->vnext->vnext;
          while(temp1->vnext!=NULL){ // iterate over each vertex of cc1
            temp_e = temp1->enext->enext;               
            while(temp_e->enext!=NULL && temp_e->val < temp2->val){// iterate ove each edges of each vertex of cc1
              if(temp_e->val == temp2->val)
               return true; // key is present, returns true
              temp_e = temp_e->enext; // in the edge list 
            }
           temp1 = temp1->vnext; 
          }
         temp2 = temp2->vnext;  
         }
       
      return false;// not present
      }
      
       /*checks whether a vertex is present in the cc1, this method used in the isReachable*/ 
      bool checkVPresentInCC(cclist_t * cc1, long key){
        slist_t *temp1, *temp2, *temp, *temp_e;
          temp1 = cc1->vnext->vnext;
          while(temp1->vnext!=NULL && temp1->val <= key){ // iterate over each vertex of cc1
              if(temp1->val == key)
               return true; // key is present, returns true
           temp1 = temp1->vnext; 
          }
      return false;// not present
      }
      //long n = 100000; 
     // int Stk[MAX]; // stack used in the Tarjan's algrithm
     // int Root[MAX];//used to hold the root vertex of the SCC
     // int Comp[MAX];//stores the component number of the vertex,
     // bool visited[MAX];// stores the status of the each vertex, visited or not
     // int top=-1;
      long push(long ** Stk, long top,long *holdtop, long v){
        top = top + 1;
        (*holdtop) = top;
       (*Stk)[top] = v;
       //cout<<" (*Stk)[top]:"<<(*Stk)[top];
      return top;
      }  
      long pop(long **Stk, long *x, long top){
        if((top) == -1){
         return -1;// stack is empty
        }
         (*x) = (*Stk)[(top)];
         // cout<<" (*Stk)[top] top x:"<<top<<" "<<(*x);
       (top) = (top) - 1;        
       return top;
      }
     long min(long x, long y){
      if(x < y) 
        return x;
      else
        return y;  
    }
/*Tarjan's algorithm to find the SCC*/
 void VISIT(slist_t *pred, slist_t *curr, long **Stk, long **Root, long **Comp, bool **visited, long top){
    long w;
    long holdtop;
    slist_t *pred2, *curr2;
        cclist_t *currn2, *predn2;
        cclist_t *newcc;
    slist_t *temp;
    (*visited)[curr->val] = true; // mark visited
    (*Root)[curr->val] = curr->val; // keep the intial root as it's own vertex
    (*Comp)[curr->val] = -1;// initial Compnent to -1, not assigend 
    top = push(&(*Stk), (top), &holdtop, curr->val);// push in to the stack
    //holdtop = top;
    //cout<<"in holdtop:"<<holdtop;
    /*iterate over all edges of the vertex*/
    for(temp = curr->enext->enext; temp->enext != NULL; temp = temp->enext){
        if(temp->val > 0){
        if((*visited)[temp->val] == false){ 
          bool flag = findCCLocate(&predn2, &currn2, &pred2, &curr2, temp->val);// find the location of the vertex in the vertex list 
          //cout<<"flag and curr2->val:"<<flag<<" "<<curr2->val<<endl;
          if(flag == true ){
          //cout<<"flag and curr2->val and holdtop:"<<flag<<" "<<curr2->val<<" "<<holdtop<<endl;
          //holdtop = top;
          VISIT(pred2, curr2, &(*Stk), &(*Root), &(*Comp), &(*visited), (top));
          //top++;
          //cout<<" Top:"<<top;
          }
          //if(temp->enext == NULL)
          // holdtop = top; 
        }
        if((*Comp)[temp->val] == -1){
          (*Root)[curr->val] = min((*Root)[curr->val], (*Root)[temp->val]);
        }
       } //end of if temp->val > 0
    }// end of for loop
    static bool p=true;
    int  finaltop;
    if(p == true){
       finaltop = holdtop;
       p = false;
       }
    //cout<<" out holdtop and finaltop:"<<holdtop<<" "<<finaltop<<endl;
    //cout<<" in root[curr->val]"<<(*Root)[curr->val]<<" "<<curr->val<<endl;
    if((*Root)[curr->val] == curr->val){
    //cout<<" in root[curr->val]"<<(*Root)[curr->val]<<endl;
      bool flag = findCCLocate(&predn2, &currn2, &pred2, &curr2, curr->val);// find the location of the vertex in the vertex list
      /* create new cc with key curr->val and insert at the begining of CCHead  */
       newcc = createCCNode(curr->val);
       //pthread_mutex_lock(&pred2->lock); // lock the pred2
       //pthread_mutex_lock(&curr2->lock); // lock the curr2
       //pthread_mutex_lock(&CCHead->lock); // lock the CCHead
       cclist_t * cp = CCHead->next;
       //pthread_mutex_lock(&cp->lock);   // lock the CCHead->next           
       if(flag == true && CCHead->next == cp ){ // check for validation
          newcc->next=cp;
          CCHead->next=newcc;
           /*add all its edges to newcc*/
          slist_t *p = newcc->vnext->vnext;
          p->enext=curr2->enext;
          //pred->vnext = curr->vnext; // delete from the old list
          //cout<<"after cc-flag and curr2->val, pred2 and currn2:"<<flag<<" "<<curr2->val<<" "<<pred2->val<<" "<<currn2->ccno<<endl;
          changePtr(&pred2, &curr2);
          //pthread_mutex_unlock(&CCHead->lock); // unlock the CCHead
          //pthread_mutex_unlock(&cp->lock);   // unlock CCHead
          //pthread_mutex_unlock(&pred2->lock); // unlock the pred2
          //pthread_mutex_unlock(&curr2->lock); // unlock the curr2
    //cclist_t * newcc = createCCNode(curr->val);
      //newcc ->next =CCHead->next ;
      //CCHead ->next =newcc ;
      /*add all its edges to newcc*/
      //slist_t *p = newcc->vnext->vnext;
      //p->enext =curr2->enext ;
      //pred->vnext = curr->vnext; // delete from the old list
       // cout<<"after cc-flag and curr2->val, pred2 and currn2:"<<flag<<" "<<curr2->val<<" "<<pred2->val<<" "<<currn2->ccno<<endl;
        
      //PrintSCC();
       long t = ccCount +1; 
       ccCount=t;
      //cout<<" out holdtop:"<<holdtop; 
       }
      //cout<<" out holdtop and top: "<<holdtop<< top; 
      do{
      slist_t *pred1, *curr1;
      cclist_t *currn1, *predn1;
      //cout<<"==top:"<<holdtop;
      finaltop = pop(&(*Stk), &w, (finaltop));
      //cout<<" w:"<<w;
      if(holdtop == -1){ // Empty error
        cout<<"Empty Error!!!!!"<<endl;   
        return ;
         }
      bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, w);// find the location of the vertex in the vertex list  
      //cout<<"flag1 and curr1->val and currn1:"<<flag1<<" "<<curr1->val<<" "<<currn1->ccno<<endl;
      if(flag1 == true && currn1->ccno != newcc->ccno){
          //cout<<"before insert-flag1 and curr1->val, pred1 and currn1:"<<flag1<<" "<<curr1->val<<" "<<pred1->val<<" "<<currn1->ccno<<endl;
         slist_t *p = pred1, *c = curr1->vnext;
         //if(ValidateV(pred1, curr1)){
         curr1->vnext =NULL;
         InsertOldVertexInNewCC(newcc, pred1, curr1); // insert the vertex in to the cc2
         //pthread_mutex_lock(&p->lock); // lock the pred1
         //pthread_mutex_lock(&c->lock); // lock the curr1
         p->vnext =c ; // change the pointer
         //pthread_mutex_unlock(&p->lock); // unlock the pred1
         //pthread_mutex_unlock(&c->lock); // unlock the curr1
             //}
         
          //slist_t *p = pred1, *c = curr1->vnext;
          //curr1->vnext =NULL ;
          //InsertOldVertexInNewCC(newcc, pred1, curr1); // insert the vertex in to the cc2
          //p->vnext =c ;
          //PrintSCC();
          //cout<<"After insert-flag1 and curr1->val, pred1 and currn1:"<<flag1<<" "<<curr1->val<<" "<<pred1->val<<" "<<currn1->ccno<<endl;
          //changePtr(&p, &c);
         // pred1 = curr1->vnext; // delete from the old list
          //PrintSCC();
          }
        if(w==curr->val) p = true; 
      }while(w != curr->val);
    }
 }   
       void changePtr(slist_t **pred, slist_t**curr){
        (*pred)->vnext =(*curr)->vnext ;
        //cout<<" pred and curr:"<<(*pred)->val<<" "<<(*curr)->val;
       
       }
      /* adding all affacted SCCs in to one SCC, after AddEdge.*/  
      void addAllAffectedSCC(cclist_t *cc1, cclist_t *cc2, long MAX){
        slist_t *pred, *curr;
        cclist_t *currn, *predn;
        slist_t *temp1, *it,* temp2 = cc2->vnext->vnext;
        long *Stk = new long[MAX]; // stack used in the Tarjan's algrithm
        long *Root = new long[MAX];//used to hold the root vertex of the SCC
        long *Comp = new long[MAX];//stores the component number of the vertex,
        bool *visited =  new bool[MAX];// stores the status of the each vertex, visited or not
        //int * top;
         long top = -1;
       //if(checkEqual(cc1, cc2)) // base case, only for two CCs
       //   return true;
       //top = -1;
      /*initilize the visited[], all are false intially*/
      for(long i = 0; i < MAX; i++){
        visited[i] = false;
        Root[i] = INT_MAX;
        Comp[i] = -1;
      }       
      // invoke limitted version of Tarjan's algorithm to find the SCC
      //cout<<"affcted-temp2:"<<temp2->val<<endl;
      VISIT(cc2->vnext, temp2, &Stk, &Root, &Comp, &visited, top);
          
     }
       
           
       
        bool AddGraphEdge(long key1, long key2, long MAX){
       pthread_mutex_lock(&lock);
                 slist_t* pred1, *curr1, *pred2, *curr2;
                cclist_t *predn1, *currn1, *predn2, *currn2;
                bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                bool flag2 = findCCLocate(&predn2, &currn2, &pred2, &curr2, key2);
                if(flag1 == false || flag2 == false){
                 pthread_mutex_unlock(&lock);
                       return false;}
                flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                if(flag1 == false){ pthread_mutex_unlock(&lock);
                     return false;}
              bool flag =  AddE(curr1->enext, key2) && AddE(curr2->enext, (-1)*key1);  
             //PrintSCC();   
              if(flag == true){ pthread_mutex_unlock(&lock);
                  return updateSCCAfterAddE(predn1, currn1, predn2, currn2, MAX);
              }
               pthread_mutex_unlock(&lock);
            return flag;  
        }
        
        
        
        
        bool ContainsE(long key1, long key2){
         pthread_mutex_unlock(&lock);
                slist_t* pred1, *curr1, *pred2, *curr2;
                cclist_t *predn1, *currn1, *predn2, *currn2;
                bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                bool flag2 = findCCLocate(&predn2, &currn2, &pred2, &curr2, key2);
                if(flag1 == false || flag2 == false)
                       return false;
                flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                if(flag1 == false)
                        return false; // key not present
                else{
                  slist_t* pred, * curr;
                 locateE(curr1->enext, &pred, &curr, key2);
                 if(curr->val == key2 ){
                       return true;
                }
                else{
                      return false;
                      }  
              }  
               pthread_mutex_unlock(&lock);        
        }

     void PrintSCCGraph(){
	cout<<"CCHead("<<CCHead->ccno<<endl;
	cclist_t *temp1 = CCHead->next;
	slist_t *temp2;
//	if(CCHead->next != CCTail){
	do{
		cout << temp1->ccno << "->";
		temp2 = temp1->vnext;
//		cout<<"VHead <"<<temp2->val<<endl;
		do{
			if(temp2!=NULL){
			cout << temp2->val << "->";			
			temp2 = temp2->vnext;
			}

		}while(temp2);
        	//cout<<temp2->val<<"> VTail"<<endl;	
        	cout<<endl;   		
		temp1 = temp1->next;
	   }while(temp1);
//	 }   
	cout<<")CCTail"<<endl;	   
        }
    void PrintSCC(){
	cout<<"CCHead("<<CCHead->ccno<<endl;
	cclist_t *temp1 = CCHead->next;
	slist_t *temp2, *temp3;
        while(temp1->next){
  		cout << temp1->ccno << "<<=";
		temp2 = temp1->vnext;
                //cout<<"VHead <"<<temp2->val<<endl;
		while(temp2->vnext){
              	cout << temp2->val << "=>";					        
		        temp3 = temp2->enext;
		        while(temp3){
        			cout << temp3->val << "=>";			
			        temp3 = temp3->enext;
			}
		        temp2 = temp2->vnext;			
                        cout<<" >";
		}
		cout << temp2->val << "=>>";
		temp1 = temp1->next;
		cout<<endl;
	   }
        cout << temp1->ccno;
	cout<<")CCTail"<<endl;	   
        }
   long totalCC(){
      return  ccCount;
   }   
   
   
   bool AddEinit(slist_t *EHead, long key){
           slist_t* pred, * curr;
            locateE(EHead, &pred, &curr, key);
            slist_t *newe = createE(key);
             ////pthread_mutex_lock(&pred->lock);
             ////pthread_mutex_lock(&curr->lock);
             if((curr->val != key) ){
             
                        newe ->enext=curr;   
                        pred ->enext =newe ;
                        ////pthread_mutex_unlock(&curr->lock);
          	        ////pthread_mutex_unlock(&pred->lock);
                        
                        return true;
                }
                else{
                       // //pthread_mutex_unlock(&curr->lock);
          	        ////pthread_mutex_unlock(&pred->lock);
                 return false;
                } 
          }  
      bool AddGraphEdge1(long key1, long key2){
         //top = -1;
                 slist_t* pred1, *curr1, *pred2, *curr2;
                cclist_t *predn1, *currn1, *predn2, *currn2;
                bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                bool flag2 = findCCLocate(&predn2, &currn2, &pred2, &curr2, key2);
               // if(flag1 == false || flag2 == false)
                //       return false;
               // flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
               // if(flag1 == false)
              //       return false;
            //  bool flag =  AddE(curr1->enext, key2) && AddE(curr2->enext, (-1)*key1);  
             //PrintSCC();   
              if(flag1 == true && flag2 == true){
                 AddEinit(curr1->enext, key2) ;
                 AddEinit(curr2->enext, (-1)*key1);
              }
            return true;  
        }         
   void create_initial_vertices(long n){
          long i,j;
          for(i=1; i <n; i++){
            AddVertexCC(i);
          }
          
          
          
          //for(i=1; i< n; i++){
           // for(j=i+1; j <n; j++)
              //AddE(curr1->enext, key2) 
              //AddE(curr2->enext, (-1)*key1); 
             // AddGraphEdge1(i,j); 
          //}
        
        }
       // bool RemoveV(long key, long MAX){
       //         return lt.Remove(lt.Head, lt.Tail, key);
       // }
        
        /*
        void print(){
                slist_t* pred = Head; 
                slist_t* curr = pred->vnext; 
                if(Head->vnext==Tail){
                        cout<<"Head("<<Head->val<<")->";
                        cout<<Tail->val<<"Tail"<<endl;
                        return;
                }
                cout<<"Head("<<Head->val<<")->";      
                while(curr != Tail){
                        cout<<curr->val<<"->";
                        pred = curr;
                        curr = curr->vnext;
                }
                cout<<Tail->val<<"Tail"<<endl;
         return;
        }
        */

};
/*
class CCGraph{
    list lt;
 public:      
   
      Graph(){
        lt.init();
      }
        
       // lt.init();
        bool AddV(int key){
           return lt.Add(lt.Head,lt.Tail,key);
        }
        bool RemoveV(int key){
                return lt.Remove(lt.Head, lt.Tail, key);
        }
        bool ContainsV(int key){
                 slist_t *g1=(slist_t*) malloc(sizeof(slist_t));
                 g1->val=0;
                 g1->vnext=NULL;
                 g1->enext=NULL;
                 if(lt.Contains(lt.Head, lt.Tail, &g1, key)){
                        //(*n) = &g1;                  
                   return true;
                   }
                 else
                    return false;  
        }
        //void locateV(slist_t ** n1, slist_t ** n2, int key);
        bool AddE(int key1, int key2){
          slist_t *u,*v;
          bool flag1 = lt.Contains(lt.Head, lt.Tail, &u, key1);
          bool flag2 = lt.Contains(lt.Head, lt.Tail, &v, key2);
          if(flag1 == false || flag2 == false)
            return false;
          flag1 = lt.Contains(lt.Head, lt.Tail, &u, key1);  
          if(flag1 == false)
            return false;
          return lt.Add(u->enext,NULL, key2);  
            
            
        }
        bool RemoveE(int key1, int key2){
         slist_t *u,*v;
          bool flag1 = lt.Contains(lt.Head, lt.Tail, &u, key1);
          bool flag2 = lt.Contains(lt.Head, lt.Tail, &v, key2);
          if(flag1 == false || flag2 == false)
            return false;
          flag1 = lt.Contains(lt.Head, lt.Tail, &u, key1);  
          if(flag1 == false)
            return false;
          return lt.Remove(u->enext,NULL, key2);  
         }
        bool ContainsE(int key1, int key2){
         slist_t *u,*v;
          bool flag1 = lt.Contains(lt.Head, lt.Tail, &u, key1);
          bool flag2 = lt.Contains(lt.Head, lt.Tail, &v, key2);
          if(flag1 == false || flag2 == false)
            return false;
          return lt.Contains(u->enext,NULL, &v, key2);  
        
        }
        
        void PrintGraph(){
	slist_t *temp1 = lt.Head;
	slist_t *temp2;
	while(temp1 != NULL){
		cout << temp1->val << "->";
		temp2 = temp1->enext;
		while(temp2 != NULL)
		{
			cout << temp2->val << " ";
			temp2 = temp2->enext;
		}
		cout << endl;
		temp1 = temp1->vnext;
	   }
        }
        void create_initial_vertices(int n){
          int i=0;
          for(;i<n;i++){
            AddV(i);
          }
        
        }
   };     
   
*/

#endif

