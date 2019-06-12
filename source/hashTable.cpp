// Adrian Castro
// CS 302
// Section 1001
// Due 04/25/2017
// Final Project, airline routing
// HashTable implementation file


#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>

#include "hashTable.h"

using namespace std;

// constructor allocates memory and initializes variables
hashTable::hashTable(){

	addrHash = new string*[initialHashSize];
	for(unsigned int i = 0; i < initialHashSize; i++){
		addrHash[i] = new string[2];
	}
	list = new int[initialHashSize];
	hashSize = initialHashSize;
	collisionCount = 0;
	entries = 0;
	reSizeCount = 0;
}

//destructor should deallocate the dynamically allocated memory
hashTable::~hashTable(){
	for(unsigned int i = 0; i < hashSize; i++){
		delete [] addrHash[i];
	}
	delete [] addrHash;
	delete[] list;  
}

//inserts the item by first hashing it and then linearly probing if the space is already filled  UT
bool hashTable::insert(const string iata, string airport){
	int trash;
	string strTrash;
	unsigned int index = 0;
	if(lookup(iata, strTrash, trash)){
		return false;
	}

	if(((float)entries/ (float)hashSize) > loadFactor){
		hashSize = (hashSize*2);
		entries = 0;
		rehash();
	}
	index =  hash(iata);
	while(true){

		if(addrHash[index%hashSize][0].empty()){
			
			addrHash[index%hashSize][0] = iata;
			addrHash[index%hashSize][1] = airport;			
			adlistIndex++;
			list[index%hashSize] = adlistIndex;
			entries++;
			return true;
		}	
		else{
			collisionCount++;
			index++;
		}
	}
}

//function is passed a zip code to search for and two empty strings if the the item
//is found then the city and state corresponding to that zipcode is passed back via reference
bool hashTable::lookup(string iata, string& airportName, int &passedIndex){
	
	unsigned int index;

	index =  hash(iata);

	while(true){
		if(addrHash[index%hashSize][0] == iata){
			airportName = addrHash[index%hashSize][1];
			passedIndex = list[index%hashSize];
			return true;
		}

		if(addrHash[index%hashSize][0].empty()){ 
			return false;
		}
		index++;
	}			 	
}

//prints hash statistics
void hashTable::showHashStats()const{

	cout << "Hash Stats" << endl;

	cout << "   Current Entries Count: " << entries << endl;
	cout << "   Current Hash Size: " << hashSize << endl;
	cout << "   Hash Resize Operations: " << reSizeCount << endl;
	cout << "   Hash Collisions: " << collisionCount << endl;

}
//prints every item in the hash table
void hashTable::printHash()const{

	for(unsigned int i = 0; i < hashSize; i++){
		if((!addrHash[i][0].empty()) && (addrHash[i][0] != "*")){
			cout << addrHash[i][0] << " : " << addrHash[i][1]<< " : " ; 
		}
	}
}

//uses the hash function to find and remove an item from the hash table
bool hashTable::remove(string zip){
	
	unsigned int index = 0;
	index =  hash(zip);
	while(true){
		if(addrHash[index%hashSize][0] == zip){
			addrHash[index%hashSize][0] = "*";
			addrHash[index%hashSize][1] = "*";
			//addrHash[index%hashSize][2] = "*";
			entries--;
			return true;
		}
		if(addrHash[index%hashSize][0].empty()){ 
			return false;
		}
		index++;
	}	
}

//function creates a new array double the size of the current array and inserts everything in the old
//array in the new array
void hashTable::rehash(){

	string **big = new string*[hashSize];
	for(unsigned int i = 0; i < (hashSize); i++){
		big[i] = new string[3];
	}

	string **temp = addrHash;
	
	addrHash = big;

	for(unsigned int i = 0; i < (hashSize/2); i++){
		if((!temp[i][0].empty()) && temp[i][0] != "*"){
			insert(temp[i][0], temp[i][1]);//, temp[i][2]);
		}
	}

	for(unsigned int i = 0; i < (hashSize/2); i++){
		delete [] temp[i];
	}
	delete [] temp;
	
	big = NULL;
	reSizeCount++;
}

// hash function uses Horner's method to pass back the hash value 
unsigned int hashTable::hash(string wrd) const{

	//hash based on horners method
	int hash = 0;
	for(int i = wrd.length()-1; i >= 0; i--){
		hash = (wrd[i] + 128*hash) % hashSize;
	}
	return hash;
	
	//ascii value hash function
	/*int hash = 0;
	for(unsigned int i = 0; i < wrd.length(); i++)
		hash += (int)(wrd[i]);
	return hash;
	*/
	/*
	int hash = stoi(wrd);
	return hash;
	*/
}

