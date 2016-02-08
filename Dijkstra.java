import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class main {
	private static Scanner inFile;
	private static Scanner inFile2;
	public static void main(String[] argv) throws IOException{ 
		try {
			if(argv.length==0) throw new FileNotFoundException("No File Found");
			File inputFile = new File(argv[0]);
			File inputFile2 = new File(argv[1]);
			inFile = new Scanner(inputFile);
			inFile2 = new Scanner(inputFile2);
			String word = inFile.next();
			int nodes = Integer.parseInt(word);
			String word2 = inFile2.next();
			int source = Integer.parseInt(word2);
			DSSS graph = new DSSS(nodes, source);
			inFile2.close();
			System.out.println("INPUT:"+source+" "+nodes);
			graph.updateBestCostArray(source, 0);
			
			//Step 1:
			while (inFile.hasNext() ) {
				int x,y,z;
				x= Integer.parseInt(inFile.next());
				y= Integer.parseInt(inFile.next());
				z= Integer.parseInt(inFile.next());
				System.out.println("INPUT:"+x+" "+y+" "+z);
				graph.plotmatrix(x,y,z);
			}
			inFile.close();
			inFile2.close();
			//Creating them both to erase what is already on it
			PrintWriter outFile = new PrintWriter(argv[2]);
			PrintWriter outFile2 = new PrintWriter(argv[3]);
			
			graph.debugPrint(argv[3]);
			graph.dijkstra(source, argv[3]);

			outFile.println("The graph has " + nodes +" nodes, the source node is "+ source);
			outFile.println();
			outFile.close(); 
			outFile2.close();
			
			//Everything is done, so print out final tree:
			for(int i=1;i<nodes+1;i++) graph.findPrintShortestPath(i, argv[2]);

		} catch (Exception e){System.out.println(e);}
      }
}

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
public class DSSS {
	private int N;
	private int sourceNode;
	private int minNode;
	private int currentNode;
	private int newCost;
	private int[][] costMatrix;
	private int[] fatherArray;
	private int[] markedArray;
	private int[] bestCostArray;
	
	public DSSS(int x, int y){
		N=x;
		sourceNode = y;
		costMatrix = new int[x+1][x+1];
		for(int i=0;i<x+1;i++){
			for(int j=0;j<x+1;j++){
				if (i==j) costMatrix[i][j]=0;
				else costMatrix[i][j] = 99999;
			}
		}
		costMatrix[1][1]=0;
		fatherArray = new int[x+1];
		markedArray = new int[x+1];
		bestCostArray = new int[x+1];
		
		//Initialize the two arrays
		for(int i =0; i< N+1; i++){
			fatherArray[i] = i;
			bestCostArray[i]= 99999;
		}	
	}
	public void plotmatrix(int x, int y, int z){
		costMatrix[x][y]=z;
	}
	public void updateMarkedArray(int x){
		markedArray[x] = 1; 
	}
	public void updateFatherArray(int x, int y){
		fatherArray[x] = y; 
	}
	public void updateBestCostArray(int x, int y){
		bestCostArray[x]=y;
	}
	public void updateMinNode(int x){
		minNode = x; 
	}
	public int computeCost(int x, int y){
		return ( bestCostArray[x] + costMatrix[x][y]);
	}
	public int findNextMinNode(){
		//min node is considered unmarked and have minimum cost 
		int tempC = 999999999; 
		int tempM = 0; 
		for(int i = 1; i < markedArray.length; i++){
			if(markedArray[i] == 0){
				if(bestCostArray[i] < tempC){
					tempC = bestCostArray[i];
					tempM = i; 
				}
			}
		}
		return tempM; 
	}
	
	//Used later to see if all nodes are unmarked, if this is true the dijkstra's algorithm can end
	public boolean allNodesUnmarked(){
		boolean temp = false;
		for(int i =1; i<markedArray.length;i++){
			if(markedArray[i] ==0){
				temp = true; 
			}
		}
		return temp; 
	}

	//Performs Dijkstra's algorithm
	public void dijkstra(int source, String output){
		updateMinNode(source);
		updateMarkedArray(source);
		
		for(int i = 1; i < markedArray.length; i++){
			if(markedArray[i] == 0){
				currentNode = i;
				newCost = computeCost(minNode, currentNode);
				if(newCost < bestCostArray[currentNode]){
					updateFatherArray(currentNode, minNode); 
					updateBestCostArray(currentNode, newCost);
				}
			} 
		}
		//Print for debugging purposes
		printBestCost(output); 	
		printFather(output);
		printMarked(output);
		
		while(allNodesUnmarked()){
			//first find the min node
			minNode = findNextMinNode();
			updateMarkedArray(minNode); 
			printMinNode(output);
			
			for(int i = 1; i < markedArray.length; i++){
				if(markedArray[i] == 0){
					currentNode = i;
					newCost = computeCost(minNode, currentNode);
					if(newCost < bestCostArray[currentNode]){
						updateFatherArray(currentNode, minNode); 
						updateBestCostArray(currentNode, newCost);
					}
				}
			}
			//This will print it each time for debugging purposes:
			printBestCost(output); 	
			printFather(output);
			printMarked(output);
		}
		//if any nodes have no path, put father as source:
		for(int i = 1; i < N+1; i++){
			if(fatherArray[i] == i){
				fatherArray[i] = sourceNode; 
			}
		}
	}
	
	public void findPrintShortestPath(int i, String output){
		PrintWriter out = null;
		try {
			out = new PrintWriter(new FileWriter(output,true));

			String myPath = Integer.toString(i); 
			int j = i; 
			
			while(fatherArray[j] != sourceNode){
				myPath = Integer.toString(fatherArray[j]) + "->" + myPath; 
				j=fatherArray[j]; 
			}
			myPath = sourceNode + "->" + myPath; 
			
			if(fatherArray[i] == sourceNode){
				myPath = Integer.toString(fatherArray[i]) + "->" + j;
			}

			out.println("The path from "+sourceNode+" to "+i+":"+myPath+": cost = "+bestCostArray[i]);
		} catch (IOException e) { out.println(e);}
		out.close();
	}
	
	//The Following methods are all for printing the different arrays:
	public void debugPrint(String output){
		PrintWriter out = null;
		try{
			out = new PrintWriter(new FileWriter(output,true));
			out.println("Cost Matrix:");
			for(int i = 1; i < (N+1) ; i++){
				for(int j = 1; j < (N+1); j++){
					if(costMatrix[i][j] == 99999) out.print("  ");
					else out.print(costMatrix[i][j]+" ");
				}
				out.println();
			}
			out.println();
		} catch (IOException e){System.out.println(e);}
		out.close();
	}

	public void printMinNode(String output){
		PrintWriter out = null;
		try {
			out = new PrintWriter(new FileWriter(output,true));
			out.println();
			out.println("The Min Node is: " + minNode);
			out.println();
		} catch (IOException e) {
			System.out.println(e);
		}
		out.close(); 
	}
	public void printFather(String outfile){
		PrintWriter out = null;
		try {
			out = new PrintWriter(new FileWriter(outfile,true));
			out.println();
			out.println("The Father Array contains: ");
			for(int i = 1; i <  fatherArray.length; i++){
				out.println(i + "\t" + fatherArray[i]);
			}
			out.println();
		} catch (IOException e) {
			out.print(e);
		}
		out.close();
	}
	public void printMarked(String output){
		PrintWriter out = null;
		try {
			out = new PrintWriter(new FileWriter(output,true));
			out.println();
			out.println("The Marked Array contains: ");
			for(int i = 1; i < markedArray.length; i++){
				out.println(i + "\t" + markedArray[i]);
			}
			out.println();
		} catch (IOException e) {
			out.println(e);
		}
		out.close(); 
	}
	public void printBestCost(String output){
		PrintWriter out = null;
		try {
			out = new PrintWriter(new FileWriter(output,true));
			out.println();
			out.println("The Best Cost Array contains: ");
			for(int i = 1; i < bestCostArray.length; i++){
				out.println(i + "\t" + bestCostArray[i]);
			}
			out.println();
		} catch (IOException e) {
			System.out.println(e);
		}
		out.close();
	}
}
