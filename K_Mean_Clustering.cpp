//
//  main.cpp
//  KMean
//
//  Created by David Futran on 11/18/15.
//  Copyright © 2015 David. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

class Node{
public:
    int x;
    int y;
    int clusterLabel;
    double distance;
    Node* next;
public:
    //Constructors:
    Node(){
        next = NULL;
    }
    Node(int ex, int ey, int c){
        x=ex; y=ey;
        clusterLabel = c;
        distance=0;
        next = NULL;
    }
    //Destructor:
    ~Node(){
        //delete next;
    }
    void setLabel(int a){
        clusterLabel = a;
    }
    void setNext(Node n){
        next = &n;
    }
    Node* getNext(){
        return next;
    }
    void printNode(){//for debugging
        cout<<"Node: "<<x<<","<<y<<","<<clusterLabel<<","<<distance<<".";
    }
    int getLabel(){
        return clusterLabel;
    }
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    void setNode(double d, int c){
        distance = d;
        clusterLabel = c;
    }
    double getDistance(){
        return distance;
    }
};
class LinkedList{
private:
    Node* dummyHead = new Node(-9999,-9999,-9999);
public:
    //Constructor:
    LinkedList(){}
    //Destructor:
    ~LinkedList()
    {
        while(dummyHead->next!= NULL){
            Node* temp = dummyHead;
            dummyHead = dummyHead->next;
            temp = NULL;
            
        }
        delete dummyHead;
    }
    void insert(int xx, int yy, int c){
        Node* newNode = new Node(xx,yy,c);
        if(dummyHead->next == NULL) dummyHead->next=newNode;
        else{
            newNode->next=dummyHead->next;
            dummyHead->next=newNode;
        }
    }
    Node deleteNode(int x){
        Node* temp=dummyHead->next;
        dummyHead->next=dummyHead->next->next;
        return *temp;
    }
    void printAllNodes(ofstream& out){
        Node* temp=dummyHead;
        while(temp->next!=NULL){
            temp=temp->next;
            out<<temp->getX()<<" "<<temp->getY()<<" "<<temp->getLabel()<<endl;
        }
    }
    Node* getHead(){
        return dummyHead;
    }
    
};
class KMean{
    struct xycoord{
        int x=0;
        int y=0;
    };
private:
    int k = 4;
    xycoord xyArray[4];
    LinkedList* lHead = new LinkedList;
    int numrow;
    int numcol;
    int **imageArray;
public:
    KMean(int x, int y){
        numcol=x;
        numrow = y;
        imageArray= new int*[x];
        for(int i = 0;i<x;++i){
            imageArray[i]=new int[y];
        }
    }
    LinkedList* getList(){
        return lHead;
    }
    void cluster(){
        int total[4];
        int numberx[4];
        int numbery[4];
        for(int i=0;i<4;i++){
            total[i]=0;
            numberx[i]=0;
            numbery[i]=0;
        }
        Node temp = *lHead->getHead();
        while(temp.next != NULL){
            temp=*temp.next;
            int place = temp.getLabel()-1;
            numberx[place]+=temp.getX();
            numbery[place]+=temp.getY();
            total[place]+=1;
        }
        for(int i = 0;i<k;i++){
            xyArray[i].x=numberx[i]/total[i];
            xyArray[i].y=numbery[i]/total[i];
        }
    }
    void plot(int x, int y, int z){
        imageArray[x][y] = z;
    }
    void replot(){
        imageArray= new int*[numrow];
        for(int i = 0;i<numrow;++i){
            imageArray[i]=new int[numcol];
        }
        Node* temp = lHead->getHead();
        while(temp->getNext()!= NULL){
            temp=temp->getNext();
            plot(temp->getX(),temp->getY(),temp->getLabel());
        }
    }
    void displayImage(ofstream& out){
        for(int i=0;i<numrow;i++){
            for(int j=0;j<numcol;j++){
                if(imageArray[i][j]>0) out<<imageArray[i][j];
                else out<<" ";
            }
            out<<endl;
        }
    }
    bool findAllDistances(){
        bool DoesSomethingChange = false;
        Node* n = lHead->getHead();
        double distance;
        int label;
        double temp;
        while(n->next != NULL){
            n=n->next;
            distance=10000;
            for(int i = 0;i<k;i++){
                //math split up for readability and potential overflow:
                double dx=(xyArray[i].x)-(n->getX());
                double dy=(xyArray[i].y)-(n->getY());
                if(dx<0) dx = dx*(-1);//i had errors where it would be negative
                if(dy<0) dy = dy*(-1);
                double x = (dx*dx)+(dy*dy);
                temp = sqrt(x);
                if(temp <distance){
                    distance = temp;
                    label = i;
                }
                //end of math
            }
            if(n->getLabel()!=label+1){
                n->setNode(distance, label+1);
                DoesSomethingChange = true;
            }
            
        }
        return DoesSomethingChange;
    }
    
    void findFirstDistance(){
        Node* n = lHead->getHead();
        double distance;
        while(n->next != NULL){
            n=n->next;
            int label = n->getLabel()-1;
            double dx=xyArray[label].x-n->getX();
            double dy=xyArray[label].y-n->getY();
            double x = (dx*dx)+(dy*dy);
            if(x<0) x*=-1;//just in case of overflow
            distance = sqrt(x);
            n->setNode(distance, label+1);
        }
    }
    void printInfo(ofstream& out){
        out<<k<<endl;
        out<<numrow<<" "<<numcol<<endl;
    }
};

int main(int argc, char** argv) {
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
        int label = -1;
        int a, b;
        infile >> a >> b;
        KMean km(a,b);
        while (infile >> a >> b) {
            label = (label+1)%4;
            km.plot(a,b,label+1);
            km.getList()->insert(a,b,label+1);
        }
        infile.close();
        //Print linkedlist for debugging purposes:
        
        ofstream outFile;
        outFile.open(argv[2]);
        km.replot();
        outFile<<"Image Array:";
        km.displayImage(outFile);
        outFile<<endl;

        //Call cluster method to find the centroids
        km.cluster();
        km.findFirstDistance();
        //Find the distance from all nodes to all clusters and see if label must be changed. If a label was changed then re-cluster.
        while(km.findAllDistances()){
            km.cluster();
            km.replot();
            outFile<<"Image Array:";
            km.displayImage(outFile);
            outFile<<endl;
        }
        outFile.close();
        outFile.open(argv[3]);
        km.printInfo(outFile);
        km.getList()->printAllNodes(outFile);
        outFile<<endl<<endl;
        
        //At the point everything should be properly labeled and then output the linked list with their labels.
       
        outFile.close();
    }
    }catch(...){
        cerr<<"Error has occured somewhere.\n";
    }
    return 0;
}


