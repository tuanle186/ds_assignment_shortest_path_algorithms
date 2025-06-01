#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include "bellman.h"
using namespace std;
#define MAX 1000


void printedge(int[]); //Function to print an edge
int edgeListGen(int[][3],int,int,int); //Function to generate a random edges list


int main() {

    string input_file_name = "EdgeList2.txt";
    char start_vertex = 67;

    // Read from file
    int edgeList[MAX][3]; // To store the graph
    int numEdges = 0; // To compute the number of edges (the number of rows)
    set<int> vertices; // To compute the number of vertices
    int src_vertex, des_vertex, weight;
    ifstream fin(input_file_name);
    while (fin >> src_vertex >> des_vertex >> weight) {
        edgeList[numEdges][0] = src_vertex;
        edgeList[numEdges][1] = des_vertex;
        edgeList[numEdges][2] = weight;
        vertices.insert(src_vertex);
        vertices.insert(des_vertex);
        numEdges++;
    }
    fin.close();

    int numVertices = vertices.size();
    cout << "Number of edges = " << numEdges << endl;
    cout << "Number of vertices = " << numVertices << endl;


    // Print the edgelist:
    // for (int i = 0; i < numEdges; i++) {
    //     printedge(edgeList[i]);
    // }

    
    // Initialize BFValue and BFPrev with all -1s
    /*
    int BFValue[numVertices];
    int BFPrev[numVertices];
    for (int i = 0; i < numVertices; i++) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    // Run the bellman-ford algorithm
    for (int i = 0; i < numVertices; i++) BF(edgeList, numEdges, start_vertex, BFValue, BFPrev);

    cout << "BFValue: ";
    for (int i = 0; i < numVertices; i++) cout << BFValue[i] << ", ";
    cout << endl;
    cout << "BFPrev: ";
    for (int i = 0; i < numVertices; i++) cout << BFPrev[i] << ", ";
    */

    
    string bf_path = BF_Path(edgeList, numEdges, 67, 117);
    cout << "bf_path = " << bf_path << endl;

    cout << "bf_path_dec_ver = ";
    for (int i = 0; i < bf_path.length(); i++) {
        if (bf_path[i] != ' ')
            cout << int(bf_path[i]) << " ";
    }

    return 0;
}



//support function definition 
void printedge(int edge[]){
    cout << (edge[0]) << ", " << (edge[1]) << ", " << edge[2] << endl;
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

