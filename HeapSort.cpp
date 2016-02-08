//
//  main.cpp
//  HeapSort
//
//  Created by David Futran on 9/17/15.
//  Copyright (c) 2015 David. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class HeapSort{
private:
    int size;
    int* myArray;
public:
    //Constructor:
    HeapSort(int x)
    {
        myArray = new int(x);
        myArray[0]=0;
        size=x;
    }
    //Destructor:
    ~HeapSort()
    {
        delete []myArray;
        myArray=NULL;
    }
    // A function to swap two integers. to be used by bubbleup and bubbledown later in the program
    void swap(int* x, int* y){
        int t = *x;
        *x = *y;
        *y = t;
    }
    //BuildHeap should go through each number and insert it into the heap
    void buildHeap(ifstream& inFile, ofstream& outFile){
        outFile<<"Build Heap:"<<endl;
        int num;
        while (inFile >> num) {
            insertOneDataItem(num);
            outFile<<"insert "<<num<<"       ";
            printTen(inFile,outFile);
            outFile<<endl;
            
        }
        outFile<<endl<<"Final Heap :"<<endl;
        printAll(inFile,outFile);
        outFile<<endl<<endl;
    }
    //while heap is not empty, keep deleting the root
    void deleteHeap(ifstream& inFile, ofstream& outFile){
        outFile<<"Delete Heap:"<<endl;
        while(!isHeapEmpty()){
            outFile<<"insert "<<deleteRoot()<<"       ";
            printTen(inFile,outFile);
            outFile<<endl;
        }
        outFile<<endl<<"Final Heap :"<<endl;
        printAll(inFile,outFile);
        outFile<<endl<<endl;
    }
    //insert an item and call bubblesort which will make sure it ends up in the correct place
    void insertOneDataItem(int x){
        if(isHeapFull()){
            cout<<"Error, the heap is full"<<endl;
            return;
        }
        myArray[0]++;
        myArray[myArray[0]]=x;
        bubbleUp();
    }
    //This method will keep swapping the child (starting at the end) with the parent until it is in the right place based on priority
    void bubbleUp(){
        int temp=myArray[0];//temp will point to the child who will be compared to its parent
        while(temp>1){
            if(myArray[temp]<myArray[temp/2]){
                swap(&myArray[temp],&myArray[temp/2]);
                temp=temp/2;
            } else {
                break;
            }
        }
    }
    //deletes and returns an item, places the last node at the root and it also calls bubbledown so that the root will be rearranged properly
    int deleteRoot(){
        if(isHeapEmpty()){
            cout<<"Error, the heap is empty"<<endl;
            return NULL;
        }
        int temp=myArray[1];
        myArray[1]=myArray[myArray[0]];
        //myArray[myArray[0]]=NULL;
        myArray[0]--;
        bubbleDown();
        return temp;
    }
    //starting with the root, this method will compare it to both children until it finds the right place. then check the child.
    void bubbleDown(){
        int temp=1;//this will be the index of the parent that will then be compared t its children
        while(temp*2<=myArray[0]){
            if(myArray[temp]>myArray[temp*2]||myArray[temp]>myArray[temp*2+1]){//if the node is is smaller then either child then swap it somewhere. otherwise break since the root is in the right place
                if((temp*2+1>myArray[0])||(myArray[temp*2]<myArray[temp*2+1])){//if the right child is past where any nodes are kept or it is smaller than the right child, swap with the left child
                    swap(&myArray[temp],&myArray[temp*2]);
                    temp=temp*2;
                } else {//otherwise swap with the right child
                    swap(&myArray[temp],&myArray[temp*2+1]);
                    temp=temp*2+1;
                }
            } else {
                break;
            }
        }
    }
    
    //Two methods for information on heap:
    bool isHeapEmpty(){
        return(myArray[0]==0);
    }
    bool isHeapFull(){
        return(size==myArray[0]);
    }
    //Two print methods. One does the first ten nodes. The other prints all nodes
    void printTen(ifstream& inFile, ofstream& outFile){//prints the first ten nodes
    
        for(int x=1;x<=10;x++){
            if(x>=myArray[0]+1)break;
            outFile<<myArray[x]<<" ";
        }
    }
    void printAll(ifstream& inFile, ofstream& outFile){//prints all the nodes
        for(int x=1;x<myArray[0]+1;x++)
            outFile<<myArray[x]<<" ";
    }
};
    int main(int argc, char** argv) {
        int sizeHeap=0;//this will hold the size of the heap we want to build
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
        infile.open(myTextFile);
     
        if(!infile.is_open()) cout<<"The File is not open"<<endl;
        else{
            int num;
            while (infile >> num) {
                sizeHeap++;
            }
            infile.close();
        }
        HeapSort heap(sizeHeap);
        
        infile.open(myTextFile);
        ofstream outFile;
        outFile.open(argv[2]);
        if(!infile.is_open()) cout<<"The File is not open"<<endl;
        else{
            heap.buildHeap(infile,outFile);
            heap.deleteHeap(infile,outFile);
            infile.close();
            outFile.close();
        }
        
        return 0;
    
        
    }

