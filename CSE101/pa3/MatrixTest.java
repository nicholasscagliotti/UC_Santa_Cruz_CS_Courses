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
public class MatrixTest{
    
    public static void main(String[] args){
        int i, j;
        int n = 100;
        Matrix A = new Matrix(n);
        Matrix B = new Matrix(n);

        A.changeEntry(1,1,1); 
        A.changeEntry(1,2,2); 
        A.changeEntry(1,3,3); 
        A.changeEntry(2,1,4); 
        A.changeEntry(2,2,5); 
        A.changeEntry(2,3,6); 
        A.changeEntry(3,1,7); 
        A.changeEntry(3,2,8); 
        A.changeEntry(3,3,9); 
        A.changeEntry(4,1,10);
        A.changeEntry(4,2,11);
        A.changeEntry(4,3,12);
        A.changeEntry(5,1,13);
        A.changeEntry(5,2,14);
        A.changeEntry(5,3,15);

        B.changeEntry(1,1,1);
        B.changeEntry(1,2,0);
        B.changeEntry(1,3,1);
        B.changeEntry(2,1,0);
        B.changeEntry(2,2,0);
        B.changeEntry(2,3,0);
        B.changeEntry(3,1,1);
        B.changeEntry(3,2,1);
        B.changeEntry(3,3,1);
        B.changeEntry(4,1,0);
        B.changeEntry(4,2,0);
        B.changeEntry(4,3,0);
        B.changeEntry(5,1,0);
        B.changeEntry(5,2,1);
        B.changeEntry(5,3,0);

        System.out.println("Size of Matrix A:");
        System.out.println(A.getSize());

        System.out.println("Size of Matrix B:");
        System.out.println(B.getSize());

        System.out.println("Matrix A:");
        System.out.print("Non-zero Entries: ");
        System.out.println(A.getNNZ());
        System.out.println(A);

        System.out.println("Matrix B:");
        System.out.print("Non-zero Entries: ");
        System.out.println(B.getNNZ());
        System.out.println(B);

        System.out.println("Matrix A * 1.5:");
        Matrix C = A.scalarMult(1.5);
        System.out.print("Non-zero Entries: ");
        System.out.println(C.getNNZ());
        System.out.println(C);

        System.out.println("Matrix A + Matrix A:");
        Matrix D = A.add(A);
        System.out.print("Non-zero Entries: ");
        System.out.println(D.getNNZ());
        System.out.println(D);

        System.out.println("Matrix A - Matrix A:");
        Matrix E = A.sub(A);
        System.out.print("Non-zero Entries: ");
        System.out.println(E.getNNZ());
        System.out.println(E);

        System.out.println("Transpose of Matrix A:");
        Matrix F = A.transpose();
        System.out.print("Non-zero Entries: ");
        System.out.println(F.getNNZ());
        System.out.println(F);

        System.out.println("Transpose of Matrix B:");
        Matrix G = B.transpose();
        System.out.print("Non-zero Entries: ");
        System.out.println(G.getNNZ());
        System.out.println(G);

        System.out.println("Matrix B * Matrix B:");
        Matrix H = B.mult(B);
        System.out.print("Non-zero Entries: ");
        System.out.println(H.getNNZ());
        System.out.println(H);

        System.out.println("Matrix A + Matrix B:");
        Matrix I = A.add(B);
        System.out.print("Non-zero Entries: ");
        System.out.println(I.getNNZ());
        System.out.println(I);
      
        System.out.println("Matrix B + Matrix A:");
        Matrix J = B.add(A);
        System.out.print("Non-zero Entries: ");
        System.out.println(J.getNNZ());
        System.out.println(J);

        System.out.println("Matrix A - Matrix B:");
        Matrix K = A.sub(B);
        System.out.print("Non-zero Entries: ");
        System.out.println(K.getNNZ());
        System.out.println(K);

        System.out.println("Matrix B - Matrix A:");
        Matrix L = B.sub(A);
        System.out.print("Non-zero Entries: ");
        System.out.println(L.getNNZ());
        System.out.println(L);

        System.out.println("Copy of Matrix A:");
        Matrix M = A.copy();
        System.out.print("Non-zero Entries: ");
        System.out.println(M.getNNZ());
        System.out.println(M);

        System.out.println("Is Matrix A equal to a copy of itself?");
        System.out.println(A.equals(M));
        System.out.println("Is Matrix A equal to Matrix B?");
        System.out.println(A.equals(B));
        System.out.println("Is Matrix A equal to itself?");
        System.out.println(A.equals(A));        

        System.out.println("Matrix A after clearing:");
        A.makeZero();
        System.out.print("Non-zero Entries: ");
        System.out.println(A.getNNZ());
        System.out.println(A);
    }
}