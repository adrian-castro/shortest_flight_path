// Adrian Castro
// CS 302
// Section 1001
// Due 04/25/2017
// Final Project, airline routing
// airlineRouter header file


#ifndef airlineRouter_h
#define airlineRouter_h

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>

#include "hashTable.h"

using namespace std;


class airlineRouter{

	struct node{
		
		string origin;
		int origTime;
		string destination;
		int destTime;
		string flightNum;
		node *next;
		
	};

	public:
		airlineRouter();
		~airlineRouter();
		bool readAirports(const string);
		bool readFlights(const string);
		void showGraphStats();
		bool checkAirportCode(const string);
		void findFlights(const string, const string, const int);


		//for the graph
		void newGraph();
		void addEdge(string, string, int, string, int);
		void dijkstraSP(string, int);

	private:
		void showPath(const int);
		void destroyGraph();
		hashTable airHash;
		node* adList;
		int vertexCount;
		int edgeCount;
		int* dist;
		node* prev;


};

#endif