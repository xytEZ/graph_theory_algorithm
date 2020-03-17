#include <utility>
#include <unordered_set>
#include <queue>
#include <vector>
#include <iostream>

#include "DijkstraFileParser.hh"

#include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  bool DijkstraAlgorithm::VertexDistanceWithTraceGreater
  ::VertexDistanceWithTraceGreater::operator()
    (const VertexDistanceWithTrace& lhs, const VertexDistanceWithTrace& rhs)
    const noexcept
  {
    return lhs.cumulativeDist > rhs.cumulativeDist;
  }
  
  DijkstraAlgorithm::DijkstraAlgorithm() :
    _desc(nullptr),
    _result({ false, 0, std::queue<VertexName_t> { } })
  { }
  
  void DijkstraAlgorithm::init(const DijkstraFileParser& parser)
  {
    _desc = &parser._desc;
  }

  void DijkstraAlgorithm::execute()
  {
    using PrQueue_t = std::priority_queue<VertexDistanceWithTrace,
					  std::vector<VertexDistanceWithTrace>,
					  VertexDistanceWithTraceGreater>;

    std::unordered_set<VertexName_t> visitedVertexSet;
    PrQueue_t vdwtQueue;

    visitedVertexSet.reserve(_desc->vertices.size());
    vdwtQueue
      .push({ _desc->startVertexName, 0, std::queue<VertexName_t> { } }); 
    while (!vdwtQueue.empty())
      {
	VertexDistanceWithTrace vdwt = std::move(vdwtQueue.top());

	if (vdwt.vertexName == _desc->endVertexName)
	  {
	    _result.pathFound = true;
	    _result.totalDistance = vdwt.cumulativeDist;
	    _result.visitedVertexQueue = std::move(vdwt.visitedVertexQueue);
	    _result.visitedVertexQueue.push(vdwt.vertexName);
	    break;
	  }
	vdwtQueue.pop();
	if (!visitedVertexSet.emplace(vdwt.vertexName).second)
	  continue;
	for (const auto& pair : _desc->vertices.at(vdwt.vertexName))
	  {
	    if (visitedVertexSet.find(pair.first) == visitedVertexSet.cend())
	      {
		VertexDistanceWithTrace newVdwt;

		newVdwt.vertexName = pair.first;
		newVdwt.cumulativeDist = vdwt.cumulativeDist + pair.second;
		newVdwt.visitedVertexQueue = vdwt.visitedVertexQueue;
		newVdwt.visitedVertexQueue.push(vdwt.vertexName);
		vdwtQueue.push(std::move(newVdwt));
	      }
	  }    
      }
  }
  
  std::ostream& DijkstraAlgorithm::graphDesc(std::ostream& os) const noexcept
  {    
    os << "Start vertex : " << _desc->startVertexName << std::endl;
    os << "End vertex : " << _desc->endVertexName << std::endl;
    os << "Edge number : " << _desc->edgeNb << std::endl;
    for (const auto& pair : _desc->vertices)
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

	std::string prevVertexName;
	std::queue<VertexName_t> cpyVisitedVertexQueue =
	  _result.visitedVertexQueue;

	while (!cpyVisitedVertexQueue.empty())
	  {
	    VertexName_t vertexName = std::move(cpyVisitedVertexQueue.front());

	    cpyVisitedVertexQueue.pop();
	    os << vertexName;
	    if (prevVertexName.empty())
	      os << " (0)";
	    else
	      os << " ("
		 << _desc->vertices.at(prevVertexName).at(vertexName)
		 << ")";
	    prevVertexName = std::move(vertexName);
	    if (!cpyVisitedVertexQueue.empty())
	      os << " -> ";
	  }
	os << std::endl;
      }
    return os;
  }
}
