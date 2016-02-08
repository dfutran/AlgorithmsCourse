import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;


public class heapSort {
	private static int[] heapArray;
	private static int size;
	private static Scanner inFile;
	
	public heapSort(int x){
		heapArray = new int[x];
		size=x;
		heapArray[0]=0;
	}
	public static void main(String[] args) throws IOException{
		int count=1;//initialize to 1 so the final variable will be the count+1
		heapSort heap;
		File inputFile = new File(args[0]);
		inFile = new Scanner(inputFile);
		try {
			if(args.length==0)throw new FileNotFoundException("No File Found");
			while (inFile.hasNext() ) {
					String word=inFile.next();//this may seem like a wasted line, but without it i had an infinite loop.
					count++;
			}
			
		} catch (Exception e){System.out.println(e);}
		inFile.close();
		heap = new heapSort(count);
		buildHeap(args[0],args[1]);
		deleteHeap(args[1]);
	}
	
	
	public static void swap(int x, int y, int[] z){
		int t = z[x];
		z[x]=z[y];
		z[y]=t;
	}
	public static void buildHeap(String in, String out){
		File inputFile = new File(in);
		try {
			inFile = new Scanner(inputFile);
			PrintWriter outFile = new PrintWriter(out);
			while (inFile.hasNext() ) {
					String word=inFile.next();
					insertOneDataItem(Integer.parseInt(word));
					outFile.print("insert "+word+"        ");
					printTen(outFile);
					outFile.println();
			}
			inFile.close();
			outFile.println();
			outFile.println("Final Heap:");
			printHeap(outFile);
			outFile.println();
			outFile.close();
		} catch (Exception e){System.out.println(e);}
		
	}
	public static void deleteHeap(String string){
		try {
			PrintWriter outFile = new PrintWriter(new FileWriter(string,true));
			outFile.println();
			outFile.println("Delete Heap:");
			while(!isHeapEmpty()){
				outFile.print("delete ");
				outFile.print(deleteRoot());
				outFile.print("        ");
				printTen(outFile);
				outFile.print("\n");
			}
			outFile.println();
			outFile.println("Final Heap: \n");
			printHeap(outFile);
			outFile.print("\n\n");
			outFile.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}
	public static void insertOneDataItem(int x){
		if(isHeapFull()){
            System.out.println("Error, the heap is full");
            return;
        }
        heapArray[0]++;
        heapArray[heapArray[0]]=x;
        bubbleUp();
	}
	public static int deleteRoot(){
		if(isHeapEmpty()){
            System.out.println("Error, the heap is empty");
            return 0;
        }
        int temp=heapArray[1];
        heapArray[1]=heapArray[heapArray[0]];
        heapArray[heapArray[0]--]=0;
        bubbleDown();
        return temp;
	}
	public static void bubbleUp(){
		int temp=heapArray[0];//temp will point to the child who will be compared to its parent
        while(temp>1){
            if(heapArray[temp]<heapArray[temp/2]){
                swap(temp,temp/2,heapArray);
                temp=temp/2;
            } else {
                break;
            }
        }
	}
	public static void bubbleDown(){
		int temp=1;//this will be the index of the parent that will then be compared t its children
        while(temp*2<=heapArray[0]){
        	int z=heapArray[temp]+1;//This needs to be greater than our current parent in case heapArray[temp*2+1] doesnt exist.
        	if(temp*2+1<=heapArray[0])z=heapArray[temp*2+1];
            if((heapArray[temp]>heapArray[temp*2])||(heapArray[temp]>z)){//if the node is is smaller then either child then swap it somewhere. otherwise break since the root is in the right place
                if((temp*2+1>heapArray[0])||(heapArray[temp*2]<heapArray[temp*2+1])){//if the right child is past where any nodes are kept or it is smaller than the right child, swap with the left child
                    swap(temp,temp*2,heapArray);
                    temp=temp*2;
                } else {//otherwise swap with the right child
                    swap(temp,temp*2+1,heapArray);
                    temp=temp*2+1;
                }
            } else {
                break;
            }
        }

	}
	public static boolean isHeapEmpty(){
		return (heapArray[0]==0);
	}
	public static boolean isHeapFull(){
		return (heapArray[0]==size);
	}
	public static void printTen(PrintWriter out){//prints first ten nodes
		for(int x=1;x<=10;x++){
            if(x>=heapArray[0]+1)break;
            out.print(heapArray[x]+" ");
        }
	}
	public static void printHeap(PrintWriter out){//prints all nodes
		for(int x=1;x<heapArray[0]+1;x++)
            out.print(heapArray[x]+" ");
	}
}
