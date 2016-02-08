//
//  main.cpp
//  Prim
//
//  Created by David Futran on 11/23/15.
//  Copyright © 2015 David. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

class graphNode{
private:
    int id;
    graphNode* next;
public:
    //Constructors:
    graphNode(){
        next = NULL;
    }
    graphNode(int x){
        id=x;
        next = NULL;
    }
    //Destructor:
    ~graphNode(){
        delete next;
    }
    void insert(){
        
    }
    void deleteNode(){
        
    }
    graphNode* getNext(){
        return next;
    }
    void setNext(graphNode* x){
        next = x;
    }
    bool isEmpty(){
        return (next==NULL);
    }
    void printNode(ofstream& out){
        out<<" "<<id;
    }
    int getID(){
        return id;
    }
};
class graphList{
private:
    graphNode* head;
    string name;
public:
    graphList(string x){
        head = new graphNode(-999);
        name=x;
    }
    void insert(int x){
        graphNode* temp = new graphNode(x);
        temp->setNext(head->getNext());
        head->setNext(temp);
    }
    void deleteNode(int x){
        graphNode* temp = head;
        while(temp->getNext() != NULL && temp->getNext()->getID() != x) temp = temp->getNext();
        temp->setNext(temp->getNext()->getNext());
    }
    void print(ofstream& out){
        out<<endl<<"Printing Set "<<name<<":"<<endl;
        graphNode* temp = head;
        while(temp->getNext() != NULL){
            temp = temp->getNext();
            temp->printNode(out);
        }
        out<<endl;
    }
    bool isEmpty(){
        return(head->getNext()==NULL);
    }
    graphNode* getHead(){
        return head;
    }
};
class edgeNode{
private:
    int Ni;
    int Nj;
    int cost;
    edgeNode* next;
public:
    //Constructors:
    edgeNode(){
        next = NULL;
    }
    edgeNode(int x, int y, int c){
        Ni=x; Nj=y;
        cost = c;
        next = NULL;
    }
    //Destructor:
    ~edgeNode(){
        delete next;
    }
    void insert(){
        
    }
    void deleteNode(){
        
    }
    edgeNode* getNext(){
        return next;
    }
    void setNext(edgeNode* x){
        next = x;
    }
    void printNode(ofstream& out){
        out<<Ni<<" "<<Nj<<" "<<cost<<" "<<endl;
    }
    int getNi(){
        return Ni;
    }
    int getNj(){
        return Nj;
    }
    int getCost(){
        return cost;
    }
};
class edgeList{
private:
    edgeNode* head;
public:
    edgeList(){
        head = new edgeNode(-999,-999,-999);
        
    }
    void insert(int x, int y, int z){
        edgeNode* newNode = new edgeNode(x,y,z);
        edgeNode* spot = head;
        if(head->getNext()==NULL) {
            head->setNext(newNode);
        } else {
            while((spot->getNext() != NULL)&&(spot->getNext()->getCost() <= z )){
                spot = spot->getNext();
            }
            newNode->setNext(spot->getNext());
            spot->setNext(newNode);
        }
    }
    void print(ofstream& out){
        edgeNode* temp = head;
        while(temp->getNext()!=NULL){
            temp=temp->getNext();
            temp->printNode(out);
        }
        out<<endl;
    }
    edgeNode* getHead(){
        return head;
    }
    void printTen(ofstream& out){
        out<<endl<<"Printing Ten Nodes:"<<endl;
        edgeNode* temp = head;
        int count=0;
        while(temp->getNext()!=NULL && count<10){
            temp=temp->getNext();
            temp->printNode(out);
            count++;
        }
        out<<endl;
    }
};
class PrimMST{
private:
    int numG;
    int totalCost=0;
    int* graphArray;
    edgeList* myEdge = new edgeList();
    graphList* A = new graphList("A");
    graphList* B = new graphList("B");
    edgeList* MSTofG = new edgeList();
public:
    PrimMST(int x){
        numG=x;
        graphArray =new int[x+1];
        for (int i=0; i<x+1; i++) {
            graphArray[i] = 0;
        }
    }
    void increase(int x, int y){
        graphArray[x]++;
        graphArray[y]++;
    }
    void printArray(ofstream& out){
        out<<endl<<"Graph Node 1D Array:"<<endl;
        for(int i=0;i<numG+1;i++){
            out<<"Node: "<<i<<" has value "<<graphArray[i]<<endl;
        }
    }
    int* getArray(){
        return graphArray;
    }
    void findFirstEdge(){
        int code=0;
        edgeNode* temp = myEdge->getHead();
        edgeNode* behind = temp;
        while(temp->getNext()!=NULL){
            behind = temp;
            temp=temp->getNext();
            if((code = inAOnce(temp->getNi(),temp->getNj()))>0){
                break;
            }
        }
        totalCost+=temp->getCost();
        behind->setNext(temp->getNext());
        temp->setNext(MSTofG->getHead()->getNext());
        MSTofG->insert(temp->getNi(),temp->getNj(),temp->getCost());
       
        if(code==1){
            A->insert(temp->getNj());
            B->deleteNode(temp->getNj());
        } else {
            A->insert(temp->getNi());
            B->deleteNode(temp->getNi());
        }
    }
    int inAOnce(int x, int y){
        int itIs = 0;
        int e = 0, f = 1;
        graphNode* tempA = A->getHead();
        while(tempA->getNext()!=NULL){
            tempA=tempA->getNext();
            if (x == tempA->getID()) e = 2;
            if (y == tempA->getID()) f = 2;
        }
        if(e==2&&f!=2) itIs = 1;
           else if(e!=2&&f==2) itIs =2;
        return itIs;
    }
    edgeList* getMyEdge(){
        return myEdge;
    }
    edgeList* getMSTofG(){
        return MSTofG;
    }
    graphList* getA(){
        return A;
    }
    graphList* getB(){
        return B;
    }
    int getTotalCost(){
        return totalCost;
    }
};

int main(int argc, char** argv) {
    int numberofNodes;
    try{
        if (argc < 2) {
            cout << "usage : " << argv[0] << " <input file>  <output file>\n";
            return -1;
        }
        cout << "Number of arguments : " << argc << "\n\n";
        for (int i = 0; i < argc; ++i)
            cout << "argv["<< i << "] : " << argv[i] << "\n";
        
        ifstream infile;
        string myTextFile = argv[1];
        //I had a strange error, where i couldnt open the argv[1] file directly. Instead I had to convert it to a string and open the string
        infile.open(argv[1]);
        
        if(!infile.is_open()) cout<<"The File is not open"<<endl;
        //else if(!outFile.is_open()) cout<<"The File is not open"<<endl;
        else{
            //Getting input file. Inserting it into LinkedList and Image array
            int a, b, c;
            infile >> a;
            numberofNodes = a;
            PrimMST graph(a);
            while (infile >> a >> b >> c) {
                graph.getMyEdge()->insert(a,b,c);
                graph.increase(a,b);
            }
            infile.close();
            //Print linkedlist for debugging purposes:
            
            ofstream outFile;
            outFile.open(argv[3]);
            graph.printArray(outFile);
            graph.getMyEdge()->printTen(outFile);
            
            int temp = 0;
            while(true){
                if(graph.getArray()[temp]==0) temp++;
                else break;
            }
            graph.getA()->insert(temp++);
            while(temp<numberofNodes+1){
                if(graph.getArray()[temp]!=0) graph.getB()->insert(temp);
                temp++;
            }
            
            while(!graph.getB()->isEmpty()){
                graph.findFirstEdge();
                outFile<<endl<<"Printing edge list:"<<endl;
                graph.getMyEdge()->print(outFile);
                graph.getA()->print(outFile);
                graph.getB()->print(outFile);
                outFile<<endl<<"printing MST: "<<endl;
                graph.getMSTofG()->print(outFile);
            }
            outFile.close();
            outFile.open(argv[2]);
            outFile<<"Final MST of G:"<<endl;
            graph.getMSTofG()->print(outFile);
            outFile<<endl<<"Total Cost: "<<graph.getTotalCost()<<endl;
            outFile.close();
        }
    }catch(...){
        cerr<<"Error has occured somewhere.\n";
    }
    return 0;
}
