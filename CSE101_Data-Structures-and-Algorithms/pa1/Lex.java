        ////
        ////
        ////            Programming Assignment #1
        ////                Integer List ADT
        ////        Created by: Nicholas Scagliotti
        ////                nscaglio@ucsc.edu
        ////                   CMPS 101
        ////                   Fall '18
        ////
        ////
import java.io.*;
import java.util.Scanner;

public class Lex{
	public static void main(String [] args) throws IOException{
		int lineCount = 0;
		Scanner input = null;
		PrintWriter output = null;

		if(args.length < 2){											//Checking to see if there are at least two arguments
			System.out.println("Usage: FileIO infile outfile");
			System.exit(1);
		}

		input = new Scanner(new File(args[0]));

		while(input.hasNextLine()){									//Counting the total lines in the scanned file
         input.nextLine();
         lineCount++;
      	}
      	input.close();

		String[] token = new String[lineCount];
		Scanner get = new Scanner(new File(args[0]));
		output = new PrintWriter(new FileWriter(args[1]));

		for(int i = 0; i < token.length; i++){						//Creates string array and fills it with contents from the file.
			token[i] = get.nextLine();								//Second array should be the same size as total lines counted.
		}
		get.close();
		output.close();

		List L = new List();										//Instantiating a new list		
		for(int i = 1; i < token.length; i++){
			if(L.index() < 0){									//For the case of an empty list
        	   		L.append(i - 1);
       			}
         		L.moveFront();
         		int j = i;
         		while(j > 0 && token[i].compareTo(token[L.get()]) >= 0){   //Comparing the values, searching for where token[i] will come before token[L.get()]
         			L.moveNext();
         			j--;
         		}
         		if(L.index() >= 0){
         			L.insertBefore(i);										//Insterting it before the value we found to come after our desired element
         		}
         		else{														//At this point we've reached the end of the array and know the current value must be the largest.
         			L.append(i);											//Simply call the append function to place it in the desired location.
         		} 
     		}

     	output = new PrintWriter(new FileWriter(args[1]));					//Printing to outfile
     	L.moveFront();														//Set cursor to front and print in order
     	int i = 0;
     	while(i < lineCount){
     		output.println(token[L.get()]);
     		L.moveNext();
     		i++;
     	}
     	output.close();
	}
}