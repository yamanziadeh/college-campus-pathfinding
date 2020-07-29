/*main.cpp*/
// Name: Yaman Ziadeh
// Created: 04/20/2020
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <deque>
#include <unordered_set>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <fstream>
#include <chrono> // runtime
#include "graph.h"
#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"

const double INF = numeric_limits<double>::max();
const string google_API_KEY = "AIzaSyCDZjH9IAbUJ0Oc0QldUWDcd8lsfpNGIxE";

using namespace std;
using namespace tinyxml2;
using namespace std::chrono;

// Class used to prioritize elements within the priority queue
class prioritize
{
public:
	bool operator()(const pair<long long, double> &p1, const pair<long long, double> &p2)
	{
		if (p1.second == p2.second)
		{
			return p1.first > p2.first;
		}
		return p1.second > p2.second;
	}
};

/* Breadth First Search */

bool isInQ(deque<long long> Q, long long el)
{
	for (auto &e : Q)
	{
		if (e == el)
		{
			return true;
		}
	}
	return false;
}

map<long long, long long> bfs(graph<long long, double> &G, map<long long, Coordinates> &Nodes, long long startV, long long destV)
{
	deque<long long> Q;
	map<long long, long long> predV;

	unordered_set<long long> visited;
	Q.push_back(startV);
	visited.insert(startV);
	predV[startV] = startV;

	while (!Q.empty())
	{
		long long curNode = Q.front();
		if (curNode == destV)
		{
			break;
		}
		Q.pop_front();
		visited.insert(curNode);

		set<long long> neigh = G.neighbors(curNode);
		for (auto &e : neigh)
		{
			if (visited.count(e) == 0)
			{
				if (isInQ(Q, e) == false)
				{
					Q.push_back(e);
					predV[e] = curNode;
				}
			}
		}
	}

	return predV;
}

/* Depth First Search */

map<long long, long long> dfs(graph<long long, double> &G, map<long long, Coordinates> &Nodes, long long startV, long long destV)
{
	map<long long, long long> predV;
	unordered_set<long long> visited;
	stack<long long> s;
	s.push(startV);
	predV[startV] = startV;

	while (!s.empty())
	{
		long long top = s.top();
		if (top == destV)
		{
			break;
		}
		s.pop();
		if (visited.count(top) == 0)
		{
			set<long long> neigh = G.neighbors(top);
			for (auto &e : neigh)
			{
				s.push(e);
				if (predV.count(e) == 0)
				{
					predV[e] = top;
				}
			}
			visited.insert(top);
		}
	}

	return predV;
}

/* A* Algorithm */

int heuristic(map<long long, Coordinates> &Nodes, long long currV, long long destV)
{
	//Manhatten Heuristic
	return (abs(Nodes[currV].Lat - Nodes[destV].Lat) + abs(Nodes[currV].Lon - Nodes[destV].Lon));
}

map<long long, long long> aStar(graph<long long, double> &G, map<long long, Coordinates> &Nodes, long long startV, long long destV)
{
	map<long long, long long> predV;
	map<long long, double> cost;
	double weight;

	priority_queue<pair<long long, double>, vector<pair<long long, double>>, prioritize> frontier;
	frontier.push(pair<long long, double>(startV, 0));

	predV[startV] = startV;
	cost[startV] = 0;

	while (!frontier.empty())
	{
		long long curr = frontier.top().first;
		frontier.pop();
		if (curr == destV)
		{
			break;
		}
		set<long long> neigh = G.neighbors(curr);
		for (auto &e : neigh)
		{
			G.getWeight(curr, e, weight);
			double eCost = cost[curr] + weight;
			if (cost.count(e) == 0 || eCost < cost[e])
			{
				cost[e] = eCost;
				double heur = eCost + heuristic(Nodes, e, destV);
				frontier.push(pair<long long, double>(e, heur));
				predV[e] = curr;
			}
		}
	}
	return predV;
}

void findPathFS(graph<long long, double> &G, map<long long, Coordinates> &Nodes, long long startV, long long destV, string alg)
{
	auto start = high_resolution_clock::now();
	map<long long, long long> predV = alg == "bfs" ? bfs(G, Nodes, startV, destV) : (alg == "dfs" ? dfs(G, Nodes, startV, destV) : aStar(G, Nodes, startV, destV));
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	stack<long long> path; // stack representing the overall path
	string URL = "http://maps.googleapis.com/maps/api/staticmap?key=" + google_API_KEY + "&size=800x800&path=color:red|";
	if (predV.count(destV) > 0)
	{
		long long curNode = destV;
		while (curNode != predV[curNode])
		{
			path.push(curNode);
			curNode = predV[curNode];
		}
		path.push(curNode);
		//cout << "Path: ";
		while (!path.empty())
		{ // A stack is used to go from the start to dest instead of the otherway round (since we reversed through predv)
			long long ID = path.top();
			URL = URL + to_string(Nodes[ID].Lat) + "," + to_string(Nodes[ID].Lon);
			//cout << ID;
			path.pop();
			//if (!path.empty()){cout  << "->";}
			if (!path.empty())
			{
				URL = URL + "|";
			}
		}
		URL = URL + "&markers=size:large|label:A|" + to_string(Nodes[startV].Lat) + "," + to_string(Nodes[startV].Lon) + "&markers=size:large|label:B|" + to_string(Nodes[destV].Lat) + "," + to_string(Nodes[destV].Lon);
		//^Inserts the buildings markers
		URL = URL + "&sensor=false";
		cout << "Runtime of Algorithm (microseconds): " << duration.count() << endl;
		cout << "Visual Path (search up the following URL): " << URL << endl;
	}
	else
	{
		cout << "Sorry, destination unreachable";
	}
}

/* Dijkstra's Algorithm */

// Function used in dijkstra's algorithm to check if el exists in vector visited
bool isIn(vector<long long> visited, long long el)
{
	for (auto &e : visited)
	{
		if (e == el)
		{
			return true;
		}
	}
	return false;
}

// Algorithm used to find the shortest path from StartPoint to every other point in G and it will output the fastest distances into 'distances'
void Dijkstra(graph<long long, double> &G,
			  long long startV,
			  map<long long, double> &distances, map<long long, long long> &predV)
{
	vector<long long> visited;
	priority_queue<pair<long long, double>, vector<pair<long long, double>>, prioritize> pq;

	for (auto &curV : G.getVertices())
	{
		distances[curV] = INF;
		predV[curV] = 0;
		// pq.push(pair<long long, double>(curV, INF));
	}
	pq.push(pair<long long, double>(startV, INF));

	distances[startV] = 0;
	pq.push(pair<long long, double>(startV, 0));

	while (!pq.empty())
	{
		pair<long long, double> curPair = pq.top();
		pq.pop();

		if (curPair.second == INF)
		{
			break;
		}
		else if (isIn(visited, curPair.first))
		{
			continue;
		}
		else
		{
			visited.push_back(curPair.first);
		}

		set<long long> neigh = G.neighbors(curPair.first);
		for (auto &e : neigh)
		{
			double weight;
			G.getWeight(curPair.first, e, weight);
			double newDist = distances[curPair.first] + weight;
			if (newDist < distances[e])
			{
				distances[e] = newDist;
				predV[e] = curPair.first;
				pq.push(pair<long long, double>(e, newDist));
			}
		}
	}
}

// Function used to run Dijkstra's algorithm and then, using the predv map, find the fastest path from the startNode to the destNode
void findPathDIJ(graph<long long, double> &G, map<long long, Coordinates> &Nodes, long long &startNode, long long &destNode)
{
	map<long long, double> distances;
	map<long long, long long> predv; // Map used to store the predecessor nodes mapped to the original nodes
	stack<long long> path;			 // stack representing the overall path
	string URL = "http://maps.googleapis.com/maps/api/staticmap?key=" + google_API_KEY + "&size=800x800&path=color:red|";

	auto start = high_resolution_clock::now();
	Dijkstra(G, Nodes[startNode].ID, distances, predv); // Run Algorithm...
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	long long curId = Nodes[destNode].ID; // Get Destination Node ID
	path.push(curId);
	while (curId != Nodes[startNode].ID && distances[curId] != INF)
	{ // Keep reversing from Destination Node to Start Node through the predv Map
		path.push(predv[curId]);
		curId = predv[curId];
	}

	if (distances[curId] != INF)
	{ // If distance is not INF, dest is reachable from start
		cout << "Distance to dest: " << distances[Nodes[destNode].ID] << " miles" << endl;
		//cout << "Path: ";
		while (!path.empty())
		{ // A stack is used to go from the start to dest instead of the otherway round (since we reversed through predv)
			long long ID = path.top();
			URL = URL + to_string(Nodes[ID].Lat) + "," + to_string(Nodes[ID].Lon);
			//cout << ID;
			path.pop();
			//if (!path.empty()){cout  << "->";}
			if (!path.empty())
			{
				URL = URL + "|";
			}
		}
		URL = URL + "&markers=size:large|label:A|" + to_string(Nodes[startNode].Lat) + "," + to_string(Nodes[startNode].Lon) + "&markers=size:large|label:B|" + to_string(Nodes[destNode].Lat) + "," + to_string(Nodes[destNode].Lon);
		//^Inserts the buildings markers
		URL = URL + "&sensor=false";
		cout << "Runtime of Algorithm (microseconds): " << duration.count() << endl;
		cout << "Visual Path (search up the following URL): " << URL << endl;
	}
	else
	{
		cout << "Sorry, destination unreachable";
	}
}

// Function used to add the Nodes as Verticies into the Graph
void addNodes(map<long long, Coordinates> &Nodes, graph<long long, double> &G)
{
	for (auto &e : Nodes)
	{ // Step 5
		G.addVertex(e.first);
	}
}

// Function used to add footways as edges into the graph
void addEdges(map<long long, Coordinates> &Nodes, vector<FootwayInfo> &Footways, graph<long long, double> &G)
{
	long long fromNode;
	long long toNode;
	double distance;
	for (auto &footway : Footways)
	{ // Step 6
		for (int i = 0; i < footway.Nodes.size(); i++)
		{
			if (i != 0)
			{
				fromNode = footway.Nodes[i - 1];
				toNode = footway.Nodes[i];
			}
			if (i != footway.Nodes.size() - 1)
			{
				fromNode = footway.Nodes[i];
				toNode = footway.Nodes[i + 1];
			}

			Coordinates fromNodeC = Nodes.at(fromNode);
			Coordinates toNodeC = Nodes.at(toNode);

			distance = distBetween2Points(fromNodeC.Lat, fromNodeC.Lon, toNodeC.Lat, toNodeC.Lon); // Calculate distance...
			G.addEdge(fromNode, toNode, distance);												   // Add edges (undirected graph so same edge going to and from both nodes)
			G.addEdge(toNode, fromNode, distance);
		}
	}
}

// Function used to filter input and find the buildings specified by the user-given inputs, returns true if buildings were found, false otherwise
bool filterInput(vector<BuildingInfo> &Buildings, BuildingInfo &start, BuildingInfo &dest, string startBuilding, string destBuilding)
{
	//Find by Abbrev
	for (auto &building : Buildings)
	{
		if (building.Abbrev == startBuilding)
		{
			start = building;
		}
	}
	for (auto &building : Buildings)
	{
		if (building.Abbrev == destBuilding)
		{
			dest = building;
		}
	}

	//Find by FullName
	if (start.Fullname == "")
	{
		for (auto &building : Buildings)
		{
			if (building.Fullname.find(startBuilding) != string::npos)
			{
				start = building;
				break;
			}
		}
		if (start.Fullname == "")
		{
			cout << "Start building not found" << endl;
			return false;
		}
	}
	if (dest.Fullname == "" && start.Fullname != "")
	{
		for (auto &building : Buildings)
		{
			if (building.Fullname.find(destBuilding) != string::npos)
			{
				dest = building;
				break;
			}
		}
		if (dest.Fullname == "")
		{
			cout << "Destination building not found" << endl;
			return false;
		}
	}
	return true;
}

// Function used to find the nearest Footway Nodes to the given buildings
void findNearestPoints(map<long long, Coordinates> &Nodes, vector<FootwayInfo> &Footways, BuildingInfo &start, BuildingInfo &dest, long long &startNode, long long &destNode)
{
	double startMin = INF;
	double destMin = INF;
	for (auto &footway : Footways)
	{
		for (auto &node : footway.Nodes)
		{
			double distance = distBetween2Points(start.Coords.Lat, start.Coords.Lon, Nodes.at(node).Lat, Nodes.at(node).Lon);
			if (distance < startMin)
			{
				startMin = distance;
				startNode = node;
			}
		}
	}
	for (auto &footway : Footways)
	{
		for (auto &node : footway.Nodes)
		{
			double distance = distBetween2Points(dest.Coords.Lat, dest.Coords.Lon, Nodes.at(node).Lat, Nodes.at(node).Lon);
			if (distance < destMin)
			{
				destMin = distance;
				destNode = node;
			}
		}
	}
}
void outputLine(string title = "")
{
	if (title == "")
	{
		cout << "----------------------------------------------------------" << endl;
	}
	else
	{
		cout << "-------------------- " << title << " ---------------------" << endl;
	}
}
void outputBuildings(vector<BuildingInfo> &Buildings)
{
	cout << endl;
	outputLine("Buildings");
	for (auto &building : Buildings)
	{
		if (building.Fullname != "DNE")
		{
			cout << building.Fullname << endl;
		}
	}
	outputLine();
	cout << endl;
}
// Function that is simply used to output some statistics calculated in the previous two functions
void outputStats(map<long long, Coordinates> &Nodes, BuildingInfo &start, BuildingInfo &dest, long long &startNode, long long &destNode)
{
	cout << "Starting Point:" << endl;
	cout << " " << start.Fullname << endl;
	cout << " (" << start.Coords.Lat << ", " << start.Coords.Lon << ")" << endl;
	cout << "Destination Point:" << endl;
	cout << " " << dest.Fullname << endl;
	cout << " (" << dest.Coords.Lat << ", " << dest.Coords.Lon << ")" << endl;
	cout << endl;
	cout << "Nearest start node:" << endl;
	cout << " " << Nodes[startNode].ID << endl;
	cout << " (" << Nodes[startNode].Lat << ", " << Nodes[startNode].Lon << ")" << endl;
	cout << "Nearest destination node:" << endl;
	cout << " " << Nodes[destNode].ID << endl;
	cout << " (" << Nodes[destNode].Lat << ", " << Nodes[destNode].Lon << ")" << endl;
	cout << endl;
}
//////////////////////////////////////////////////////////////////
//
// main
//
int main()
{
	map<long long, Coordinates> Nodes; // maps a Node ID to it's coordinates (lat, lon)
	vector<FootwayInfo> Footways;	   // info about each footway, in no particular order
	vector<BuildingInfo> Buildings;	   // info about each building, in no particular order
	XMLDocument xmldoc;

	cout << "** Navigating College open street map **" << endl;
	cout << endl;
	cout << std::setprecision(8);

	string def_filename = "uic.osm";
	string filename;

	cout << "Enter map filename> ";
	getline(cin, filename);
	if (filename == "")
	{
		filename = def_filename;
	}
	filename = "osmFiles/" + filename;
	//
	// Load XML-based map file
	//
	if (!LoadOpenStreetMap(filename, xmldoc))
	{
		cout << "**Error: unable to load open street map." << endl;
		cout << endl;
		return 0;
	}
	//
	// Read the nodes, which are the various known positions on the map:
	//
	int nodeCount = ReadMapNodes(xmldoc, Nodes);
	//
	// Read the footways, which are the walking paths:
	//
	int footwayCount = ReadFootways(xmldoc, Footways);
	//
	// Read the university buildings:
	//
	int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);
	//
	// Stats
	//
	assert(nodeCount == Nodes.size());
	assert(footwayCount == Footways.size());
	assert(buildingCount == Buildings.size());

	cout << endl;
	cout << "# of nodes: " << Nodes.size() << endl;
	cout << "# of footways: " << Footways.size() << endl;
	cout << "# of buildings: " << Buildings.size() << endl;

	outputBuildings(Buildings);

	graph<long long, double> G;
	addNodes(Nodes, G);			  // Add Nodes as verticies
	addEdges(Nodes, Footways, G); // Add Footways as edges with weights

	cout << "# of vertices: " << G.NumVertices() << endl;
	cout << "# of edges: " << G.NumEdges() << endl;
	cout << endl;

	vector<string> Algorithms = {"Dijkstras", "A-Star", "Breadth First Search", "Depth First Search"};
	string algNum = "1000";
	string algorithm;

	cout << "Algorithms: " << endl;
	for (int i = 0; i < Algorithms.size(); i++)
	{
		cout << " " << i << ") " << Algorithms[i] << endl;
	}
	cout << endl;
	while (stoi(algNum) >= Algorithms.size())
	{
		cout << "Enter Algorithm Number: ";
		getline(cin, algNum);
		cout << endl;
	}
	algorithm = Algorithms[stoi(algNum)];

	//
	// Navigation from building to building
	//
	string startBuilding, destBuilding;
	BuildingInfo start, dest;
	cout << "Enter start (partial name or abbreviation), or #> ";
	getline(cin, startBuilding);

	while (startBuilding != "#")
	{
		cout << "Enter destination (partial name or abbreviation)> ";
		getline(cin, destBuilding);

		bool success = filterInput(Buildings, start, dest, startBuilding, destBuilding); // Find buildings
		if (success)
		{
			long long startNode;
			long long destNode;
			findNearestPoints(Nodes, Footways, start, dest, startNode, destNode); // Find nearest points
			outputStats(Nodes, start, dest, startNode, destNode);				  // Outputs stats calculated previously
			cout << "Navigating with " << algorithm << "..." << endl;
			switch (stoi(algNum))
			{
			case 0:
				findPathDIJ(G, Nodes, startNode, destNode);
				break;
			case 1:
				findPathFS(G, Nodes, startNode, destNode, "as");
				break;
			case 2:
				findPathFS(G, Nodes, startNode, destNode, "bfs");
				break;
			case 3:
				findPathFS(G, Nodes, startNode, destNode, "dfs");
				break;
			}
			cout << endl;
		}

		start.Fullname = "";
		dest.Fullname = "";
		cout << endl;
		cout << "Enter start (partial name or abbreviation), or #> ";
		getline(cin, startBuilding);
	}

	//
	// done:
	//
	cout << "** Done **" << endl;

	return 0;
}