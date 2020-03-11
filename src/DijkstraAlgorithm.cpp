#include <utility>
#include <unordered_set>
#include <queue>
#include <vector>
#include <iostream>

#include "DijkstraFileParser.hh"

#include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  bool DijkstraAlgorithm::NodeDistanceWithTraceGreater
  ::NodeDistanceWithTraceGreater::operator()
    (const NodeDistanceWithTrace& lhs, const NodeDistanceWithTrace& rhs)
    const noexcept
  {
    return lhs.cumulativeDist > rhs.cumulativeDist;
  }
  
  DijkstraAlgorithm::DijkstraAlgorithm() :
    _desc(nullptr),
    _result({ false, 0, std::queue<NodeName_t> { } })
  { }
  
  void DijkstraAlgorithm::init(const DijkstraFileParser& parser)
  {
    _desc = &parser._desc;
    graphDescribe();
  }

  void DijkstraAlgorithm::accept(AGraphResPrinter& printer) const noexcept
  {
    printer.init(*this);
  }

  void DijkstraAlgorithm::execute()
  {
    using PrQueue_t = std::priority_queue<NodeDistanceWithTrace,
					  std::vector<NodeDistanceWithTrace>,
					  NodeDistanceWithTraceGreater>;

    std::unordered_set<NodeName_t> visitedNodeSet;
    PrQueue_t ndwtQueue;

    visitedNodeSet.reserve(_desc->nodes.size());
    ndwtQueue.push({ _desc->startNodeName, 0, std::queue<NodeName_t> { } }); 
    while (!ndwtQueue.empty())
      {
	NodeDistanceWithTrace ndwt = std::move(ndwtQueue.top());

	if (ndwt.nodeName == _desc->endNodeName)
	  {
	    _result.pathFound = true;
	    _result.totalDistance = ndwt.cumulativeDist;
	    _result.visitedNodeQueue = std::move(ndwt.visitedNodeQueue);
	    _result.visitedNodeQueue.push(ndwt.nodeName);
	    break;
	  }
	ndwtQueue.pop();
	if (!visitedNodeSet.emplace(ndwt.nodeName).second)
	  continue;
	for (const auto& pair : _desc->nodes.at(ndwt.nodeName))
	  {
	    if (visitedNodeSet.find(pair.first) == visitedNodeSet.cend())
	      {
		NodeDistanceWithTrace newNdwt;

		newNdwt.nodeName = pair.first;
		newNdwt.cumulativeDist = ndwt.cumulativeDist + pair.second;
		newNdwt.visitedNodeQueue = ndwt.visitedNodeQueue;
		newNdwt.visitedNodeQueue.push(ndwt.nodeName);
		ndwtQueue.push(std::move(newNdwt));
	      }
	  }    
      }
  }

  void DijkstraAlgorithm::graphDescribe() const noexcept
  {
    std::cout << "Start node : " << _desc->startNodeName << std::endl;
    std::cout << "End node : " << _desc->endNodeName << std::endl;
    std::cout << "Arc number : " << _desc->arcNb << std::endl;
    for (const auto& pair : _desc->nodes)
      {
	std::cout << pair.first << " neighbors : ";

	auto start = pair.second.cbegin();
	auto end = pair.second.cend();

	while (start != end)
	  {
	    std::cout << start->first << " (" << start->second << ")";
	    if (std::next(start) != end)
	      std::cout << ", ";
	    ++start;
	  }
	std::cout << std::endl;
      }
    std::cout << std::endl;
  }
}
