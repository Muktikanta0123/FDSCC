/*
* SCC_Graph_seq.cpp:
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
#include<float.h>
#include<stdint.h>
#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
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
#define MAX 10000000

int d;
int SUCC[MAX];
int PREC[MAX];
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

typedef struct Gnode{
	int val; // data
	struct Gnode *vnext; // pointer to the next vertex
	struct Gnode *enext; // pointer to the next adjancy list
}slist_t;

class list1{
 public:
        slist_t *Head, *Tail;
        // for initilization of the list
        void init(){
                Head = (slist_t*) malloc(sizeof(slist_t));
                Head ->val = INT_MIN;
                Head ->vnext = NULL;
                Head ->enext = NULL;
                Tail = (slist_t*) malloc(sizeof(slist_t));
                Tail ->val = INT_MAX;
                Tail ->vnext = NULL;
                Tail ->enext = NULL;
                Head->vnext=Tail;
        }
        slist_t* createV(int key){
                slist_t* EHead = (slist_t*) malloc(sizeof(slist_t));
                EHead ->val = INT_MIN;
                EHead ->vnext = NULL;
                EHead ->enext = NULL;
                slist_t *ETail = (slist_t*) malloc(sizeof(slist_t));
                ETail ->val = INT_MAX;
                ETail ->vnext = NULL;
                EHead ->enext = NULL;
                EHead->enext=ETail;
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext = NULL;
                temp ->enext = NULL;
                temp ->enext = EHead;
                return temp;
        }
        
        slist_t* createE(int key){
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext = NULL;
                temp ->enext = NULL;
                return temp;
        }
        void locate(slist_t* head, slist_t* tail, slist_t ** n1, slist_t ** n2, int key){
                  //         cout<<"loc begins"<<endl;
                slist_t* pred = head; 
                slist_t* curr = pred->vnext; 
                if(head->vnext == tail){
                        (*n1) = head;
                        (*n2) = tail;
                }
                if(head == Head){
                while(curr != tail && curr->val < key){
                        pred = curr;
                        curr = curr->vnext;
                       }
                (*n1) = pred;
                (*n2) = curr;
                }
                else
                {       curr = pred->enext; 
                        while(curr != tail && curr->val < key){
                        pred = curr;
                        curr = curr->enext;
                       }
                (*n1) = pred;
                (*n2) = curr;
                }
        }
        bool Contains(slist_t * h, slist_t *t, slist_t ** n, int key){
           //cout<<"con begins"<<endl;
                slist_t* pred= (slist_t*) malloc(sizeof(slist_t));
                pred->val=0;
                pred->vnext=NULL;
                pred->enext=NULL;
                slist_t *curr= (slist_t*) malloc(sizeof(slist_t));
                curr->val=0;
                curr->vnext=NULL;
                 curr->enext=NULL;
                locate(h, t, &pred, &curr, key);
                 //if(h==Head){
                /*if(Head->vnext == Tail){
                        return false;
                }*/
                if(curr->val == key){
                        (*n) = curr; 
                        return true;       // found it, return success, true
                }
                else
                        return false; // key not present
                
        }

        bool Add(slist_t * h, slist_t *t, int key){
                // allcate new node
                slist_t* pred, *curr;
                locate(h, t, &pred, &curr, key);
                if(h == Head){
                slist_t *newv = createV(key);
                if(curr->val != key){
                        newv ->vnext = curr;   
                        pred ->vnext = newv;
                        return true;
                }
                else
                 return false;
               }
               else{
               slist_t *newe = createE(key);
                if(curr->val != key){
                        newe ->enext = curr;   
                        pred ->enext = newe;
                        return true;
                }
                else
                 return false;
          }  
   }

    bool Remove(slist_t *h, slist_t *t, int key){
        slist_t* pred, *curr;
        locate(h, t, &pred, &curr, key);
        if(h==Head){
                if(curr->val == key){
                        pred ->vnext = curr->vnext;
                        return true;
                }
                else
                        return false;
        }
         else{
         if(curr->val == key){
                        pred ->enext = curr->enext;
                        return true;
                }
                else
                        return false;
         
         }
        } 
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
};


typedef struct CCnode{
	int ccno; // data
	struct Gnode *vnext; // pointer to the Vertex List of the CC
	struct CCnode *next; // pointer to the the next CC
}cclist_t;

unsigned int ccid = 1;
int ccCount = 1;
class SCC{

        cclist_t *CCHead, *CCTail; // sentinals for CC list
        //list lt;
        // for initilization of the list
     public:
        SCC(){
                CCHead = (cclist_t*) malloc(sizeof(cclist_t));
                CCHead ->ccno = INT_MIN;
                CCHead ->vnext = NULL;
                CCHead ->next = NULL;
                CCTail = (cclist_t*) malloc(sizeof(cclist_t));
                CCTail ->ccno = INT_MAX;
                CCTail ->vnext = NULL;
                CCTail ->next = NULL;
                CCHead->next=CCTail;
                
        }
        cclist_t* createCCNode(int key){
                /*sentinal */
                slist_t* VHead = (slist_t*) malloc(sizeof(slist_t));
                VHead ->val = INT_MIN;
                VHead ->vnext = NULL;
                VHead ->enext = NULL;
                /*sentinal */                
                slist_t *VTail = (slist_t*) malloc(sizeof(slist_t));
                VTail ->val = INT_MAX;
                VTail ->vnext = NULL;
                VTail ->enext = NULL;
                /*new vertex created and inserted in between VHead and VTail */                
                slist_t* EHead = (slist_t*) malloc(sizeof(slist_t));
                EHead ->val = INT_MIN;
                EHead ->vnext = NULL;
                EHead ->enext = NULL;
                slist_t *ETail = (slist_t*) malloc(sizeof(slist_t));
                ETail ->val = INT_MAX;
                ETail ->vnext = NULL;
                EHead ->enext = NULL;
                EHead ->enext = ETail;
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext = NULL;
                temp ->enext = NULL;
                temp ->enext = EHead;
                /* creation of intial vertex, itself a new connected components */
                temp ->vnext = VTail;
                VHead->vnext=temp;
                /* cc node creation */
                cclist_t * temp1 = (cclist_t*) malloc(sizeof(cclist_t));
                temp1 ->ccno = ccid++;
                temp1 ->vnext = VHead;
                temp1 ->next = NULL;
                return temp1;
        }
        cclist_t* createNewCCNodeWithOldVertex(slist_t * oldV, int cc_num){
                /*sentinal */
                slist_t* VHead = (slist_t*) malloc(sizeof(slist_t));
                VHead ->val = INT_MIN;
                VHead ->vnext = NULL;
                VHead ->enext = NULL;
                /*sentinal */                
                slist_t *VTail = (slist_t*) malloc(sizeof(slist_t));
                VTail ->val = INT_MAX;
                VTail ->vnext = NULL;
                VTail ->enext = NULL;
                /* old vertex inserted in between VHead & VTail, itself a new connected components */
                VHead->vnext = oldV;
                oldV->vnext = VTail;
                /* cc node creation */
                cclist_t * temp1 = (cclist_t*) malloc(sizeof(cclist_t));
                temp1 ->ccno = cc_num;
                temp1 ->vnext = VHead;
                temp1 ->next = NULL;
                return temp1;
        }
         void locateV(slist_t* head, slist_t* tail, slist_t ** n1, slist_t ** n2, int key){
                  //         cout<<"loc begins"<<endl;
                slist_t* pred = head; 
                slist_t* curr = pred->vnext; 
                while(curr != tail && curr->val < key){
                        pred = curr;
                        curr = curr->vnext;
                       }
                (*n1) = pred;
                (*n2) = curr;
               
        }
        /* Insert old vertex into the newly created cc, sorted place*/
        void InsertOldVertexInNewCC(cclist_t* cc, slist_t* pred, slist_t * oldV){
                slist_t *pred1, *curr1;
                
                locateV(cc->vnext, NULL, &pred1, &curr1, oldV->val);
//                  cout<<"before insert curr->val, pred and ccno:"<<curr1->val<<" "<<pred1->val<<" "<<cc->ccno<<endl;
        
               /* old vertex inserted in between pred & curr in the cc */
               // slist_t*p = oldV->vnext;
                
                oldV->vnext = curr1;
                pred1->vnext = oldV;
              //  pred->vnext = oldV->vnext;
              //PrintSCC();  
        }
        
        /*we don't need this method*/
        slist_t* createV(int key){
                slist_t* EHead = (slist_t*) malloc(sizeof(slist_t));
                EHead ->val = INT_MIN;
                EHead ->vnext = NULL;
                EHead ->enext = NULL;
                slist_t *ETail = (slist_t*) malloc(sizeof(slist_t));
                ETail ->val = INT_MAX;
                ETail ->vnext = NULL;
                EHead ->enext = NULL;
                EHead ->enext = ETail;
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext = NULL;
                temp ->enext = NULL;
                temp ->enext = EHead;
                return temp;
        }
        
        slist_t* createE(int key){
                slist_t * temp = (slist_t*) malloc(sizeof(slist_t));
                temp ->val = key;
                temp ->vnext = NULL;
                temp ->enext = NULL;
                return temp;
        }
        /*finds the cc present in the graph, returns the reference of that cc */
         bool findCC(cclist_t **n, int key){
                 cclist_t *temp1 = CCHead->next;
	         slist_t *temp2;
	         while(temp1 != CCTail){
		        temp2 = temp1->vnext;
		        while(temp2 != NULL && temp2->val <= key ){
		                if(temp2->val == key){
		                 (*n) = temp1;
		                 return true;
		                 }
			        temp2 = temp2->vnext;
		        }
                	temp1 = temp1->next;
	           }
	       return false;    
        }
     /*it will return the reference to the pred & curr cc and the references to the pred & curr vertex having val=key*/
         bool findCCLocate(cclist_t **n1, cclist_t** n2, slist_t **pred, slist_t ** curr, int key){
                 cclist_t *temp = CCHead, *temp1 = CCHead->next;
	         slist_t *temp2, *temp3 ;
	         while(temp1 != CCTail){
		        temp2 = temp1->vnext;
		        temp3 = temp2->vnext;
		        while(temp3->vnext != NULL && temp3->val <= key ){
		                if(temp3->val == key){
		                 (*n1) = temp; // pred cc
		                 (*n2) = temp1; // curr cc
		                 (*pred) = temp2; // pred vertex
		                 (*curr) = temp3; // curr vertex
		                 return true;
		                 }
		                temp2 = temp3; 
			        temp3 = temp3->vnext;
		        }
		        temp = temp1;
                	temp1 = temp1->next;
	           }
	       return false;    
        }
       
        bool ContainsV(int key){
                slist_t *pred, *curr;
                cclist_t *currn, *predn;
                bool flag =  findCCLocate(&predn, &currn, &pred, &curr, key);
               // locateV(h, t, &pred, &curr, key);
                if(flag == true ){
                        cout<< "the cc is:"<< currn->ccno; 
                        return true; //found it, return success, true
                }
                else
                        return false; // key not present
                
        }
        bool AddCC(int key){
            /* Add CCNode at the begining, means just after CCHead */
                cclist_t *newcc = createCCNode(key);                
                newcc ->next = CCHead->next;
                CCHead ->next = newcc;
                ccCount = ccCount + 1;
          }
         bool AddVertex(slist_t * Vhead, int key){
         
          slist_t* pred, *curr;
                locateV(Vhead, NULL, &pred, &curr, key);
                slist_t *newv = createV(key);
                if(curr->val != key){
                        newv ->vnext = curr;   
                        pred ->vnext = newv;
                        return true;
                }
                else
                 return false;
               }
         bool AddVertexCC(int key){
        /* Add vertex in the CC list of VHead(h) in the stored position */
                slist_t* pred, *curr;
                cclist_t *n;
                bool flag = findCC(&n, key);
                if(flag == false){
                return AddCC(key);
                /*
                locateV(n->vnext, NULL, &pred, &curr, key);
                slist_t *newv = createV(key);
                if(curr->val != key){ // if vertex is not present just add it 
                        newv ->vnext = curr;   
                        pred ->vnext = newv;
                        return true; // return true
                }
                else  // else also return true(false)
                 return false; 
               }
               else*/
                 
             } 
             return false;  
           }  
         bool RemoveVertexCC(int key){
                        /* Delete the vertex from the CC list */
                slist_t* pred, *curr;
                cclist_t *n;
                bool flag = findCC(&n, key); 
                if(flag == true){
                locateV(n->vnext, NULL, &pred, &curr, key);
                if(curr->val == key){ // if vertex is present just remove it 
                        pred ->vnext = curr->vnext;   
                        return true; // return true
                }
                else  // else also returns false
                 return false; 
               }
               
               else
                 return false;   
              }                                       
  /* it wll detele all empty cc, means if no vertex present in a cc*/
    void RemoveCC(){
         cclist_t* pred = CCHead; 
         cclist_t* curr = pred->next; 
         while(curr != CCTail){
               if(curr->vnext->vnext-> vnext == NULL){ // empty cc, only VHead->vnext = VTail
                   ccCount = ccCount - 1;  // decrement the cc counter
                   pred -> next = curr->next; // delete the cc from the SCCs
                }
                pred = curr; 
                curr = curr->next;
         }
        } 
               /*finds the location of the key, it returns the pred(:n1) & curr(:n2)*/         
    void locateE(slist_t* EHead, slist_t ** n1, slist_t ** n2, int key){
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
  bool AddE(slist_t * EHead, int key){
           slist_t* pred, * curr;
            locateE(EHead, &pred, &curr, key);
            slist_t *newe = createE(key);
                if(curr->val != key){
                        newe ->enext = curr;   
                        pred ->enext = newe;
                        return true;
                }
                else
                 return false;
          }  
              
          
         
        /*Removes the edge node in the edge list of a given vertex*/        
        bool RemoveE(slist_t * EHead, int key){
           slist_t* pred, * curr;
            locateE(EHead, &pred, &curr, key);
            if(curr->val == key){
                        pred ->enext = curr->enext;
                        return true;
                }
                else
                        return false;
         
         }
          
        
        
        /*checking a vertex present in the same cc, this function used in the DFSFW and DFSBW*/
        bool findVSameCC(slist_t *slHead, slist_t**pred, slist_t **curr, int key){
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
        void findVInCC(slist_t *slHead, int j, slist_t **oldV){
           slist_t *temp = slHead->vnext;
           while(temp->vnext != NULL){
              if(temp->val == j){
                 (*oldV) = temp;
                 return;            
              }
             temp = temp->vnext;
           }
        }
        /*DFS of the forward traversal*/
        void DFSFW(slist_t * slHead, slist_t* it_edge, int num_cc, int cc){
             slist_t * curr, *pred;//used for iteration of each vertex in findVSameCC
             SUCC[it_edge->val] = num_cc; // asign the new num_cc to the vertex
             /*iterate over all out going edges in the edge list of the vertex(i->val > 0)*/
             for(slist_t* i = it_edge->enext->enext; i->enext != NULL; i = i->enext){
                     if(i->val > 0 && findVSameCC(slHead,&pred, &curr, i->val)){ //check for out-going edge and the vertex is belongs to same cc   
                        if(SUCC[i->val] == cc){ // check for old cc equal to same as cc
                           DFSFW(slHead, curr, num_cc, cc);  
                     }
                   }  
            }
        }
        /*DFS of the forward traversal*/      
        void DFSBW(slist_t * slHead, slist_t* it_edge, int num_cc, int cc){
         slist_t * curr, *pred;//used for iteration of each vertex in findVSameCC
             PREC[it_edge->val] = num_cc;// asign the new num_cc to the vertex
              /*iterate over all in coming edges in the edge list of the vertex(i->val < 0)*/
             for(slist_t* i=it_edge->enext->enext; i->enext !=NULL; i = i->enext){
                if(i->val < 0 && findVSameCC(slHead,&pred, &curr, i->val*(-1))){ // check for in-coming edge and the vertex is belongs to same cc   
                     if(PREC[i->val*(-1)] == cc){
                           DFSBW(slHead, curr, num_cc, cc);  
                     }
                   }  
                }
        }
        
        bool findSCCafterRemE(slist_t * slHead, int cc){
         
          int num_cc =  ccid;// getting last cc id of SCC
          slist_t * it, *pred, *oldV,*curr;
          cclist_t * newcc;
          for(int i = 0; i <= MAX; i++){
             SUCC[i] = PREC[i] = 0;
          }
         // cclist_t *newcc;
          /*iterate over all vertices of that SCC*/
          for(it = slHead->vnext; it->vnext != NULL; it = it ->vnext){
              
                if(((SUCC[it->val] == cc) || (SUCC[it->val] == 0))){ // if cc == old or zero
                    DFSFW(slHead, it, num_cc, cc); // call the DFS on the forward vertices
                    DFSBW(slHead, it, num_cc, cc); // call the DFS on the backward vertices
           //  cout<<"SUCC[it->val] and PREC[it->val]:"<<  SUCC[it->val] <<" "<< PREC[it->val]  <<endl;
                    int st = 1; // test for the first vertex of the SCC
                    for(int j = 1; j <= MAX ; j++) { // iterate over all vertices            
                        if(findVSameCC(slHead, &pred, &oldV, j)){ // vertex belongs to same cc
                            if(SUCC[j] != PREC[j]) // check for unprocessed vertex for next cc 
                              SUCC[j] = PREC[j] = cc; // assign the old cc if not same cc
                            else{ // add all in to the SCC graph with new CC node having ccid = num_cc
                                 if(st == 1){ // for the first vertex for the new cc node creation
                                    // findVInCC(slHead, j, &oldV); // search the vertex 
                                    // newcc = createNewCCNodeWithOldVertex(oldV, num_cc); // create a new cc for the first  
                                    slist_t *pred2, *curr2;
                                        cclist_t *currn2, *predn2;
                                    bool flag = findCCLocate(&predn2, &currn2, &pred2, &curr2, j);// find the location of the vertex in the vertex list
                                      /* create new cc with key curr->val and insert at the begining of CCHead  */
                                       newcc = createCCNode(j);
                                      newcc ->next = CCHead->next;
                                      CCHead ->next = newcc;
                                      /*add all its edges to newcc*/
                                      slist_t *p = newcc->vnext->vnext;
                                      p->enext = curr2->enext;
                                      //pred->vnext = curr->vnext; // delete from the old list
                                   //     cout<<"after cc-flag and curr2->val, pred2 and currn2:"<<flag<<" "<<curr2->val<<" "<<pred2->val<<" "<<currn2->ccno<<endl;
                                      changePtr(&pred2, &curr2);
                                      //PrintSCC();
                                        
                                       ccCount = ccCount + 1;
                                 }
                                 else{ // rest vertcies are added to the newely created cc
                                  // findVInCC(slHead, j, &oldV); // find the vertex from the old cc
                                   //InsertOldVertexInNewCC(newcc, pred, oldV); // insert the vertex in the new cc
                                   slist_t *pred1, *curr1;
                                   cclist_t *currn1, *predn1;
                                   bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, j);// find the location of the vertex in the vertex list  
                             // cout<<"flag1 and curr1->val and currn1:"<<flag1<<" "<<curr1->val<<" "<<currn1->ccno<<endl;
                                  if(flag1 == true && currn1->ccno != newcc->ccno){
                                  //cout<<"before insert-flag1 and curr1->val, pred1 and currn1:"<<flag1<<" "<<curr1->val<<" "<<pred1->val<<" "<<currn1->ccno<<endl;
                                  slist_t *p = pred1, *c = curr1->vnext;
                                  curr1->vnext = NULL;
                                  InsertOldVertexInNewCC(newcc, pred1, curr1); // insert the vertex in to the cc2
                                  p->vnext = c;
                                }
                               } 
                            }  
                      }
                    }
                    num_cc = num_cc + 1;
                    ccid = num_cc;
                }
                
            }
            return true;               
        }
        
        
           bool RemoveGraphEdge(int key1, int key2){
                 slist_t* pred1, *curr1, *pred2, *curr2;
                cclist_t *predn1, *currn1, *predn2, *currn2;
                /*it will return the reference to the predn1 &currn1 and the references to the pred1 & curr1 
                of the vertex  having val=key1*/                
                bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                bool flag2 = findCCLocate(&predn2, &currn2, &pred2, &curr2, key2);
                if(flag1 == false || flag2 == false)
                       return false;
                flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                if(flag1 == false)
                     return false;
                             /*Removes the edge node in the edge list of a given vertex*/        
              bool flag =  RemoveE(curr1->enext, key2) && RemoveE(curr2->enext, (-1)*key1);  
              if(flag == true){
                  return updateSCCAfterRemoeveE(predn1, currn1, predn2, currn2);//, curr1, curr2, key1, key2);
              
              }
            return false;  
        }
        
        /* check if both vertices are not present in the same CC, do nothing. Else call the limited version of Tarjan's algo to find the all scc, process the vertecies and edges which are in that S
        CC*/
        bool updateSCCAfterRemoeveE(cclist_t *predcc1, cclist_t *currcc1, cclist_t *predcc2, cclist_t *currcc2){
         //cout<<"cc1 and cc2:"<<currcc1->ccno<< " "<<currcc2->ccno<<endl;
         if(currcc1->ccno != currcc2->ccno){
              return true;          // do nothing
         }
         else{ /* find all scc, process on the verticess and edges present in that SCC*/
             bool flag = findSCCafterRemE(currcc1->vnext, currcc1->ccno);
             if(flag == true){
               //predcc1 -> next = currcc1->next; // remove old cc from the cc-list
               return true;
             }
         }
         return false; 
        }
         /* check is there any in-coming edge from cc2 to cc1, used in the findSCCafterAddE() method*/
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
        void findSCCafterAddE(cclist_t *predcc1, cclist_t *currcc1, cclist_t *predcc2, cclist_t *currcc2){

          slist_t * it, *oldV,*curr;
          
         // if(!findVEdgeDiffCC(currcc1->vnext, currcc2->vnext)){
           //     cout<<"different cc!!"<<endl;
             // return false;// no change in the cc    
            //}
          //else{  
          // cout<<currcc1->ccno <<" "<<currcc2->ccno<<endl;
              addAllAffectedSCC(currcc1, currcc2);
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
      bool isReachable(cclist_t *cc1, cclist_t *cc2){
        slist_t *pred, *curr;
        cclist_t *currn, *predn;

        slist_t * temp2 = cc2->vnext->vnext, *temp1, *it;
      //  if(checkEqual(cc1, cc2)) // base case, only for two CCs
        //  return true;
       while(temp2->vnext != NULL){ // iterate over each vertex of cc2
      // cout<<"temp2:"<<temp2->val<<endl;
             //int p;cout<<"hello"<<p<<endl; cin>>p;           
        temp1 = temp2; //temp1 used for iteration of all vertices and edges in cc2  
        bool *  visited = new bool[n]; // create a visiting array
        for(int i =0; i < n; i++) // intially all vertices are false
                visited[i] = false;
        list<int> que; // queue for processing the reachablity
        visited[temp1->val] = true;//first item is visited
        que.push_back(temp1->val);// push in to the que
       // cout<<"temp1:"<<temp1->val<<endl;
            // cout<<"h"<<temp1->val<<endl; 
            //cin>>p;   
        while(!que.empty()){ // check until que is not empty
//cout<<"s:"<<s<<endl;
            int s = que.front(); // get the front item of the que
                que.pop_front(); // pop from the que
            //bool flag1 = Contains(Head, Tail, &temp, s);  
          bool flag = checkVPresentInCC(cc1, s);//checks whether s is present in the cc1 
          //cout<<"flag-checkVPresentInCC and s:"<<flag<<" "<<s<<endl;
          
          if(flag == true)
            return true;// is rechable
          bool flag1 = findCCLocate(&predn, &currn, &pred, &curr, s);// find the location of the vertex in the vertex list
          //cout<<"flag1-findCCLocate and curr:"<<flag1<<" "<<curr<<endl;
         // if(flag1 == true && currn->ccno != cc2->ccno){
          temp1 = curr->enext; // Ehead
          for(it = temp1->enext; it->enext != NULL; it = it ->enext){ // iterate over each edges of the vertex it
          if(it->val > 0){ // only +ve values(only out going edges)
bool flag2 = checkVPresentInCC(cc1, it->val);//checks whether s is present in the cc1 
         // cout<<"flag2-checkVPresentInCC:"<<flag2<<endl;
          
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
             }// end of while que != empty
           temp2 = temp2->vnext;
           }// end of outer while loop
            // int p;cout<<"hello"<<p<<endl; cin>>p;               
        return false;                
     }
        
        bool updateSCCAfterAddE(cclist_t *predcc1, cclist_t *currcc1, cclist_t *predcc2, cclist_t *currcc2){
        
         if(currcc1->ccno == currcc2->ccno){
              return true;          // do nothing
         }
         else{ /* find all sccs which are affecting, process all vertices and edges present in all the SCCs*/
               // cout<<currcc1->ccno <<" "<<currcc2->ccno<<endl;
             bool reach =  isReachable(currcc1, currcc2); // check is there a path from from currcc2 to currcc1
   //int p;cout<<"reach"<<reach<<endl; cin>>p;
    // cout<<"reach:"<<reach<<endl;
             if(reach == false){
                              
               return false;// no change in SCC, so return
              } 
             /*if there is a path from currcc2 to currcc1, we need to update all scc, which are affected*/  
             findSCCafterAddE(predcc1, currcc1, predcc2, currcc2);
             //bool flag = findSCCafterAddE(predcc1, currcc1, predcc2, currcc2);
            // if(flag == true){
              // predcc1 -> next = currcc1->next; // remove old cc from the cc-list
               return true;
            // }
         }
 
        }
        
      int n = 1000000; 
      /*checks whether a vertex is present in the cc, this method used in the isReachable*/ 
      bool checkEqual(cclist_t * cc1, cclist_t * cc2){
        slist_t *temp1, *temp2 = cc2->vnext->vnext, *temp, *temp_e;
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
      bool checkVPresentInCC(cclist_t * cc1, int key){
        slist_t *temp1, *temp2, *temp, *temp_e;
          temp1 = cc1->vnext->vnext;
          while(temp1->vnext!=NULL && temp1->val <= key){ // iterate over each vertex of cc1
              if(temp1->val == key)
               return true; // key is present, returns true
             
           temp1 = temp1->vnext; 
          }
       
      return false;// not present
      }
      int Stk[MAX]; // stack used in the Tarjan's algrithm
      int Root[MAX];//used to hold the root vertex of the SCC
      int Comp[MAX];//stores the component number of the vertex,
      bool visited[MAX];// stores the status of the each vertex, visited or not
      int top=-1;
      void push(int v){
       Stk[++top] = v;
      }  
      int pop(){
        if(top == -1){
         return -1;// stack is empty
        }
       return Stk[top--];
      }
     int min(int x, int y){
      if(x < y)
        return x;
      else
        return y;  
    }
/*Tarjan's algorithm to find the SCC*/
 void VISIT(slist_t *pred, slist_t *curr){
    int w;
    slist_t *pred2, *curr2;
        cclist_t *currn2, *predn2;
    slist_t *temp;
    visited[curr->val] = true; // mark visited
    Root[curr->val] = curr->val; // keep the intial root as it's own vertex
    Comp[curr->val] = -1;// initial Compnent to -1, not assigend 
    push(curr->val);// push in to the stack
    /*iterate over all edges of the vertex*/
    for(temp = curr->enext->enext; temp->enext != NULL; temp = temp->enext){
        if(temp->val > 0){
        if(visited[temp->val] == false){ 
            bool flag = findCCLocate(&predn2, &currn2, &pred2, &curr2, temp->val);// find the location of the vertex in the vertex list 
        //    cout<<"flag and curr2->val:"<<flag<<" "<<curr2->val<<endl;
            if(flag == true){
              VISIT(pred2, curr2);
              }
        }
        if(Comp[temp->val] == -1){
          Root[curr->val] = min(Root[curr->val], Root[temp->val]);
        }
       } //end of if temp->val > 0
    }// end of for loop
    if(Root[curr->val] == curr->val){
      bool flag = findCCLocate(&predn2, &currn2, &pred2, &curr2, curr->val);// find the location of the vertex in the vertex list
      /* create new cc with key curr->val and insert at the begining of CCHead  */
      cclist_t * newcc = createCCNode(curr->val);
      newcc ->next = CCHead->next;
      CCHead ->next = newcc;
      /*add all its edges to newcc*/
      slist_t *p = newcc->vnext->vnext;
      p->enext = curr2->enext;
      //pred->vnext = curr->vnext; // delete from the old list
       // cout<<"after cc-flag and curr2->val, pred2 and currn2:"<<flag<<" "<<curr2->val<<" "<<pred2->val<<" "<<currn2->ccno<<endl;
        
      changePtr(&pred2, &curr2);
      //PrintSCC();
        
       ccCount = ccCount + 1;
      do{
      slist_t *pred1, *curr1;
      cclist_t *currn1, *predn1;
      w = pop();
      if(w == -1){ // Empty error
        cout<<"Empty Error!!"<<endl;   
        return ;
       }
      bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, w);// find the location of the vertex in the vertex list  
     // cout<<"flag1 and curr1->val and currn1:"<<flag1<<" "<<curr1->val<<" "<<currn1->ccno<<endl;
      if(flag1 == true && currn1->ccno != newcc->ccno){
          //cout<<"before insert-flag1 and curr1->val, pred1 and currn1:"<<flag1<<" "<<curr1->val<<" "<<pred1->val<<" "<<currn1->ccno<<endl;
          slist_t *p = pred1, *c = curr1->vnext;
          curr1->vnext = NULL;
          InsertOldVertexInNewCC(newcc, pred1, curr1); // insert the vertex in to the cc2
          p->vnext = c;
          //PrintSCC();
          //cout<<"After insert-flag1 and curr1->val, pred1 and currn1:"<<flag1<<" "<<curr1->val<<" "<<pred1->val<<" "<<currn1->ccno<<endl;
          //changePtr(&p, &c);
         // pred1 = curr1->vnext; // delete from the old list
          //PrintSCC();
          }
      }while(w != curr->val);
    }
 }   
       void changePtr(slist_t **pred, slist_t**curr){
        (*pred)->vnext = (*curr)->vnext;
       
       }
      /* adding all affacted SCCs in to one SCC, after AddEdge.*/  
      void addAllAffectedSCC(cclist_t *cc1, cclist_t *cc2){
        slist_t *pred, *curr;
        cclist_t *currn, *predn;
        slist_t * temp2 = cc2->vnext->vnext, *temp1, *it;
       //if(checkEqual(cc1, cc2)) // base case, only for two CCs
       //   return true;
       top = -1;
      /*initilize the visited[], all are false intially*/
      for(int i = 0; i < MAX; i++){
        visited[i] = false;
        Root[i] = INT_MAX;
        Comp[i] = -1;
      }
      // invoke limitted version of Tarjan's algorithm to find the SCC
    //  cout<<"affcted-temp2:"<<temp2->val<<endl;
      VISIT(cc2->vnext, temp2);
      /*
       while(temp2->vnext!=NULL){ // iterate over each vertex of cc2
        temp1 = temp2; //temp1 used for iteration of all vertices and edges in cc2  
        bool *  visited = new bool[n]; // create a visiting array
        for(int i =0; i < n; i++) // intially all vertices are false
                visited[i] = false;
        list<int> que; // queue for processing the reachablity
        visited[temp1->val] = true;//first item is visited
        que.push_back(temp1->val);// push in to the que
        while(!que.empty()){ // check until que is not empty
            int s = que.front(); // get the front item of the que
                que.pop_front(); // pop from the que
            //bool flag1 = Contains(Head, Tail, &temp, s);  
          bool flag = checkVPresentInCC(cc1, s);//checks whether s is present in the cc1 
          if(flag == true)
            return true;// is rechable
          bool flag1 = findCCLocate(&predn, &currn, &pred, &curr, s);// find the location of the vertex in the vertex list
         // if(flag1 == true && currn->ccno != cc2->ccno){
          temp1 = curr->enext; // Ehead
          for(it = temp1->enext; it->enext != NULL; it = it ->enext){ // iterate over each edges of the vertex it
             if(checkVPresentInCC(cc1, it->val)) // check if the vertex is present in the cc1
                 return true; // reachable
             if(!visited[it->val]){ // check if not visited
                 visited[it->val] = true; // mark as visited
                 que.push_back(it->val); // push into que
                 }
               }    
             }// end of while que != empty
           }// end of outer while loop
           */
        //return false;                
     }
       
           
       
        bool AddGraphEdge(int key1, int key2){
          top = -1;
                 slist_t* pred1, *curr1, *pred2, *curr2;
                cclist_t *predn1, *currn1, *predn2, *currn2;
                bool flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                bool flag2 = findCCLocate(&predn2, &currn2, &pred2, &curr2, key2);
                if(flag1 == false || flag2 == false)
                       return false;
                flag1 = findCCLocate(&predn1, &currn1, &pred1, &curr1, key1);
                if(flag1 == false)
                     return false;
              bool flag =  AddE(curr1->enext, key2) && AddE(curr2->enext, (-1)*key1);  
            // PrintSCC();        
              if(flag == true){
                  return updateSCCAfterAddE(predn1, currn1, predn2, currn2);
              }
            return flag;  
        }
        
        
        
        
        bool ContainsE(int key1, int key2){
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
                 if(curr->val == key2){
                        //pred ->enext = curr->enext;
                        return true;
                }
                else
                        return false;
              }          
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
   int totalCC(){
      return      ccCount;
   }  
    void create_initial_vertices(int n){
          int i=0;
          for(;i<n;i++){
            AddVertexCC(i);
          }
        
        } 
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

