//
//  main.cpp
//  RadixSort
//
//  Created by David Futran on 10/18/15.
//  Copyright © 2015 David. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Node{
public:
    string data;
    Node* next;
public:
    //Constructors:
    Node(){
        next = NULL;
    }
    Node(string x){
        data = x;
        next = NULL;
    }
    //Destructor:
    ~Node(){
        delete next;
    }
    void setData(string x){
        data=x;
    }
    void setNext(Node n){
        next = &n;
    }
    string getData(){
        return data;
    }
    Node getNext(){
        return *next;
    }
};

class LinkedStack{
private:
    Node* Top;
public:
    LinkedStack(){}//empty
    
    string pop(){
        if(isEmpty()){
            return "empty";
        }else{
            string temp = Top->getData();
            Top = Top->next;
            
            return temp;
        }
    }
    void push(string x){
        if(isEmpty()) Top = new Node(x);
        else{
            Node* temp = new Node(x);
            temp->next=(Top);
            Top=temp;
        }
    }
    bool isEmpty(){
        return (Top == NULL);
    }
    Node* getTop() const{
        return Top;
    }
    void printStack(ofstream& output){
        Node* temp = Top;
        output<<"Top ";
        while(temp->next!= NULL){
            string x;//This will hold the data of the next node. Store NULL if it is empty, otherwise store the data
            if(temp->next == NULL) x="NULL";
            else x = temp->next->data;
            output<<"--> ("<<temp->data<<","<<x<<") ";
            temp=temp->next;
        }
    }
};

class Queue{
private:
    Node* dummy = new Node("dummy");
    Node* head= dummy;
    Node* tail= dummy;
public:
    Queue(){}//empty
    
    void addTail(string x){
        if(isEmpty()){
            head=new Node("dummy");
            tail=new Node(x);
            head->next=tail;
        }else{
            tail->next = new Node(x);
            tail = tail->next;
        }
    }
    string deleteHead(){
        string temp = head->data;
        head = head->next;
        return temp;
    }
    bool isEmpty(){
        return (head==NULL);
    }
    void printQueue(ofstream& output){
        if(head!=NULL){
            Node* temp = head;
            output<<"Head ";
            while(temp->next!= NULL){
                string x;//This will hold the data of the next node. Store -1 if it is empty, otherwise store the data
                if(temp->next== NULL) x="NULL";
                else x = temp->next->data;
                output<<"--> ("<<temp->data<<","<<x<<") ";
                temp=temp->next;
            }
            output<<"--> Tail"<<endl;
        } else {
            output<< "Head --> (dummy,NULL) --> Tail"<<endl;
        }
    }
};

class HashTable{
private:
    int size = 10;
    Queue* myHash;
public:
    HashTable(){
        myHash = new Queue[size];
        for(int i =0; i<size; i++){
            myHash[i] = *new Queue();//initalize all 10 hashtables
        }
    }
    void printTable(ofstream& out){
        for(int i =0;i<10;i++){
            out<<"Printing Queue "<<i<<":"<<endl;
            myHash[i].printQueue(out);
        }
    }
    int hashFunc(string digit){
        return stoi(digit);
    }
    
    Queue* getMyHash(){
        return myHash;
    }
    int getIndex(string data, int x){
        if(data.length()<(x+1))return 0;
        else if(data.length()==(x+1)){
            string temp = data.substr(0,1);
            return stoi(temp);
        } else{
            //string myDigit = &data.at((data.length()-x-1));
            string temp = data.substr((data.length()-x-1),1);
            return stoi(temp);
        }
    }
};

class RadixSort{
private:
    HashTable* hashes[2];
    int currentTable;
    int currentDigit;
    int previousTable;
    int totalDigits;
public:
    RadixSort(){
        currentTable = 0;
        currentDigit = 0;
        previousTable = 1;
        hashes[0] = new HashTable();
        hashes[1] = new HashTable();
    }
    void setTotalDigits(int total){
        totalDigits = total;
    }
    int getCurrentDigit(){
        return currentDigit;
    }
    void increaseCurrentDigit(){
        currentDigit++;
    }
    HashTable getCurrentHashTable(){
        return *hashes[currentTable];
    }
    HashTable getPreviousHashTable(){
        return *hashes[previousTable];
    }
    void switchTables(){
        int x=currentTable;
        currentTable=previousTable;
        previousTable = x;
    }
    int getPreviousTable(){
        return previousTable;
    }
    int getCurrentTable(){
        return currentTable;
    }
    int getTotalDigits(){
        return totalDigits;
    }
 
};
//A recursive function to find how many digits there are in a number
int findTotalDigits(int x){
    if(x == 0) return 0;
    else return (findTotalDigits(x/10) + 1);
}

int main(int argc, const char * argv[]) {
    cout<<"begin"<<endl;
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
        infile.open(myTextFile);
        ofstream outFile;
        //I had this next bit of code to work with a catch argument, but i would strangely get a segmentation fault if it was in the code
        /*
        if(!infile){
            throw "Fail";
        }
        */
        outFile.open(argv[2]);
        if(!infile.is_open()) cout<<"The File is not open"<<endl;
        else{
            //start with filling up the stack
            LinkedStack* myStack;
            string n;
            int largestNum =0;
            int num;
            myStack = new LinkedStack();
            while (infile >> num) {
                if(num>largestNum) largestNum=num;
                string temp = to_string(num);
                myStack->push(temp);
            }
            outFile<<endl;
            infile.close();
            
            //finally print the stack and largest number
            outFile<<"Final Stack:"<<endl;
            myStack->printStack(outFile);
            outFile<<endl<<endl<<"The largest number in input data is: "<<largestNum<<endl<<endl;
            
            //initialzie RadixSort and set total digits
            RadixSort radix;
            int total = findTotalDigits(largestNum);
            radix.setTotalDigits(total);
            
            //get current digit from Node, hash and add to tail of queue
            string fromStackTop = myStack->pop();
            while(!(fromStackTop == "empty")){
                string myData = fromStackTop;
                string myDigit = &myData.at(myData.length()-(radix.getCurrentDigit()+1));
                
                int index = radix.getCurrentHashTable().hashFunc(myDigit);
                //Node* in = new Node(fromStackTop);
                
                radix.getCurrentHashTable().getMyHash()[index].addTail(fromStackTop);
                outFile<<myData <<" is sent to queue " <<index<<endl;
                radix.getCurrentHashTable().getMyHash()[index].printQueue(outFile);
                outFile<<endl;
                fromStackTop = myStack->pop();
            }
            //print out the 10 buckets of the hashtable
            outFile<<endl<<"Current HashTable"<<endl<<endl;
            radix .getCurrentHashTable().printTable(outFile);
            
            radix.increaseCurrentDigit();
            radix.switchTables();
            
            //5.0:
            while(radix.getTotalDigits()>radix.getCurrentDigit()){
                outFile<<endl<<"Transferring data to next HashTable with new CurrentDigit"<<endl;
                radix.getCurrentHashTable() = *new HashTable();
                for(int i =0;i < 10;i++){
                    radix.getPreviousHashTable().getMyHash()[i].deleteHead();
                    while(!radix.getPreviousHashTable().getMyHash()[i].isEmpty()){
                        string tempData = radix.getPreviousHashTable().getMyHash()[i].deleteHead();
                        int index = radix.getCurrentHashTable().getIndex(tempData, radix.getCurrentDigit());
                        radix.getCurrentHashTable().getMyHash()[index].addTail(tempData);
                        outFile<<tempData <<" is sent to queue " <<index<<endl;
                        radix.getCurrentHashTable().getMyHash()[index].printQueue(outFile);
                        outFile<<endl;
                    }
                }
                outFile<<endl<<"Print Current HashTable"<<endl<<endl;
                radix .getCurrentHashTable().printTable(outFile);
            
                radix.switchTables();
                radix.increaseCurrentDigit();
            
            }
            outFile<<endl<<"Data successuly Sorted"<<endl;
            outFile.close();
        }
    //The catch argument is strangely neccessary. without them, i got a segmentation fault
    }catch(...){
        cerr<<"Error has occured somewhere.\n";
    }
    return 0;
    
}
