//
//  main.cpp
//  FutranD_InsertionSort_CPP
//
//  Created by David Futran on 9/7/15.
//  Copyright (c) 2015 David. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class LinkedList{
    struct Node
    {
        int data;
        Node* next;
    };
private:
    Node *dummyHead = new Node;
public:
    //Constructor:
    LinkedList()
    {
        dummyHead->data=-9999;
    }
    //Destructor:
    ~LinkedList()
    {
        Node *temp = dummyHead;
        while(temp!=NULL){
            dummyHead=dummyHead->next;
            delete temp;
            temp=dummyHead;
        }
    }
    void insert(int x)
    {
        Node* temp=dummyHead;
        Node* newNode = new Node();
        newNode->data=x;
        //First find the correct place to insert the new node
        while(temp->next!= NULL && temp->next->data<=x) temp=temp->next;
        //if there is a duplicate node, print it out, but dont add a node
        if(temp->data == x) cout<<"Duplicate found: "<<x<<endl;
        else{
            newNode->next=temp->next;
            temp->next=newNode;
        }
    }
    void printTenNodes(ofstream& output)
    {
        Node* temp=dummyHead;
        if(dummyHead->next==NULL){
            cout<<"Lacking a second node"<<endl;
            return;
        }
        cout<<"listhead"<<" -->"<<" ("<<dummyHead->data<<","<<temp->next->data<<")";
        output<<"listhead"<<" -->"<<" ("<<dummyHead->data<<","<<temp->next->data<<")";
        for(int i=0;i<9;i++){
            if(temp->next== NULL) break;
            temp=temp->next;
            int x;//This will hold the data of the next node. Store -1 if it is empty, otherwise store the data
            if(temp->next== NULL) x=-1;
            else x=temp->next->data;
            cout<<"--> ("<<temp->data<<","<<x<<") ";
            output<<"--> ("<<temp->data<<","<<x<<") ";
        }
    
    }
};
int main(int argc, char** argv) {
    LinkedList* myList = new LinkedList();
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
    
    ofstream outFile;
    outFile.open(argv[2]);
    if(!infile.is_open()) cout<<"The File is not open"<<endl;
    else{
        int num;
        while (infile >> num) {
            // will parse every number in the file skipping over whitespace ...
            //insert the next number
            myList->insert(num);
            //Then print up to ten nodes of the list
            myList->printTenNodes(outFile);
            cout<<endl;
            outFile<<endl;
        }
        infile.close();
        outFile.close();
    }
    return 0;
}
