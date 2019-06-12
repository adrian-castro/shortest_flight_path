# shortest_flight_path

Motivation: Given a set of flight options, a business traveler typically desires a flight or set of flights from a origin location to a specific destination optimized based on some critieria, typically earliest arrival time.

Objective: Given a set of flight options, design a program to find a travel itinerary based on the earliest possible arrival time from the provided origin at the selected destination.

Functionalities:

Read and store the airport data
Read and store the flight data
Check/verify airport code validity
Find and show the travel itinerary
Given the origin airport, destination airport, and earliest allowable start time
A minimum layover of 30 minutes per stop (if any stops)
Graph statisitcs for reference
Approach
Viewed as a graph problem where the airports are verticies's and the flights are direct graph edges, I implemented Dijkstra's single source shortest path algorithm to optimize for arrival times in order to determine the flight path.

To implement the graph an adjacency list was used of node type flightNodes for the graph of flights. Because the graph is very sparse, an adjacency list would be a better choice over an adjacency matrix due to memory efficiency. The flight information was stored in the flightNode structure, and the departing airport code was used as the column indices. Each flight with that specific departing airport code was attached in the format of a linked list to the head. The specific position of each airport on the adjacency list was determined through the hash table, where the hash table functioned as a look up table to determine the index of airport codes

To efficiently reference each airport, a hash table with a perfect hash function was used to store the airport information. A 2-dimensional array was dynamically allocated where the airport code, airport information, the UTC offset, and the adjacency list index reference was stored for each airport. This information was later accessed when printing out the full path of a flight.

A priority queue was used as an intermediate data structure in dijkstra’s shortest algorithm. In an effort to produce the fastest arrival time of any airport from a given airport, the arrival time was used to determine the priority of the flight. A flight of higher priority was placed before flights with later arrival times. The pirority was updated after each removal or addition to the data structure.

Even with over 5,000 airports and over 1,000,000 flights, not every airport will be reachable
