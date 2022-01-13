/*
 * File:
 *   SCCmain.cpp
 *
 * Author(s):
 *   Muktikanta Sa<muktikanta.sa@gmail.com>
 *   
 * Description:
 *   Sequential implementation of a SCC Graph
 * Copyright (c) 2017.
 *
* Last Modified :28/10/2017
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

#include"SCC_lazy_remv_withDIE.cpp"


//using namespace std;


int main(int argc, char*argv[]){
int ch;
bool flag;
int x;
long int key,key1,key2;
SCC G;
//G.PrintSCCGraph();
int vertexID = 11;
G.create_initial_vertices(vertexID);
G.PrintSCCGraph();
do{
printf("\n1:AddVertexCC\n2:RemoveVertexCC\n3:RemoveCC\n4:AddGraphEdge\n5:RemoveGraphEdge\n6:conV\n7:ContainsE\n8:PrintSCCGraph\n9:Number of CC\n -1:exit\n");
printf("\nEnter the choice:");
cin>>ch;
switch(ch){
	case 1: printf("\nEnter the node key value to add:");
		cin>>key;
		flag=G.AddVertexCC(key);
		vertexID ++;
		if(flag==true)
		  cout<<key<<" added"<<endl;
		else
		  cout<<key<<" not added"<<endl;  
		break;
	case 2: printf("\nEnter the node key value to remove:");
		cin>>key;
		//flag=G.RemoveVertexCC(key);
		flag = G.RemoveSCCV(key, vertexID);
		if(flag==true)
		  cout<<key<<" removed"<<endl;
		else
		  cout<<key<<" not present"<<endl;  
		break;

	case 3: //printf("\nEnter the node key value to search:");
		//cin>>key;
		//cclist_t *n;
		G.RemoveCC();
		//if(flag==true)
		  //cout<<key<<" present"<<endl;
		//else
		//  cout<<key<<" not present"<<endl;  
		break;
  
	case 4: printf("\nEnter the key1 and key2 to add edge :");
		cin>>key1>>key2;
		flag=G.AddGraphEdge(key1,key2, vertexID);
		if(flag==true)
		  cout<<key1<<","<<key2<<" added"<<endl;
		else
		  cout<<" not added"<<endl;  
		break;

	case 5:	printf("\nEnter the key1 and key2 value to removeE:");
		cin>>key1>>key2;
		flag=G.RemoveGraphEdge(key1, key2, vertexID);
		if(flag==true)
		  cout<<key1<<","<<key2<<" removed"<<endl;
		else
		  cout<<" not removed"<<endl;  
		break;
	case 6: printf("\nEnter the key value to search:");
		cin>>key;
		flag=G.ContainsV(key);
		if(flag==true)
		  cout<<key<<" present"<<endl;
		else
		  cout<<" not present"<<endl;  
		break; 			
	case 7: printf("\nEnter the key1 and key2 value to search:");
		cin>>key1>>key2;
		flag=G.ContainsE(key1, key2);
		if(flag==true)
		  cout<<key1<<","<<key2<<" present"<<endl;
		else
		  cout<<" not present"<<endl;  
		break;
		
	case 8: printf("\nThe current Graph is:\n");
		G.PrintSCC();
		break;	
        case 9: 
		cout<<"Number of CC"<<G.totalCC()<<endl;
		
		break;
           case -1:break;
	default:printf("\nWrong choice!!\n");
	}	
 }while(ch!=-1);
return 0;
}

