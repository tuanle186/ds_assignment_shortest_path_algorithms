#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "bellman.h"
using namespace std;
#define MAX 1000

//Function to print an edge
void printedge(int[]);
//Function to generate a random edges list
int edgeListGen(int[][3],int,int,int);


//main Function
int main() {
    //Function name for checking:
    // enum Func_check {BF,BF_Path,Traveling,none};
    
    bool randGen = 0;
    int edgeList[MAX][3];

    int numEdges = 8;
    int numVertices = 6;
    int initlimit = 15;

    if (!randGen) {
        //Read from file
        ifstream fin("EdgeList.txt");
        for (int i = 0; i < numEdges; i++) {
            fin >> edgeList[i][0] >> edgeList[i][1] >> edgeList[i][2];
        }
        fin.close();
    } else {
        // Generate a random edgelist:
        if (edgeListGen(edgeList,numEdges,numVertices,initlimit) < 0) return -1;
    }

    // Print the generated edgelist:
    // for (int i = 0; i < numEdges; i++) {
    //     printedge(edgeList[i]);
    // }
    

    // Initialize BFValue and BFPrev with all -1s
    int BFValue[numVertices];
    int BFPrev[numVertices];
    for (int i = 0; i < numVertices; i++) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    char start_vertices = 67;

    for (int i = 0; i < numVertices; i++) {
        BF(edgeList, numEdges, start_vertices, BFValue, BFPrev);
    }

    for (int i = 0; i < numVertices; i++) {
        cout << BFValue[i] << ", ";
    }

    cout << endl;

    for (int i = 0; i < numVertices; i++) {
        cout << char(BFPrev[i]) << ", ";
    }

    return 0;
}



//support function definition 
void printedge(int edge[]){
    cout << char(edge[0]) << ", " << char(edge[1]) << ", " << edge[2] << endl;
}

int edgeListGen(int edgeList[][3],int numEdges,int numVertices,int initlimit=1){
    /*
    edgeList: output generated edgeList
    numEdges: number of edges
    numVertices: number vertices
    initlimit: if <=1 all edges weight is 1, else, edges weight random int [1,initlimit]
    */
    
    //random generator init
    random_device rd;
    mt19937 gen(rd());
    
    //exception
    if(numEdges>(numVertices*(numVertices-1))/2){
        cout<<"cannot create simple graph";
        return -1;
    }
    if(numEdges<numVertices-1){
        cout<<"cannot create a connected (weak) graph";
        return -1;
    }
    
    //generate random vertices' names
    int* verList=new int[numVertices];
    vector<int> verName;
    for(int i=33;i<=126;i++){verName.push_back(i);}
    shuffle(verName.begin(),verName.end(),gen);
    for(int i=0;i<numVertices;i++){verList[i]=verName[i];}
    
    //generate random edges, ensure that each vertices will have at least 1 edges;
    bool flag=0;
    vector<pair<int,int>> fullList;
    for(int i=0;i<numVertices;i++){
        for(int j=0;j<numVertices;j++){
            if(i==j) break;
            fullList.push_back({verList[i],verList[j]});
        }
    }
    int* checkList=new int[numVertices];
    while (!flag){
        shuffle(fullList.begin(),fullList.end(),gen);    
        for(int i=0;i<numEdges;i++){
            edgeList[i][0]=fullList[i].first;
            edgeList[i][1]=fullList[i].second;
        }
        int count=0;
        
        for(int i=0;i<numEdges;i++){
            if(count==numVertices) {
                flag=1; break;
            }
            bool found0=0;
            bool found1=0;
            for(int j=0;j<count;j++){
                if(!found0&&edgeList[i][0]==checkList[j]){
                    found0=1;
                }
                if(!found1&&edgeList[i][1]==checkList[j]){
                    found1=1;
                }
                if(found0&&found1) break;
            }
            if(!found0){
                checkList[count++]=edgeList[i][0];
            }
            if(!found1){
                checkList[count++]=edgeList[i][1];
            }
            //cout<<"reshuffle edgeList\n";
        }
    }
    delete[] checkList;
    
    ofstream fout("EdgeList.txt");
    //generate the weights
    if(initlimit<=1){//All the weights will be 1
        for(int i=0;i<numEdges;i++){
            edgeList[i][2]=1;            
        }
    }
    else{//randomize edges' weights
        uniform_int_distribution<int> dist(0,initlimit);
        for(int i=0;i<numEdges;i++){
            edgeList[i][2]=dist(gen);            
            fout<<edgeList[i][0]<<" "<<edgeList[i][1]<<" "<<edgeList[i][2]<<endl;
        }
    }
    fout.close();
    delete []verList;
    return 1;
}

