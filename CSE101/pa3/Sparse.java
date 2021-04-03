        ////
        ////
        ////            Programming Assignment #3
        ////                  Matrix ADT
        ////        Created by: Nicholas Scagliotti
        ////                nscaglio@ucsc.edu
        ////                   CMPS 101
        ////                   Fall '18
        ////
        ////
import java.io.*;
import java.util.*;

public class Sparse{

    public static void main (String[] args) throws IOException{
        int n = 0;                          //Initialising variables for use input Sparse
        int a = 0;
        int b = 0;
        Matrix A = null;
        Matrix B = null;
       
        Scanner input = null;
        PrintWriter output = null;
 
        if (args.length != 2){              //If total arguments != 2, quit and display error message
            System.err.println ("Error: Total amount of arguments is not equal to 2.");
            System.exit(1);
        }
       
        try
        {                                   //Try catch to make sure neither arguments are null     
            input = new Scanner (new File (args[0]));
            output = new PrintWriter(new FileWriter(args[1]));
        }
        catch (IOException e){
            e.printStackTrace();
        }
      
        while (input.hasNextInt()){         //Reading input file line by line
            n = input.nextInt();
            a = input.nextInt();
            b = input.nextInt();      
            A = new Matrix(n);
            B = new Matrix(n);

            for (int i = 1; i <= a; i++){   //Loop to create matrix A     
                int rowA = input.nextInt();
                int colA = input.nextInt();
                double valA = input.nextDouble();
                A.changeEntry(rowA, colA, valA);
            }
            for (int j = 1; j <= b; j++){   //Loop to create matrix B           
                int rowB = input.nextInt();
                int colB = input.nextInt();
                double valB = input.nextDouble();
                B.changeEntry(rowB, colB, valB);
            }
        }

        //Output to match desired style stated in pa3 handout
        output.println("A has "+A.getNNZ()+" non-zero entries:");
        output.println(A);
        output.println("B has "+B.getNNZ()+" non-zero entries:");
        output.println(B);    
        output.println("(1.5)*A =");
        output.println(A.scalarMult(1.5)); 
        output.println("A+B =");
        output.println(A.add(B));
        output.println("A+A =");
        output.println(A.add(A));  
        output.println("B-A =");
        output.println(B.sub(A));   
        output.println("A-A =");
        output.println(A.sub(A));   
        output.println("Transpose(A) =");
        output.println(A.transpose());    
        output.println("A*B =");
        output.println(A.mult(B));    
        output.println("B*B =");
        output.println(B.mult(B));
                
        input.close();                          //Can't forget to close the input and output
        output.close();
    }
}