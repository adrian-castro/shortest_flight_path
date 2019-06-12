// Adrian Castro
// CS 302
// Section 1001
// Due 04/25/2017
// Final Project, airline routing
// airlineRouter Class Implementation file


#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <iomanip> 

#include "airlineRouter.h"
#include "priorityQueue.h"

using namespace std;



//Class constructor sets all pointers to NULL and counters to zero. 
airlineRouter::airlineRouter(){
	
	if(adList != NULL)
		adList = NULL;
	if(dist != NULL)
		dist = NULL;
	if(prev != NULL)
		prev = NULL;

	vertexCount = 0;
	edgeCount = 0;
}

// Class destructor deallocates all allocated memory by calling private destroyGraph()
// and resets counters.
airlineRouter::~airlineRouter(){
	
	destroyGraph();
	vertexCount = 0;
	edgeCount = 0;
}

// Function opens the airport data file and stores in all the airports names and codes
// into airHash hash table. 
bool airlineRouter::readAirports(const string file){

	fstream infile;
	string airportName;
	string iata;
	string trash;

	infile.open(file);

	if(infile.fail())
		return false;
	
	else{
		
		while(!infile.eof()){
			
			getline(infile, trash);
			if(trash.length() == 0)
				break;
			
			airportName = trash.substr(0, 55);
			iata = trash.substr(111,3);
	//		cout << iata << " " << airportName << endl;
			//hashes and inserts as a head node 
			airHash.insert(iata, airportName);
			vertexCount++;
		}
		//airHash.showHashStats();
		//creates new graph to that is the size of out vertex count
		newGraph();
		infile.close();
	}
	return true;
}

bool airlineRouter::readFlights(const string file){
	
	fstream infile;

	string bigline;
	string to;
	string toTime;
	string from;
	string fromTime;
	string flightNum;
	string apName;
	int unusedInd;

	//int vars
	int toTimeint;
	int fromTimeint;

	infile.open(file);
	
	//checks if file was opened 
	if(infile.fail())
		return false;

	else{
		while(!infile.eof()){

			getline(infile, bigline);
			
			//checks for empty line
			if(bigline.length() == 0)
				break;
			flightNum = bigline.substr(0, 6);
			from = bigline.substr(8, 3);
			fromTime = bigline.substr(12, 4);

			//converts our time from string to int before inserting to graph node
			fromTimeint = stoi(fromTime);

			to = bigline.substr(19, 3);
			
			toTime = bigline.substr(23, 4);

			//converts time to int 
			toTimeint = stoi(toTime);

			// debug line cout << flightNum << "  " << from << " " << fromTime << " " << to << " " << toTime << endl;

			// adds the edge to graph if both airports are valid
			if(airHash.lookup(from, apName, unusedInd) && airHash.lookup(to, apName, unusedInd)){
				edgeCount++;
				addEdge(flightNum, from, fromTimeint, to, toTimeint);
			}
		}
		infile.close();
	}
	return true;
}

// function destroys any graph that might already be made and then makes a new graph 
// that has is initialized to the size of our vertex count.
void airlineRouter::newGraph(){

	destroyGraph();

	adList = new node[vertexCount];

	for(int i = 0; i < vertexCount; i++){
		adList[i].next = NULL;
	}
}

// function adds an edge to our graph by first getting the index of our source node 
// by calling the look up function from our hash table then adding the edge to the 
// front of the list. 
void airlineRouter::addEdge(string flightNumber, string from, int fromTime,
							string to, int toTime){

	int index; 
	string trash;

	//uses hashtable to get the index for the adlist
	airHash.lookup(from, trash, index);

	//create temp node to add the edge
	node *temp = new node;

	//sets all fields 
	temp->next = adList[index].next;

	temp->origin = from;
	temp->origTime = fromTime;
	temp->destination = to;
	temp->destTime = toTime;
	temp->flightNum = flightNumber;

	adList[index].next = temp;
}

// function calls the hash table's lookup function to check if the airport code 
// from the flight is a valid airport. 
bool airlineRouter::checkAirportCode(const string iata){

	int indi;
	string trash;

	if(!airHash.lookup(iata, trash, indi))
		return false;
	else
		return true;
}

// Function outputs the number of nodes and edges.
void airlineRouter::showGraphStats(){

	cout << "Graph Statistics:" << endl;
	cout << "   Nodes: " << vertexCount << endl;
	cout << "   Edges: " << edgeCount << endl;
	cout << endl;

}

// Function goes through our adjacency lists and frees all allocated memory.
void airlineRouter::destroyGraph(){
	
	if(adList != NULL){
		node *iterator = new node;
	
		for(int i = 0; i < vertexCount; i++){
			iterator = adList[i].next;
			while(iterator != NULL){
				iterator = adList[i].next->next;
				delete adList[i].next;
				adList[i].next = iterator;
			}
		}
		delete [] adList;
	}	
}

// Function is passed two airports and an time. Function fist checks if there are any outgoing 
// flights from the source airport. Function will output error message if there are no outgoing 
// flights. Function then calls dijkstraSP() function to fill our prev[] array. Function then calls
// showPath() function to output the flight path with the earliest arrival time. 
void airlineRouter::findFlights(const string source, const string dest, const int etime){

	string Oap;
	string Dap;
	int origIndex;
	int destIndex;
	
	airHash.lookup(source, Oap, origIndex);
	
	airHash.lookup(dest, Dap, destIndex);

	if(adList[origIndex].next == NULL){
		cout << endl << "No flights from " << source << endl << endl;
	}
	
	else{
		
		dijkstraSP(source, etime);
		
		cout << endl;
		cout << "--------------------------------------------" << endl;
		cout << "From / To" << endl;
		cout << "  " << source << " - " << Oap << endl;
		cout << "  " << dest << " - " << Dap << endl;
		cout << endl;

		if(prev[destIndex].flightNum == "-1")
			cout << "  " << dest << " is not reachable from " << source << "." << endl << endl;
		else
			showPath(destIndex);
	}
}

// Function is passed the destination airport and recursively prints the flight path with the earliest
// arrival time using the prev[] array.
void airlineRouter::showPath(const int source){
	
	if(prev[source].flightNum == "-1")
		return;
	else{
		string ap;
		int destIndex;
		string pass;
		pass = prev[source].origin;
		airHash.lookup(pass, ap, destIndex);
		showPath(destIndex);
		cout << "Flight:" << endl;
		cout << "  " << prev[source].origin << "-" << prev[source].destination << " :: DeptTime: " << prev[source].origTime << "   ArrvTime: " << prev[source].destTime << "   " << endl;
		cout << "  Flight Number: " << prev[source].flightNum << endl << endl;  
	}
}

// Function uses a modified Dijkstra's shortest path algorithm to find the flight 
// path with the earliest arrival time. Function is passed the source node and the 
// earliest departure time. 
void airlineRouter::dijkstraSP(string origin, int earliest){

	int u;
	int source;
	string ap;

	priorityQueue<int> myQueue;

	dist = new int[vertexCount];
	prev = new node[vertexCount];

	airHash.lookup(origin, ap, source);
	
	//initailizes all values in distance and prev array to infinity and -1 respectively.
	for(int i = 0; i < vertexCount; i++){
		dist[i] = std::numeric_limits<int>::max();
		prev[i].flightNum = "-1";
	}

	//inserts source node to prioQ
	myQueue.insert(source, earliest);
	dist[source] = 0;

	while(!myQueue.isEmpty()){
		
		//pop top value		
		myQueue.deleteMin(u, earliest);
		
		node *iterator = new node;
		iterator = adList[u].next;
		
		//traverses the linked list with iterator
		while( iterator != NULL){

			int v;
			airHash.lookup((iterator->destination), ap, v);
			
			if(earliest < (iterator->origTime) && (iterator->destTime) < dist[v] && (iterator->origTime)<(iterator->destTime)){	
				dist[v] = (iterator->destTime);//arriveTime;
				prev[v] = *iterator;
				int queTime = iterator->destTime;
				queTime+=30;
				if((queTime%100) >= 60){
					queTime+=40;
				}
				myQueue.insert(v, queTime);
			}
			iterator = iterator->next;
		}
	}
}
