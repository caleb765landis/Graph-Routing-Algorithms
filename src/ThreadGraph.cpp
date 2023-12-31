#include "ThreadGraph.h"
#include <typeinfo>

ThreadGraph::ThreadGraph(std::string filename)
{
	std::ifstream file;
	std::queue<std::string> nodeList;

	try{
		file.open(filename);

		// loop through file
		while(!file.eof()){
			std::string line;
			std::getline(file, line);
			nodeList.push(line);
		}

		file.close();
	} catch(...){ /* It'll just re-throw from here and be someone else's problem */}


	// scrape the node list that is full lof string and need to be turned into
	// usable integer indexes of node names
	while(!nodeList.empty()){
		// removes the last element of the queue because of that pesky newline character that
		// getline gives at the end.
		if(nodeList.size() == 1){
			nodeList.pop();
			continue;
		}
		
		// create a temp
		std::string temp = nodeList.front();

		// find the first two rows that include the number of nodes and the number of edges
		// in the graph. with the "#" signifier and find the words "nodes" or "edges" to
		// figure out which line we are on
		if(temp.at(0) == '#'){
			if(temp.find("nodes") <= temp.length())
				// split the string by looking for the "= " sign wich is at the end and take everything
				// after it
				this->numNodes = std::stoi(temp.substr(temp.find("= ") + 2, (temp.find("= ") + 2) - (temp.length() - 1)));
			else if (temp.find("edges") <= temp.length())
				// same as numnodes
				this->numEdges = std::stoi(temp.substr(temp.find("= ") + 2, (temp.find("= ") + 2) - (temp.length() - 1)));
		}
		else{
			// split from the ":" sign
			if(temp.find(':') <= temp.length()){
				// store the node ID number before the ":"
				std::string nodeID = temp.substr(0, temp.find(':'));
				// store everything after the ":" sign
				std::string nbors = temp.substr(temp.find(':') + 1, temp.find(':') - (temp.length() - 1) );

				// loop through everything after the ":" sign neglecting commas "," and store in the graph as
				// integers
				std::string node = "";
				for(int i = 0; i < nbors.length(); i++){
					if(nbors.at(i) != ',' && i != nbors.length() - 1){
						node += nbors.at(i);
						// this->graph[std::stoi(nodeID)].push_back(uint16_t(nbors.at(i) - '0'));
					}
					else if (i == nbors.length() - 1)
					{
						node += nbors.at(i);
						this->graph[std::stoi(nodeID)].push_back(std::stoi(node));
					}
					else{
						if(node.length() > 0)
							this->graph[std::stoi(nodeID)].push_back(std::stoi(node));
						node = "";
					}
				}
			}
		}
		nodeList.pop();
	}
};

// getters for the graph.
uint16_t ThreadGraph::getNumNodes() const {return this->numNodes;};
uint16_t ThreadGraph::getNumEdges() const {return this->numEdges;};
ThreadGraph::neighbors ThreadGraph::getNeighbors(uint16_t index) const {return this->graph.at(index);};
std::string ThreadGraph::nborsToString(uint16_t index) const
{
	ThreadGraph::neighbors nbors = getNeighbors(index);
	std::string info = "";
	std::vector<uint16_t>::iterator it;
	for(it = nbors.begin(); it < nbors.end(); it++){
		info += std::to_string(*it) + ",";
	}

	return info;
}