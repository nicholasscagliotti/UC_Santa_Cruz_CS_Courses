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
public class Matrix{

    private class Entry{            //Entry class to create and store both values
        int column;
        double data;

        public Entry(int column, double data){        //Entry Instantiator
            this.column = column;
            this.data = data;
        }

        public String toString(){
            return "(" + column + ", " + data + ")";
        }

        public boolean equals(Object o){
            boolean flag = false;
            Entry compare;

            if(o instanceof Entry){
                compare = (Entry)o;
                flag = (compare.column == this. column);
                flag = (compare.data == this.data);
            }
            return flag;
        }
    }
    
    private List[] row;
    private int NNZ = 0;
    private int size = 0;

    Matrix(int n) {                         //Matrix Constructor            
        if(n < 1){
            throw new RuntimeException ("Error: Invalid Matrix size. Enter a value greater than or equal to 1.");
        }
        else{
            size = n;
            row = new List[size + 1];
            for(int i = 1; i <= size; i++){
                row[i] = new List();
            }      
        }
    }

        //
        //         Access Functions
        //

    int getSize(){                          //Returns n, the number of both rows and columns in this matrix
        return size;
    }

    int getNNZ(){                           //Returns the total of non-zero entries in this matrix
        return NNZ;
    }

    public boolean equals(Object o){        //Overrides Object's built-in equals() method
        Matrix temp;
        boolean flag = true;

        if(o instanceof Matrix){
            temp = (Matrix) o;
            if(getSize() != temp.getSize()){
                flag = false;
            }
            for(int i = 1; i <= getSize(); i++){
                if(!(row[i].equals(temp.row[i]))){
                    flag = false;
                }
            }
        }
        return flag;
    }      

        //
        //         Manipulation Functions
        //

    void makeZero(){                        //Sets this matrix to the zero state using clear function
        for(int i = 1; i <= getSize(); i++){
            row[i] = new List();
        }
        NNZ = 0;
    }

    Matrix copy(){                          //Returns a new matrix that is a copy of this matrix
        Matrix copy = new Matrix(size);

        for(int i = 1; i < getSize(); i++){  //i represents number of rows in this loop
            if(row[i].length() > 0){
                row[i].moveFront();
                while(row[i].index != -1){
                    Entry temp = (Entry) row[i].get();
                    copy.changeEntry(i, temp.column, temp.data);
                    row[i].moveNext();
                }
            }
        }
        return copy;   
    }

    void changeEntry(int i, int j, double x){   //Changes the entry in the ith row and jth column of this matrix to x
        if(i >= getSize() && i <= 1){
            throw new RuntimeException("Error: Called changeEntry() on invalid indices.");
        }
        if(j >= getSize() && j <= 1){
            throw new RuntimeException("Error: Called changeEntry() on invalid indices.");
        }
        if(row[i].length == 0){
            if(x != 0){                     //Don't care about if x == 0
                Entry E = new Entry(j, x);
                row[i].append(E);
                NNZ++;
            }
        }
        else{
            row[i].moveFront();
            for(int count = 1; count <= j; count++){
                Entry temp = (Entry) row[i].get();
                if(temp.column == j){
                    if(x == 0){
                        row[i].delete();
                        NNZ--;
                        break;
                    }
                    else{
                        temp.data = x;
                        break;
                    }
                }
                else if(temp.column > j){
                    if(x == 0){
                        break;
                    }
                    else{
                        row[i].insertBefore(new Entry(j, x));
                        NNZ++;
                        break;
                    }
                }
                else{
                    row[i].moveNext();
                    if(row[i].index() == -1){
                        if(x == 0){
                            break;
                        }
                        else{
                            row[i].append(new Entry(j, x));
                            NNZ++;
                            break;
                        }
                    }
                }
            }
        }
    }

    Matrix scalarMult(double x){            //Returns a new matrix equal to the scalar product of this matrix with x
        Matrix scalar = new Matrix(size);

        for(int i = 1; i <= getSize(); i++){
            if(row[i].length > 0){
                row[i].moveFront();
                while(row[i].index() > -1){
                    Entry temp = (Entry)row[i].get();
                    scalar.changeEntry(i, temp.column, temp.data * x);
                    row[i].moveNext();
                }
            }
        }
        return scalar;
    }

    Matrix add(Matrix M){                   //Returns a new matrix equal to the sum of this matrix and matrix M
        if(getSize() != M.getSize()){
            throw new RuntimeException("Error: Called add() with different size matrices.");
        }
        Matrix add = new Matrix(getSize());

        if(this == M){                      //Returns scalarMult(2) of the original matrix if both matrices have the same entries
            return this.scalarMult(2);
        }
        for(int i = 1; i <= getSize(); i++){
            List one = row[i];
            List two = M.row[i];

            one.moveFront();
            two.moveFront();
            while(one.index() >= 0 && two.index() >= 0){
                Entry entry1 = (Entry)one.get();
                Entry entry2 = (Entry)two.get();
                if(entry1.column == entry2.column){
                    if(0 != entry1.data + entry1.data){
                        add.changeEntry(i, entry1.column, (entry1.data + entry2.data));
                        one.moveNext();
                        two.moveNext();
                    }
                    else{
                        one.moveNext();
                        two.moveNext();
                    }
                }
                else if(entry1.column > entry2.column){                                  
                    add.changeEntry(i, entry2.column, entry2.data);
                    two.moveNext();
                }
                else{
                    add.changeEntry(i, entry1.column, entry1.data);
                    one.moveNext();
                }
            }
            if(one.index() == two.index()){
                continue;
            }
            List three;
            if(one.index() > -1){
                three = one;
            }
            else{
                three = two;
            }
            while(three.index() > -1){
                Entry entry3 = (Entry)three.get();
                add.changeEntry(i, entry3.column, entry3.data);
                three.moveNext();
            }
        }
        return add;
    }

    Matrix sub(Matrix M){                   //Returns a new matrix equal to the difference of this matrix and matrix M
        if(getSize() != M.getSize()){
            throw new RuntimeException("Error: Called sub() with different size matrices.");
        }
        Matrix sub = new Matrix(getSize());

        if(this == M){
            return sub;                     //Returns the newly created empty matrix if both matrices are the same
        }
        for(int i = 1; i <= getSize(); i++){
            List one = row[i];
            List two = M.row[i];
            one.moveFront();
            two.moveFront();
            while(one.index() > -1 && two.index() > -1){
                Entry entry1 = (Entry)one.get();
                Entry entry2 = (Entry)two.get();
                if(entry1.column == entry2.column){
                    if(0 != entry1.data - entry2.data){
                    sub.changeEntry(i, entry1.column, entry1.data - entry2.data);
                    one.moveNext();
                    two.moveNext();
                    }
                    else{
                        one.moveNext();
                        two.moveNext();
                    }   
                }
                else if(entry1.column > entry2.column){
                    sub.changeEntry(i, entry2.column, -entry2.data);
                    two.moveNext();
                }
                else{
                    sub.changeEntry(i, entry1.column, entry1.data);
                    one.moveNext();
                }
            }
            if(one.index() == two.index()){
                continue;
            }
            List three;
            if(one.index() > -1){
                three = one;
            }
            else{
                three = two;
            }
            int sign = 0;                   //Int to keep track of whether or not the resulting data will be negative or positive
            if(one.index() > -1){
                sign = 1;
            }
            else{
                sign = -1;
            }
            while(three.index() > -1){
                Entry entry3 = (Entry)three.get();
                sub.changeEntry(i, entry3.column, entry3.data * sign);
                three.moveNext();
            }
        }
        return sub;
    }

    Matrix transpose(){                     //Returns a new matrix which is the transpose of this matrix
        Matrix trans = new Matrix(size);

        for(int i = 1; i <= getSize(); i++){
            if(row[i].length() != 0){
                row[i].moveFront();
                for(int j = 1; j <= row[i].length(); j++){
                    if(row[i].index() != -1){
                        Entry temp = (Entry)row[i].get();
                        trans.changeEntry(temp.column, i, temp.data);
                        row[i].moveNext();
                    }
                }
            }
        }
        return trans;
    }

    Matrix mult(Matrix M){                  //Returns a new matrix which is the product of this matrix and matrix M
        if(getSize() != M.getSize()){
                throw new RuntimeException("Error: Called mult() function on unequal matrices.");
        }
        Matrix temp = M.transpose();
        Matrix mult = new Matrix(getSize());

        for(int i = 1; i <= getSize(); i++){
            if(row[i].length == 0){
                continue;
            }
            for(int j = 1; j <= temp.getSize(); j++){
                if(temp.row[j].length() == 0){
                    continue;
                }      
                else{
                    double dot = dot(row[i], temp.row[j]);
                    if(dot != 0.0){
                        mult.changeEntry(i, j, dot);
                    }
                }
            }
        }
        return mult;
    }

        //
        //         Other Functions
        //

    public String toString(){               //Override's Object's built-in toString() method
        String string = "";

        for(int i = 1; i <= getSize(); i++){
            if(row[i].length > 0){
                string += (i + ": " + row[i] + "\n");
            }
        }
        return string;
    }

    private static double dot(List L, List K){              //Private helper function for Matrix mult()
        double product = 0;                 //Returns the dot product of the matrix
        
        L.moveFront();
        K.moveFront();
        while(L.index() != -1 && K.index() != -1){
            Entry newL = (Entry)L.get();
            Entry newK = (Entry)K.get();
            if(newL.column < newK.column){
                L.moveNext();
            }
            else if(newL.column > newK.column){
                K.moveNext();
            }
            else{
                product += (newL.data * newK.data);
                L.moveNext();
                K.moveNext();
            }
        }
        return product;
    }
}