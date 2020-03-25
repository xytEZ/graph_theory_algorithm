#include <iostream>
#include <utility>

#include "BellmanFordFileParser.hh"

#include "BellmanFordAlgorithm.hh"

namespace graph::bellman_ford
{
  constexpr Distance_t BellmanFordAlgorithm::INFINITE_VALUE;
  
  BellmanFordAlgorithm::BellmanFordAlgorithm() :
    _graph(nullptr),
    _result { false, false, { }, { } },
    _report(_graph, _result)
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
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      bestDistMap.try_emplace(srcVertex, INFINITE_VALUE);
    bestDistMap.try_emplace(_graph->endVertexName, INFINITE_VALUE);
    bestDistMap.at(_graph->startVertexName) = 0;

    bool modification = true;
    
    for (std::uint32_t i = 1; i <= _graph->vertexNb - 1 && modification; ++i)
      {
	modification = false;
	for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
	  {
	    for (const auto& [destVertex, srcToDestDist] : neighboringVertices)
	      relax(bestDistMap,
		    predecessorMap,
		    modification,
		    srcVertex,
		    destVertex,
		    srcToDestDist);
	  }
      }
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      {
	for (const auto& [destVertex, srcToDestDist] : neighboringVertices)
	  {
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
	  bool& modification,
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
	modification = true;
      }
  }

  const BellmanFordAlgoReport& BellmanFordAlgorithm::getReport() const noexcept
  {
    return _report;
  }
}
