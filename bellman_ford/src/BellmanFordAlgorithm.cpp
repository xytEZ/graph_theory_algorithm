#include <iostream>
#include <utility>

#include "BellmanFordFileParser.hh"

#include "BellmanFordAlgorithm.hh"

namespace graph::bellman_ford
{
  constexpr Distance_t BellmanFordAlgorithm::INFINITE_VALUE;
  
  BellmanFordAlgorithm::BellmanFordAlgorithm() :
    _graph(nullptr),
    _result { false, false, { } }
  { }

  void BellmanFordAlgorithm::init(const BellmanFordFileParser& parser)
  {
    _graph = &parser._graph;
  }

  void BellmanFordAlgorithm::execute()
  {
    std::unordered_map<VertexName_t, Distance_t> bestDistMap;

    bestDistMap.reserve(_graph->vertexNb);
    for (const auto& pair : _graph->vertices)
      {
	const VertexName_t& srcVertex = pair.first;
	
	bestDistMap.try_emplace(srcVertex, INFINITE_VALUE);
      }
    bestDistMap.try_emplace(_graph->endVertexName, INFINITE_VALUE);
    bestDistMap.at(_graph->startVertexName) = 0;
    for (std::uint32_t i = 0; i < _graph->vertexNb; ++i)
      {
	for (const auto& pair : _graph->vertices)
	  {
	    const VertexName_t& srcVertex = pair.first;
	    const NeighboringVertices_t& neighboringVertices = pair.second;
	    
	    for (const auto& pair2 : neighboringVertices)
	      {
		const VertexName_t& destVertex = pair2.first;
		Distance_t srcToDestDist = pair2.second;

		relax(bestDistMap, srcVertex, destVertex, srcToDestDist);
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
  }
  
  void BellmanFordAlgorithm
  ::relax(std::unordered_map<VertexName_t, Distance_t>& bestDistMap,
	  const std::string& srcVertex,
	  const std::string& destVertex,
	  Distance_t srcToDestDist) const noexcept
  {
    Distance_t srcVertexDist = bestDistMap.at(srcVertex);
    Distance_t destVertexDist = bestDistMap.at(destVertex);

    if (srcVertexDist != INFINITE_VALUE
	&& srcVertexDist + srcToDestDist < destVertexDist)
      bestDistMap.at(destVertex) = srcVertexDist + srcToDestDist;
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
    os << std::endl;
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
      }
    return os;
  }
}
