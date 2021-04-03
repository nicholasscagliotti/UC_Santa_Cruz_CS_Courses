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
public class List{

	private class Node{
		int data;
		Node next, prev;

		Node(int data){                                         //Node Constructor
			this.data = data;
			next = null;
			prev = null;
		}
		
	}

	private Node front, back, cursor;				//Fields to maintain desired information
	private int length, index;

	List() {										// List Constructor
            front = back = cursor = null; 
            length = 0; 
            index = -1;
        }	
        
	boolean isEmpty(){						//Checks to see if the list is empty. True if it's empty, false if it's not.
		return length == 0 && index < 0;                           //Len should be == 0 and index should be < 0.
	}

		//
		//			Access Functions
		//

	boolean equals(List L){                                         //Checks to see if List and L are the same sequence of integers.
		boolean flag = true;                                    //Returns true if they match, false if they don't.
		Node a  = this.front;
		Node b  = L.front;

		if(this.length == L.length){
			while(flag && a != null){
				flag = (a.data == b.data);
				a = a.next;
				b = b.next;
			}
			return flag;
		}
		else{
			return false;
		}
	}

      
	int length(){							//Returns the length of the array
		return length;
	}

	int index(){							//Returns the current index
		return index;
	}

	int front(){							//Returns the value of the element in the front of the array
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called front() while list is empty.");       //Error case for an empty list
		}
		return front.data;
	}

	int back(){							//Returns the value of the element in the back of the array
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called back() while list is empty.");
		}
		return back.data;
	}

	int get(){					//Returns the value of the element which the cursor is on
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called get() while list is empty.");
		}
		if(cursor == null){
			throw new RuntimeException("Error: Called get() while cursor is undefined.");   //Error case for a null cursor
		}
		return cursor.data;
	}

		//
		//			Manipulation Procedures
		//

	void clear(){							//Clears the List, making it an empty list
		front = back = cursor = null;
		length = 0;
		index = -1;
	}

	void moveFront(){						//Places the cursor under the front element
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called moveFront() while list is empty.");
		}
		else{
			cursor = front; //0
			index = 0;
		}
	}

	void moveBack(){						//Places the cursor under the back element
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called moveBack() while list is empty.");
		}
		else {
			cursor = back; // length - 1
			index = length - 1;
		}
	}
	void movePrev(){						//Moves the cursor towards the front of of the list.
		if(index == 0 || index == -1){                          //If cursor is already on the first element, cursor becomes undefined.
			cursor = null;
			index = -1;
		} 
		else if(index > 0 && index <= length -1){
			cursor = cursor.prev;
			index--;
		}
	}

	void moveNext(){						//Moves the cursor towards the back of the list.
		if(index == length - 1 || index == -1){                    //If cursor is already on the last element, cursor becomes undefined.
			cursor = null;
			index = -1;
		}
		else if(index >= 0 && index < length - 1){
			cursor = cursor.next;
			index++;
		}
	}
	

	void prepend(int data){						//Inserts new element ahead of the current front element 
		Node n = new Node(data);

		if(this.isEmpty()){
			front = back = n;
		}
		else{
			front.prev = n;
			n.next = front;
			front = n;
			index++;
		}
		length++;
	}

	void append(int data){                                          //Inserts new element behind the current back element
		Node n = new Node(data);
		if(this.isEmpty()){
			front = back = n;
		}
		else{
			back.next = n;
			n.prev = back;
			back = n;
		}
		length++;
	}

	void insertBefore(int data){                                    //Inserts a new element before the current cursor
		Node n = new Node(data);

		if(this.isEmpty()){
			throw new RuntimeException("Error: Called insertBefore() while list is empty.");
		}
		else if(cursor == null){
			throw new RuntimeException("Error: Called insertBefore() while cursor is undefined.");
		}
		else if(cursor == front){                               //If cursor is on front element, calling the prepend function saves time and prevents any issues
			prepend(data);
		}
		else{
			n.next = cursor;
			n.prev = cursor.prev;
			cursor.prev.next = n;
                        cursor.prev = n;
			index++;
			length++;
		}

	}

	void insertAfter(int data){                                    //Inserts a new element after the current cursor
		Node n = new Node(data);

		if(this.isEmpty()){
			throw new RuntimeException("Error: Called insterAfter() while list is empty.");
		}
		else if(cursor == null){
			throw new RuntimeException("Error: Called insertAfter() while cursor is undefined.");
		}
		else if(cursor == back){
			append(data);                                  //If cursor is on front element, calling the append function saves time and prevents any issues
		}
		else{
			
			n.next = cursor.next;
			n.prev = cursor;
                        cursor.next.prev = n;
			cursor.next = n;
			length++;
		}
	}

	void deleteFront(){
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called deleteFront() while list is empty.");
		}
		else if(this.length == 1){                     //Single case for a list with only one element
                        front = back = cursor = null;
			index = -1;
			length--;
		}
		else{                                       //Case for all lists with length > 1
                        front.next.prev = null;
			front = front.next;
			length--;
		}
	}

	void deleteBack(){
		if(this.isEmpty()){
                        throw new RuntimeException("Error: Called deleteBack() while list is empty.");
		}
                else if(this.length == 1){                     //Single case for a list with only one element
                        front = back = cursor = null;
			index = -1;
			length--;
                }
		else{                                       //Case for all lists with length > 1
			back.prev.next = null;
			back = back.prev;
			length--;
		}
	}

	void delete(){                                          
		if(this.isEmpty()){
			throw new RuntimeException("Error: Called delete() while list is empty or cursor is undefined.");
		}
		else{
			if(cursor.prev != null){
				cursor.prev.next = cursor.next;
			}
			if(cursor.next != null){
				cursor.next.prev = cursor.prev;
			}
			cursor = null;
			length--;
		}
	}
        
        //
        //          Other Methods
        //
        
        @ Override public String toString(){			//Overrides Object's toString() method, returning a string representation of the list
                if(this.isEmpty()){
                      return "Error: Called print function while list is empty.";      
                }
                else{
                    String string = "";
                    Node n = front;
                    for(int i = 0; i < length; i++){
                        string += n.data +" ";
                        n = n.next;
                    }
                    return string;
                }
	}
        
        List copy(){                                            //Returns a copy of the desired list
		List copy = new List();
		Node n = this.front;
		while(n != null){
			copy.append(n.data);
			n = n.next;
		}
                return copy;
	}
}