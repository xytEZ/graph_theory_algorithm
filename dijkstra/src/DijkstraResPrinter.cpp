#include <iostream>

#include "DijkstraAlgorithm.hh"

#include "DijkstraResPrinter.hh"

namespace graph::dijkstra
{
  DijkstraResPrinter::DijkstraResPrinter() :
    _desc(nullptr),
    _result(nullptr)
  { }
  
  void DijkstraResPrinter::init(const DijkstraAlgorithm& dijkstraAlgo)
  {
    _desc = dijkstraAlgo._desc;
    _result = &const_cast<DijkstraAlgorithm&>(dijkstraAlgo)._result;
  }

  void DijkstraResPrinter::printAlgoResult() const noexcept
  {
    if (!_result->pathFound)
      std::cout << "Path not found" << std::endl;
    else
      {
	std::cout << "Best distance : "
		  << _result->totalDistance
		  << std::endl;
	std::cout << "Best path : ";

	std::string prevNodeName;
	
	while (!_result->visitedNodeQueue.empty())
	  {
	    NodeName_t nodeName = std::move(_result->visitedNodeQueue.front());
	    
	    _result->visitedNodeQueue.pop();
	    std::cout << nodeName;
	    if (prevNodeName.empty())
	      std::cout << " (0)";
	    else
	      std::cout << " ("
			<< _desc->nodes.at(prevNodeName).at(nodeName)
			<< ")";
	    prevNodeName = std::move(nodeName);
	    if (!_result->visitedNodeQueue.empty())
	      std::cout << " -> ";
	  }
	std::cout << std::endl;
      }
  }
}
