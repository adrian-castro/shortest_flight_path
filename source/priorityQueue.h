//  Adrian Castro
//  CS 302
//  section 1001
//  Priority Queue implementation file

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream> 
#include <cmath>



using namespace std;

//template header file
template<class myType>
class  priorityQueue{

	struct heapNode{
		int priority;
		myType item;
	};

	public:
		priorityQueue(int = 5000);
		~priorityQueue();
		int entries() const;
		void insert(const myType, const int);
		bool deleteMin(myType &, int &);
		bool isEmpty() const;
		void printHeap() const;
		bool readData(const string);
	private:
		bool changePrio(const myType, int);
		void reheapUp(int);
		void reheapDown(int);
		void buildHeap();
		void resize();
		int count;
		int heapSize;
		heapNode *myHeap;
};


//constuctor that that makes our priority queue of size 5000
//unless a bigger number is passed to the constructor in which case it 
//will be set to the bigger number
template<class myType>
priorityQueue<myType>::priorityQueue(int x): heapSize (6000){
	
	if(x>6000){
		myHeap = new heapNode[x];
	}
	myHeap = new heapNode[heapSize];
	
	count = 0;
}

//class destructor deallocated the memory allocated for the heap
template<class myType> 
priorityQueue<myType>::~priorityQueue(){
	if(myHeap != NULL){
		delete [] myHeap;
	}
}

//returns the number of entries in the heap
template<class myType>
int priorityQueue<myType>::entries()const{

	return count;
}

//funciton is passed an item to be an inserted and the priority of that item
//function will resize if there is not enough space in the array for the next entrie
template<class myType> 
void priorityQueue<myType>::insert(const myType thing, const int num){
	
	if(!changePrio(thing, num)){
		//inserts according to our order requirment
		count++;
		if(count < heapSize){
			myHeap[count].item = thing;
			myHeap[count].priority = num;
		}
		else{
			resize();
			myHeap[count].item = thing;
			myHeap[count].priority = num;
		}
		//maintains ordering requirments
		reheapUp(count);
	}
}

//function checks if the item is already in the PQ and only changes the 
//priority if it is. 
template<class myType>
bool priorityQueue<myType>::changePrio(const myType item, int prio){
	for(int i = 1; i < count; i++){
		if(myHeap[i].item == item){
			myHeap[i].priority = prio;
			return true;
		}
	}
	return false;
}

//delete min is passed an empty item and an empty num for the priority
//function returns the item and its priority via reference and then 
//deletes the item with highest priority that is the first element in 
//the array. After the item is deleted the last item is moved to the top and is
//bubbled down.
template<class myType>
bool priorityQueue<myType>::deleteMin(myType &thing, int &num){
	if(count == 0)
		return false;

	thing = myHeap[1].item;
	num = myHeap[1].priority;
	myHeap[1].item = myHeap[count].item;
	myHeap[1].priority = myHeap[count].priority;
	count--;
	reheapDown(1);
	return true;
}

//isEmpty returns true if there are no items in the prioqueue and false if it is not empty
template<class myType> 
bool priorityQueue<myType> :: isEmpty()const{
	if(count==0)
		return true;
	else 
		return false;
}

//Print heap function prints the items in the heap by level
template<class myType>
void priorityQueue<myType> :: printHeap() const{
	int powCount = 0;
	//print levels
	int j = 1;
	for(int i = 1; i <= count; i++){
		cout << myHeap[i].item << "  " << myHeap[i].priority << endl;
		//cout << j << endl;
		if(j == pow(2, powCount)){
			cout << endl;
			powCount++;
			j = 1;
		}
		else{
			j++;
		}
	}
	cout<< endl;
}

//read data reads in our items an their priority all at once without the ordering requirements
//and at the end builds the heap
template<class myType> 
bool priorityQueue<myType> :: readData(const string filename){
	fstream in;

	in.open(filename);
	while(!in.eof()){
		if(count < heapSize-1){
			count++;
			in >> myHeap[count].item;
			in >> myHeap[count].priority;
		}
		else{
			resize();
		} 
	}
	in.close();
	buildHeap();
	return true;
}

//reheapUp is passes a value that serves as an index of where we
//start bubbling up the item. The function recursively moves the item up in the heap until
//only items with a lower priority are beneath it.
template<class myType>
void priorityQueue<myType> :: reheapUp(int last){
	
	int parent; 
	if(last  > 1){
		parent = last/2;
		if(myHeap[last].priority < myHeap[parent].priority){
			int tempOne;
			myType tempTwo;
			tempOne = myHeap[last].priority;
			tempTwo = myHeap[last].item;
			myHeap[last] = myHeap[parent];
			myHeap[parent].priority = tempOne;
			myHeap[parent].item = tempTwo;
			reheapUp(parent);
		}
	}
}

//reheaDown is passed an index and the function starts in that place and recursively moves the 
//item down by swapping until only items with less priority are beneath it
template<class myType>
void priorityQueue<myType> :: reheapDown(int parent){
	
	int leftChild;
	int rightChild; 
	if(parent <= count/2){
		leftChild = parent*2;
		rightChild = (parent*2)+1;
		if(rightChild <= count){
			if((myHeap[leftChild].priority < myHeap[parent].priority) || (myHeap[rightChild].priority < myHeap[parent].priority)){
				if(myHeap[leftChild].priority <= myHeap[rightChild].priority){
					int tempOne;
					myType tempTwo;
					tempOne = myHeap[leftChild].priority;
					tempTwo = myHeap[leftChild].item;

					myHeap[leftChild] = myHeap[parent];
					myHeap[parent].priority = tempOne;
					myHeap[parent].item = tempTwo;
					reheapDown(leftChild);
				}
				else{
					int tempOne;
					myType tempTwo;
					tempOne = myHeap[rightChild].priority;
					tempTwo = myHeap[rightChild].item;

					myHeap[rightChild] = myHeap[parent];
					myHeap[parent].priority = tempOne;
					myHeap[parent].item = tempTwo;
					reheapDown(rightChild);
				}
			}
		}
		else{
			if(myHeap[leftChild].priority < myHeap[parent].priority){
					int tempOne;
					myType tempTwo;
					tempOne = myHeap[leftChild].priority;
					tempTwo = myHeap[leftChild].item;

					myHeap[leftChild] = myHeap[parent];
					myHeap[parent].priority = tempOne;
					myHeap[parent].item = tempTwo;
					reheapDown(leftChild);
			}
		}
	}
} 

//buildHeap starts at the "last"/rightmost parent and calls the reheapDown function
//The function moves right to left and upwards applying the reheapDown function 
//to each node until the whole queue is in order. 
template<class myType>
void priorityQueue<myType> :: buildHeap(){
	for(int i = count/2; i >= 1; i--){
		reheapDown(i);
	}	
}

//resize function puts all existing nodes into another node twice its size
template<class myType> 
void priorityQueue<myType>::resize(){
	
	heapSize = 2*heapSize;
	heapNode *big = new heapNode[heapSize];;

	for(int i = 1; i < heapSize/2; i++){
		big[i].item = myHeap[i].item;
		big[i].priority = myHeap[i].priority;
	}
	myHeap = big;
	big = NULL;
}

















