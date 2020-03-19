#include <iostream>
#include <utility>
#include <stack>

#include "BellmanFordFileParser.hh"

#include "BellmanFordAlgorithm.hh"

namespace graph::bellman_ford
{
  constexpr Distance_t BellmanFordAlgorithm::INFINITE_VALUE;
  
  BellmanFordAlgorithm::BellmanFordAlgorithm() :
    _graph(nullptr),
    _result { false, false, { }, { } }
  { }

  void BellmanFordAlgorithm::init(const BellmanFordFileParser& parser)
  {
    _graph = &parser._graph;
  }

  void BellmanFordAlgorithm::execute()
  {
    std::unordered_map<VertexName_t, Distance_t> bestDistMap;
    std::unordered_map<VertexName_t, VertexName_t> predecessorMap;

    bestDistMap.reserve(_graph->vertexNb);
    for (const auto& pair : _graph->vertices)
      {
	const VertexName_t& srcVertex = pair.first;
	
	bestDistMap.try_emplace(srcVertex, INFINITE_VALUE);
      }
    bestDistMap.try_emplace(_graph->endVertexName, INFINITE_VALUE);
    bestDistMap.at(_graph->startVertexName) = 0;
    for (std::uint32_t i = 1; i <= _graph->vertexNb - 1; ++i)
      {
	for (const auto& pair : _graph->vertices)
	  {
	    const VertexName_t& srcVertex = pair.first;
	    const NeighboringVertices_t& neighboringVertices = pair.second;
	    
	    for (const auto& pair2 : neighboringVertices)
	      {
		const VertexName_t& destVertex = pair2.first;
		Distance_t srcToDestDist = pair2.second;

		relax(bestDistMap,
		      predecessorMap,
		      srcVertex,
		      destVertex,
		      srcToDestDist);
	      }
	  }
      }
    for (const auto& pair : _graph->vertices)
      {
	const VertexName_t& srcVertex = pair.first;
	const NeighboringVertices_t& neighboringVertices = pair.second;
	
	for (const auto& pair2 : neighboringVertices)
	  {
	    const VertexName_t& destVertex = pair2.first;
	    Distance_t srcToDestDist = pair2.second;
	    Distance_t srcVertexDist = bestDistMap.at(srcVertex);
	    Distance_t destVertexDist = bestDistMap.at(destVertex);

	    if (srcVertexDist + srcToDestDist < destVertexDist)
	      {
		_result.negativeCycle = true;
		return;
	      }
	  }
      }
    _result.pathFound = true;
    _result.bestDistMap = std::move(bestDistMap);
    _result.predecessorMap = std::move(predecessorMap);
  }
  
  void BellmanFordAlgorithm
  ::relax(std::unordered_map<VertexName_t, Distance_t>& bestDistMap,
	  std::unordered_map<VertexName_t, VertexName_t>& predecessorMap,
	  const std::string& srcVertex,
	  const std::string& destVertex,
	  Distance_t srcToDestDist) const noexcept
  {
    Distance_t srcVertexDist = bestDistMap.at(srcVertex);
    Distance_t destVertexDist = bestDistMap.at(destVertex);

    if (srcVertexDist != INFINITE_VALUE
	&& srcVertexDist + srcToDestDist < destVertexDist)
      {
	bestDistMap.at(destVertex) = srcVertexDist + srcToDestDist;
	
	auto res = predecessorMap.try_emplace(destVertex, srcVertex);

	if (!res.second)
	  res.first->second = srcVertex;
      }
  }

  std::ostream& BellmanFordAlgorithm::description(std::ostream& os)
    const noexcept
  {
    os << "Start vertex : " << _graph->startVertexName << std::endl;
    os << "End vertex : " << _graph->endVertexName << std::endl;
    os << "Vertex number : " << _graph->vertexNb << std::endl;
    os << "Edge number : " << _graph->edgeNb << std::endl;
    for (const auto& pair : _graph->vertices)
      {
	const VertexName_t& srcVertex = pair.first;
	const NeighboringVertices_t& neighboringVertices = pair.second;
	
        os << srcVertex << " neighbors : ";

	auto start = neighboringVertices.cbegin();
	auto end = neighboringVertices.cend();

	while (start != end)
	  {
	    os << start->first << " (" << start->second << ")";
	    if (std::next(start) != end)
	      os << ", ";
	    ++start;
	  }
	os << std::endl;
      }
    os << std::endl << std::endl;
    return os;
  }

  std::ostream& BellmanFordAlgorithm::result(std::ostream& os) const noexcept
  {
    if (!_result.pathFound)
      {
	os << "Path not found";
	if (_result.negativeCycle)
	  os << ". Negative cycle !";
	os << std::endl;
      }
    else
      {
	os << "Vertex   Best distance from start vertex"
	   << " until destination vertex"
	   << std::endl;
	for (const auto& pair : _result.bestDistMap)
	  {
	    const VertexName_t& destVertex = pair.first;
	    Distance_t destVertexDist = pair.second;
	    
	    os << destVertex << "\t\t\t\t" << destVertexDist << std::endl;
	  }
	os << std::endl
	   << "Best distance from start to end vertex : "
	   << _result.bestDistMap.at(_graph->endVertexName)
	   << std::endl;

	std::stack<VertexName_t> bestPathStack;
	VertexName_t predVertexName = _graph->endVertexName;
	std::unordered_map<VertexName_t, VertexName_t>::const_iterator it;
	
	while ((it = _result.predecessorMap.find(predVertexName))
	       != _result.predecessorMap.cend())
	  {
	    bestPathStack.push(predVertexName);
	    predVertexName = it->second;
	  }
	bestPathStack.push(predVertexName);

	std::string prevVertexName;
	
	os << "Best path : ";
	while (!bestPathStack.empty())
	  {
	    VertexName_t vertexName = std::move(bestPathStack.top());

	    bestPathStack.pop();
	    os << vertexName;
	    if (prevVertexName.empty())
	      os << " (0)";
	    else
	      os << " ("
		 << _graph->vertices.at(prevVertexName).at(vertexName)
		 << ")";
	    prevVertexName = std::move(vertexName);
	    if (!bestPathStack.empty())
	      os << " -> ";
	  }
	os << std::endl;
      }
    return os;
  }
}
