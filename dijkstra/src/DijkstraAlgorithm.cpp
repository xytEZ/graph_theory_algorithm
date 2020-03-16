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
  
  std::ostream& DijkstraAlgorithm::graphDesc(std::ostream& os) const noexcept
  {    
    os << "Start node : " << _desc->startNodeName << std::endl;
    os << "End node : " << _desc->endNodeName << std::endl;
    os << "Arc number : " << _desc->arcNb << std::endl;
    for (const auto& pair : _desc->nodes)
      {
	os << pair.first << " neighbors : ";

	auto start = pair.second.cbegin();
	auto end = pair.second.cend();

	while (start != end)
	  {
	    os << start->first << " (" << start->second << ")";
	    if (std::next(start) != end)
	      os << ", ";
	    ++start;
	  }
	os << std::endl;
      }
    os << std::endl;
    return os;
  }

  std::ostream& DijkstraAlgorithm::graphRes(std::ostream& os) const noexcept
  {
    if (!_result.pathFound)
      os << "Path not found" << std::endl;
    else
      {
	os << "Best distance : "
	   << _result.totalDistance
	   << std::endl;
	os << "Best path : ";

	std::string prevNodeName;
	std::queue<NodeName_t> cpyVisitedNodeQueue = _result.visitedNodeQueue;

	while (!cpyVisitedNodeQueue.empty())
	  {
	    NodeName_t nodeName = std::move(cpyVisitedNodeQueue.front());

	    cpyVisitedNodeQueue.pop();
	    os << nodeName;
	    if (prevNodeName.empty())
	      os << " (0)";
	    else
	      os << " ("
		 << _desc->nodes.at(prevNodeName).at(nodeName)
		 << ")";
	    prevNodeName = std::move(nodeName);
	    if (!cpyVisitedNodeQueue.empty())
	      os << " -> ";
	  }
	os << std::endl;
      }
    return os;
  }
}
