import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class main {
	private static Scanner inFile;
	private static LinkedStack myStack = new LinkedStack();
    private static int largestNum = 0;
	public static void main(String[] argv) throws IOException{ 
		try {
			if(argv.length==0) throw new FileNotFoundException("No File Found");
			File inputFile = new File(argv[0]);
			inFile = new Scanner(inputFile);
			while (inFile.hasNext() ) {
				String word=inFile.next();//this may seem like a wasted line, but without it i had an infinite loop.
				largestNum=findMax(Integer.parseInt(word),largestNum);
				myStack.push(word);
			}
			inFile.close();
			PrintWriter outFile = new PrintWriter(argv[1]);
			//initialzie RadixSort and set total digits
			RadixSort radix = new RadixSort();
			int total = findTotalDigits(largestNum);
			radix.setTotalDigits(total);
          
			//get current digit from Node, hash and add to tail of queue
			String fromStackTop = myStack.pop();
			while(true){
				String myData = fromStackTop;
				String myDigit = myData.substring(myData.length()-1);
              
				int index = radix.getCurrentHashTable().hashFunc(myDigit);              
				radix.getCurrentHashTable().getMyHash(index).addTail(fromStackTop);
				outFile.println();
				outFile.println(myData+" is sent to queue "+index);
				radix.getCurrentHashTable().getMyHash(index).printQueue(outFile);
				fromStackTop = myStack.pop();
				if(myStack.isEmpty()) break;
			}
			//print out the 10 buckets of the hashtable
			outFile.println();
			outFile.println("Current HashTable");
			outFile.println();
			radix.getCurrentHashTable().printTable(outFile);
          
			radix.increaseCurrentDigit();
			radix.switchTables();
          
			//5.0:
			while(radix.getTotalDigits() > radix.getCurrentDigit()){
				//outFile<<endl<<"Transferring data to next HashTable with new CurrentDigit"<<endl;
				radix.resetCurrentTable(); 
				for(int i =0;i < 10;i++){
					radix.getPreviousHashTable().getMyHash(i).deleteHead();
					while(!radix.getPreviousHashTable().getMyHash(i).isEmpty()){
						String tempData = radix.getPreviousHashTable().getMyHash(i).deleteHead();
						int index = radix.getCurrentHashTable().getIndex(tempData, radix.getCurrentDigit());
						radix.getCurrentHashTable().getMyHash(index).addTail(tempData);
						outFile.println(tempData +" is sent to queue " + index);
						radix.getCurrentHashTable().getMyHash(index).printQueue(outFile);
						outFile.println();
					}
				}
             
              outFile.println();
              outFile.println("Print Current HashTable");
              radix.getCurrentHashTable().printTable(outFile);
              outFile.println();
              radix.switchTables();
              radix.increaseCurrentDigit();
			} 
			outFile.println();
            outFile.println("Data successfully Sorted");
            outFile.close();
       
		} catch (Exception e){System.out.println(e);}
          
      }
	private static int findMax(int x, int y) {
		if(x>y) return x;
		else return y;
	}
	//A recursive function to find how many digits there are in a number
	private static int findTotalDigits(int x){
	    if(x == 0) return 0;
	    else return (findTotalDigits(x/10) + 1);
	}
	
  //The catch argument is strangely neccessary. without them, i got a segmentation fault
  
}

public class Node {
    private String data;
    private Node next;
    //Constructors:
    public Node(){
        next = null;
    }
    public Node(String x){
        data = x;
        next = null;
    }
    public void setData(String x){
        data=x;
    }
    public void setNext(Node n){
        next = n;
    }
    public String getData(){
        return data;
    }
    public Node getNext(){
        return next;
    }
};

public class Queue {

    private Node head;
    private Node tail;

    public Queue(){
    	head = new Node("dummy");
    	tail=head;
    }
    
    public void addTail(String x){
            tail.setNext(new Node(x));
            tail = tail.getNext();
    }
    public String deleteHead(){
        String temp = head.getData();
        head = head.getNext();
        return temp;
    }

	boolean isEmpty(){
        return (head==null);
    }
	
    void printQueue(PrintWriter output){
        if(head!=null){
            Node temp = head;
            output.print("Head ");
            while(temp.getNext()!= null){
                String x;//This will hold the data of the next node. Store -1 if it is empty, otherwise store the data
                if(temp.getNext() == null) x="NULL";
                else x = temp.getNext().getData();
                output.print("--> ("+temp.getData()+","+x+") ");
                temp=temp.getNext();
            }
            output.print("--> Tail");
            output.println();
        } else {
            output.println("Head --> (dummy,NULL) --> Tail");
        }
    }
};


public class RadixSort {


    private HashTable hashes[]= new HashTable[2];
    private int currentTable;
    private int currentDigit;
    private int previousTable;
    private int totalDigits;

    public RadixSort(){
        currentTable = 0;
        currentDigit = 0;
        previousTable = 1;
        hashes[0] = new HashTable();
        hashes[1] = new HashTable();
    }
    public void setTotalDigits(int total){
        totalDigits = total;
    }
    public int getCurrentDigit(){
        return currentDigit;
    }
    public void increaseCurrentDigit(){
        currentDigit++;
    }
    public HashTable getCurrentHashTable(){
        return hashes[(currentTable%2)];
    }
    public HashTable getPreviousHashTable(){
        return hashes[previousTable%2];
    }
    public void switchTables(){
        currentTable=(currentTable +1)%2;
        previousTable =(previousTable + 1)%2;
    }
    public int getPreviousTable(){
        return previousTable;
    }
    public int getCurrentTable(){
        return currentTable;
    }
    public int getTotalDigits(){
        return totalDigits;
    }
    public void resetCurrentTable(){
    	hashes[currentTable] = new HashTable();
    }
 
};


public class HashTable {

    private Queue[] myHash = new Queue[10];

    public HashTable(){
        //Queue[] myHash = new Queue[size];
        for(int i =0; i<10; i++){
            myHash[i] = new Queue();//initalize all 10 hashtables
        }
    }
    
    public void printTable(PrintWriter out){
        for(int i =0;i<10;i++){
            out.println("Printing Queue "+i+":");
            myHash[i].printQueue(out);
        }
    }
    public int hashFunc(String digit){
        return Integer.parseInt(digit);
    }
    
    public Queue getMyHash(int x){
    	if(x>9){
    		System.out.print("error returning hash 0");
    		return myHash[0];
    	}
    	else return myHash[x];
    }
    public int getIndex(String data, int x){
        if(data.length()<(x+1))return 0;
        else if(data.length()==(x+1)){
            String temp = data.substring(0,1);
            return Integer.parseInt(temp);
        } else{
            String temp = data.substring((data.length()-x-1),data.length()-x);
            return Integer.parseInt(temp);
        }
    }
	
};

public class LinkedStack{
	private Node Top;
	    
	public LinkedStack(){
		Top = new Node();
	}
	    
	public String pop(){
		if(isEmpty()){
			return ("empty");
	    }else{
	    	String temp = Top.getData();
	        Top = Top.getNext();
	        return temp;
	    }
	}
	public void push(String x){
		if(isEmpty()) Top = new Node(x);
		else{
			Node temp = new Node(x);
			temp.setNext(Top);
	    	Top=temp;
	    }
	}
	public boolean isEmpty(){
	        return (Top == null);
	}
	public Node getTop(){
	        return Top;
	}
	void printStack(PrintWriter output){
		Node temp = Top;
		output.println("Top ");
	    while(temp.getNext()!= null){
	    	String x;//This will hold the data of the next node. Store NULL if it is empty, otherwise store the data
	    	if(temp.getNext() == null) x="NULL";
	    	else x = temp.getNext().getData();
	    	output.println("--> ("+temp.getData()+","+x+") ");
	    	temp=temp.getNext();
	    }
	}
}
