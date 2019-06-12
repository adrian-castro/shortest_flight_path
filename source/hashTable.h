// Adrian Castro
// CS 302
// Section 1001
// Due 04/25/2017
// Final Project, airline routing
// hashTable header file


#include <iostream>
#include <fstream>
#include <string>


using namespace std;


class hashTable{
	
	public:

		hashTable();
		~hashTable();
		bool insert(const string, const string);
		bool lookup(const string, string&, int&);
		bool remove(const string);
		void printHash() const;
		void showHashStats() const;
		
	private:

		unsigned int hash(string) const;
		void rehash();
		unsigned int hashSize;
		unsigned int collisionCount;
		unsigned int entries;
		unsigned int reSizeCount; 
		int adlistIndex;
		string **addrHash;
		int *list;
		static constexpr double loadFactor = 0.65;
		static constexpr unsigned int initialHashSize = 20000;

};

