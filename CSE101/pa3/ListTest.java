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
public class ListTest{

        public static void main(String[] args0){
        List A = new List();
                List B = new List();

                for(int i = 1; i <= 50; i++){
                        A.append(i);
                        B.prepend(i);
                }

                System.out.println("List A: \n" + A);
                System.out.println("List B: \n" + B);

                for(A.moveFront(); A.index()>=0; A.moveNext()){
                        System.out.print(A.get() + " ");
                }
                System.out.println();
                for(B.moveBack(); B.index()>=0; B.movePrev()){
                        System.out.print(B.get() + " ");
                }
                System.out.println();

                System.out.println("Does List A = List B? " + A.equals(B));
                System.out.println("Does List A = List A? " + A.equals(A));
                System.out.println("Does List B = List B? " + B.equals(B));

                A.moveFront();
                for(int i = 0; i < 10; i++){
                        A.moveNext();   //Puts ut as index 10
                }
                A.insertAfter(100);     //Should be index 11

                for(int i = 0; i < 24; i++){
                        A.moveNext();   //Puts us at index 35
                }
                A.insertBefore(200); 
                System.out.println("List A after inserting values at different indices: " + A);

                A.moveBack();
                for(int i = 0; i < 25; i++){
                        A.movePrev();   //Puts us at index 25
                }
                A.delete(); 
                System.out.println("List A after deleting index 25: " + A);
                System.out.println("List A length after deleting a single index: " + A.length());

                A.deleteFront();
                System.out.println("List A with front deleted: " + A);
                System.out.println("List A length after deleting front: " + A.length());        

                A.deleteBack();
                System.out.println("List A with back deleted: " + A);
                System.out.println("List A length after deleting back: " + A.length());

                A.clear();
                System.out.println("List A length after clearing the list: " + A.length());

                B.clear();
                System.out.println("List B length after clearing the list: " + B.length());
        }
}